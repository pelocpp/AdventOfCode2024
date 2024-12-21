// https://www.reddit.com/r/adventofcode/comments/1ha2mhp/2024_day_9_i_am_so_confused_about_the_id_rule_for/

// ===========================================================================
// Puzzle09.cpp
// ===========================================================================

#include <algorithm>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#include <vector>
#include <string_view>

// ===========================================================================
// global data

static std::string_view g_filenameTestData{ "Puzzle09_TestData.txt" };
static std::string_view g_filenameRealData{ "Puzzle09_RealData.txt" };

// ===========================================================================
// types

using FileID = short;
using FileIDRepresentation = std::vector<FileID>;
using ForwardIterator = std::vector<FileID>::iterator;
using BackwardIterator = std::vector<FileID>::reverse_iterator;

// ===========================================================================
// forward declarations

static std::string readPuzzleFromFile(const std::string_view filename);
static FileIDRepresentation convertDiskMapToFileIDRepresentation(const std::string map);
static void moveFileBlocks(FileIDRepresentation& representation);
static void printFileIDRepresentation(const FileIDRepresentation& representation);
static size_t calcChecksum(const FileIDRepresentation& representation);
static ForwardIterator searchFreeSpan(FileIDRepresentation& representation, size_t lengtH, ForwardIterator maxPosition);
static void copyFile(FileIDRepresentation& representation, BackwardIterator sourcePos, ForwardIterator targetPos, size_t length);

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
    representation.reserve(100'000);

    FileID currentID{};
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

    std::println("Last file ID: {}", currentID-1);
    std::println("Length of File Representation: : {}", representation.size());

    return representation;
}

static void moveFileBlocks (FileIDRepresentation& representation)
{
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
        FileID id = *reversePos;
        *reversePos = -1;
        *pos = id;

        // advance both iterators
        ++pos;
        ++reversePos;

        // just for testing
        // printFileIDRepresentation(representation);
    }
}

static void moveFileBlocksEx(FileIDRepresentation& representation) {

    ForwardIterator pos{ representation.begin() };
    BackwardIterator reversePosBegin{ representation.rbegin() };
    BackwardIterator reversePosEnd{ };

    while (true) {

        // search begin and end of last file
        reversePosBegin = std::find_if(
            reversePosBegin,
            representation.rend(),
            [](auto elem) {
                return (elem == -1) ? false : true;
            }
        );

        // retrieve ID of this file
        FileID id{ *reversePosBegin };
        reversePosEnd = reversePosBegin;
        while (*reversePosEnd == id) {
            ++reversePosEnd;
        }

        // calculate size of this file
        std::ptrdiff_t length{ reversePosEnd - reversePosBegin };

        // search a span of free blocks of this length (to the left)
        ForwardIterator maxPosition{ reversePosBegin.base() };
        ForwardIterator freeSpan = searchFreeSpan(representation, length, maxPosition);

        if (freeSpan != maxPosition) {
            // move file completely to the left side 
            copyFile(representation, reversePosBegin, freeSpan, length);
        }
        else {
            // no space available, move to next file 
        }

        // advance to next file
        reversePosBegin += length;

        // end of move detection ?????????????????
        // print current file ID

      //  printFileIDRepresentation(representation);

        //std::println("Current File ID: {}", id);

        // File with ID 0 needn't to be moved
        if (id == 1) {
            break;
        }
    }



}

static ForwardIterator searchFreeSpan(FileIDRepresentation& representation, size_t length, ForwardIterator maxPosition)
{
    ForwardIterator it{ representation.begin() };

    while (true)
    {
        // search next free block
        while (*it != -1) {
            ++it;
            if (it == maxPosition) {
                return maxPosition;
            }
        }

        // check, if free space is huge enough
        ForwardIterator tmp{ it };
        size_t count{};

        while (count != length) {

            if (*tmp == -1) {
                ++tmp;
                if (tmp == maxPosition) {
                    return maxPosition;
                }

                ++count;
            }
            else {
                break;
            }
        }

        if (count == length) {
            return it;
        }
        else {
            // advance position
            it += count;
            continue;
        }
    }
}

static void copyFile(FileIDRepresentation& representation, BackwardIterator sourcePos, ForwardIterator targetPos, size_t length) {

    for (size_t i{}; i != length; ++i) {

        *targetPos = *sourcePos;
        *sourcePos = -1;
        ++targetPos;
        ++sourcePos;
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

static size_t calcChecksumEx(const FileIDRepresentation& representation) {

    size_t checksum{};

    for (size_t n{}; auto id : representation) {

        if (id != -1) {
            checksum += (n * representation[n]);
        }
    
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


}

// ===========================================================================
// part one

static void puzzle_09_part_one()
{
    std::string map = readPuzzleFromFile(g_filenameRealData);
    FileIDRepresentation representation{ convertDiskMapToFileIDRepresentation(map) };
    //printFileIDRepresentation(representation);

    moveFileBlocks(representation);

    // printFileIDRepresentation(representation);
    std::println("Checksum: {}", calcChecksum(representation));  // expected 6259790630969
}

// ===========================================================================
// part two

static void puzzle_09_part_two_test()
{
    std::string map = readPuzzleFromFile(g_filenameTestData);
    FileIDRepresentation representation{ convertDiskMapToFileIDRepresentation(map) };
    printFileIDRepresentation(representation);

    moveFileBlocksEx(representation);

    printFileIDRepresentation(representation);
    std::println("Checksum: {}", calcChecksumEx(representation));  // expected 2858
}

static void puzzle_09_part_two()
{
    std::string map{ readPuzzleFromFile(g_filenameRealData) };
    FileIDRepresentation representation{ convertDiskMapToFileIDRepresentation(map) };

    moveFileBlocksEx(representation);

    std::println("Checksum: {}", calcChecksumEx(representation));  // expected 2858
}

// ===========================================================================
// main

void puzzle_09()
{
    //puzzle_09_part_one();
    
    //puzzle_09_part_two_test();

    // a) 6259790630969
    // b) 6289564433984

    puzzle_09_part_two(); // 6289564433984
}

// ===========================================================================
// End-of-File
// ===========================================================================
