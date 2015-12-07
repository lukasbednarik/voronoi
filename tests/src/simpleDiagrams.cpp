#include "tests.h"


TEST(A_OneLine)
{
	std::vector<Voronoi::Point> sites;
	sites.emplace_back(0.2, 0.7);
	sites.emplace_back(0.4, 0.1);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	printEdges(edges, 'A');


	//const Voronoi::Point e_1(0.0, 0.3);
	//const Voronoi::Point b_1(1.0, 0.633333);
	//const size_t c_1 = 1;
}


TEST(B_TwoLines_Nonintersecting)
{
	std::vector<Voronoi::Point> sites;
	sites.emplace_back(0.2, 0.7);
	sites.emplace_back(0.3, 0.6);
	sites.emplace_back(0.4, 0.1);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	printEdges(edges, 'B');

	//const Voronoi::Point b_2(0.0, 0.28);
	//const Voronoi::Point e_2(1.0, 0.48);
	//const size_t c_2 = 2;
}


TEST(C_ThreeLines_OneVertexEvent)
{
	std::vector<Voronoi::Point> sites;
	sites.emplace_back(0.2, 0.7);
	sites.emplace_back(0.2, 0.2);
	sites.emplace_back(0.4, 0.1);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	printEdges(edges, 'C');

	//const Voronoi::Point b_3(0.225, 0);
	//const Voronoi::Point e_3(0.45, 0.45);
	//const size_t c_3 = 3;
}


// Test H -- same as C but mirrored on Y = 0.5
TEST(H_ThreeLines_OneVertexEvent)
{
	std::vector<Voronoi::Point> sites;
	sites.emplace_back(0.2, 0.3);
	sites.emplace_back(0.2, 0.8);
	sites.emplace_back(0.4, 0.9);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	printEdges(edges, 'H');
}


// Test I -- same as C but mirrored on X = 0.5
TEST(I_ThreeLines_OneVertexEvent)
{
	std::vector<Voronoi::Point> sites;
	sites.emplace_back(0.8, 0.7);
	sites.emplace_back(0.8, 0.2);
	sites.emplace_back(0.6, 0.1);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	printEdges(edges, 'I');
}


TEST(E_Test)
{
	std::vector<Voronoi::Point> sites;
	sites.emplace_back(0.2, 0.7);
	sites.emplace_back(0.2, 0.2);
	sites.emplace_back(0.4, 0.1);
	sites.emplace_back(0.9, 0.7);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	printEdges(edges, 'E');
}


TEST(G_Test)
{
	std::vector<Voronoi::Point> sites;
	sites.emplace_back(0.8, 0.7);
	sites.emplace_back(0.8, 0.2);
	sites.emplace_back(0.6, 0.1);
	sites.emplace_back(0.1, 0.7);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	printEdges(edges, 'G');
}


TEST(F_Test)
{
	std::vector<Voronoi::Point> sites;
	sites.emplace_back(0.2, 0.7);
	sites.emplace_back(0.9, 0.2);
	sites.emplace_back(0.6, 0.1);
	sites.emplace_back(0.9, 0.7);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	printEdges(edges, 'F');
}
