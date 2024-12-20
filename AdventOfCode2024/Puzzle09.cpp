// https://www.reddit.com/r/adventofcode/comments/1ha2mhp/2024_day_9_i_am_so_confused_about_the_id_rule_for/

// ===========================================================================
// Puzzle09.cpp
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
#include <string_view>

// ===========================================================================
// global data

static std::string_view g_filenameTestData{ "Puzzle09_TestData.txt" };
static std::string_view g_filenameRealData{ "Puzzle09_RealData.txt" };

// ===========================================================================
// types

using FileIDRepresentation = std::vector<short>;
using ForwardIterator = std::vector<short>::iterator;
using BackwardIterator = std::vector<short>::reverse_iterator;

// ===========================================================================
// forward declarations

static std::string readPuzzleFromFile(const std::string_view filename);
static FileIDRepresentation convertDiskMapToFileIDRepresentation(const std::string map);
static void moveFileBlocks(FileIDRepresentation& representation);
static void printFileIDRepresentation(const FileIDRepresentation& representation);
static size_t calcChecksum(const FileIDRepresentation& representation);

// ===========================================================================
// input & output

static std::string readPuzzleFromFile(const std::string_view filename) {

    std::ifstream file{ filename.data()};
    std::string line{};

    if (file.is_open()) {

        // read a single line
        std::getline(file, line);

        file.close();
    }
    else {

        std::println("Unable to open file {} !", filename);
    }

    return line;
}

// ===========================================================================
// 
// types / logic

// convert disk map to File ID representation
static FileIDRepresentation convertDiskMapToFileIDRepresentation(const std::string map) {

    FileIDRepresentation representation{};
    representation.reserve(1000);

    short currentID{};
    size_t size{ map.size() };
    size_t pos{};

    while (pos != size) {

        char ch{ map[pos] };

        size_t value{ size_t(ch) - '0' };

        if (pos % 2 == 0) {

            // char represents number of blocks of file
            for (size_t i{}; i != value; ++i) {
                representation.push_back(currentID);
            }

            // skip to next ID
            ++currentID;
        }
        else {

            // char represents free spaces
            for (size_t i{}; i != value; ++i) {
                representation.push_back(-1);
            }
        }

        ++pos;
    }

    return representation;
}

static void moveFileBlocks (FileIDRepresentation& representation) {

    // search next free space position
    ForwardIterator pos{ representation.begin() };
    BackwardIterator reversePos{ representation.rbegin() };

    while (true) {

        // search next free space position
        pos = std::find_if(
            pos,
            representation.end(),
            [](auto elem) {
                return (elem == -1) ? true : false;
            }
        );

        // search next file block at the end of the representation
        reversePos = std::find_if(
            reversePos,
            representation.rend(),
            [](auto elem) {
                return (elem == -1) ? false : true;
            }
        );

        // are they pointing to the same location
        ForwardIterator it = reversePos.base();
        if (it == pos)
            break;

        // move file block from end to begin
        short block = *reversePos;
        *reversePos = -1;
        *pos = block;

        // advance both iterators
        ++pos;
        ++reversePos;

        // just for testing
        // printFileIDRepresentation(representation);
    }
}

static size_t calcChecksum(const FileIDRepresentation& representation) {

    size_t n{};
    size_t checksum{};

    while (representation[n] != -1) {

        checksum += (n * representation[n]);
        ++n;
    }

    return checksum;
}

static void printFileIDRepresentation(const FileIDRepresentation& representation) {

    for (auto elem : representation) {

        if (elem == -1) {
            std::print(".");
        }
        else {
            std::print("{}", elem);
        }
    }

    std::println();
}

// ===========================================================================
// testing

static void test_01() {

    std::string map = readPuzzleFromFile(g_filenameRealData);
    FileIDRepresentation representation{ convertDiskMapToFileIDRepresentation(map) };
    //printFileIDRepresentation(representation);

    moveFileBlocks(representation);

    // printFileIDRepresentation(representation);

    std::println("Checksum: {}", calcChecksum(representation));  // expected 6259790630969
}

// ===========================================================================
// part one

static void puzzle_09_part_one()
{
    std::string map{ readPuzzleFromFile(g_filenameRealData) };
    FileIDRepresentation representation{ convertDiskMapToFileIDRepresentation(map) };
    printFileIDRepresentation(representation);
}

// ===========================================================================
// part two

static void puzzle_09_part_two()
{
    readPuzzleFromFile(g_filenameRealData);
}

// ===========================================================================
// main

void puzzle_09()
{
    test_01();
    // puzzle_09_part_one();
    //puzzle_09_part_two(); 
}

// ===========================================================================
// End-of-File
// ===========================================================================
