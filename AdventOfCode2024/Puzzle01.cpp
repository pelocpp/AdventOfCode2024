// ===========================================================================
// Puzzle01.cpp
// ===========================================================================

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

extern std::vector<int> locationIds01;
extern std::vector<int> locationIds02;

static std::vector<int> testLocationIds01
{
    3, 4, 2, 1, 3, 3
};

static std::vector<int> testLocationIds02
{
   4, 3, 5, 3, 9, 3
};

static size_t puzzle_01_part_one (std::vector<int> locations1, std::vector<int> locations2)
{
    size_t size{ locations1.size() };

    size_t sum{};

    while (size != 0)
    {
        // find minimum in both location containers
        auto pos1 = std::min_element(locations1.begin(), locations1.end());
        auto pos2 = std::min_element(locations2.begin(), locations2.end());

        // calculate distance between them
        auto diff = std::abs(*pos1 - *pos2);

        // add distance
        sum += diff;

        // set both locations within the containers to maximum int (instead of deleting them)
        *pos1 = INT_MAX;
        *pos2 = INT_MAX;

        size--;
    }

    return sum;
}

static size_t puzzle_01_part_two(std::vector<int> locations1, std::vector<int> locations2)
{
    // note: I need copies of the location containers, because I'm modifiying the content

    size_t similarityScore{};

    std::for_each(

        locations1.begin(),
        locations1.end(),

        [&](auto leftElem) {

            size_t partialScore{};

            size_t count{};

            std::for_each(

                locations2.begin(),
                locations2.end(),

                [&](auto rightElem) {

                    if (leftElem == rightElem) {
                        count++;
                    }
                }
            );

            similarityScore += (leftElem * count);
        }
    );

    return similarityScore;
}

static void puzzle_01_part_one()
{
    // testing part one
    size_t testSum { puzzle_01_part_one(testLocationIds01, testLocationIds02) };
    std::println("Test Sum:   {}", testSum);        // expected 11

    size_t sum{ puzzle_01_part_one(locationIds01, locationIds02) };
    std::println("Puzzle Sum: {}", sum);            // result: 1151792

    // testing part two
    size_t testScore{ puzzle_01_part_two(testLocationIds01, testLocationIds02) };
    std::println("Test Score:   {}", testScore);    // expected 31

    size_t score{ puzzle_01_part_two(locationIds01, locationIds02) };
    std::println("Puzzle Score: {}", score);        // result:  21790168

}

void puzzle_01()
{
    std::println("Puzzle 1:");

    puzzle_01_part_one();
}

// ===========================================================================
// End-of-File
// ===========================================================================
