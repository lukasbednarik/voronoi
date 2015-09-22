#ifndef PARABOLATREE_H
#define PARABOLATREE_H

#include "point.h"
#include <memory>


namespace Voronoi
{
	class Event;


	/// One parabola node in a tree
	class ParabolaNode
	{
	public:
		/// Constructor
		ParabolaNode(const Point * site);

		/// Return true if this is a leaf.
		bool isLeaf() const;

		/// The parabola focus. Value is valid only for leafs.
		const Point * site() const;

		/// Return event for this node
		Event * event();

		/// Set an event for this node
		void setEvent(Event * event);

		ParabolaNode * leftSibling();
		ParabolaNode * rightSibling();

	private:
		friend class Beachline;

		const Point * _site;
		ParabolaNode * _parent;
		ParabolaNode * _leftSibling;
		ParabolaNode * _rightSibling;
		std::unique_ptr<ParabolaNode> _leftChild;
		std::unique_ptr<ParabolaNode> _rightChild;
		Event * _event;

		// Helper functions
		void setLeftChild(std::unique_ptr<ParabolaNode> parabolaTree);
		void setRightChild(std::unique_ptr<ParabolaNode> parabolaTree);
		std::unique_ptr<ParabolaNode> releaseLeftChild();
		std::unique_ptr<ParabolaNode> releaseRightChild();
		static void _cross(ParabolaNode * left, ParabolaNode * right);
	};


	/// Beachline creates the "beachline" (or also "borderline").
	class Beachline
	{
	public:
		/// Construct a parabola (child) on beachline
		///
		/// @return New created parabola.
		ParabolaNode * emplaceParabola(const Point * site);

		/// Remove this parabola (child) from the beachline
		void removeParabola(ParabolaNode * parabolaNode);

		/// Return a parabola under the given point [x, sweepline_y].
		ParabolaNode * findParabola(const Point & point);

	private:
		std::unique_ptr<ParabolaNode> _root;
	};
}


#endif  // PARABOLATREE_H
