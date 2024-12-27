
// ===========================================================================
// Puzzle12.cpp
// ===========================================================================

// https://forum.ada-lang.io/t/2024-day-12-garden-groups-spoiler/1587/2

#include "../Logger/ScopedTimer.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
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
    std::array<std::array<char, Dimension>, Dimension>   m_garden;
    std::array<std::array<char, Dimension>, Dimension>   m_area;
    char                                                  m_plant;

    std::list<std::pair<size_t, size_t>>                  m_listArea;

public:
    // c'tor
    GardenGroups() : m_garden{}, m_area{}, m_plant{ '0' } {
    
        for (auto& row : m_area) {
            std::fill(row.begin(), row.end(), '0');
        }
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

                    m_garden[row][col] = ch;
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

        size_t count{};

        for (auto& row : m_area) {
            size_t result{ static_cast<size_t> (std::count(row.begin(), row.end(), m_plant)) };
            count += result;
        }

        return count;
    }

    size_t getPerimeterOfCurrentArea() {

        size_t perimeter{ 4 * getSizeOfCurrentArea () };

        for (size_t neighbours{};  const auto& [row, col] : m_listArea) {

            // is there a neighbour above
            if (row == 0) {
                neighbours++;
            }
            else if (m_garden[row - 1][col] == m_plant) {
                neighbours++;
            }

            // is there a neighbour to the right
            if (col == Dimension - 1) {
                neighbours++;
            }
            else if (m_garden[row][col + 1] == m_plant) {
                neighbours++;
            }

            // is there a neighbour below
            if (row == Dimension - 1) {
                neighbours++;
            }
            else if (m_garden[row + 1][col] == m_plant) {
                neighbours++;
            }

            // is there a neighbour to the left
            if (col == 0) {
                neighbours++;
            }
            else if (m_garden[row][col - 1] == m_plant) {
                neighbours++;
            }

            // subtract number of neighbours from perimeter
            perimeter -= neighbours;
        }

        return perimeter;
    }

    void searchArea(size_t row, size_t col, char plant) {

        m_plant = plant;

        searchArea(row, col);
    }

    void searchArea(size_t row, size_t col) {

        std::println("searchArea: {},{}", row, col);

        if (m_garden[row][col] == m_plant) {


            // WEITER ; HIER MUSS EIN KLEINER BUCHSTABE REIN, sonst geht das Flood Fill nicht ....
            m_garden[row][col] = '0';    // clear this position in the garden 
            //m_area[row][col] = m_plant;  // set plot in area

            // Alternative: all plant to area list
            m_listArea.push_back({ row, col });

            // flood fill algorithm
            if (col + 1 < Dimension) {
                searchArea(row, col + 1); // bottom
            }

            if (col != 0) {
                searchArea(row, col - 1); // above
            }

            if (row + 1 < Dimension) {
                searchArea(row + 1, col); // right
            }

            if (row != 0) {
                searchArea(row - 1, col); // left
            }
        }
    }

    void printArea() {

        for (const auto& [row, col] : m_listArea) {
            std::print("[{},{}]", row, col);
        }
        std::println();
    }

private:
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
    garden.searchArea(0, 0, 'A');
    size_t size{ garden.getSizeOfCurrentArea() };
    std::println("Size: {}", size);
    garden.printArea();
    size_t perimeter{ garden.getPerimeterOfCurrentArea() };
    std::println("Perimeter: {}", perimeter);
}

// ===========================================================================
// part one

static void puzzle_12_part_one()
{

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
    puzzle_12_test_01();

    //puzzle_12_part_one();
    //puzzle_12_part_two();
}

// ===========================================================================
// End-of-File
// ===========================================================================
