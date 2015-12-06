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


Voronoi::Generator::Generator(const std::vector<Point> & sites, const BoundingBox & boundingBox) :
	_boundingBox(boundingBox)
{
	// TODO: Odstranit sites ktere jsou mimo Max Min x y limity.

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

		/*

	// Finish edges
	const double minX = _boundingBox.MinX;
	const double maxX = _boundingBox.MaxX;
	const double minY = _boundingBox.MinY;
	const double maxY = _boundingBox.MaxY;
	const double offset = 10.0 * (maxY - minY);
	assert(offset > 0);


	// Finish edges with no end
	auto left = _beachline.root();
	while (!left->isLeaf()) {
		left = left->leftChild();
	}

	while (left->rightSibling()) {
		// Every parabol has edge set. Each parabol's edge has no end
		double x = parabolaIntersectionX(left->site(), left->rightSibling()->site(), minY - offset);
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
	}

	// Remove edges which are out of range
	for (auto it = _edges.begin(); it != _edges.end(); ++it) {
		if (it->begin().x() < minX || it->begin().x() > maxX || it->begin().y() < minY || it->begin().y() > maxY) {
			if (it == _edges.begin()) {
				_edges.erase(it);
				it = _edges.begin();
			}
			else {
				_edges.erase(it--);
			}
			continue;
		}
	}
	

	// Posun konce ostatnich vrcholu
	// !! Tohle jsou konce ktere byly pred Connect neighbours zacatky a proto nejsou upraveny!
	for (auto it = _edges.begin(); it != _edges.end(); ++it) {
		const Point end = it->end();
		double x = end.x();
		double y = end.y();

		const double f = coefficientF(it->begin(), end);
		const double g = coefficientG(it->begin(), end);
		if (x < minX) {
			x = minX;
			y = f * x + g;
		}
		else if (x > maxX) {
			x = minX;
			y = f * x + g;
		}*/
		/*
		if (y < minY) {
			y = minY;
			x = (y - g) / f;
		}
		else if (y > maxY) {
			y = maxY;
			x = (y - g) / f;
		}

		it->setEnd(Point(x, y));
		
	}
	*/
}


void Voronoi::Generator::_circleEvent(ParabolaNode * parabola, const double sweepline)
{
	// Find left and right parabola
	auto left = parabola->leftSibling();
	auto right = parabola->rightSibling();
	if (!left || !right || left->site() == right->site()) {
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
	parabola->setEvent(event.get());
	_eventQueue.push(std::move(event));
}


void Voronoi::Generator::_processEvent(const SiteEvent * event)
{
	auto newParabola = _beachline.emplaceParabola(event->site());
	auto left = newParabola->leftSibling();
	auto right = newParabola->rightSibling();

	// The `left` is superior to `right`. See note below.
	if (!left) {
		return;
	}

	// For simplicity we suppose the "left" always exists.
	// This is ensured by our `Compare` functional.
	assert(!right || left->site() == right->site());
	const double sweepline = event->site().y();

	// Create a new (dangling) edge
	Point begin = (left->site() + event->site()) / 2.0;
	_edges.emplace_back(left->site(), event->site());
	auto firstEdge = &_edges.back();
	firstEdge->setBegin(begin);

	_edges.emplace_back(event->site(), left->site());
	auto secondEdge = &_edges.back();
	secondEdge->setBegin(begin);

	left->setEdge(firstEdge);
	newParabola->setEdge(secondEdge);
	secondEdge->neighbour = firstEdge;

	// TODO neighbour (co je right napravo) by mohl byt vlastnici pointer, pak ho stejne smazeme...

	// Check fircle event
	if (left) {
		_circleEvent(left, sweepline);
	}
	if (right) {		
		// The event can sometimes be at the same position as previous "left",
		// but this "right" belongs to another ("right") parabola.
		_circleEvent(right, sweepline);  // s right je to spravne!
	}
}


void Voronoi::Generator::_processEvent(VertexEvent * event)
{
	// Check fircle event
	const double sweepline = event->site().y();

	// Left and right always exists in vertex event
	auto left = event->parabolaNode()->leftSibling();
	auto right = event->parabolaNode()->rightSibling();

	// Finish two edges
	if (left->site().x() < event->site().x()) {
		left->setEdgeEnd(event->circumcenter());  // nastavujeme konec pro prave pokracovani
	}
	else {
		// TODO Protahnout ke kraji??
		//left->setEdgeEnd(Point(-1, -1));

		// BUG Timto prepiseme existujici a spravnou hodnotu!!! Musime lepe promyslet vztah paraboly a edge, možna způsob "levá parabola" je špatně.
	}

	if (event->site().x() < right->site().x()) {
		event->parabolaNode()->setEdgeEnd(event->circumcenter());  // nastavujeme konec pro leve pokracovani
	}
	else {
		// TODO Protahnout ke kraji??
		//left->setEdgeEnd(Point(-1, -1));
	}



	left->setEdge(nullptr);  // TODO Lze po odladeni smazat
	event->parabolaNode()->setEdge(nullptr);  // TODO Lze po odladeni smazat
	
	// Remove this parabola (this also disables events with this parabola's site]
	_beachline.removeParabola(event->parabolaNode());
	assert(left->site() != right->site()); // left and right parabolas can't have the same focus

	if (sweepline < _boundingBox.MinY) {
		// Don't generate another event if we are below MinY.
		return;
	}

	// Create a new (dangling) edge
	_edges.emplace_back(left->site(), right->site());
	left->setEdge(&_edges.back());
	left->setEdgeBegin(event->circumcenter());

	_circleEvent(left, sweepline);
	_circleEvent(right, sweepline);
}

