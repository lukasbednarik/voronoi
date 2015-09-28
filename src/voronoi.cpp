#include "voronoi.h"
#include "geometry.h"
#include "make_unique.h"
#include <algorithm>
#include <cassert>
#include <functional>


Voronoi::Generator::Generator(const std::vector<Point> & sites) :
	_sites(sites),
	_eventQueue(compareEvents)
{	
	for (const auto & site : _sites) {
		_eventQueue.push(make_unique<SiteEvent>(&site));
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



	for (auto it = _edges.begin(); it != _edges.end(); ++it) {
		// If any edge has no end end the begin is out of range, delete it
		if (!it->end()) {
			const double x = it->begin()->x();
			const double y = it->begin()->y();
			if (x < minX || x > maxX || y < minY || y > maxY) {
				_edges.erase(it--);
			}
		}
	}

	// Finish edges with no end
	auto left = _beachline.root();
	while (!left->isLeaf()) {
		left = left->leftChild();
	}
	while (left->rightSibling()) {
		if (!left->edge()) {
			left = left->rightSibling();
			continue;
		}

		double x = parabolaIntersectionX(*left->site(), *left->rightSibling()->site(), minY - offset);
		double y = getParabolaY(*left->site(), minY - offset, x);



		left->edge()->setEnd(Point(x, y));
		left = left->rightSibling();
	}

	// Crop the edge if it doesn't fit into the box
	for (auto it = _edges.begin(); it != _edges.end(); ++it) {
		const double begX = it->begin()->x();
		const double begY = it->begin()->y();
		if (begX < minX) {

		}
		else if (begX > maxX) {

		}
		if (begY < minY) {

		}
		else if (begY > maxY){

		}

		// TODO the same for end

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
	auto circumcenter = Circumcenter(*left->site(), *parabola->site(), *right->site());
	if (!circumcenter) {
		return;
	}
	auto radius = CircleRadius(*circumcenter, *parabola->site());
	const double bottomCirclePoint = circumcenter->y() - radius;
	if (bottomCirclePoint >= sweepline) {
		return;
	}

	// Create Vertex event
	_tempVertex.emplace_back(circumcenter->x(), bottomCirclePoint);
	auto event = make_unique<VertexEvent>(&_tempVertex.back());
	event->circumcenter = *circumcenter;
	event->setParabolaNode(parabola);
	parabola->setEvent(event.get());        // JENOM Kvuli tomuhle nemuze byt metoda konstantni. Pak nekonst i metody processEvent.
	_eventQueue.push(std::move(event));
}


void Voronoi::Generator::_processEvent(const SiteEvent * event)
{
	auto newParabola = _beachline.emplaceParabola(event->site());
	auto left = newParabola->leftSibling();
	auto right = newParabola->rightSibling();

	// Create a new edge
	const double sweepline = event->site()->y();
	if (left && right) {
		assert(left->site() == right->site());

		// Create a new (dangling) edge
		Point begin(event->site()->x(), getParabolaY(*left->site(), sweepline, event->site()->x()));
		_edges.emplace_back(left->site(), event->site());
		_edges.back().setBegin(begin);
		left->setEdge(&_edges.back());
	}
	
	// Check fircle event
	if (left) {
		_circleEvent(left, sweepline);
	}
	if (right) {
		// Pokud left->site() == right->site(), nemusime tohle zkouset.
		// Jediny problem nastane pokud se s novym vrcholem trefime presne na prusecik parabol.

		//_circleEvent(right, sweepline);
	}
}


void Voronoi::Generator::_processEvent(VertexEvent * event)
{
	// Left and right always exists in vertex event
	auto left = event->parabolaNode()->leftSibling();
	auto right = event->parabolaNode()->rightSibling();

	// Finish edges
	left->edge()->setEnd(event->circumcenter);
	event->parabolaNode()->edge()->setEnd(event->circumcenter);

	// Create a new (dangling) edge
	_edges.emplace_back(left->site(), right->site());
	left->setEdge(&_edges.back());
	left->edge()->setBegin(event->circumcenter);
	
	// Remove this parabola
	_beachline.removeParabola(event->parabolaNode());
	assert(left->site() != right->site()); // left and right parabolas can't have the same focus
	
	// Check fircle event
	const double sweepline = event->site()->y();
	_circleEvent(left, sweepline);
	_circleEvent(right, sweepline);
}

