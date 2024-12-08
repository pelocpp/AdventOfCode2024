// ===========================================================================
// Puzzle05.cpp
// ===========================================================================

#include <fstream>
#include <list>
#include <print>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// ===========================================================================
// global data

static std::unordered_map<int, std::unordered_set<int>> g_rules;

std::string g_filenameTestData{ "Puzzle05_TestData.txt"};
std::string g_filenameData{ "Puzzle05_Data.txt" };

// ===========================================================================
// input

// 
static void puzzle_05_read_data(const std::string& fileName)
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

            std::println("Rule: {}", line);

            // retrieve two page numbers
            std::string spage1 = line.substr(0, 2);
            std::string spage2 = line.substr(3, 5);

            int page1 = std::stoi(spage1);
            int page2 = std::stoi(spage2);

            std::unordered_map<int, std::unordered_set<int>>::iterator pos;

            pos = g_rules.find(page1);

            // update rules container
            if (! ( pos == g_rules.end())) {

                // key always present - update set of rules
                auto& value = g_rules[page1];
                value.insert(page2);
            }
            else {

                // key not yet present - insert first set of rules
                std::unordered_set<int> set{ page2 };
                g_rules.insert(std::make_pair(page1, set));
            }
        }

        // read second part: updates
        while (std::getline(file, line)) {

            std::println("Update: {}", line);
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", fileName);
    }

   // return data;
}


// ===========================================================================
// main

void puzzle_05()
{
    puzzle_05_read_data(g_filenameTestData);
}

// ===========================================================================
// End-of-File
// ===========================================================================
