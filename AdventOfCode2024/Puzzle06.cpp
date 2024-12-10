// ===========================================================================
// Puzzle05.cpp
// ===========================================================================

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <print>
#include <string>

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
    Guard() : m_row{}, m_col{}, m_currentDirection{ Direction::Upwards }  {}

    bool canMove() {

        // is there any abstacle in front of the guard
        if (m_currentDirection == Direction::Upwards) {

            if (m_board[m_row - 1][m_col] == '#') {
                return false;
            }
        }
        else if (m_currentDirection == Direction::ToTheRight) {

            if (m_board[m_row][m_col + 1] == '#') {
                return false;
            }
        }
        else if (m_currentDirection == Direction::Downwards) {

            if (m_board[m_row + 1][m_col] == '#') {
                return false;
            }
        }
        else if (m_currentDirection == Direction::ToTheLeft) {

            if (m_board[m_row][m_col - 1] == '#') {
                return false;
            }
        }

        return true;
    }

    bool isLeavingMap() {

        // is leaving map
        if (m_currentDirection == Direction::Upwards) {

            if (m_row == 0) {
                return true;
            }
        }
        else if (m_currentDirection == Direction::ToTheRight) {

            if (m_col == Size - 1) {
                return true;
            }
        }
        else if (m_currentDirection == Direction::Downwards) {

            if (m_row == Size - 1) {
                return true;
            }
        }
        else if (m_currentDirection == Direction::ToTheLeft) {

            if (m_col == 0) {
                return true;
            }
        }

        return false;
    }

    void advanceGuard() {

        // place 'X' on current position
        m_board[m_row][m_col] = 'X';

        // move guard one position and set guard's icon
        if (m_currentDirection == Direction::Upwards) {
            --m_row;
            m_board[m_row][m_col] = '^';
        }
        else if (m_currentDirection == Direction::ToTheRight) {
            ++m_col;
            m_board[m_row][m_col] = '>';
        }
        else if (m_currentDirection == Direction::Downwards) {
            ++m_row;
            m_board[m_row][m_col] = 'v';
        }
        else if (m_currentDirection == Direction::ToTheLeft) {
            --m_col;
            m_board[m_row][m_col] = '<';
        }
    }

    void play() {

        while (true) {

            if (isLeavingMap()) {

                // end of puzzle reached
                break;
            }

            if (!canMove()) {

                // reached obstacle, need to change direction
                changeDirection();
            }
            else {

                // just advance one step in the current direction
                advanceGuard();
            }

            // testing
            std::println("Press any key ...");
            char ch;
            std::cin >> ch;

            printBoard();
        }
    }

    void changeDirection() {

        if (m_currentDirection == Direction::Upwards) {
            m_currentDirection = Direction::ToTheRight;
        }
        else if (m_currentDirection == Direction::ToTheRight) {
            m_currentDirection = Direction::Downwards;
        }
        else if (m_currentDirection == Direction::Downwards) {
            m_currentDirection = Direction::ToTheLeft;
        }
        else if (m_currentDirection == Direction::ToTheLeft) {
            m_currentDirection = Direction::Upwards;
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

    void printBoard() {

        for (const auto& row : m_board) {

            for (char ch : row) {
                std::print("{}", ch);
            }

            std::println();
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
    testGuard.play();


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
