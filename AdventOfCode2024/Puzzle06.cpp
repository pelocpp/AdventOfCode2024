// ===========================================================================
// Puzzle06.cpp
// ===========================================================================

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <print>
#include <string>


// ===========================================================================
// global data

static std::string g_filenameTestData{ "Puzzle06_TestData.txt" };
static std::string g_filenameRealData{ "Puzzle06_RealData.txt" };

static constexpr int TestSize = 10;
static constexpr int RealSize = 130;

static constexpr int MaxPathTestLength = 1000;
static constexpr int MaxPathRealLength = 20000;

// TO BE SET !!!!
static constexpr int Size = RealSize;
static constexpr int MaxPathLength = MaxPathRealLength;

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
    int                                       m_row;
    int                                       m_col;
    Direction                                 m_currentDirection;

    int                                       m_rowNextObstacle;
    int                                       m_colNextObstacle;
    int                                       m_rowLastObstacle;
    int                                       m_colLastObstacle;

    int                                       m_rowInitialGuardPosition;
    int                                       m_colInitialGuardPosition;

    int                                       m_pathLengthEx;
    int                                       m_countEndlessLoops;

public:
    // c'tor
    Guard()
        : m_board{}, m_row{}, m_col{}, m_currentDirection{ Direction::Upwards }, 
        m_rowNextObstacle{}, m_colNextObstacle{ -1 }, m_rowLastObstacle{ -1 }, m_colLastObstacle{ -1 }, m_pathLengthEx{}, m_countEndlessLoops{},
        m_rowInitialGuardPosition{}, m_colInitialGuardPosition{}
    {
    }

    // getter
    size_t getPathLength  () const {

        size_t total{ 0 };

        for (const auto& row : m_board) {

            size_t count{ static_cast<size_t> (std::count(row.begin(), row.end(), 'X')) };
            total += count;
        }

        return total;
    }

    int getCountEndlessLoops() const {

        return m_countEndlessLoops;
    }

    // public interface
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
                m_board[m_row][m_col] = 'X';
                return true;
            }
        }
        else if (m_currentDirection == Direction::ToTheRight) {

            if (m_col == Size - 1) {
                m_board[m_row][m_col] = 'X';
                return true;
            }
        }
        else if (m_currentDirection == Direction::Downwards) {

            if (m_row == Size - 1) {
                m_board[m_row][m_col] = 'X';
                return true;
            }
        }
        else if (m_currentDirection == Direction::ToTheLeft) {

            if (m_col == 0) {
                m_board[m_row][m_col] = 'X';
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

        m_pathLengthEx++;
    }

    void setNextObstacle() {

        // clear last obstacle
        if (m_rowLastObstacle != -1 && m_colLastObstacle != -1) {
            m_board[m_rowLastObstacle][m_colLastObstacle] = '.';
        }

        // move current obstacle index to next valid board position
        ++m_colNextObstacle;
        {
            if (m_colNextObstacle == Size) {

                m_colNextObstacle = 0;
                ++m_rowNextObstacle;
            }
        }

        // set obstacle at next valid position
        for (int i = m_rowNextObstacle; i < Size; ++i) {

            for (int j = m_colNextObstacle; j < Size; ++j) {

                if (m_board[m_rowNextObstacle][m_colNextObstacle] == '#')
                    continue;
            
                m_board[m_rowNextObstacle][m_colNextObstacle] = '#';

                // store next obstacles position
                m_rowLastObstacle = m_rowNextObstacle;
                m_colLastObstacle = m_colNextObstacle;

                return;
            }
        }
    }

    void playEx() {
    
        for (const auto& row : m_board) {

            for (const auto& elem : row) {

                // clear lengtg counter
                m_pathLengthEx = 0;

                // restore guards initial position
                m_board[m_rowInitialGuardPosition][m_colInitialGuardPosition] = '^';

                // initialize guards member for a restart
                m_currentDirection = Direction::Upwards;
                m_row = m_rowInitialGuardPosition;
                m_col = m_colInitialGuardPosition;

                // set next obstacle
                setNextObstacle();

                // move guard
                while (m_pathLengthEx <= MaxPathLength) {

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

                    // std::println("Path Length: {}", m_pathLengthEx);
                }

               // std::println("Path Length: {}", m_pathLengthEx);

               // printBoard();

                // has there been an endless loop?
                if (m_pathLengthEx >= MaxPathLength) {

                    ++m_countEndlessLoops;

                    // print current obstacle
                    // std::println("Obstacle: row = {}, col = {}", m_rowLastObstacle, m_colLastObstacle);
                }
            }
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
        }
    }

    void changeDirection() {

        if (m_currentDirection == Direction::Upwards) {
            m_currentDirection = Direction::ToTheRight;
            m_board[m_row][m_col] = '>';
        }
        else if (m_currentDirection == Direction::ToTheRight) {
            m_currentDirection = Direction::Downwards;
            m_board[m_row][m_col] = 'v';
        }
        else if (m_currentDirection == Direction::Downwards) {
            m_currentDirection = Direction::ToTheLeft;
            m_board[m_row][m_col] = '<';
        }
        else if (m_currentDirection == Direction::ToTheLeft) {
            m_currentDirection = Direction::Upwards;
            m_board[m_row][m_col] = '^';
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

                        m_rowInitialGuardPosition = row;
                        m_colInitialGuardPosition = col;
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
    //Guard<Size> testGuard{};

    //testGuard.initBoardFromFile(g_filenameTestData);
    //testGuard.printBoard();
    //testGuard.play();
    //testGuard.printBoard();

    //std::println("Path Length: {}", testGuard.getPathLength());
}

static void puzzle_06_part_one ()
{
    std::unique_ptr<Guard<Size>> guard{ std::make_unique<Guard<Size>>() };
    guard->initBoardFromFile(g_filenameRealData);
    guard->play();
    std::println("Path Length: {}", guard->getPathLength());
}

static void puzzle_06_second_run()
{
    Guard<TestSize> testGuard{};

    testGuard.initBoardFromFile(g_filenameTestData);
    testGuard.printBoard();
    testGuard.playEx();

    std::println("Count endless Loops: {}", testGuard.getCountEndlessLoops());
}

static void puzzle_06_part_two()
{
    std::unique_ptr<Guard<Size>> guard{ std::make_unique<Guard<Size>>() };

    guard->initBoardFromFile(g_filenameRealData);
    guard->playEx();

    std::println("Count endless Loops: {}", guard->getCountEndlessLoops());
}

// ===========================================================================
// main

void puzzle_06()
{
    //puzzle_06_first_run();
    //puzzle_06_part_one();      // expected 5516
    //puzzle_06_second_run();
    puzzle_06_part_two();        // expected 2008
}

// ===========================================================================
// End-of-File
// ===========================================================================
