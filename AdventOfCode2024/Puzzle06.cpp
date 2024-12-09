// ===========================================================================
// Puzzle05.cpp
// ===========================================================================

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <list>
#include <print>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

// ===========================================================================
// global data

static std::string g_filenameTestData{ "Puzzle06_TestData.txt" };
static std::string g_filenameRealData{ "Puzzle06_RealData.txt" };

constexpr int TestSize = 10;
constexpr int RealSize = 130;

// ===========================================================================
// types

// ===========================================================================
// forward declarations

// ===========================================================================
// types / logic



enum class Direction { Upwards, ToTheRight, Downwards, ToTheLeft };

template <int Size>
class Guard
{
private:
    std::array <std::array <int, Size>, Size> m_board;

    int       m_row;
    int       m_col;

    Direction m_currentDirection;

public:
    Guard() : m_row{}, m_col{}, m_currentDirection{ Upwards }  {}

    bool canMove();

    bool leavingMap();

    void doMove();

    void changeDirection();

    void printBoard() {

        for (const auto& row : m_board) {

            for (char ch : row) {
                std::print("{}", ch);
            }

            std::println();
        }
    }

    void initBoardFromFile(const std::string& filename) {

        std::ifstream file{ filename };

        if (file.is_open()) {

            // read line per line
            std::string line{};

            int row{ -1 };

            while (std::getline(file, line)) {

                if (line.empty()) {
                    break;
                }

                assert(line.size() == Size);

                ++row;

                // copy line into board
                for (int col{}; char ch : line) {

                    m_board[row][col] = ch;

                    // store initial position of guard
                    if (ch == '^') {
                        m_row = row;
                        m_col = col;
                    }

                    ++col;
                }
            }

            file.close();
        }
        else {

            std::println("Unable to open file {} !", filename);
        }
    }
};

// ===========================================================================
// test

static void puzzle_06_first_run()
{
    Guard<TestSize> testGuard{};

    testGuard.initBoardFromFile(g_filenameTestData);
    testGuard.printBoard();
}


// ===========================================================================
// main

void puzzle_06()
{
    puzzle_06_first_run();
}

// ===========================================================================
// End-of-File
// ===========================================================================
