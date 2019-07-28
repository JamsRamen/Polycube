#include "cli.h"

#include <iostream>
#include <chrono>

#include "../util/vector.h"
#include "../util/matrix.h"
#include "../util/lattice.h"
#include "../util/util.h"
#include "../polycube/polycube.h"
#include "../polycube/polycube_list.h"
#include "../polycube/board.h"
#include "../polycube/puzzle.h"

#include "read.h"
#include "parse.h"
#include "format.h"

long long currentTime () {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main (int argc, char** argv) {
	
	auto puzzleData = read::commandLine(argc, argv);
	/* auto puzzle = parse::puzzle(puzzleData);

	std::cout << "\nInitiating search for solutions.\n";
	
	auto startTime = currentTime();
	auto solutions = puzzle.solve();
	auto endTime = currentTime();

	std::cout << "Search completed in " << endTime - startTime << " milliseconds.\n\n";
	std::cout << format::solutions(solutions);
	*/

	Lattice<int, 3> a { { { 1, 2 }, { 1, 1 } }, { { 1, 2 }, { 2, 2 } } };
	Board<3> b (a);
	std::vector<Board<3>> s = { b, b };

	std::cout << format::solutions(s);

	return 0;
}
