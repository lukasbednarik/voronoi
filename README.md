# Warning!

This implementation doesn't work in some corner cases and it's not completely finished! It's very difficult to get it right and there's a reason why there are so few implementations of Steven Fortune's agorithm in C++ on the internet.

If you fix it, send me a message.

# Voronoi diagram generator

*Voronoi diagram generator with Steven Fortune's sweep line algorithm in C++11.*

The goal of this project is to provide elegant implementation of Steven Fortune's algorithm in modern C++11. The implementation is short, runs in `n log n` and uses a wide variety of C++11 and STL features. In time of this writting (2015) there are only a few implementations of this algorithm across the internet and all of them are written in "ugly" C-style C++. Moreover most of those implementations origin from the original Fortune's implementation in C.

Steven Fortune's algorithm is elegant, but the implementation is very tricky as it requires to cover all corner cases which are hard to spot.

## Project files

Unit test projects for VS 2015 and cmake are in "test" directory.

- Visual Studio 2015: test and project file.
- cmake: test and project CMakeList.txt.
- Qt project: project file file in src directory.

## Performance test

TODO Add proof the implementation runs in `n log n`.

TODO The performance is not a priority, but the test will be done as soon as any working version will be ready.

## Fortune's sweep line algorithm

The idea of all sweep algorithms is to discover all "upcoming" events in an efficient manner. The problem with Voronoi diagram is it's hard to predict when another event will occur. When sweep line's moving downwards "unanticipated events" already form new vertices of Voronoi diagram.

TODO: image


