#include "voronoi.h"
#include "geometry.h"
#include "make_unique.h"
#include <algorithm>
#include <cassert>


namespace
{

	// y = f * x + g
	double coefficientF(const Voronoi::Point & begin, const Voronoi::Point & end)
	{
		return (end.y() - begin.y()) / (end.x() - begin.x());
	}


	double coefficientG(const Voronoi::Point & begin, const Voronoi::Point & end)
	{
		return begin.y() - coefficientF(begin, end) * begin.x();
	}

}


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
		double x = parabolaIntersectionX(left->rightSibling()->site(), left->site(), minY - offset);  // BUG fce je nachylna na poradi parabol. To neni univerzalni!
		double y = getParabolaY(left->site(), minY - offset, x);
		left->setEdgeEnd(Point(x, y));




		// TODO spojeni s neighbour obsahuje i dokonceni (protahnuti) pokud neighbour nema konec. 



		left = left->rightSibling();
	}


	// Crop the edge if it doesn't fit into the box
	for (auto it = _edges.begin(); it != _edges.end(); ++it) {
		double begX = it->begin().x();
		double begY = it->begin().y();
		const double f = coefficientF(it->begin(), it->end());
		const double g = coefficientG(it->begin(), it->end());
		if (begX < minX) {
			begX = minX;
			begY = f * begX + g;
		}
		else if (begX > maxX) {
			begX = maxX;
			begY = f * begX + g;
		}
		if (begY < minY) {
			begY = minY;
			begX = (begY - g) / f;

		}
		else if (begY > maxY){
			begY = maxY;
			begX = (begY - g) / f;
		}
		it->setBegin(Point(begX, begY));

		// the same for end
		double endX = it->end().x();
		double endY = it->end().y();
		if (endX < minX) {
			endX = minX;
			endY = f * endX + g;
		}
		else if (endX > maxX) {
			endX = maxX;
			endY = f * endX + g;
		}
		if (endY < minY) {
			endY = minY;
			endX = (endY - g) / f;

		}
		else if (endY > maxY) {
			endY = maxY;
			endX = (endY - g) / f;
		}
		it->setEnd(Point(endX, endY));

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
	//Point begin(event->site().x(), getParabolaY(left->site(), sweepline, event->site().x()));
	Point begin((left->site().x() + event->site().x()) / 2, (left->site().y() + event->site().y()) / 2);
	_edges.emplace_back(left->site(), event->site());
	_edges.back().setBegin(begin);
	left->setEdge(&_edges.back());

	_edges.emplace_back(event->site(), right->site());
	_edges.back().setBegin(begin);
	newParabola->setEdge(&_edges.back());  // TODO Toto je pripad kdy se edge muze skladat ze 2 casti! Obe casti nutno pozdeji propojit... Pointer na neighbour.
	
	// TODO Pokud se novy vrchol trefi presne mezi, muze byt problem. Nutno ukoncit edge... (symetricky k vertexEvent...)

	// Check fircle event
	if (left) {
		_circleEvent(left, sweepline);
	}
	if (right) {

		if (left->leftSibling() && right->rightSibling() && left->leftSibling()->site() == right->rightSibling()->site()) {
			return;
		}

		// Pokud left->leftSibling->site == right->rightSibling->site, prida se dvakrat event na stejne misto!. Check this! TODO
		_circleEvent(right, sweepline);
	}
}


void Voronoi::Generator::_processEvent(VertexEvent * event)
{
	// Left and right always exists in vertex event
	auto left = event->parabolaNode()->leftSibling();
	auto right = event->parabolaNode()->rightSibling();

	// Finish two edges
	left->setEdgeEnd(event->circumcenter());
	event->parabolaNode()->setEdgeEnd(event->circumcenter());

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

