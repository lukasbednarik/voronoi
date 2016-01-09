// The MIT License (MIT)
//
// Copyright (c) 2015 Lukáš Bednařík l.bednarik@gmail.com
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
#include "event.h"
#include "edge.h"
#include <memory>
#include <cassert>


namespace Voronoi
{
	class Parabola
	{
	public:
		/// Constructor
		Parabola();

		/// Convenience constructor
		Parabola(const Point & site);

		/// Destructor
		virtual ~Parabola() {}

		/// Return true if this Parabola is invalid (no site set)
		bool isValid() const;

		/// Set this parabola to be empty (invalid)
		void setInvalid();

		/// The parabola focus. Value is valid only for leafs.
		Point site() const;

		void setSite(const Point & site);

		/// Return edge
		Edge * edge();

		/// Set edge
		void setEdge(Edge * edge);

		/// Return event for this parabola
		VertexEvent * event();

		/// Set an event for this parabola
		void setEvent(VertexEvent * event);

	private:
		Point _site;
		Edge * _edge;
		VertexEvent * _event;
	};


	/// One parabola node in a tree
	///
	/// We suppose ParabolaNode having both children or none!
	class ParabolaNode : public Parabola
	{
	public:
		/// Constructor
		ParabolaNode();

		/// Convenience constructor
		ParabolaNode(const Point & site);

		/// Destructor
		~ParabolaNode() override {}

		/// Construct a parabola child
		///
		/// @return New created parabola.
		ParabolaNode * emplaceParabola(const Point & site);

		/// Remove this parabola (child) from the beachline
		void removeParabola(ParabolaNode * parabolaNode);

		/// Return a parabola under the given point [x, sweepline_y].
		ParabolaNode * findParabola(const Point & point);
		
		/// Family functions
		ParabolaNode * parent();
		const ParabolaNode * parent() const;
		ParabolaNode * leftSibling();
		ParabolaNode * rightSibling();
		const ParabolaNode * leftSibling() const;
		const ParabolaNode * rightSibling() const;
		ParabolaNode * leftChild();
		ParabolaNode * rightChild();
		const ParabolaNode * leftChild() const;
		const ParabolaNode * rightChild() const;

	private:
		ParabolaNode * _parent;
		ParabolaNode * _leftSibling;
		ParabolaNode * _rightSibling;
		std::unique_ptr<ParabolaNode> _leftChild;
		std::unique_ptr<ParabolaNode> _rightChild;

		// Helper functions
		void _createChildren(const Point & leftSite, const Point & rightSite);
		void _move(std::unique_ptr<ParabolaNode> parabola);  ///< Copy everything from parabola to this
		static std::unique_ptr<ParabolaNode> _releaseTwin(ParabolaNode * parabola);  ///< Find parent's another child and release it
		static void _setParentsTwin(ParabolaNode * parent, std::unique_ptr<ParabolaNode> parabola);
		static void _cross(ParabolaNode * left, ParabolaNode * right);
	};


	/// Convenience function. Return true if ParabolaNode is a leaf.
	///
	/// We suppose ParabolaNode having both children or none!
	bool isLeaf(const ParabolaNode * parabolaNode);
}



// Implementation

inline Voronoi::Parabola::Parabola() :
	_edge(nullptr),
	_event(nullptr)
{
}


inline Voronoi::Parabola::Parabola(const Point & site) :
	_site(site),
	_edge(nullptr),
	_event(nullptr)
{
}


inline void Voronoi::Parabola::setInvalid()
{
	_site = Point();
}


inline bool Voronoi::Parabola::isValid() const
{
	return !_site.isNull();
}


inline Voronoi::Point Voronoi::Parabola::site() const
{
	return _site;
}


inline void Voronoi::Parabola::setSite(const Point & site)
{
	_site = site;
}


inline Voronoi::Edge * Voronoi::Parabola::edge()
{
	return _edge;
}


inline void Voronoi::Parabola::setEdge(Edge * edge)
{
	_edge = edge;
}


inline Voronoi::VertexEvent * Voronoi::Parabola::event()
{
	return _event;
}


inline void Voronoi::Parabola::setEvent(VertexEvent * event)
{
	if (_event) {
		_event->disable();
	}
	_event = event;
}


inline Voronoi::ParabolaNode * Voronoi::ParabolaNode::parent()
{
	return _parent;
}


inline const Voronoi::ParabolaNode * Voronoi::ParabolaNode::parent() const
{
	return _parent;
}


inline Voronoi::ParabolaNode * Voronoi::ParabolaNode::leftSibling()
{
	assert(isLeaf(this));
	return _leftSibling;
}


inline Voronoi::ParabolaNode * Voronoi::ParabolaNode::rightSibling()
{
	assert(isLeaf(this));
	return _rightSibling;
}


inline const Voronoi::ParabolaNode * Voronoi::ParabolaNode::leftSibling() const
{
	assert(isLeaf(this));
	return _leftSibling;
}


inline const Voronoi::ParabolaNode * Voronoi::ParabolaNode::rightSibling() const
{
	assert(isLeaf(this));
	return _rightSibling;
}


inline const Voronoi::ParabolaNode * Voronoi::ParabolaNode::leftChild() const
{
	return _leftChild.get();
}


inline const Voronoi::ParabolaNode * Voronoi::ParabolaNode::rightChild() const
{
	return _rightChild.get();
}


inline Voronoi::ParabolaNode * Voronoi::ParabolaNode::leftChild()
{
	return _leftChild.get();
}


inline Voronoi::ParabolaNode * Voronoi::ParabolaNode::rightChild()
{
	return _rightChild.get();
}


inline bool Voronoi::isLeaf(const ParabolaNode * parabolaNode)
{
	return !parabolaNode->leftChild() && !parabolaNode->rightChild();
}


#endif  // PARABOLATREE_H
