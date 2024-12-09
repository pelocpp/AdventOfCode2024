// ===========================================================================
// Puzzle05.cpp
// ===========================================================================

#include <algorithm>
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

static std::unordered_map<int, std::set<int>> g_rules;
static std::list<std::vector<int>> g_updates; // vector

static std::string g_filenameTestData{ "Puzzle05_TestData.txt"};
static std::string g_filenameRealData{ "Puzzle05_Data.txt" };

// ===========================================================================
// forward declarations

static void parseUpdateLine(const std::string& line, std::vector<int>& list);
static bool isUpdateCorrect(const std::vector<int>& list, int& mid);
static void reorderIncorrectUdate(std::vector<int>& update, int& mid);

static void puzzle_part_one(const std::string& filename);
static void puzzle_part_two(const std::string& filename);

static void puzzle_part_two_simple_test(std::vector<int>& update);
static void puzzle_part_two_simple_test(std::vector<int>& update);

// ===========================================================================
// input

static void puzzle_05_parse_data(const std::string& fileName)
{
    std::ifstream file{ fileName };

    static std::vector<std::string> data;

    if (file.is_open()) {

        // read first part: ordering rules

        std::string line{};
        while (std::getline(file, line)) {

            if (line.empty()) {
                break;
            }

            // std::println("Rule: {}", line);

            // retrieve two page numbers
            std::string spage1 = line.substr(0, 2);
            std::string spage2 = line.substr(3, 5);

            int page1 = std::stoi(spage1);
            int page2 = std::stoi(spage2);

            std::unordered_map<int, std::set<int>>::iterator pos;

            pos = g_rules.find(page1);

            // update rules container
            if (! ( pos == g_rules.end())) {

                // key always present - update set of rules
                auto& value = g_rules[page1];
                value.insert(page2);
            }
            else {

                // key not yet present - insert first set of rules
                std::set<int> set{ page2 };
                g_rules.insert(std::make_pair(page1, set));
            }
        }

        // read second part: updates
        while (std::getline(file, line)) {

            std::println("Update: {}", line);

            std::vector<int> vec{};
            parseUpdateLine(line, vec);
            g_updates.push_back(vec);
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", fileName);
    }
}

static void parseUpdateLine(const std::string& line, std::vector<int>& list)
{
    std::regex expression{ "([1-9][0-9]+)" };

    auto first = std::sregex_iterator(
        line.begin(),
        line.end(),
        expression
    );

    auto iterator{ first };

    while (iterator != std::sregex_iterator{}) {

        const auto& token = *iterator;
        // std::println("Found Token: {}", token.str());

        // add "page number" to list
        list.push_back(std::stoi (token.str()));

        ++iterator;
    }

    std::ptrdiff_t const count{
        std::distance(first, std::sregex_iterator{})
    };

    std::println("Count: {}", count);
}

// ===========================================================================
// logic

static bool isUpdateCorrect(const std::vector<int>& vec, int& mid)
{
    auto pos = std::adjacent_find(

        vec.begin(),
        vec.end(),

        [](int n, int m) {

            // retrieve list of rules for page n
            const auto& rules = g_rules[n];

            // search for m
            if (rules.contains(m)) {

                // proceed to next pair of page numbers
                return false;
            }
            else {

                // no rule found - abort this verification
                //std::println("ERROR: No rule for {}|{} found !", n, m);
                return true;
            }
        }
    );

    bool isCorrect = (pos == vec.end()) ? true : false;

    if (isCorrect) {

        mid = *(vec.begin() + vec.size() / 2);

    }

    return isCorrect;
}

static void reorderIncorrectUdate(std::vector<int>& update, int& mid)
{
    auto pos = std::adjacent_find(

        update.begin(),
        update.end(),

        [](int& n, int& m) {

            // retrieve list of rules for page n
            const auto& rules = g_rules[n];

            // search for m
            if (rules.contains(m)) {

                // proceed to next pair of page numbers
                return false;
            }
            else {

                // no rule found - trying to repair this update - swap page numbers
                std::swap(n, m);

                return false;
            }
        }
    );

    mid = *(update.begin() + update.size() / 2);
}

// ===========================================================================
// logic

static void puzzle_part_one(const std::string& filename)
{
    puzzle_05_parse_data(filename);

    int total{};

    for (const auto& update : g_updates) {

        std::print("{} ", update);

        int mid{};

        bool isCorrect{ isUpdateCorrect(update, mid) };

        total += mid;

        std::println("Update correct: {} - mid: {}", isCorrect, mid);
    }

    std::println("Total: {}", total);
}

static void puzzle_part_two_simple_test()
{
    std::vector<int> update1{ 75,97,47,61,53 };
    std::vector<int> update2{ 61,13,29 };
    std::vector<int> update3{ 97,13,75,29,47 };

    puzzle_part_two_simple_test(update1);
}

static void puzzle_part_two_simple_test(std::vector<int>& update)
{
    puzzle_05_parse_data(g_filenameTestData);

    int total{};

    std::print("Incorrect Update: {}", update);

    int mid{};

    bool isCorrect{ isUpdateCorrect(update, mid) };

    if (!isCorrect) {

        do {
            reorderIncorrectUdate(update, mid);

            std::println("Modified Update: {}", update);

        } while (!isUpdateCorrect(update, mid));

        total += mid;
    }

    std::println("Update is now correct: {}", isCorrect);

    std::println("Total: {}", total);
}

static void puzzle_part_two(const std::string& filename)
{
    puzzle_05_parse_data(filename);

    int total{};

    for (auto& update : g_updates) {

        int mid{};

        bool isCorrect{ isUpdateCorrect(update, mid) };

        if (!isCorrect) {

            do {
                reorderIncorrectUdate(update, mid);
            } while (!isUpdateCorrect(update, mid));

            total += mid;
        }
    }

    std::println("Total: {}", total);
}

// ===========================================================================
// main

void puzzle_05()
{
    // puzzle_part_one(g_filenameRealData);  //  6034
    // puzzle_part_two_simple_test();

    puzzle_part_two(g_filenameRealData);     // 6305
}

// ===========================================================================
// End-of-File
// ===========================================================================
