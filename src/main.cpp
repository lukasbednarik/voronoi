#include "voronoi.h"
#include <iostream>

// TODO PIMPL idiom
// POZN Jiny algoritmus: Delaunay triangulation - flip edge algorithm.
// POZN Jiny algoritmus: Half plane intersection (n*n*log(n)), sweepline má n*log(n).
// TODO Beachline::emplaceParabola OSETRIT POKUD JE VICE PARABOL NA STEJNEM Y NA ZACATKU.

// Cmake a c++11? Cmake na windows? Executable or lib? Unit tests with qt or VS unittest++?
// --- Udelal bych cmake a VS. Unit test na Windows only.
// Jak se pro *.pro nastavi default src directory, aby tam nebyly to složky?
// Je unittest++ i pro cmake?


int main (int, char *[])
{
	std::vector<Voronoi::Point> sites;  // center points on [0, 1] interval
	sites.emplace_back(0.1, 0.9);
	sites.emplace_back(0.2, 0.7);
	sites.emplace_back(0.3, 0.6);
	sites.emplace_back(0.8, 0.4);
	sites.emplace_back(0.4, 0.1);
	sites.emplace_back(0.9, 0.5);
	sites.emplace_back(0.4, 0.7);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();

	std::cout << "Edge count: " << edges.size() << std::endl;

	for (const auto & edge : edges) {
		auto begin = edge.begin();
		auto end = edge.end();

		if (begin) {
			std::cout << "beg(" << begin->x() << ", " << begin->y() << ")";
		}
		if (begin && end) {
			std::cout << " -> ";
		}
		else {
			std::cout << std::endl;
		}
		if (end) {
			std::cout << "end(" << end->x() << ", " << end->y() << ")\n";
		}
	}


	/*
	// TESTS
	Voronoi::Beachline beachline;
	Voronoi::Point x(3.0, 3.0);
	Voronoi::Point y(2.0, 2.0);
	Voronoi::Point z(1.0, 1.8);

	beachline.emplaceParabola(&x);
	beachline.emplaceParabola(&y);
	beachline.emplaceParabola(&z);
	auto found = beachline.findParabola(Voronoi::Point(7.0, 0.5));
	*/


	system("pause");  // Works on windows only
	return 0;
}

