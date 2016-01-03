#include "voronoi.h"
#include "geometry.h"
#include "make_unique.h"
#include <algorithm>
#include <cassert>


namespace
{
const double Epsilon = 1e-12;

/// Return true if Point is zero point
bool isZero(const Voronoi::Point & point)
{
	return std::abs(point.x()) < Epsilon && std::abs(point.y()) < Epsilon;
}


/// y = f * x + g
double coefficientF(const Voronoi::Point & begin, const Voronoi::Point & end)
{
	return (end.y() - begin.y()) / (end.x() - begin.x());
}


/// y = f * x + g
double coefficientG(const Voronoi::Point & begin, const double f)
{
	return begin.y() - f * begin.x();
}
}  // end of anonymous namespace


Voronoi::Generator::Generator(const std::vector<Point> & sites, const BoundingBox & boundingBox) :
	_boundingBox(boundingBox)
{
	_siteEventQueue.reserve(sites.size());
	for (const auto & site : sites) {
		if (site.x() > boundingBox.MinX && site.x() < boundingBox.MaxX && site.y() > boundingBox.MinY && site.y() < boundingBox.MaxY) {
			_siteEventQueue.emplace_back(site);
		}
	}
	std::sort(_siteEventQueue.begin(), _siteEventQueue.end(), &operator>);
	_generate();
}


std::list<Voronoi::Edge> Voronoi::Generator::getEdges() const
{
	return _edges;
}


// TODO: Musime umet zamenit Min / Max a vetsi/mesi pro přesahy.
void Help(Voronoi::Edge * edge, const Voronoi::BoundingBox & boundingBox)
{
	if (edge->end().x() == 0 && edge->end().y() == 0) {  // TODO !!!!! Vytvorit nejaky "not set" tag misto nuly!

		const double f = -1.0 / coefficientF(edge->left(), edge->right());
		const double g = coefficientG(edge->begin(), f);

		if (edge->left().y() == edge->right().y()) {  // f == inf
			double x = edge->begin().x();
			double y = boundingBox.MaxY;
			edge->setEnd(Voronoi::Point(x, y));
		}
		else if (f > 0) {
			// elongate upwards
			double x = boundingBox.MaxX;
			double y = f * boundingBox.MaxX + g;
			if (y > boundingBox.MaxY) {
				y = boundingBox.MaxY;
				x = (boundingBox.MaxY - g) / f;
			}
			edge->setEnd(Voronoi::Point(x, y));


		}
		else if (f < 0) {
			// elongate downwards
			double x = boundingBox.MaxX;
			double y = f * boundingBox.MaxX + g;
			if (y < boundingBox.MinY) {
				y = boundingBox.MinY;
				x = (boundingBox.MinY - g) / f;
			}
			edge->setEnd(Voronoi::Point(x, y));
		}
		else {  // f == 0, left.x == right.x
			double y = g;
			double x = boundingBox.MinX;
			edge->setEnd(Voronoi::Point(x, y));
		}

	}
}


void HelpNeighbour(Voronoi::Edge * edge, const Voronoi::BoundingBox & boundingBox)
{
	if (edge->end().x() == 0 && edge->end().y() == 0) {  // TODO !!!!! Vytvorit nejaky "not set" tag misto nuly!
		const double f = -1.0 / coefficientF(edge->left(), edge->right());
		const double g = coefficientG(edge->begin(), f);


		if (edge->left().y() == edge->right().y()) {  // f == inf
			double x = edge->begin().x();
			double y = boundingBox.MinY;
			edge->setEnd(Voronoi::Point(x, y));
		}
		else if (f > 0) {
			// elongate upwards
			double x = boundingBox.MinX;
			double y = f * boundingBox.MinX + g;
			if (y < boundingBox.MinY) {
				y = boundingBox.MinY;
				x = (boundingBox.MinY - g) / f;
			}
			edge->setEnd(Voronoi::Point(x, y));
		}
		else if (f < 0) {
			// elongate downwards
			double x = boundingBox.MinX;
			double y = f * boundingBox.MinX + g;
			if (y > boundingBox.MaxY) {
				y = boundingBox.MaxY;
				x = (boundingBox.MaxY - g) / f;
			}
			edge->setEnd(Voronoi::Point(x, y));
		}
		else {  // f == 0
			double y = g;
			double x = boundingBox.MaxX;
			edge->setEnd(Voronoi::Point(x, y));
		}


	}
}


void Voronoi::Generator::_generate()
{
	auto siteIt = _siteEventQueue.begin();
	while (!_vertexEventQueue.empty() || siteIt != _siteEventQueue.end()) {
		if (!_vertexEventQueue.empty() && siteIt != _siteEventQueue.end()) {
			auto vertexEvent = _vertexEventQueue.top().get();
			if (vertexEvent->site() < siteIt->site()) {
				_processEvent(&*siteIt);
				++siteIt;

			}
			else {
				auto event = _vertexEventQueue.top().get();
				if (!event->isDisabled()) {
					_processEvent(event);
				}
				_vertexEventQueue.pop();
			}

		}
		else if (!_vertexEventQueue.empty()) {
			auto event = _vertexEventQueue.top().get();
			if (!event->isDisabled()) {
				_processEvent(event);
			}
			_vertexEventQueue.pop();
		}
		else {  // siteIt != _siteEventQueue.end()
			_processEvent(&*siteIt);
			++siteIt;
		}


	}


	// Handle site events
	for (auto it = _edges.begin(); it != _edges.end(); ++it) {
		if (!it->twin) {  // Site events only
			continue;
		}

		// TODO twin ma definovany jenom 1 edge ze 2. Mohl by to byt i vlastnici pointer.

		Help(&*it, _boundingBox);
		HelpNeighbour(it->twin, _boundingBox);



		// TODO Vertex eventy - prodloužit a taky jsou tam nějake fake neexistujici.

		// TODO Asi by stačilo projit jenom vertexy v Beachline a dokončit ji. To by ušetřilo nějaký čas.


		// TODO Spojit site eventy do jednoho edge
		//twin->setBegin(it->end());
		//_edges.erase(it--);
	}

	// Remove degenerate vertices
	for (auto it = _edges.begin(); it != _edges.end(); ++it) {
		if (isZero(it->begin() - it->end())) {
			// Vertex event is never first in _edges, so this erase is correct.
			_edges.erase(it--);
		}
	}
	
	// Remove duplicated edges
	for (auto it = _edges.begin(); it != _edges.end();) {
		auto first = *it;
		++it;
		if (it == _edges.end()) {
			break;
		}

		const Point begin = it->begin() - first.begin();
		const Point end = it->end() - first.end();
		if (isZero(begin) && isZero(end)) {
			// Vertex event is never first in _edges, so this erase is correct.
			_edges.erase(it--);
		}
	}
	


	/*
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
		auto twin = it->twin;
		if (twin) {
			// Connect two twin edges
			twin->setBegin(it->end());
			_edges.erase(it--);
		}
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
	if (bottomCirclePoint <= _boundingBox.MinY) {
		// Don't generate another event if we are below MinY
		return;
	}
	if (bottomCirclePoint > sweepline + Epsilon * sweepline) {
		return;
	}

	// Create Vertex event
	auto event = make_unique<VertexEvent>(Point(circumcenter->x(), bottomCirclePoint));
	event->setCircumcenter(*circumcenter);
	event->setParabolaNode(parabola);
	parabola->setEvent(event.get());
	_vertexEventQueue.push(std::move(event));
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
	secondEdge->twin = firstEdge;

	// TODO twin (co je right napravo) by mohl byt vlastnici pointer, pak ho stejne smazeme...

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

	if (event->site().x() < right->site().x()) {  // TODO `<=` or `<` ?
		event->parabolaNode()->setEdgeEnd(event->circumcenter());  // nastavujeme konec pro leve pokracovani
	}
	
	// Remove this parabola (this also disables events with this parabola's site]
	_beachline.removeParabola(event->parabolaNode());
	assert(left->site() != right->site()); // left and right parabolas can't have the same focus

	// Create a new (dangling) edge
	_edges.emplace_back(left->site(), right->site());
	left->setEdge(&_edges.back());
	left->setEdgeBegin(event->circumcenter());

	_circleEvent(left, sweepline);
	_circleEvent(right, sweepline);
}

