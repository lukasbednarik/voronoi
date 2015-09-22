#include "voronoi.h"
#include "geometry.h"
#include "make_unique.h"
#include <algorithm>


Voronoi::Generator::Generator(const std::vector<Point> & sites) :
	_sites(sites)
{	
	_findEdges();
}


std::vector<Voronoi::Edge> Voronoi::Generator::getEdges() const
{
	return _edges;
}


void Voronoi::Generator::_findEdges()
{
	std::vector<std::unique_ptr<Event>> eventQueue;
	for (const auto & site : _sites) {
		eventQueue.push_back(make_unique<Event>(&site));
	}
	

	for (size_t i = 0; i < eventQueue.size(); ++i) {
		// Sort the queue
		std::sort(eventQueue.begin(), eventQueue.end(),
			[] (const std::unique_ptr<Event> & left, const std::unique_ptr<Event> & right) {
				return left->site()->y() > right->site()->y();
			}
		);

		// Skip disabled event
		Event * event = eventQueue[i].get();
		if (event->isDisabled()) {
			continue;
		}

		CircleEventType newEvent(nullptr, nullptr);
		if (event->isSiteEvent()) {
			// Site event
			newEvent = _processSiteEvent(event);
		}
		else {
			// Vertex event
			newEvent = _processVertexEvent(event);
		}
		if (newEvent.first) {
			eventQueue.push_back(std::move(newEvent.first)); // TODO MOZNA OPRAVDU LEPSI BACK INSERTER NEBO MEMBER VARIABLE...
		}
		if (newEvent.second) {
			eventQueue.push_back(std::move(newEvent.second));
		}
	}
}


std::unique_ptr<Voronoi::Event> Voronoi::Generator::_circleEvent(ParabolaNode * parabola, const double sweepline)
{
	// Find left and right parabola
	auto left = parabola->leftSibling();
	auto right = parabola->rightSibling();
	if (!left || !right) {
		return nullptr;
	}

	// Check if the bottom point of the circumcircle lies under the sweepline
	auto circumcenter = Circumcenter(*left->site(), *parabola->site(), *right->site());
	if (!circumcenter) {
		return nullptr;
	}
	auto radius = CircleRadius(*circumcenter, *parabola->site());
	const double bottomCirclePoint = circumcenter->y() - radius;
	if (bottomCirclePoint >= sweepline) {
		return nullptr;
	}

	// Create Vertex event
	_vertices.emplace_back(circumcenter->x(), bottomCirclePoint);
	auto event = make_unique<Event>(&_vertices.back(), false);
	event->setParabolaNode(parabola);
	parabola->setEvent(event.get());
	return event;
}


Voronoi::Generator::CircleEventType Voronoi::Generator::_processSiteEvent(const Event * event)
{
	const double sweepline = event->site()->y();
	
	auto newParabola = _beachline.emplaceParabola(event->site());
	auto left = newParabola->leftSibling();
	auto right = newParabola->rightSibling();

	if (left && right) {
		// Create a new (dangling) edge
		_edges.emplace_back(left->site(), right->site());

		printf("dangling edge\n");
	}

	// Check circle events
	CircleEventType newEvent(nullptr, nullptr);
	if (left) {
		newEvent.first = _circleEvent(left, sweepline);
	}
	if (right) {
		newEvent.second = _circleEvent(right, sweepline);
	}
	return newEvent;
}


Voronoi::Generator::CircleEventType Voronoi::Generator::_processVertexEvent(Event * event)
{
	const double sweepline = event->site()->y();


	// Ukonci edges kolem parabolaNode.
	printf("%f, %f\n", event->site()->x(), event->site()->y()); // BUG SITE X je snizene o polomer kruhu, viz: const double bottomCirclePoint = circumcenter->y() - radius;


	// Remove this parabola
	auto left = event->parabolaNode()->leftSibling();
	auto right = event->parabolaNode()->rightSibling();
	event->parabolaNode()->event()->disable();
	_beachline.removeParabola(event->parabolaNode());

	if (left && right) {
		// Create a new (dangling) edge
		_edges.emplace_back(left->site(), right->site());
	}

	// Check circle events
	CircleEventType newEvent(nullptr, nullptr);
	if (left) {
		newEvent.first = _circleEvent(left, sweepline);
	}
	if (right) {
		newEvent.second = _circleEvent(right, sweepline);
	}
	return newEvent;
}

