#include "beachline.h"
#include "geometry.h"
#include "make_unique.h"
#include <string>


namespace
{
	/// Disable all event's containing parabola's site
	void disableEvents(Voronoi::ParabolaNode * parabola)
	{
		if (parabola->leftSibling()) {
			parabola->leftSibling()->setEvent(nullptr);
		}
		if (parabola->rightSibling()) {
			parabola->rightSibling()->setEvent(nullptr);
		}
		parabola->setEvent(nullptr);
	}
}


Voronoi::ParabolaNode::ParabolaNode() :
	_parent(nullptr),
	_leftSibling(nullptr),
	_rightSibling(nullptr)
{
}


Voronoi::ParabolaNode::ParabolaNode(const Point & site) :
	Parabola(site),
	_parent(nullptr),
	_leftSibling(nullptr),
	_rightSibling(nullptr)
{
}


void Voronoi::ParabolaNode::_createChildren(const Point & leftSite, const Point & rightSite)
{
	_leftChild = make_unique<ParabolaNode>(leftSite);
	_leftChild->_parent = this;
	_rightChild = make_unique<ParabolaNode>(rightSite);
	_rightChild->_parent = this;
	_cross(_leftSibling, leftChild());
	_cross(leftChild(), rightChild());
	_cross(rightChild(), _rightSibling);
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


std::unique_ptr<Voronoi::ParabolaNode> Voronoi::ParabolaNode::_releaseTwin(Voronoi::ParabolaNode * parabola)
{
	Voronoi::ParabolaNode * parent = parabola->parent();
	std::unique_ptr<Voronoi::ParabolaNode> otherBranch;
	if (parent->leftChild() == parabola) {
		otherBranch = std::move(parent->_rightChild);
		parent->_rightChild.reset();
	}
	else if (parent->rightChild() == parabola) {
		otherBranch = std::move(parent->_leftChild);
		parent->_leftChild.reset();
	}
	else {
		throw std::logic_error("_releaseTwin: Invalid parent!");
	}
	otherBranch->_parent = nullptr;
	return otherBranch;
}


void Voronoi::ParabolaNode::_move(std::unique_ptr<ParabolaNode> parabola)
{
	_leftSibling = parabola->_leftSibling;
	_rightSibling = parabola->_rightSibling;
	_leftChild = std::move(parabola->_leftChild);
	_rightChild = std::move(parabola->_rightChild);
	setSite(parabola->site());
	setEdge(parabola->edge());
	setEvent(parabola->event());
}


void Voronoi::ParabolaNode::_setParentsTwin(ParabolaNode * parent, std::unique_ptr<ParabolaNode> parabola)
{
	auto grandparent = parent->parent();
	parabola->_parent = grandparent;
	if (grandparent->leftChild() == parent) {
		grandparent->_leftChild = std::move(parabola);
	}
	else if (grandparent->rightChild() == parent) {
		grandparent->_rightChild = std::move(parabola);
	}
	else {
		throw std::logic_error("_setParentsTwin: Invalid parent!");
	}
}


Voronoi::ParabolaNode * Voronoi::ParabolaNode::emplaceParabola(const Point & site)
{
	if (!isValid()) {
		setSite(site);
		return this;
	}

	ParabolaNode * parabola = findParabola(site);
	assert(parabola->site().y() >= site.y());

	// disable event including parabola->sites in the middle of a triple.
	parabola->setEvent(nullptr);

	// Handle special case when: one_parabola_in_root && parabolaSite.y() == site.y()
	const Point parabolaSite = parabola->site();
	if (isLeaf(this) && parabolaSite.y() == site.y()) {
		if (site.x() < parabolaSite.x()) {
			parabola->_createChildren(site, parabolaSite);
			return parabola->leftChild();
		}
		else {
			parabola->_createChildren(parabolaSite, site);
			return parabola->rightChild();
		}
	}
	else if (site.x() < parabolaSite.x()) {
		// Create new parabola branch
		parabola->_createChildren(Point(), parabolaSite);
		parabola->leftChild()->_createChildren(parabolaSite, site);

		// Set edge to right sibling from the original parabola
		parabola->_rightChild->setEdge(parabola->edge());  // the rightmost parabola
		parabola->setEdge(nullptr);

		return parabola->leftChild()->rightChild();
	}
	else {
		// Create new parabola branch
		parabola->_createChildren(parabolaSite, Point());
		parabola->rightChild()->_createChildren(site, parabolaSite);

		// Set edge to right sibling from the original parabola
		parabola->rightChild()->_rightChild->setEdge(parabola->edge());  // the rightmost parabola
		parabola->setEdge(nullptr);

		return parabola->rightChild()->leftChild();
	}
}


void Voronoi::ParabolaNode::removeParabola(ParabolaNode * parabola)
{
	assert(isLeaf(parabola));

	// Check if parabola is root
	if (parabola == this) {
		setInvalid();
	}

	// Any event involving parabolaNode->site() should be deleted.
	disableEvents(parabola);

	// Set siblings
	ParabolaNode::_cross(parabola->_leftSibling, parabola->_rightSibling);

	// Release parent's another child
	auto otherBranch = _releaseTwin(parabola);

	// Move parent's another child into parent's parent
	if (parabola->parent()->parent()) {
		_setParentsTwin(parabola->parent(), std::move(otherBranch));
	}
	else {
		/// Make "otherBranch" to be "this"
		_move(std::move(otherBranch));
	}
}


Voronoi::ParabolaNode * Voronoi::ParabolaNode::findParabola(const Point & point)
{
	ParabolaNode * par = this;

	// Binary search
	while (!isLeaf(par)) {
		// Find the closest leave which is on the left of current node
		const ParabolaNode * left = par->leftChild();
		while (!isLeaf(left)) {
			left = left->rightChild();
		}

		// Find the closest leave which is on the right of current node
		const ParabolaNode * right = par->rightChild();
		while (!isLeaf(right)) {
			right = right->leftChild();
		}

		// "x" is the intersection of two parabolas
		const double x = parabolaIntersectionX(left->site(), right->site(), point.y());
		if (x > point.x()) {
			par = par->_leftChild.get();
		}
		else {
			par = par->_rightChild.get();
		}
	}
	return par;
}

