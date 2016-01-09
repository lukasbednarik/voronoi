#include "tests.h"
#include <iostream>


namespace
{
	/// Helper function for `printEdges`
	std::string printPoint(const Voronoi::Point & point)
	{
		if (point.isNull()) {
			return "(-, -)";
		}
		else {
			std::ostringstream str;
			str << "(" << point.x() << ", " << point.y() << ")";
			return str.str();
		}
	}
}


void printEdges(const std::list<Voronoi::Edge> & edges, char test)
{
	std::cout << "\n --Test " << test << "--" << std::endl;
	for (const auto & edge : edges) {
		const auto begin = edge.begin();
		const auto end = edge.end();
		const auto left = edge.left();
		const auto right = edge.right();
		std::ostringstream str;
		str << "beg" << printPoint(edge.begin()) << " -> " << printPoint(edge.end());
		for (std::streamoff i = str.tellp(); i < 37; ++i) str << " ";
		str << "[left" << printPoint(edge.left()) << "; right" << printPoint(edge.right()) << "]\n";
		std::cout << str.str();
	}
}

