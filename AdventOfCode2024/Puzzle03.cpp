// ===========================================================================
// Puzzle03.cpp
// ===========================================================================

#include <cassert>
#include <fstream>
#include <print>
#include <regex>
#include <string>

// ===========================================================================
// Part 01

// ===========================================================
// input

static std::string puzzle_03_read_real_data()
{
    std::string name{ "Puzzle03_Data.txt" };

    std::ifstream file{ name };

    std::string text{};

    if (file.is_open()) {

        std::string line{};

        while (std::getline(file, line)) {

            size_t len = line.length();

            char ch{ line[len - 1] };

            std::println("Line Length: {}", len);
            std::println("Last Char: '{}'", ch);

            text += line;
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", name);
    }

    std::println("Total Text Length: {}", text.length());

    return text;
}


// ===========================================================
// logic


// ===========================================================
// test

static void puzzle_03_test_simple_regex_01()
{
    // simple example - counting the number of all matches

    std::regex expression{ "mul\\([1-9][0-9]{0,2},[1-9][0-9]{0,2}\\)" };

    std::string text = { "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))" };

    auto first = std::sregex_iterator(
        text.begin(),
        text.end(),
        expression
    );

    auto iterator{ first };

    while (iterator != std::sregex_iterator{}) {

        const auto& token = *iterator;
        std::println("Found Token: {}", token.str());
        ++iterator;
    }

    std::ptrdiff_t const count{
        std::distance(first, std::sregex_iterator{})
    };

    std::println("Count: {}", count);
}

static void puzzle_03_test_simple_regex_02()
{
    // simple example - counting the number of all matches

    std::regex expression{ "mul\\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\\)" };

    std::string text = { "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))" };

    int result{};

    auto first = std::sregex_iterator(
        text.begin(),
        text.end(),
        expression
    );

    auto iterator{ first };

    while (iterator != std::sregex_iterator{}) {

        const auto& token = *iterator;
        std::println("Found Token: {} - Size: {}", token.str(), token.size());

        for (size_t i{}; i < token.size(); ++i) {
            std::println("{}: '{}'", i, token[i].str());
        }

        // calculate result of this multiplication
        int op1 = std::stoi(token[1].str());
        int op2 = std::stoi(token[2].str());
        std::println("1. Operand: {} - 2. Operand: {}", op1, op2);
        
        result += (op1 * op2);

        ++iterator;
    }

    std::ptrdiff_t const count{
        std::distance(first, std::sregex_iterator{})
    };

    std::println("Total Result: {}", result);

    std::println("Count: {}", count);
}

static void puzzle_03_test_simple_regex_03()
{
    // simple example - counting the number of all matches

    std::regex expression{ "mul\\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\\)" };

    std::string text = { puzzle_03_read_real_data() };

    int result{};

    auto first{
        std::sregex_iterator(
            text.begin(),
            text.end(),
            expression
        )
    };

    auto iterator{ first };

    while (iterator != std::sregex_iterator{}) {

        const auto& token = *iterator;
        assert(token.size() == 3);

        // calculate result of this multiplication
        int op1 = std::stoi(token[1].str());
        int op2 = std::stoi(token[2].str());
        result += (op1 * op2);

        ++iterator;
    }

    std::ptrdiff_t const count{
        std::distance(first, std::sregex_iterator{})
    };

    std::println("Total number of valid MUL-Operations: {}", count);

    std::println("Result: {}", result);
}

// ===========================================================================
// Part 02

static void puzzle_03_test_simple_regex_part_02_test_01()
{
    // simple example - counting the number of all matches

    std::regex expression{ "do\\(\\)|don't\\(\\)|mul\\([1-9][0-9]{0,2},[1-9][0-9]{0,2}\\)" };

    std::string text = { "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))" };

    auto first = std::sregex_iterator(
        text.begin(),
        text.end(),
        expression
    );

    auto iterator{ first };

    while (iterator != std::sregex_iterator{}) {

        const auto& token = *iterator;
        std::println("Found Token: {}", token.str());
        ++iterator;
    }

    std::ptrdiff_t const count{
        std::distance(first, std::sregex_iterator{})
    };

    std::println("Count: {}", count);
}

static void puzzle_03_test_simple_regex_part_02_test_02()
{
    // simple example - counting the number of all matches

    std::regex expression{ "do\\(\\)|don't\\(\\)|mul\\([1-9][0-9]{0,2},[1-9][0-9]{0,2}\\)" };

    std::string text = { "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))" };

    auto first = std::sregex_iterator(
        text.begin(),
        text.end(),
        expression
    );

    auto iterator{ first };

    while (iterator != std::sregex_iterator{}) {

        const auto& token = *iterator;
        std::string stoken{ token.str() };
        std::println("Found Token: {}", stoken);
        ++iterator;
    }

    std::ptrdiff_t const count{
        std::distance(first, std::sregex_iterator{})
    };

    std::println("Count: {}", count);
}

enum class Operation { Noop, Mul, Do, DoNot };

static void puzzle_03_part_02()
{
    // simple example - counting the number of all matches

    std::regex expression{ "do\\(\\)|don't\\(\\)|mul\\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\\)" };

    std::string text = { puzzle_03_read_real_data() };

    int result{};
    bool enabled{ true };
    Operation currentOperation{ Operation::Noop };

    auto first{
        std::sregex_iterator(
            text.begin(),
            text.end(),
            expression
        )
    };

    auto iterator{ first };

    while (iterator != std::sregex_iterator{}) {

        const auto& token = *iterator;
        std::string stoken{ token.str() };

        // extract kind of operation
        if (stoken[0] == 'm') {
            currentOperation = Operation::Mul;
        }
        else if (stoken == "do()") {
            currentOperation = Operation::Do;
        }
        else {
            currentOperation = Operation::DoNot;
        }

        // handle operation
        if (currentOperation == Operation::Do) {
            enabled = true;
        }
        else if (currentOperation == Operation::DoNot) {
            enabled = false;
        }
        else if (currentOperation == Operation::Mul) {

            assert(token.size() == 3);

            if (enabled) {

                // calculate result of this multiplication
                int op1 = std::stoi(token[1].str());
                int op2 = std::stoi(token[2].str());
                result += (op1 * op2);
            }
        }

        ++iterator;
    }

    std::ptrdiff_t const count{
        std::distance(first, std::sregex_iterator{})
    };

    std::println("Total Result: {}", result);

    std::println("Count: {}", count);
}

// ===========================================================================

void puzzle_03()
{
    std::println("Puzzle 3:");

    // puzzle_03_test_simple_regex_02();
    // puzzle_03_test_simple_regex_03();     // Result: 179834255
    // ===================================
    // puzzle_03_test_simple_regex_part_02_test_01();
    // puzzle_03_test_simple_regex_part_02_test_02();
    // puzzle_03_part_02();                     // Result:  80570939
}

// ===========================================================================
// End-of-File
// ===========================================================================
