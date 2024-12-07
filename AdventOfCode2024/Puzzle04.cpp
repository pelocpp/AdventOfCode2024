// ===========================================================================
// Puzzle04.cpp
// ===========================================================================

#include <cassert>
#include <fstream>
#include <print>
#include <regex>
#include <string>
#include <list>
#include <string>


// Find all occurences of a substring
// https://stackoverflow.com/questions/4034750/find-all-substrings-occurrences-and-locations

// ===========================================================================
// Part 01

static constexpr int Size = 5;

static char test_data [Size][Size]  =
{
    { '1', '2', '3', '4', '5' },
    { 'a', 'b', 'c', 'd', 'e' },
    { '6', '7', '8', '9', '0' },
    { 'A', 'B', 'C', 'D', 'E' },
    { 'V', 'W', 'X', 'Y', 'Z' }
};

std::list <std::string> searchStrings;

// ===========================================================
// input

static void create_search_strings_01_horizontal_left_to_right()
{
    std::string s{};

    for (int i{}; i != Size; ++i) {

        s.clear();

        for (int j{}; j != Size; ++j) {

            s += test_data[i][j];
        }

        searchStrings.push_back(s);
    }
}

static void create_search_strings_02_vertical_top_to_bottom()
{
    std::string s{};

    for (int j{}; j != Size; ++j) {

        s.clear();

        for (int i{}; i != Size; ++i) {

            s += test_data[i][j];
        }

        searchStrings.push_back(s);
    }
}

static void create_search_strings_03_diagonal_top_to_bottom_left_to_right()
{
    std::string s{};

    // lower part of diagonal
    for (int i{ 1 }; i != Size ; ++i) {

        int row{ i };
        int col{ 0 };

        s.clear();

        while (col != Size && row != Size) {

            s += test_data[row][col];

            row++;
            col++;
        }

        searchStrings.push_back(s);
    }

    // upper part of diagonal
    for (int j{ 1 }; j != Size; ++j) {

        int row{ 0 };
        int col{ j };

        s.clear();

        while (col != Size && row != Size) {

            s += test_data[row][col];

            row++;
            col++;
        }

        searchStrings.push_back(s);
    }

    // diagonal itself
    s.clear();
    for (int i{ }; i != Size; ++i) {
        s += test_data[i][i]; 
    }
    searchStrings.push_back(s);
}

static void create_search_strings_04_diagonal_top_to_bottom_right_to_left()
{
    std::string s{};

    // lower part of diagonal
    for (int i{ 1 }; i != Size; ++i) {

        int row{ i };
        int col{ Size - 1 };

        s.clear();

        while (col != 0 && row != Size) {

            s += test_data[row][col];

            row++;
            col--;
        }

        searchStrings.push_back(s);
    }

    // upper part of diagonal
    for (int j{ Size - 2 }; j != -1; --j) {

        int row{ 0 };
        int col{ j };

        s.clear();

        while (col != -1 && row != Size) {

            s += test_data[row][col];

            row++;
            col--;
        }

        searchStrings.push_back(s);
    }

    // diagonal itself
    s.clear();
    for (int i{ }; i != Size; ++i) {
        s += test_data[i][Size - 1 - i];
    }
    searchStrings.push_back(s);
}

static void create_search_strings_05_horizontal_right_to_left()
{
    std::string s{};

    for (int i{}; i != Size; ++i) {

        s.clear();

        for (int j{ Size - 1 }; j != -1; --j) {

            s += test_data[i][j];
        }

        searchStrings.push_back(s);
    }
}

static void create_search_strings_06_vertical_bottom_to_top()
{
    std::string s{};

    for (int j{ 0 }; j != Size; ++j) {

        s.clear();

        for (int i{ Size - 1 }; i != -1; --i) {

            s += test_data[i][j];
        }

        searchStrings.push_back(s);
    }
}

static void create_search_strings_07_diagonal_bottom_to_top_right_to_left()
{
    std::string s{};

    //// lower part of diagonal
    //for (int i{ Size - 1 }; i != 0; --i) {

    //    int row{ Size - 1 };
    //    int col{ i - 1 };

    //    s.clear();

    //    while (col != -1 && row != -1) {

    //        s += test_data[row][col];

    //        row--;
    //        col--;
    //    }

    //    searchStrings.push_back(s);
    //}

    // upper part of diagonal
    // WEITER  WEITER  WEITER  WEITER  WEITER  WEITER  WEITER  WEITER  WEITER  WEITER  WEITER  WEITER 
    for (int j{ 1 }; j != Size; ++j) {

        int row{ 0 };
        int col{ j };

        s.clear();

        while (col != Size && row != Size) {

            s += test_data[row][col];

            row++;
            col++;
        }

        searchStrings.push_back(s);
    }

    //// diagonal itself
    //s.clear();
    //for (int i{ }; i != Size; ++i) {
    //    s += test_data[i][i];
    //}
    //searchStrings.push_back(s);
}

static void puzzle_04_create_search_strings()
{
    //create_search_strings_01_horizontal_left_to_right();
    //create_search_strings_02_vertical_top_to_bottom();
    //create_search_strings_03_diagonal_top_to_bottom_left_to_right();
    //create_search_strings_04_diagonal_top_to_bottom_right_to_left();
   // create_search_strings_05_horizontal_right_to_left();
    //create_search_strings_06_vertical_bottom_to_top();
    create_search_strings_07_diagonal_bottom_to_top_right_to_left();
}

static void puzzle_create_test()
{
    puzzle_04_create_search_strings();
}


// ===========================================================
// logic

// ===========================================================
// test

// ===========================================================================
// Part 02

// ===========================================================================

void puzzle_04()
{
    puzzle_create_test();
}

// ===========================================================================
// End-of-File
// ===========================================================================
