#include "parse.h"

#include <sstream>
#include <vector>
#include <regex>

#include "read.h"
#include "exceptions.h"

namespace parse {
    std::vector<std::string> split (const std::string& data, char split) {
        if (data == "") return std::vector<std::string>();

        std::vector<std::string> res { "" };

        for (char c : data) {
            if (c == split) res.push_back("");
            else res.back() += c;
        }

        return res;
    }

    std::vector<std::string> split (const std::string& data) {
        std::vector<std::string> res;
        std::string line;
        std::stringstream stream (data);

        while (getline(stream, line)) {
            res.push_back(line);
        }

        return res;
    }

    Puzzle<3> puzzle (const std::string& data) {
        auto dataSections = split(data, '#');

        if (dataSections.size() < 4) {
            throw err::PuzzleFormat("too few sections");
        }

        std::unordered_set<std::string> importedFiles;
        auto polycubeMap = import(dataSections[0], importedFiles);
        
        auto segment = polycubes(dataSections[1]);
        polycubeMap.insert(segment.begin(), segment.end());

        auto puzzleBoard = board(dataSections[2]);
        auto puzzleCubes = polycubeList(dataSections[3], polycubeMap);

        return Puzzle<3>(puzzleBoard, puzzleCubes);
    }

    std::unordered_map<std::string, Polycube<3>> import (const std::string& data, std::unordered_set<std::string>& visited) {
        std::unordered_map<std::string, Polycube<3>> res;
        auto files = split(data);

        for (const auto& file : files) {
            if (file == "" || visited.count(file)) continue;
            visited.insert(file);

            auto dataSections = split(read::file(file), '#');

            if (dataSections.size() >= 1) {
                auto segment = import(dataSections[0], visited);
                res.insert(segment.begin(), segment.end());
            }

            if (dataSections.size() >= 2) {
                auto segment = polycubes(dataSections[1]);
                res.insert(segment.begin(), segment.end());
            }
        }

        return res;
    }

    std::unordered_map<std::string, Polycube<3>> polycubes (const std::string& data) {
        std::unordered_map<std::string, Polycube<3>> res;
        auto lines = split(data);
        std::string name, cubeData, nil;

        for (const auto& line : lines) {
            std::stringstream stream (line);

            if (name == "") {
                stream >> name;

                if (!std::regex_match(name, std::regex("[A-Za-z0-9]*"))) {
                    throw err::PolycubeName(name);
                }

                for (char& c : name) c = std::tolower(c);
            }
            else if (stream >> nil) { // if the line is not empty
                cubeData += line + "\n";
            }
            else {
                res[name] = polycube(cubeData);

                name = cubeData = "";
            }
        }

        if (name != "") {
            res[name] = polycube(cubeData);
        }

        return res;
    }
    

    Polycube<3> polycube (const std::string& data) {
        std::vector<Lattice<bool, 2>> cube;

        auto lines = split(data);

        for (const auto& line : lines) {
            std::vector<Lattice<bool, 1>> plane;
            std::stringstream stream (line);
            std::string word;

            while (stream >> word) {
                std::vector<Lattice<bool, 0>> row;

                for (char c : word) {
                    row.push_back(c != '.');
                }

                plane.push_back(row);
            }

            cube.push_back(plane);
        }

        return Polycube<3>(Lattice<bool, 3>(cube));
    }
    
    Board<3> board (const std::string& data) {
        std::stringstream stream (data);

        int x, y, z, sturdyLayers;
        stream >> z >> x >> y;

        if (!(stream >> sturdyLayers)) {
            sturdyLayers = y;
        }

        return Board<3>(Lattice<int, 3>(Vector<3>{ x, y, z }, 0), sturdyLayers);
    }
    
    std::vector<Polycube<3>> polycubeList (const std::string& data, const std::unordered_map<std::string, Polycube<3>>& polycubeMap) {
        std::vector<Polycube<3>> res;
        std::stringstream stream (data);
        std::string name, lowerName;

        while (stream >> name) {
            lowerName = name;
            for (char& c : lowerName) c = std::tolower(c);
            
            if (polycubeMap.count(lowerName) == 0) {
                throw err::PolycubeNotFound(name);
            }

            res.push_back(polycubeMap.at(lowerName));
        }

        return res;
    }
}