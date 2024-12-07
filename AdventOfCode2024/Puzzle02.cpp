// ===========================================================================
// Puzzle02.cpp
// ===========================================================================

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <print>
#include <sstream>
#include <string>
#include <vector>

// ===========================================================
// input

static std::list <std::vector<int>> puzzle_02_test_data()
{
    std::list <std::vector<int>> list;

    list.push_back({ 7, 6, 4, 2, 1 });
    list.push_back({ 1, 2, 7, 8, 9 });
    list.push_back({ 9, 7, 6, 2, 1 });
    list.push_back({ 1, 3, 2, 4, 5 });
    list.push_back({ 8, 6, 4, 4, 1 });
    list.push_back({ 1, 3, 6, 7, 9 });

    return list;
}

static std::list <std::vector<int>> puzzle_02_real_data()
{
    std::list <std::vector<int>> reports{};

    std::string name{ "Puzzle02_Data.txt" };
    std::ifstream file{ name };

    if (file.is_open()) {

        std::string line{};

        while (std::getline(file, line)) {

            std::vector<int> report;

            std::stringstream ss{ line };
            std::string token;

            // extract all tokens from this line
            while (ss >> token) { 

                int number = std::stoi(token);
                report.push_back(number);
            }

            reports.push_back(report);
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", name);
    }

    return reports;
}

static std::list<std::vector<int>> puzzle_02_partial_real_data(int lineNumber)
{
    std::list <std::vector<int>> reports{};

    int lineCounter{};

    std::string name{ "Puzzle02_Data.txt" };
    std::ifstream file{ name };

    if (file.is_open()) {

        std::string line{};

        while (std::getline(file, line)) {

            std::println("next line: {}", line);

            std::vector<int> report;

            std::stringstream ss{ line };
            std::string token;

            // extract all tokens from this line
            while (ss >> token) {

                int number = std::stoi(token);
                report.push_back(number);
            }

            reports.push_back(report);

            lineCounter++;
            if (lineCounter >= lineNumber) {
                break;
            }
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", name);
    }

    return reports;
}

// ===========================================================
// logic

static bool validateReport (const std::vector<int>& report)
{
    enum class ReportStatus { Indifferent, Increasing, Decreasing };

    ReportStatus status{ ReportStatus::Indifferent };

    bool firstComparison{ true };

    auto pos = std::adjacent_find(

        report.begin(),
        report.end(),
        
        [&] (int first, int second) -> bool {

            // check first pair
            if (firstComparison) {

                if (first == second) {

                    status = ReportStatus::Indifferent;
                }
                else if (first < second) {
                    
                    int diff = second - first;
                    if (diff >= 4) {
                        status = ReportStatus::Indifferent;
                    }
                    else {
                        status = ReportStatus::Increasing;
                    }
                }
                else
                {
                    int diff = first - second;
                    if (diff >= 4) {
                        status = ReportStatus::Indifferent;
                    }
                    else {
                        status = ReportStatus::Decreasing;
                    }

                }

                firstComparison = false;
                return false;
            }

            // check remaining pairs
            if (status == ReportStatus::Increasing) {

                if (first >= second) {
                    status = ReportStatus::Indifferent;
                }
                else {
                    int diff = second - first;
                    if (diff >= 4) {
                        status = ReportStatus::Indifferent;
                    }
                }
            }
            else if (status == ReportStatus::Decreasing) {

                if (second >= first) {
                    status = ReportStatus::Indifferent;
                }
                else {
                    int diff = first - second;
                    if (diff >= 4) {
                        status = ReportStatus::Indifferent;
                    }
                }
            }

            return false;
        }
    );

    return (status == ReportStatus::Indifferent) ? false : true;
}

static std::list<std::vector<int>> createPartialReports(const std::vector<int>& report)
{
    std::list<std::vector<int>> reports{};

    for (int index{}; index != report.size(); ++index) {

        std::vector<int> tmp{report};
        auto begin{ tmp.begin() };
        tmp.erase(begin + index);
        reports.push_back(tmp);
    }

    return reports;
}

// ===========================================================
// test

static size_t puzzle_02_test_01 ()
{
    std::vector<int> report{ 1, 3, 6, 7, 9 };

    bool result{ validateReport(report) };

    std::println("{}", result);

    return 1;
}

static size_t puzzle_02_test_02()
{
    auto testData{ puzzle_02_test_data() };

    size_t safeReports{};

    for (const auto& report : testData) {

        bool safe{ validateReport(report) };
        std::println("{}", safe);

        if (safe) {
            safeReports++;
        }
    }

    return safeReports;
}

static size_t puzzle_02_test_03_part_one()
{
    // auto testData{ puzzle_02_partial_real_data(100) };
    auto testData{ puzzle_02_real_data() };

    size_t safeReports{};

    for (const auto& report : testData) {

        bool safe{ validateReport(report) };

        if (safe) {
            safeReports++;
        }
    }

    return safeReports;
}

static void puzzle_02_test_04_part_two_create_partial_reports()
{
    std::vector<int> report{ 7, 6, 4, 2, 1 };
    std::println("{}", report);

    std::list<std::vector<int>> reports{ createPartialReports(report) };
    for (auto& report : reports) {

        std::println("{}", report);
    }
}

static size_t puzzle_02_test_03_part_two()
{
    auto testData{ puzzle_02_real_data() };

    size_t safeReports{};

    for (const auto& report : testData) {

        bool safe{ validateReport(report) };

        if (safe) {
            safeReports++;
        }
        else
        {
            // reduce report by one level - one after the other
            std::list<std::vector<int>> partialReports{ createPartialReports(report) };

            for (const auto& report : partialReports) {

                bool safe{ validateReport(report) };
                if (safe) {
                    safeReports++;
                    break;
                }
            }
        }
    }

    return safeReports;
}

void puzzle_02()
{
    std::println("Puzzle 2:");

    size_t safeReports{};

    safeReports = puzzle_02_test_01();
    std::println("Safe Reports I:  {}", safeReports);

    safeReports = puzzle_02_test_02();
    std::println("Safe Reports I:  {}", safeReports);

    safeReports = puzzle_02_test_03_part_one();
    std::println("Safe Reports I:  {}", safeReports);   // Expecting 326

    // puzzle_02_test_04_part_two_create_partial_reports();

    safeReports = puzzle_02_test_03_part_two();
    std::println("Safe Reports II: {}", safeReports);   // Expecting 381
}

// ===========================================================================
// End-of-File
// ===========================================================================
