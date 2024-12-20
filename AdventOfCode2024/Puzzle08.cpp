// ===========================================================================
// Puzzle08.cpp
// ===========================================================================

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <print>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// ===========================================================================
// global data

static std::string g_filenameTestData{ "Puzzle08_TestData.txt" };
static std::string g_filenameRealData{ "Puzzle08_RealData.txt" };

static size_t  g_size{};

static std::unordered_map<char, std::set<std::pair<int, int>>> g_antennaMap{};
static std::set<std::pair<int, int>> g_antinodesSet{};

// ===========================================================================
// types

// ===========================================================================
// forward declarations
// 
// ===========================================================================
// input & output

static void readPuzzleFromFile(const std::string& filename) {

    std::ifstream file{ filename };

    int row{ - 1};

    if (file.is_open()) {

        // read line per line
        std::string line{};

        while (std::getline(file, line)) {

            if (line.empty()) {
                break;
            }

            ++row;

            std::println("Line: {} !", line);

            if (g_size == 0) {
                g_size = line.length();
            }

            // add contents of this line to antanna map
            for (int col{ -1 };  char ch : line) {

                ++col;

                if (ch == '.') {
                    continue;
                }
                else {

                    // does antenna already exist
                    std::unordered_map<char, std::set<std::pair<int, int>>>::iterator pos{ 
                        g_antennaMap.find(ch)
                    };

                    if (pos == g_antennaMap.end()) {

                        // antenna not found
                        std::set<std::pair<int, int>> s{};
                        s.insert(std::pair<int, int> { row, col });
                        g_antennaMap[ch] = s;
                    }
                    else {

                        // antenna found
                        auto& s{ pos->second };
                        s.insert(std::pair<int, int> { row, col });
                    }
                }
            }
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", filename);
    }
}

// ===========================================================================
// types / logic

static void computeSetOfAntinodes(const std::set<std::pair<int, int>>& s);

static bool isAntinodeValid(const std::pair<int, int>& s);

static void computeSetOfAntinodes() 
{
    for ( auto& entry : g_antennaMap) {

        char key{ entry.first };
        auto& s{ entry.second };

        // compute antinodes of this frequency
        computeSetOfAntinodes(s);
    }
}


static bool isAntinodeValid(const std::pair<int, int>& point) {

    const auto& [row, col] = point;

    if (row < 0 || row >= g_size) {
        return false;
    }
    
    if (col < 0 || col >= g_size) {
        return false;
    }

    return true;
}


static void computeSetOfAntinodes(const std::set<std::pair<int, int>>& s)
{
    if (s.size() == 1) {
        // nothing to compute
        return;
    }

    // antenna pairs
    std::list<std::pair<std::pair<int, int>, std::pair<int, int>>> antennaPairs{};

    for (const auto& point2 : s) {

        for (const auto& point1 : s) {

            if (point1 == point2) {
                continue;
            }

            std::pair<std::pair<int, int>, std::pair<int, int>> tmp;

            if (point1.first <= point2.first) {

                tmp.first = point1;
                tmp.second = point2;
            }
            else {

                tmp.first = point2;
                tmp.second = point1;
            }

            antennaPairs.push_back(tmp);
        }
    }

    // compute antinodes
    for (const auto& elem : antennaPairs) {

        const std::pair<int, int>& point1{ elem.first };
        const std::pair<int, int>& point2{ elem.second };

        int deltaRow{ point2.first - point1.first };
        int deltaCol{ point2.second - point1.second };

        std::pair<int, int> antinode1{ point1.first - deltaRow , point1.second - deltaCol };
        std::pair<int, int> antinode2{ point2.first + deltaRow , point2.second + deltaCol };

        // für beide antinodes checken, ob diese überhaupt noch auf dem Brett liegen ???????????????
        if (isAntinodeValid(antinode1)) {
            g_antinodesSet.insert(antinode1);
        }

        if (isAntinodeValid(antinode2)) {
            g_antinodesSet.insert(antinode2);
        }
    }
}

// ===========================================================================
// part one

static void puzzle_08_part_one()
{
    readPuzzleFromFile(g_filenameRealData);
    computeSetOfAntinodes();
    std::println("Found antinodes: {} !", g_antinodesSet.size());   // expected 367
}

// ===========================================================================
// part two


static void computeSetOfAntinodesEx(const std::set<std::pair<int, int>>& s);

static void computeSetOfAntinodesEx()
{
    for (auto& entry : g_antennaMap) {

        char key{ entry.first };
        auto& s{ entry.second };

        // compute antinodes of this frequency
        computeSetOfAntinodesEx(s);
    }
}

static void computeSetOfAntinodesEx(const std::set<std::pair<int, int>>& s)
{
    if (s.size() == 1) {
        // nothing to compute
        return;
    }

    // antenna pairs
    std::list<std::pair<std::pair<int, int>, std::pair<int, int>>> antennaPairs{};

    for (const auto& point2 : s) {

        for (const auto& point1 : s) {

            if (point1 == point2) {
                continue;
            }

            std::pair<std::pair<int, int>, std::pair<int, int>> tmp;

            if (point1.first <= point2.first) {

                tmp.first = point1;
                tmp.second = point2;
            }
            else {

                tmp.first = point2;
                tmp.second = point1;
            }

            antennaPairs.push_back(tmp);
        }
    }

    // compute antinodes
    for (const auto& elem : antennaPairs) {

        const std::pair<int, int>& point1{ elem.first };
        const std::pair<int, int>& point2{ elem.second };

        // antennas are also antinodes
        g_antinodesSet.insert(point1);
        g_antinodesSet.insert(point2);

        int deltaRow{ point2.first - point1.first };
        int deltaCol{ point2.second - point1.second };

        // create all antinodes in line above
        std::pair<int, int> antinode1{ point1.first - deltaRow , point1.second - deltaCol };
        while (isAntinodeValid(antinode1)) {

            g_antinodesSet.insert(antinode1);

            // create next antinode in line
            antinode1 = std::pair<int, int> { antinode1.first - deltaRow , antinode1.second - deltaCol };
        }

        // create all antinodes in line below
        std::pair<int, int> antinode2{ point2.first + deltaRow , point2.second + deltaCol };
        while (isAntinodeValid(antinode2)) {

            g_antinodesSet.insert(antinode2);

            // create next antinode in line
            antinode2 = std::pair<int, int>{ antinode2.first + deltaRow , antinode2.second + deltaCol };
        }
    }
}

static void puzzle_08_part_two()
{
    readPuzzleFromFile(g_filenameRealData);
    computeSetOfAntinodesEx();
    std::println("Found antinodes: {} !", g_antinodesSet.size());   // expected 367
}

// ===========================================================================
// main

void puzzle_08()
{
    //puzzle_08_part_one();
    puzzle_08_part_two();     // expected 1285
}

// ===========================================================================
// End-of-File
// ===========================================================================
