#include "voronoi.h"
#include "geometry.h"
#include "make_unique.h"
#include <algorithm>
#include <cassert>


namespace
{
/// y = f * x + g
double coefficientF(const Voronoi::Point & begin, const Voronoi::Point & end)
{
	return (end.y() - begin.y()) / (end.x() - begin.x());
}


/// y = f * x + g
double coefficientG(const Voronoi::Point & begin, const Voronoi::Point & end)
{
	return begin.y() - coefficientF(begin, end) * begin.x();
}
}  // end of anonymous namespace


Voronoi::Generator::Generator(const std::vector<Point> & sites) :
	_eventQueue(compareEvents)
{	
	for (const auto & site : sites) {
		_eventQueue.push(make_unique<SiteEvent>(site));
	}
	_generate();
}


std::list<Voronoi::Edge> Voronoi::Generator::getEdges() const
{
	return _edges;
}


void Voronoi::Generator::_generate()
{
	while (!_eventQueue.empty()) {
		auto event = _eventQueue.top().get();

		// Skip disabled event
		if (event->isDisabled()) {
			_eventQueue.pop();
			continue;
		}

		// Process events
		if (event->isSiteEvent()) {
			// Site event
			_processEvent(static_cast<SiteEvent *>(event));
		}
		else {
			// Vertex event
			_processEvent(static_cast<VertexEvent *>(event));
		}

		_eventQueue.pop();
	}

	// Finish edges
	const double minX = 0.0;
	const double maxX = 1.0;
	const double minY = 0.0;
	const double maxY = 1.0;
	const double offset = 10.0;


	// Finish edges with no end
	auto left = _beachline.root();
	while (!left->isLeaf()) {
		left = left->leftChild();
	}
	while (left->rightSibling()) {
		// Every parabol has edge set. Each parabol's edge has no end
		double x = parabolaIntersectionX(left->rightSibling()->site(), left->site(), minY - offset);
		double y = getParabolaY(left->site(), minY - offset, x);
		const Point end(x, y);
		const double f = coefficientF(left->edge()->begin(), end);
		const double g = coefficientG(left->edge()->begin(), end);

		if (x < minX) {
			x = minX;
			y = f * x + g;
		}
		else if (x > maxX) {
			x = maxX;
			y = f * x + g;
		}

		if (y < minY) {
			y = minY;
			x = (y - g) / f;
		}
		else if (y > maxY) {
			y = maxY;
			x = (y - g) / f;
		}


		left->setEdgeEnd(Point(x, y));
		left = left->rightSibling();
	}

	// Connect neighbours
	for (auto it = _edges.begin(); it != _edges.end(); ++it) {
		auto neighbour = it->neighbour;
		if (neighbour) {
			// Connect two neighbour edges
			neighbour->setBegin(it->end());
			_edges.erase(it--);
		}
		else if (it != _edges.begin() && it->end().y() > maxY) {  // TODO ????
			//_edges.erase(it--);
		}
	}

}


void Voronoi::Generator::_circleEvent(ParabolaNode * parabola, const double sweepline)
{
	// Find left and right parabola
	auto left = parabola->leftSibling();
	auto right = parabola->rightSibling();
	if (!left || !right) {
		return;
	}

	// Check if the bottom point of the circumcircle lies under the sweepline
	auto circumcenter = Circumcenter(left->site(), parabola->site(), right->site());
	if (!circumcenter) {
		return;
	}
	auto radius = CircleRadius(*circumcenter, parabola->site());
	const double bottomCirclePoint = circumcenter->y() - radius;
	if (bottomCirclePoint >= sweepline) {
		return;
	}

	// Create Vertex event
	auto event = make_unique<VertexEvent>(Point(circumcenter->x(), bottomCirclePoint));
	event->setCircumcenter(*circumcenter);
	event->setParabolaNode(parabola);
	parabola->setEvent(event.get());        // JENOM Kvuli tomuhle nemuze byt metoda konstantni. Pak nekonst i metody processEvent. Presunout to do Parabola nebo Event!
	_eventQueue.push(std::move(event));
}


void Voronoi::Generator::_processEvent(const SiteEvent * event)
{
	auto newParabola = _beachline.emplaceParabola(event->site());
	auto left = newParabola->leftSibling();
	auto right = newParabola->rightSibling();

	if (!left && !right) {
		return;
	}
	assert(left->site() == right->site());
	const double sweepline = event->site().y();

	// Create a new (dangling) edge
	Point begin = (left->site() + event->site()) / 2.0;
	_edges.emplace_back(left->site(), event->site());
	auto firstEdge = &_edges.back();
	firstEdge->setBegin(begin);

	_edges.emplace_back(event->site(), right->site());
	auto secondEdge = &_edges.back();
	secondEdge->setBegin(begin);


	// BUG JE TO TAM DVAKRAT
	left->setEdge(firstEdge);
	newParabola->setEdge(secondEdge);
	secondEdge->neighbour = firstEdge;

	// TODO neighbour (co je right napravo) by mohl byt vlastnici pointer, pak ho stejne smazeme...

	// Check fircle event
	if (left) {
		_circleEvent(left, sweepline);
	}
	if (right) {

		if (left->leftSibling() && right->rightSibling() && left->leftSibling()->site() == right->rightSibling()->site()) {
			// Pokud left->leftSibling->site == right->rightSibling->site, prida se dvakrat event na stejne misto! Toto je oprava.
			return;
		}

		_circleEvent(right, sweepline);
	}
}


void Voronoi::Generator::_processEvent(VertexEvent * event)
{
	// Left and right always exists in vertex event
	auto left = event->parabolaNode()->leftSibling();
	auto right = event->parabolaNode()->rightSibling();

	// Finish two edges
	left->setEdgeEnd(event->circumcenter());  // nastavujeme konec pro prave pokracovani
	event->parabolaNode()->setEdgeEnd(event->circumcenter());  // nastavujeme konec pro leve pokracovani
	left->setEdge(nullptr);
	event->parabolaNode()->setEdge(nullptr);  // Tyto dva nullptr lze po odladeni smazat

	// Create a new (dangling) edge
	_edges.emplace_back(left->site(), right->site());
	left->setEdge(&_edges.back());
	left->setEdgeBegin(event->circumcenter());
	
	// Remove this parabola
	_beachline.removeParabola(event->parabolaNode());
	assert(left->site() != right->site()); // left and right parabolas can't have the same focus
	
	// Check fircle event
	const double sweepline = event->site().y();
	_circleEvent(left, sweepline);
	_circleEvent(right, sweepline);
}

