
// ===========================================================================
// Puzzle11.cpp
// ===========================================================================

#include "../Logger/ScopedTimer.h"

#include <algorithm>
#include <cassert>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

// ===========================================================================
// global data

static std::string_view g_filenameTestData{ "Puzzle11_TestData.txt" };
static std::string_view g_filenameRealData{ "Puzzle11_RealData.txt" };

// ===========================================================================
// types

class PlutonianPebbles
{
protected:
    std::forward_list<size_t> m_pebbles{};
    size_t                    m_size;

public:
    PlutonianPebbles() : m_pebbles{}, m_size{} {}

    virtual void readPuzzleFromFile(const std::string_view filename)
    {
        std::ifstream file{ filename.data() };
        std::string line{};

        if (file.is_open()) {

            // read a single line
            std::getline(file, line);

            std::stringstream ss{ line };
            std::string token;

            auto pos{ m_pebbles.before_begin() };

            // extract all pebbles from this line
            while (ss >> token) {

                // convert std::string to size_t
                size_t pebble{};
                std::istringstream iss{ token };
                iss >> pebble;

                // insert pebble at end of list
                m_pebbles.insert_after(pos, pebble);
                m_size++;

                ++pos;
            }

            file.close();
        }
        else {

            std::println("Unable to open file {} !", filename);
        }
    }

    virtual void printPebbles()
    {
        for (const auto pebble: m_pebbles) {
            std::print("{} ", pebble);
        }
        std::println();
    }

    virtual void blink()
    {
        auto pos{ m_pebbles.begin() };

        while (pos != m_pebbles.end()) {

            size_t pebble{ *pos };

            if (pebble == 0) {
                *pos = 1;
            }
            else if (hasEvenDigits(pebble)) {

                const auto [leftHalf, rightHalf] = splitPebble(pebble);
                *pos = leftHalf;
                m_pebbles.insert_after(pos, rightHalf);
                m_size++;

                ++pos; // skip new right half
            }
            else {
                *pos *= 2024;
            }

            ++pos;
        }
    }

    void blinking(size_t count)
    {
        for (size_t i{}; i != count; ++i) {
            std::println("{} blinking ...", (i+1));
            blink();
            printPebbles();
        }
    }

    virtual size_t size()
    {
        return m_size;
    }

protected:
    static std::pair<size_t, size_t> splitPebble(size_t pebble)
    {
        std::string s{ std::to_string(pebble) };

        size_t length{s.size()};

        length /= 2;
        std::string leftPart{ s.substr(0,length) };
        std::string rightPart{ s.substr(length) };

        // convert two halfs to size_t
        size_t left{}, right{};
        std::istringstream iss{ leftPart };
        iss >> left;
        iss = std::istringstream{ rightPart };
        iss >> right;

        return { left , right };
    }

    static bool hasEvenDigits(size_t pebble)
    {
        int numDigits{};
        while (pebble != 0) {
            pebble /= 10;
            ++numDigits;
        }
        return (numDigits % 2) == 0;
    }
};

class AdvancedPlutonianPebbles : public PlutonianPebbles
{
private:
    std::unordered_map<size_t, size_t> m_countedPebbles;

public:
    AdvancedPlutonianPebbles() : PlutonianPebbles {} {}

    virtual void readPuzzleFromFile(const std::string_view filename) override {

        PlutonianPebbles::readPuzzleFromFile(filename);

        // initialize dictionary
        for (auto pebble : m_pebbles) {
            m_countedPebbles[pebble] = 1;
        }
    }

    virtual void blink() override
    {
        std::unordered_map copyMap{ m_countedPebbles };

        for (auto [pebble, count] : copyMap) {

            if (pebble == 0) {
                m_countedPebbles[pebble] = count - 1;
                m_countedPebbles[1] = count + 1;
            }
            else if (hasEvenDigits(pebble)) {  // TODO : schneller berechnen

                const auto [leftHalf, rightHalf] = splitPebble(pebble);

                m_countedPebbles[pebble] = count - 1;  // remove current pebble

                // add two new pebbles
                m_countedPebbles[leftHalf] += 1;
                m_countedPebbles[rightHalf] += 1;
            }
            else {
                m_countedPebbles[pebble * 2024] += 1;
            }
        }
    }

    virtual size_t size()
    {
        m_size = 0;

        for (auto [pebble, count] : m_countedPebbles) {
            m_size += count;
        }

        return m_size;
    }

    virtual void printPebbles() override
    {
        for (const auto [pebble, count] : m_countedPebbles) {
            std::print("{} [{}] ", pebble, count);
        }
        std::println();
    }
};

// ===========================================================================
// forward declarations

// ===========================================================================
// input & output

// ===========================================================================
// types / logic

// ===========================================================================
// testing

static void puzzle_10_test()
{
    ScopedTimer watch{};
    PlutonianPebbles pebbles;
    pebbles.readPuzzleFromFile(g_filenameTestData);
    pebbles.printPebbles();
    //std::println("Size: {}", pebbles.size());
    pebbles.blinking(5);
    //std::println("Size: {}", pebbles.size());   // expected 213625 (25 times blinking)
}

static void puzzle_11_test()
{
    ScopedTimer watch{};
    AdvancedPlutonianPebbles pebbles;
    pebbles.readPuzzleFromFile(g_filenameTestData);
    pebbles.printPebbles();
    //std::println("Size: {}", pebbles.size());
    pebbles.blinking(3);
    //std::println("Size: {}", pebbles.size());   // expected 213625 (25 times blinking)
}

static void puzzle_12_test()
{
}

// ===========================================================================
// part one

static void puzzle_11_part_one()
{
    ScopedTimer watch{};
    PlutonianPebbles pebbles;
    pebbles.readPuzzleFromFile(g_filenameRealData);
    pebbles.printPebbles();
    std::println("Size: {}", pebbles.size());
    pebbles.blinking(25);
    std::println("Size: {}", pebbles.size());   // expected 213625 (25 times blinking)
}

// ===========================================================================
// part two

static void puzzle_11_part_two()
{
    ScopedTimer watch{};
    AdvancedPlutonianPebbles pebbles;
    pebbles.readPuzzleFromFile(g_filenameRealData);
    pebbles.printPebbles();
    std::println("Size: {}", pebbles.size());
    pebbles.blinking(75);
    std::println("Size: {}", pebbles.size());   // expected 
}

// ===========================================================================
// main

void puzzle_10()
{
    //puzzle_10_test();
    puzzle_11_test();
    //puzzle_12_test();

    //puzzle_11_part_one();
    //puzzle_11_part_two();
}

// ===========================================================================
// End-of-File
// ===========================================================================
