// ===========================================================================
// Puzzle04.cpp
// ===========================================================================

#include <fstream>
#include <list>
#include <print>
#include <string>
#include <vector>


// Find all occurences of a substring
// https://stackoverflow.com/questions/4034750/find-all-substrings-occurrences-and-locations

// ===========================================================================
// Part 01

static constexpr int SimpleSize = 5;

static char simple_test_data [SimpleSize][SimpleSize]  =
{
    { '1', '2', '3', '4', '5' },
    { 'a', 'b', 'c', 'd', 'e' },
    { '6', '7', '8', '9', '0' },
    { 'A', 'B', 'C', 'D', 'E' },
    { 'V', 'W', 'X', 'Y', 'Z' }
};

// ===========================================================

static constexpr int AocSize = 10;

static char aoc_test_data[AocSize][AocSize] =
{
    { 'M', 'M', 'M', 'S', 'X', 'X', 'M', 'A', 'S', 'M' },
    { 'M', 'S', 'A', 'M', 'X', 'M', 'S', 'M', 'S', 'A' },
    { 'A', 'M', 'X', 'S', 'X', 'M', 'A', 'A', 'M', 'M' },
    { 'M', 'S', 'A', 'M', 'A', 'S', 'M', 'S', 'M', 'X' },
    { 'X', 'M', 'A', 'S', 'A', 'M', 'X', 'A', 'M', 'M' },
    { 'X', 'X', 'A', 'M', 'M', 'X', 'X', 'A', 'M', 'A' },
    { 'S', 'M', 'S', 'M', 'S', 'A', 'S', 'X', 'S', 'S' },
    { 'S', 'A', 'X', 'A', 'M', 'A', 'S', 'A', 'A', 'A' },
    { 'M', 'A', 'M', 'M', 'M', 'X', 'M', 'M', 'M', 'M' },
    { 'M', 'X', 'M', 'X', 'A', 'X', 'M', 'A', 'S', 'X' }
};

// ===========================================================

static constexpr int AocSizePartTwo = 10;

static char aoc_test_data_part_two[AocSize][AocSize] =
{
    { '.', 'M', '.', 'S', '.', '.', '.', '.', '.', 'A', },
    { '.', '.', 'A', '.', '.', 'M', 'S', 'M', 'S', '.', },
    { '.', 'M', '.', 'S', '.', 'M', 'A', 'A', '.', '.', },
    { '.', '.', 'A', '.', 'A', 'S', 'M', 'S', 'M', '.', },
    { '.', 'M', '.', 'S', '.', 'M', '.', '.', '.', '.', },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', },
    { 'S', '.', 'S', '.', 'S', '.', 'S', '.', 'S', '.', },
    { '.', 'A', '.', 'A', '.', 'A', '.', 'A', '.', '.', },
    { 'M', '.', 'M', '.', 'M', '.', 'M', '.', 'M', '.', },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', }
};

// ===========================================================

static constexpr int RealAocSize = 140;

static char aoc_real_data[RealAocSize][RealAocSize] {};

// ===========================================================
// ===========================================================

static constexpr int Size = RealAocSize;

std::list <std::string> searchStrings;

// ===========================================================
// input / output

static std::vector<std::string> puzzle_04_read_real_data_into_vector()
{
    std::string name{ "Puzzle04_Data.txt" };

    std::ifstream file{ name };

    static std::vector<std::string> data;

    if (file.is_open()) {

        std::string line{};
        while (std::getline(file, line)) {
            data.push_back(std::move(line));
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", name);
    }

    return data;
}

static void copy_vector_to_real_data(const std::vector<std::string>& data)
{
    for (int row{}; const auto & line : data) {

        for (int col{}; char ch : line) {

            aoc_real_data[row][col] = ch;
            ++col;
        }
        ++row;
    }
}

static void printSearchStrings(const std::list<std::string>& list)
{
    for (const auto& s : list) {
        std::println("{}", s);
    }
}

// ===========================================================
// logic

static int countXMASSingleLine(const std::string& line) {

    std::string xmas{ "XMAS" };
    size_t pos{ line.find(xmas, 0) };
    int count{};

    while (pos != std::string::npos)
    {
        ++count;
        pos = line.find(xmas, pos + 1);
    }

    return count;
}

static int countXMAS() {

    int count{};

    for (const auto& line : searchStrings) {

        count += countXMASSingleLine(line);
    }

    return count;
}

static void create_search_strings_01_horizontal_left_to_right(char(*data)[Size])
{
    std::string s{};
    s.reserve(100);

    for (int i{}; i != Size; ++i) {

        s.clear();

        for (int j{}; j != Size; ++j) {

            s += data[i][j];
        }

        searchStrings.push_back(s);
    }
}


static void create_search_strings_02_vertical_top_to_bottom(char(*data)[Size])
{
    std::string s{};
    s.reserve(100);

    for (int j{}; j != Size; ++j) {

        s.clear();

        for (int i{}; i != Size; ++i) {

            s += data[i][j];
        }

        searchStrings.push_back(s);
    }
}

static void create_search_strings_03_diagonal_top_to_bottom_left_to_right(char(*data)[Size])
{
    std::string s{};
    s.reserve(100);

    // lower part of diagonal
    for (int i{ 1 }; i != Size ; ++i) {

        int row{ i };
        int col{ 0 };

        s.clear();

        while (col != Size && row != Size) {

            s += data[row][col];

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

            s += data[row][col];

            row++;
            col++;
        }

        searchStrings.push_back(s);
    }

    // diagonal itself
    s.clear();
    for (int i{ }; i != Size; ++i) {
        s += data[i][i]; 
    }
    searchStrings.push_back(s);
}

static void create_search_strings_04_diagonal_top_to_bottom_right_to_left(char(*data)[Size])
{
    std::string s{};
    s.reserve(100);

    // lower part of diagonal
    for (int i{ 1 }; i != Size; ++i) {

        int row{ i };
        int col{ Size - 1 };

        s.clear();

        while (col != 0 && row != Size) {

            s += data[row][col];

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

            s += data[row][col];

            row++;
            col--;
        }

        searchStrings.push_back(s);
    }

    // diagonal itself
    s.clear();
    for (int i{ }; i != Size; ++i) {
        s += data[i][Size - 1 - i];
    }
    searchStrings.push_back(s);
}

static void create_search_strings_05_horizontal_right_to_left(char(*data)[Size])
{
    std::string s{};
    s.reserve(100);

    for (int i{}; i != Size; ++i) {

        s.clear();

        for (int j{ Size - 1 }; j != -1; --j) {

            s += data[i][j];
        }

        searchStrings.push_back(s);
    }
}

static void create_search_strings_06_vertical_bottom_to_top(char(*data)[Size])
{
    std::string s{};
    s.reserve(100);

    for (int j{ 0 }; j != Size; ++j) {

        s.clear();

        for (int i{ Size - 1 }; i != -1; --i) {

            s += data[i][j];
        }

        searchStrings.push_back(s);
    }
}

static void create_search_strings_07_diagonal_bottom_to_top_right_to_left(char(*data)[Size])
{
    std::string s{};
    s.reserve(100);

    // lower part of diagonal
    for (int i{ Size - 1 }; i != 0; --i) {

        int row{ Size - 1 };
        int col{ i - 1 };

        s.clear();

        while (col != -1 && row != -1) {

            s += data[row][col];

            row--;
            col--;
        }

        searchStrings.push_back(s);
    }

    // upper part of diagonal
    for (int i{ Size - 2 }; i != -1; --i) {

        int row{ i };
        int col{ Size - 1 };

        s.clear();

        while (col != -1 && row != -1) {

            s += data[row][col];

            row--;
            col--;
        }

        searchStrings.push_back(s);
    }

    // diagonal itself
    s.clear();
    for (int i{ Size - 1 }; i != -1; --i) {
        s += data[i][i];
    }
    searchStrings.push_back(s);
}

static void create_search_strings_08_diagonal_bottom_to_top_left_to_right(char(*data)[Size])
{
    std::string s{};
    s.reserve(100);

    // lower part of diagonal
    for (int i{ Size - 2 }; i != -1; --i) {

        int row{ i };
        int col{ 0 };

        s.clear();

        while (col != Size - 1 && row != -1) {

            s += data[row][col];

            row--;
            col++;
        }

        searchStrings.push_back(s);
    }

    // upper part of diagonal
    for (int j{ 1 }; j != Size; ++j) {

        int row{ Size - 1 };
        int col{ j };

        s.clear();

        while (col != Size && row != -1) {

            s += data[row][col];

            row--;
            col++;
        }

        searchStrings.push_back(s);
    }

    // diagonal itself
    s.clear();
    for (int i{ Size - 1 }; i != -1; --i) {
        s += data[i][Size - i - 1];
    }
    searchStrings.push_back(s);
}

static void create_search_strings(char data[Size][Size])
{
    create_search_strings_01_horizontal_left_to_right(data);
    create_search_strings_02_vertical_top_to_bottom(data);
    create_search_strings_03_diagonal_top_to_bottom_left_to_right(data);
    create_search_strings_04_diagonal_top_to_bottom_right_to_left(data);
    create_search_strings_05_horizontal_right_to_left(data);
    create_search_strings_06_vertical_bottom_to_top(data);
    create_search_strings_07_diagonal_bottom_to_top_right_to_left(data);
    create_search_strings_08_diagonal_bottom_to_top_left_to_right(data);
}

static void puzzle_create_part_one()
{
    //create_search_strings(aoc_test_data);
    //printSearchStrings(searchStrings);
    //int count{ countXMAS() };

    //std::vector<std::string> data{ puzzle_04_read_real_data_into_vector() };
    //copy_vector_to_real_data(data);
    //create_search_strings(aoc_real_data);
    ////printSearchStrings(searchStrings);
    //int count{ countXMAS() };
    //std::println("Total Count: {}", count);   // expected 2583
}



// ===========================================================
// logic

// ===========================================================
// test

// ===========================================================================
// Part 02

static int search_crossed_xmas(char data[Size][Size])
{
    int count{};

    for (int row{}; row < Size - 2; ++row) {

        for (int col{}; col < Size - 2; ++col) {

            // search for crossed XMAS

            // M.M
            // .A.
            // S.S
            if (data[row][col] == 'M' && data[row][col + 2] == 'M' && data[row + 1][col + 1] == 'A' && data[row + 2][col] == 'S' && data[row + 2][col + 2] == 'S') {
                ++count;
            }
            // S.S
            // .A.
            // M.M
            else if (data[row][col] == 'S' && data[row][col + 2] == 'S' && data[row + 1][col + 1] == 'A' && data[row + 2][col] == 'M' && data[row + 2][col + 2] == 'M') {
                ++count;
            }
            // M.S
            // .A.
            // M.S
            else if (data[row][col] == 'M' && data[row][col + 2] == 'S' && data[row + 1][col + 1] == 'A' && data[row + 2][col] == 'M' && data[row + 2][col + 2] == 'S') {
                ++count;
            }
            // S.M
            // .A.
            // S.M
            else if (data[row][col] == 'S' && data[row][col + 2] == 'M' && data[row + 1][col + 1] == 'A' && data[row + 2][col] == 'S' && data[row + 2][col + 2] == 'M') {
                ++count;
            }
        }
    }

    return count;
}

//static void puzzle_create_part_two_test_data()
//{
//    int count{ search_crossed_xmas(aoc_test_data_part_two) };
//    std::println("Total Count Part Two: {}", count);   // expected 9
//}

static void puzzle_create_part_two()
{
    std::vector<std::string> data{ puzzle_04_read_real_data_into_vector() };
    copy_vector_to_real_data(data);
    int count{ search_crossed_xmas(aoc_real_data) };
    std::println("Total Count Part Two: {}", count);   // expected 1978
}

// ===========================================================================

void puzzle_04()
{
    // puzzle_create_part_one();
    puzzle_create_part_two();
}

// ===========================================================================
// End-of-File
// ===========================================================================
