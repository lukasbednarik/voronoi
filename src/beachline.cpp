#include "beachline.h"
#include "geometry.h"
#include "event.h"
#include "make_unique.h"
#include <cassert>
#include <string>


Voronoi::ParabolaNode::ParabolaNode(const Point * site) :
	_site(site),
	_parent(nullptr),
	_leftSibling(nullptr),
	_rightSibling(nullptr),
	_event(nullptr),
	_edge(nullptr)
{
}


bool Voronoi::ParabolaNode::isLeaf() const
{
	return !_leftChild && !_rightChild;
}


const Voronoi::Point * Voronoi::ParabolaNode::site() const
{
	return _site;
}


Voronoi::VertexEvent * Voronoi::ParabolaNode::event()
{
	return _event;
}


void Voronoi::ParabolaNode::setEvent(VertexEvent * event)
{
	_event = event;
}


Voronoi::Edge * Voronoi::ParabolaNode::edge()
{
	return _edge;
}


const Voronoi::Edge * Voronoi::ParabolaNode::edge() const
{
	return _edge;
}


void Voronoi::ParabolaNode::setEdge(Edge * edge)
{
	_edge = edge;
}


Voronoi::ParabolaNode * Voronoi::ParabolaNode::leftSibling()
{
	return _leftSibling;
}


Voronoi::ParabolaNode * Voronoi::ParabolaNode::rightSibling()
{
	return _rightSibling;
}


const Voronoi::ParabolaNode * Voronoi::ParabolaNode::leftSibling() const
{
	return _leftSibling;
}


const Voronoi::ParabolaNode * Voronoi::ParabolaNode::rightSibling() const
{
	return _rightSibling;
}


const Voronoi::ParabolaNode * Voronoi::ParabolaNode::leftChild() const
{
	return _leftChild.get();
}


const Voronoi::ParabolaNode * Voronoi::ParabolaNode::rightChild() const
{
	return _rightChild.get();
}


Voronoi::ParabolaNode * Voronoi::ParabolaNode::leftChild()
{
	return _leftChild.get();
}


Voronoi::ParabolaNode * Voronoi::ParabolaNode::rightChild()
{
	return _rightChild.get();
}


void Voronoi::ParabolaNode::setLeftChild(std::unique_ptr<ParabolaNode> parabolaTree)
{
	_leftChild = std::move(parabolaTree);
	_leftChild->_parent = this;
}


void Voronoi::ParabolaNode::setRightChild(std::unique_ptr<ParabolaNode> parabolaTree)
{
	_rightChild = std::move(parabolaTree);
	_rightChild->_parent = this;
}


std::unique_ptr<Voronoi::ParabolaNode> Voronoi::ParabolaNode::releaseLeftChild()
{
	auto child = std::move(_leftChild);
	child->_parent = nullptr;
	_leftChild.reset();
	return child;
}


std::unique_ptr<Voronoi::ParabolaNode> Voronoi::ParabolaNode::releaseRightChild()
{
	auto child = std::move(_rightChild);
	child->_parent = nullptr;
	_rightChild.reset();
	return child;
}


void Voronoi::ParabolaNode::_cross(ParabolaNode * left, ParabolaNode * right)
{
	if (left) {
		left->_rightSibling = right;
	}
	if (right) {
		right->_leftSibling = left;
	}
}


Voronoi::ParabolaNode * Voronoi::Beachline::emplaceParabola(const Point * site)
{
	assert(site);

	if (!_root) {
		_root = make_unique<ParabolaNode>(site);
		return _root.get();
	}

	ParabolaNode * parabola = findParabola(*site);
	const Point * parabolaSite = parabola->site();
	assert(parabolaSite->y() >= site->y());
	assert(parabola->isLeaf());


	// disable events
	if (parabola->event()) {
		parabola->event()->disable();
		parabola->setEvent(nullptr);  // TODO Function Prabola::DisableEvent() ktery nastavi 2 veci.
	}


	// Implementace je tragicka, pac musime kopirovat vsechny promenne paraboly (napr. edge) rucne. TODO TODO zlepsit.

	if (site->x() < parabolaSite->x()) {
		parabola->setRightChild(make_unique<ParabolaNode>(parabolaSite));
		parabola->setLeftChild(make_unique<ParabolaNode>(nullptr));
		auto left = parabola->_leftChild.get();
		left->setLeftChild(make_unique<ParabolaNode>(parabolaSite));
		left->setRightChild(make_unique<ParabolaNode>(site));

		// Set siblings
		ParabolaNode::_cross(parabola->_leftSibling, left->_leftChild.get());
		ParabolaNode::_cross(left->_leftChild.get(), left->_rightChild.get());
		ParabolaNode::_cross(left->_rightChild.get(), parabola->_rightChild.get());
		ParabolaNode::_cross(parabola->_rightChild.get(), parabola->_rightSibling);

		// Set edge to right sibling from the original parabola
		parabola->_rightChild->setEdge(parabola->edge());  // the rightmost parabola
		parabola->setEdge(nullptr);

		return left->_rightChild.get();
	}
	else {
		parabola->setLeftChild(make_unique<ParabolaNode>(parabolaSite));
		parabola->setRightChild(make_unique<ParabolaNode>(nullptr));
		auto right = parabola->_rightChild.get();
		right->setRightChild(make_unique<ParabolaNode>(parabolaSite));
		right->setLeftChild(make_unique<ParabolaNode>(site));

		// Set siblings
		ParabolaNode::_cross(parabola->_leftSibling, parabola->_leftChild.get());
		ParabolaNode::_cross(parabola->_leftChild.get(), right->_leftChild.get());
		ParabolaNode::_cross(right->_leftChild.get(), right->_rightChild.get());
		ParabolaNode::_cross(right->_rightChild.get(), parabola->_rightSibling);

		// Set edge to right sibling from the original parabola
		right->_rightChild->setEdge(parabola->edge());  // the rightmost parabola
		parabola->setEdge(nullptr);

		return right->_leftChild.get();
	}
}


void Voronoi::Beachline::removeParabola(ParabolaNode * parabolaNode)
{
	if (!parabolaNode->isLeaf()) {
		printf("DEBUG");
	}

	assert(parabolaNode->isLeaf());

	// disable events
	if (parabolaNode->_leftSibling && parabolaNode->_leftSibling->event()) {
		parabolaNode->_leftSibling->event()->disable();
		parabolaNode->_leftSibling->setEvent(nullptr);
	}
	if (parabolaNode->_rightSibling && parabolaNode->_rightSibling->event()) {
		parabolaNode->_rightSibling->event()->disable();
		parabolaNode->_rightSibling->setEvent(nullptr);
	}
	if (parabolaNode->event()) {
		parabolaNode->event()->disable();
		parabolaNode->setEvent(nullptr);
	}


	// Set siblings
	ParabolaNode::_cross(parabolaNode->_leftSibling, parabolaNode->_rightSibling);

	// Find parent's another child
	ParabolaNode * parent = parabolaNode->_parent;
	std::unique_ptr<ParabolaNode> otherBranch;
	if (parent->_leftChild.get() == parabolaNode) {
		otherBranch = parent->releaseRightChild();
	}
	else if (parent->_rightChild.get() == parabolaNode) {
		otherBranch = parent->releaseLeftChild();
	}
	else {
		throw std::logic_error("Beachline::remove: Invalid parent!");
	}

	// Move parent's another child into parent's parent
	ParabolaNode * grandparent = parent->_parent;
	if (grandparent) {
		if (grandparent->_leftChild.get() == parent) {
			grandparent->setLeftChild(std::move(otherBranch));
		}
		else if (grandparent->_rightChild.get() == parent) {
			grandparent->setRightChild(std::move(otherBranch));
		}
		else {
			throw std::logic_error("Beachline::remove: Invalid parent!");
		}
	}
	else {
		_root = std::move(otherBranch);
	}
}


Voronoi::ParabolaNode * Voronoi::Beachline::findParabola(const Point & point)
{
	if (!_root) {
		return nullptr;
	}

	ParabolaNode * parabola = _root.get();

	// Binary search
	while (!parabola->isLeaf()) {
		// Find the closest leave which is on the left of current node
		const ParabolaNode * left = parabola->leftChild();
		while (!left->isLeaf()) left = left->rightChild();

		// Find the closest leave which is on the right of current node
		const ParabolaNode * right = parabola->rightChild();
		while (!right->isLeaf()) right = right->rightChild();




		// "x" is the intersection of two parabolas
		const double x = parabolaIntersectionX(*left->site(), *right->site(), point.y());
		if (x > point.x()) {
			parabola = parabola->_leftChild.get();
		}
		else {
			parabola = parabola->_rightChild.get();
		}
	}
	return parabola;
}


const Voronoi::ParabolaNode * Voronoi::Beachline::root() const
{
	return _root.get();
}


Voronoi::ParabolaNode * Voronoi::Beachline::root()
{
	return _root.get();
}
