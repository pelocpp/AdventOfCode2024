
// ===========================================================================
// Puzzle10.cpp
// ===========================================================================

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ===========================================================================
// global data

static std::string_view g_filenameTestData{ "Puzzle10_TestData.txt" };
static std::string_view g_filenameRealData{ "Puzzle10_RealData.txt" };

// ===========================================================================
// types

template <size_t Size> 
class TopographicMap
{
private:

    using Solution = std::list<std::pair<int,int>>;
    using ListSolutions = std::list<Solution>;

    std::array<std::array<int, Size>, Size> m_map;            // topographic map
    Solution                                m_currentTrail;   // trail being in construction
    ListSolutions                           m_trails;         // list of found trail

public:
    TopographicMap() : m_map{} {}

    void readPuzzleFromFile(const std::string_view filename) {
    
        std::ifstream file{ filename.data()};

        if (file.is_open()) {

            // read line per line
            std::string line{};

            int row{ -1 };

            while (std::getline(file, line)) {

                if (line.empty()) {
                    break;
                }

                assert(line.size() == Size);
                // std::println("Line: {}", line);

                ++row;

                // copy line into map
                for (int col{}; char ch : line) {

                    m_map[row][col] = ch - '0';
                    ++col;
                }
            }

            file.close();
        }
        else {

            std::println("Unable to open file {} !", filename);
        }
    }

    void printMap()
    {
        for (const auto& row : m_map) {
            for (const auto height : row) {
                std::print("{}", height);
            }
            std::println();
        }
    }

    void searchTrail(std::pair<int, int> coord) {

        searchTrailHelper(coord, 0, 1);

    }

private:
    void searchTrailHelper(std::pair<int, int> coord, size_t height, size_t length) {

        m_currentTrail.push_back(coord);

        if (length == 10) {

            // add found solution to the list of all solutions
            m_trails.push_back(m_currentTrail);
        }
        else {

            // determine list of possible next moves
            std::vector<std::pair<int, int>> steps{ nextSteps(coord, height+1) };

            // do next moves sequential
            for (const std::pair<int, int>& step : steps) {
                searchTrailHelper(step, height + 1, length + 1);
            }

        }

        m_currentTrail.pop_back();
    }

    std::vector<std::pair<int, int>> nextSteps(std::pair<int, int> coord, size_t height) {

        std::vector<std::pair<int, int>> steps{};

        auto [row, col] = coord;

        // check upper step
        if (row != 0) {
            if (m_map[row - 1][col] == height + 1) {
                steps.push_back({ row - 1 , col});
            }
        }

        // check step to the right
        if (col != Size-1) {
            if (m_map[row][col+1] == height + 1) {
                steps.push_back({ row , col + 1 });
            }
        }

        // check step to the left
        if (col != 0) {
            if (m_map[row][col-1] == height + 1) {
                steps.push_back({ row , col - 1 });
            }
        }

        // check lower step
        if (row != Size-1) {
            if (m_map[row + 1][col] == height + 1) {
                steps.push_back({ row + 1 , col });
            }
        }

        return steps;
    }
};


// ===========================================================================
// forward declarations


// ===========================================================================
// input & output

//static std::string readPuzzleFromFile(const std::string_view filename) {
//
//    std::ifstream file{ filename.data() };
//    std::string line{};
//
//    if (file.is_open()) {
//
//        // read a single line
//        std::getline(file, line);
//
//        file.close();
//    }
//    else {
//
//        std::println("Unable to open file {} !", filename);
//    }
//
//    return line;
//}

// ===========================================================================
// 
// types / logic

// ===========================================================================
// testing

static void puzzle_10_test()
{
    TopographicMap<4> map;
    map.readPuzzleFromFile(g_filenameTestData);
    map.printMap();
    map.searchTrail({0, 0});
}

// ===========================================================================
// part one

static void puzzle_10_part_one()
{
}

// ===========================================================================
// part two

static void puzzle_10_part_two()
{
}

// ===========================================================================
// main

void puzzle_10()
{
    puzzle_10_test();

    //puzzle_10_part_one();
    //puzzle_10_part_two();
}

// ===========================================================================
// End-of-File
// ===========================================================================
