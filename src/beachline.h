// The MIT License (MIT)
//
// Copyright (c) 2015 Lukáš Bednařík
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef PARABOLATREE_H
#define PARABOLATREE_H

#include "point.h"
#include <memory>


namespace Voronoi
{
	class VertexEvent;
	class Edge;


	/// One parabola node in a tree
	class ParabolaNode
	{
	public:
		/// Default constructor
		ParabolaNode();

		/// Constructor
		ParabolaNode(const Point & site);

		/// Return true if this is a leaf.
		bool isLeaf() const;

		/// The parabola focus. Value is valid only for leafs.
		Point site() const;

		/// Return event for this node
		VertexEvent * event();

		/// Set an event for this node
		void setEvent(VertexEvent * event);

		void setEdge(Edge * edge);
		void setEdgeBegin(const Point & begin);
		void setEdgeEnd(const Point & end);
		const Edge * edge() const { return _edge; }  // TODO This is for debug only.

		ParabolaNode * leftSibling();
		ParabolaNode * rightSibling();
		const ParabolaNode * leftSibling() const;
		const ParabolaNode * rightSibling() const;
		const ParabolaNode * leftChild() const;
		const ParabolaNode * rightChild() const;  // FOR DEBUG. WE WANT better implementation!
		ParabolaNode * leftChild();
		ParabolaNode * rightChild();  // FOR DEBUG. WE WANT better implementation!

	private:
		friend class Beachline;

		Point _site;
		ParabolaNode * _parent;
		ParabolaNode * _leftSibling;
		ParabolaNode * _rightSibling;
		std::unique_ptr<ParabolaNode> _leftChild;
		std::unique_ptr<ParabolaNode> _rightChild;
		VertexEvent * _event;
		Edge * _edge;

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
		ParabolaNode * emplaceParabola(const Point & site);

		/// Remove this parabola (child) from the beachline
		void removeParabola(ParabolaNode * parabolaNode);

		/// Return a parabola under the given point [x, sweepline_y].
		ParabolaNode * findParabola(const Point & point);

		/// Get root
		const ParabolaNode * root() const;
		ParabolaNode * root();

	private:
		std::unique_ptr<ParabolaNode> _root;
	};
}


#endif  // PARABOLATREE_H
