#include "test1.h"


QTEST_APPLESS_MAIN(VoronoiTest)



// To run multiple test classes
//int main(int argc, char** argv)
//{
//   int status = 0;
//   auto ASSERT_TEST = [&status, argc, argv](QObject* obj) {
//	 status |= QTest::qExec(obj, argc, argv);
//	 delete obj;
//   };
//   ASSERT_TEST(new TestQString());
//   //ASSERT_TEST(new Test2());
//   return status;
//}



/**********************************************************************/
// TODO PIMPL idiom
// POZN Jiny algoritmus: Delaunay triangulation - flip edge algorithm.
// POZN Jiny algoritmus: Half plane intersection (n*n*log(n)), sweepline m� n*log(n).
// TODO Beachline::emplaceParabola testovat POKUD JE VICE PARABOL NA STEJNEM Y NA ZACATKU.
/**********************************************************************/



