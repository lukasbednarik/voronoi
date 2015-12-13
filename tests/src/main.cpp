#include <vector>
#include <string>
#include "tests.h"
#include "TestReporterStdout.h"


/// @mainpage
///
/// @section id1 Run selected test
///
/// Run selected test: "VoronoiTest.exe Test1 Test2".
/// Run selected suite: "VoronoiTest.exe suite Suite1 Suite2".
///
/// Automatic start of unit tests after compiling can be turned on / off in
/// "Properties/Build Events/Post-build event/Use in build"


int main(int argc, char * argv[])
{
	if (argc > 1) {
		std::vector<std::string> arguments;
		arguments.assign(argv + 1, argv + argc);

		// If the first argument is "suite" we search the list of all suites.
		const bool isSuite = (arguments[0] == "suite");
		const UnitTest::TestList & allTests(UnitTest::Test::GetTestList());
		UnitTest::TestList selectedTests;
		UnitTest::Test * test = allTests.GetHead();
		while (test) {
			for (size_t i = 0; i < arguments.size(); ++i) {
				if (isSuite && arguments[i] == test->m_details.suiteName) {
					selectedTests.Add(test);
				}
				else if (!isSuite && arguments[i] == test->m_details.testName) {
					selectedTests.Add(test);
				}
			}
			test = test->m_nextTest;
		}

		// Run only selected tests
		UnitTest::TestReporterStdout reporter;
		UnitTest::TestRunner runner(reporter);
		selectedTests.Add(0); // This line is bug fix for old UnitTest++.
		return runner.RunTestsIf(selectedTests, 0, UnitTest::True(), 0);
	}
	else {
		return UnitTest::RunAllTests();
	}
}
