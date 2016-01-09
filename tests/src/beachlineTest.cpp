#include "tests.h"

using namespace Voronoi;


SUITE(BeachlineTest)
{
	TEST(Parabola_NoSite_IsInvalid)
	{
		Parabola parabola;
		CHECK(!parabola.isValid());
	}


	TEST(Parabola_SiteConstructor_IsValid)
	{
		Parabola parabola(Point(0,0));
		CHECK(parabola.isValid());
	}


	TEST(Parabola_SiteInvalid_IsInvalid)
	{
		Point p;
		Parabola parabola(p);
		CHECK(!parabola.isValid());
	}


	TEST(Parabola_SiteSet_IsValid)
	{
		Parabola parabola;
		parabola.setSite(Point(0, 0));
		CHECK(parabola.isValid());
	}


	TEST(ParabolaNode_Empty_NoParrent)
	{
		ParabolaNode root;
		CHECK(!root.parent());
	}


	TEST(ParabolaNode_EmplaceParabola_ReturnsRoot)
	{
		ParabolaNode root;
		auto newParabola = root.emplaceParabola(Point(0, 0));
		CHECK(!root.parent());
		CHECK(newParabola = &root);
	}


	TEST(ParabolaNode_Emplace2Parabolas_NewParabolaIsPresent)
	{
		ParabolaNode root;
		auto first = root.emplaceParabola(Point(1, 1));
		auto second = root.emplaceParabola(Point(0, 0));
		CHECK(first == second->parent());
		//CHECK(&root == first->parent());
		//CHECK(&root == second->parent());
	}

}
