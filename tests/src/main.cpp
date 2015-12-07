#include <vector>
#include <string>
#include "tests.h"
#include "TestReporterStdout.h"


/// @mainpage
///
/// @section id1 Spuštìní vybraného testu
///
/// Spuštìní vybraného testu: "area_test.exe Test1 Test2" spustí vybrané testy,
/// "area_test.exe suite Suite1 Suite2" spustí vybrané suite.
///
/// V project "Properties/Build Events/Post-build event/Use in build"
/// lze zapnout nebo vypnout spuštìní testu po kompilaci.


int main(int argc, char * argv[])
{
	if (argc > 1) {
		std::vector<std::string> arguments;
		arguments.assign(argv + 1, argv + argc);

		// Pokud je první argument "suite", hledáme v seznamu všech suite
		const bool isSuite = (arguments[0] == "suite");

		// Projdi seznam testù a pøidej ty, jejich jména se shodují
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

		// Spus jen vybrané testy
		UnitTest::TestReporterStdout reporter;
		UnitTest::TestRunner runner(reporter);
		selectedTests.Add(0); // Pravdìpodobnì bug. Nutno pøidat, aby se nespustily všechny další testy.
		return runner.RunTestsIf(selectedTests, 0, UnitTest::True(), 0);
	}
	else {
		// Spus všechny testy
		return UnitTest::RunAllTests();
	}
}
