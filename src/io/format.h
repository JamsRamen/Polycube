#pragma once

#include <string>
#include <vector>

#include "board.h"

namespace format {
    std::string countable (int, std::string, std::string);
    std::string countable (int, std::string);
    std::string cube (int, bool);
    std::string solutions (const std::vector<Board<3>>&, bool, int, int);
    std::string board (const Board<3>&, bool);
}