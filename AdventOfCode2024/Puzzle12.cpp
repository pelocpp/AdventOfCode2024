
// ===========================================================================
// Puzzle12.cpp
// ===========================================================================

// https://forum.ada-lang.io/t/2024-day-12-garden-groups-spoiler/1587/2

// https://advent-of-code.xavd.id/writeups/2024/day/12/

#include "../Logger/ScopedTimer.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

// ===========================================================================
// global data

static std::string_view g_filenameTestData{ "Puzzle12_TestData.txt" };
static std::string_view g_filenameRealData{ "Puzzle12_RealData.txt" };

// ===========================================================================
// types

template <size_t Dimension>
class GardenGroups
{
private:
    std::unique_ptr<std::array<std::array<char, Dimension>, Dimension>>   m_garden;
    std::list<std::pair<size_t, size_t>>                                  m_listArea;
    char                                                                  m_plant;
    size_t                                                                m_totalPrice;

public:
    // c'tor
    GardenGroups() 
        : m_garden{}, m_listArea{}, m_plant{ '0' }, m_totalPrice{}
    {
        m_garden = std::make_unique <std::array<std::array<char, Dimension>, Dimension>> ();
    }

    // public interface
    void initBoardFromFile(std::string_view filename) {

        std::ifstream file{ filename.data()};

        if (file.is_open()) {

            // read line per line
            std::string line{};

            int row{ -1 };

            while (std::getline(file, line)) {

                if (line.empty()) {
                    break;
                }

                assert(line.size() == Dimension);

                ++row;

                // copy line into board
                for (int col{}; char ch : line) {

                    (*m_garden)[row][col] = ch;
                    ++col;
                }
            }

            file.close();
        }
        else {

            std::println("Unable to open file {} !", filename);
        }
    }

    size_t getSizeOfCurrentArea() {

        return m_listArea.size();
    }

    size_t getTotalPrice() {

        return m_totalPrice;
    }

    size_t getPerimeterOfCurrentArea() {

        // Perimeter:
        // 4 * size of area - (count of neighbours of each individual unit) 

        size_t perimeter{ 4 * getSizeOfCurrentArea() };
        size_t neighbours{};
        char plant{ m_plant + ('a' - 'A') };

        for (const auto& [row, col] : m_listArea) {

            // is there another neighbour above
            if (row != 0 && (*m_garden)[row - 1][col] == plant) {
                neighbours++;
            }

            // is there another neighbour to the right
            if ((col != Dimension - 1) && (*m_garden)[row][col + 1] == plant) {
                neighbours++;
            }

            // is there another neighbour below
            if ((row != Dimension - 1) && (*m_garden)[row + 1][col] == plant) {
                neighbours++;
            }

            // is there another neighbour to the left
            if (col != 0 && (*m_garden)[row][col - 1] == plant) {
                neighbours++;
            }
        }

        // subtract number of neighbours from perimeter
        perimeter -= neighbours;

        return perimeter;
    }

    void searchArea(size_t row, size_t col, char plant) {

        m_listArea.clear();
        m_plant = plant;
        searchArea(row, col);

        // TEST
        printArea();
        size_t size{ getSizeOfCurrentArea() };
        std::println("  Size: {}", size);
        size_t perimeter{ getPerimeterOfCurrentArea() };
        std::println("  Perimeter: {}", perimeter);
    }

    void searchAllAreas() {

        m_totalPrice = 0;

        for (size_t row{};  const auto & plantRow : *m_garden) {
            for (size_t col{}; auto plant : plantRow) {
                if (std::isupper(plant)) {

                    searchArea(row, col, plant);

                    m_totalPrice += (getSizeOfCurrentArea() * getPerimeterOfCurrentArea());
                }
                ++col;
            }
            ++row;
        }
    }

    void printArea() {

        for (const auto& [row, col] : m_listArea) {
            std::print("[{},{}]", row, col);
        }
        std::println();
    }

private:
    void searchArea(size_t row, size_t col) {

        if ((*m_garden)[row][col] == m_plant) {

            (*m_garden)[row][col] += ('a' - 'A');   // modifiy this position in the garden 
            m_listArea.push_back({ row, col });     // add plot to list of area

            // flood fill algorithm
            if (col + 1 < Dimension) {
                searchArea(row, col + 1); // right
            }

            if (col != 0) {
                searchArea(row, col - 1); // left
            }

            if (row + 1 < Dimension) {
                searchArea(row + 1, col); // below
            }

            if (row != 0) {
                searchArea(row - 1, col); // above
            }
        }
    }
};


// ===========================================================================
// forward declarations

// ===========================================================================
// input & output

// ===========================================================================
// types / logic

// ===========================================================================
// testing

static void puzzle_12_test_01()
{
    GardenGroups<4> garden;
    garden.initBoardFromFile(g_filenameTestData);
    garden.searchArea(1, 0, 'B');
    size_t size{ garden.getSizeOfCurrentArea() };
    std::println("Size: {}", size);
    garden.printArea();
    size_t perimeter{ garden.getPerimeterOfCurrentArea() };
    std::println("Perimeter: {}", perimeter);
}

static void puzzle_12_test_02()
{
    GardenGroups<10> garden;
    garden.initBoardFromFile(g_filenameTestData);
    garden.searchAllAreas();
    std::println("Total Price: {}", garden.getTotalPrice());
}

// ===========================================================================
// part one

static void puzzle_12_part_one()
{
    ScopedTimer watch{};
    GardenGroups<140> garden;
    garden.initBoardFromFile(g_filenameRealData);
    garden.searchAllAreas();
    std::println("Total Price: {}", garden.getTotalPrice());
}

// ===========================================================================
// part two

static void puzzle_12_part_two()
{

}

// ===========================================================================
// main

void puzzle_12()
{
    //puzzle_12_test_01();
    puzzle_12_test_02();

    //puzzle_12_part_one();        // expected 1518548
    //puzzle_12_part_two();
}

// ===========================================================================
// End-of-File
// ===========================================================================
