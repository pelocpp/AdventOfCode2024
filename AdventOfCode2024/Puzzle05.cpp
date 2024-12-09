// ===========================================================================
// Puzzle05.cpp
// ===========================================================================

#include <algorithm>
#include <fstream>
#include <list>
#include <print>
#include <regex>
#include <set>
#include <vector>
#include <string>
#include <unordered_map>

// ===========================================================================
// global data

static std::unordered_map<int, std::set<int>> g_rules;
static std::vector<std::list<int>> g_updates;

std::string g_filenameTestData{ "Puzzle05_TestData.txt"};
std::string g_filenameData{ "Puzzle05_Data.txt" };

// ===========================================================================
// forward declarations

static void parseUpdateLine(const std::string& line, std::list<int>& list);
static bool verifyUpdate(const std::list<int>& list);

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

            std::list<int> list{};
            parseUpdateLine(line, list);
            g_updates.push_back(list);
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", fileName);
    }
}

static void parseUpdateLine(const std::string& line, std::list<int>& list)
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

static bool verifyUpdate(const std::list<int>& list)
{
    auto pos = std::adjacent_find(

        list.begin(),
        list.end(),
        [](int n, int m){

            std::println("n: {} - m: {}", n, m);

            // retrieve list of rules for page n
            const auto& rules = g_rules[n];

            // search for m
            if (!rules.contains(m)) {

                // proceed to next pair of page numbers

                std::println("Found rule n|m !", n, m);

                return false;
            }
            else {

                // no rule found - abort this verification

                std::println("ERROR: No rule for n|m found !", n, m);

                return true;
            }
        }
    );

    return (pos == list.end()) ? true : false;
}

// ===========================================================================
// main

void puzzle_05()
{
    puzzle_05_parse_data(g_filenameTestData);

    const auto& testList = g_updates[0];

    verifyUpdate(testList);
}

// ===========================================================================
// End-of-File
// ===========================================================================
