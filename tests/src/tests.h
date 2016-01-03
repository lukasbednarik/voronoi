#ifndef TESTS_H
#define TESTS_H

#include "Voronoi.h"
#include "UnitTest++.h"
#include <iostream>


/// Simple debug function to print all edges for the user
inline void printEdges(const std::list<Voronoi::Edge> & edges, char test)
{
	std::cout << "   Test " << test << std::endl;
	for (const auto & edge : edges) {
		auto begin = edge.begin();
		auto end = edge.end();
		std::cout << "beg(" << begin.x() << ", " << begin.y() << ")";
		std::cout << " -> ";
		std::cout << "end(" << end.x() << ", " << end.y() << ")";
		auto left = edge.left();
		auto right = edge.right();
		std::cout << "  |  left(" << left.x() << ", " << left.y() << ")";
		std::cout << " -> ";
		std::cout << "right(" << right.x() << ", " << right.y() << ")\n";
	}
}


#endif  // TESTS_H
