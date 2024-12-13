// ===========================================================================
// Puzzle07.cpp
// ===========================================================================

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
#include <sstream>
#include <stack>
#include <vector>
#include <string>

// https://www.geeksforgeeks.org/binary-tree-in-cpp/
// 
// https://www.geeksforgeeks.org/print-leaf-nodes-left-right-binary-tree/
// 

// ===========================================================================
// global data

static std::string g_filenameTestData{ "Puzzle07_TestData.txt" };
static std::string g_filenameRealData{ "Puzzle07_RealData.txt" };

// ===========================================================================
// types

struct Equation
{
    size_t           m_value{};
    std::vector<int> m_operands{};
};

static std::vector<Equation> g_equations{};

// ===========================================================================
// forward declarations

static void readPuzzleFromFile(const std::string& filename);
static void puzzle_07_parseLine(const std::string& line, Equation& equation);

// ===========================================================================
// input & output

static void readPuzzleFromFile(const std::string& filename) {

    std::ifstream file{ filename };

    if (file.is_open()) {

        // read line per line
        std::string line{};

        while (std::getline(file, line)) {

            if (line.empty()) {
                break;
            }

            //std::println("Line: {} !", line);

            Equation equation;
            puzzle_07_parseLine(line, equation);
            g_equations.push_back(equation);
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", filename);
    }
}

static void puzzle_07_parseLine(const std::string& line, Equation& equation)
{
    std::string delimiter{ ":" };
    size_t pos{ line.find(delimiter) };
    std::string token{ line.substr(0, pos) };

    // convert std::string to size_t
    std::istringstream iss{ token };
    iss >> equation.m_value;

    std::string tail{ line.substr(pos + 2) };
    delimiter = " ";
    int operand{};

    while ((pos = tail.find(delimiter)) != std::string::npos) {

        token = tail.substr(0, pos);
        operand = std::stoi(token);
        equation.m_operands.push_back(operand);

        tail.erase(0, pos + 1);
    }

    operand = std::stoi(tail);
    equation.m_operands.push_back(operand);
}

// ===========================================================================
// types / logic

class SimpleBinaryTree
{
private:
    class Node {
    public:
        size_t m_data;
        Node* m_left;
        Node* m_right;

        Node(size_t value) : m_data{ value }, m_left{ nullptr }, m_right{ nullptr } {}
    };

private:
    Node* m_root;

public:
    SimpleBinaryTree() : m_root{ nullptr } {}

    void inorderInsert(int elem)
    {
        if (m_root == nullptr) {
            m_root = new Node(elem);
        }
        else
        {
            inorderInsertRecursive(m_root, elem);
        }
    }

    void inorderInsertRecursive(Node* node, int elem)
    {
        if (node->m_left != nullptr && node->m_right != nullptr)
        {
            inorderInsertRecursive(node->m_left, elem);
            inorderInsertRecursive(node->m_right, elem);
        }
        else
        {
            node->m_left = new Node(node->m_data + elem);
            node->m_right = new Node(node->m_data * elem);
        }
    }

    void printLeaveNodes()
    {
        printLeaveNodesInternal(m_root);
    }

    void printLeaveNodesInternal(Node* node)
    {
        if (node == nullptr) {
            return;
        }

        std::stack<Node*> stack;
        stack.push(node);

        while (! stack.empty()) {

            node = stack.top();
            stack.pop();

            //if (node->m_left == nullptr && node->m_right == nullptr) {
            //}

            if (node->m_right != nullptr) {
                stack.push(node->m_right);
            }

            if (node->m_left != nullptr) {
                stack.push(node->m_left);
            }
        }
    }

    std::vector<size_t> getLeaveNodes()
    {
        std::vector<size_t> leaveNodes{};

        getLeaveNodesInternal(m_root, leaveNodes);

        return leaveNodes;
    }

   void getLeaveNodesInternal(Node* node, std::vector<size_t>& values)
    {
        if (node == nullptr) {
            return;
        }

        std::stack<Node*> stack;
        stack.push(node);

        while (!stack.empty()) {

            node = stack.top();
            stack.pop();

            if (node->m_left == nullptr && node->m_right == nullptr) {

                values.push_back(node->m_data);
            }

            if (node->m_right != nullptr) {
                stack.push(node->m_right);
            }

            if (node->m_left != nullptr) {
                stack.push(node->m_left);
            }
        }
    }
};

// ===========================================================================================

class SimpleTernaryTree
{
private:
    class Node {
    public:
        size_t m_data;

        Node* m_left;
        Node* m_middle;
        Node* m_right;

        Node(size_t value) : m_data{ value }, m_left{ nullptr }, m_middle{ nullptr },  m_right { nullptr } {}
    };

private:
    Node* m_root;

public:
    SimpleTernaryTree() : m_root{ nullptr } {}

    void inorderInsert(size_t elem)
    {
        if (m_root == nullptr) {
            m_root = new Node{ elem };
        }
        else
        {
            inorderInsertRecursive(m_root, elem);
        }
    }

    size_t concatenate(size_t first, size_t second) {

        std::string s1{ std::to_string(first) };
        std::string s2{ std::to_string(second) };

        // convert std::string to size_t
        std::istringstream iss{ s1 + s2 };
        size_t result{};
        iss >> result;
        return result;
    }

    void inorderInsertRecursive(Node* node, size_t elem)
    {
        if (node->m_left != nullptr && node->m_middle != nullptr && node->m_right != nullptr)
        {
            inorderInsertRecursive(node->m_left, elem);
            inorderInsertRecursive(node->m_middle, elem);
            inorderInsertRecursive(node->m_right, elem);
        }
        else
        {
            size_t concatenatedNumber = concatenate(node->m_data, elem);

            node->m_left = new Node(node->m_data + elem);
            node->m_middle = new Node(concatenatedNumber);
            node->m_right = new Node(node->m_data * elem);
        }
    }

    void printLeaveNodes()
    {
        printLeaveNodesInternal(m_root);
    }

    void printLeaveNodesInternal(Node* node)
    {
        if (node == nullptr) {
            return;
        }

        std::stack<Node*> stack;
        stack.push(node);

        while (!stack.empty()) {

            node = stack.top();
            stack.pop();

            //if (node->m_left == nullptr && node->m_middle == nullptr && node->m_right == nullptr) {
            //    // std::println("Data: {}", node->m_data);
            //}

            if (node->m_right != nullptr) {
                stack.push(node->m_right);
            }

            if (node->m_middle != nullptr) {
                stack.push(node->m_middle);
            }

            if (node->m_left != nullptr) {
                stack.push(node->m_left);
            }
        }
    }

    std::vector<size_t> getLeaveNodes()
    {
        std::vector<size_t> leaveNodes{};

        getLeaveNodesInternal(m_root, leaveNodes);

        return leaveNodes;
    }

    void getLeaveNodesInternal(Node* node, std::vector<size_t>& values)
    {
        if (node == nullptr) {
            return;
        }

        std::stack<Node*> stack;
        stack.push(node);

        while (!stack.empty()) {

            node = stack.top();
            stack.pop();

            if (node->m_left == nullptr && node->m_middle == nullptr && node->m_right == nullptr) {

                values.push_back(node->m_data);
            }

            if (node->m_right != nullptr) {
                stack.push(node->m_right);
            }

            if (node->m_middle != nullptr) {
                stack.push(node->m_middle);
            }

            if (node->m_left != nullptr) {
                stack.push(node->m_left);
            }
        }
    }
};

// ===========================================================================
// test

static void test_01()
{
    Equation equation;

    puzzle_07_parseLine("6780304916: 251 7 5 8 657 64 8 3 8 5", equation);
}

static void test_02()
{
    SimpleBinaryTree tree{};

    tree.inorderInsert(2);
    tree.inorderInsert(3);
    tree.inorderInsert(4);
    tree.inorderInsert(5);

    tree.printLeaveNodes();
}

static bool validateEquation(const Equation& equation)
{
    SimpleBinaryTree tree{};

    for (int op : equation.m_operands) {
        tree.inorderInsert(op);
    }

    std::vector<size_t> results{ tree.getLeaveNodes() };

    auto pos{ std::find(
        std::begin(results),
        std::end(results),
        equation.m_value
    ) };

    return ! (pos == results.end());
}

static bool validateEquationEx(const Equation& equation)
{
    SimpleTernaryTree tree{};

    for (int op : equation.m_operands) {
        tree.inorderInsert(op);
    }

    tree.printLeaveNodes();

    std::vector<size_t> results{ tree.getLeaveNodes() };

    auto pos{ std::find(
        std::begin(results),
        std::end(results),
        equation.m_value
    ) };

    return !(pos == results.end());
}

static void validateAllEquations(size_t& result)
{
    result = 0;

    for (const auto& equation : g_equations) {

        bool isValid{ validateEquation(equation) };
        if (isValid) {
            result += equation.m_value;
        }
        else {
        }
    }
}

static void validateAllEquationsEx(size_t& result)
{
    result = 0;

    for (int counter{};  const auto & equation : g_equations) {

        bool isValid{ validateEquationEx(equation) };
        if (isValid) {
            result += equation.m_value;
        }

        ++counter;

        if (counter % 50 == 0) {
            std::println("Done: {}", counter);
        }
    }
 }

static void test_03()
{
    readPuzzleFromFile(g_filenameTestData);
}

// works for read data and for test data
static void puzzle_07_part_one()
{
    readPuzzleFromFile(g_filenameTestData);
    //readPuzzleFromFile(g_filenameRealData);

    size_t totalCalibrationResult{};
    validateAllEquations(totalCalibrationResult);
    std::println("Result: {}", totalCalibrationResult);
}

static void test_04()
{
    Equation equation;

    // 7290: 6 8 6 15

    equation.m_value = 7290;
    equation.m_operands.push_back(6);
    equation.m_operands.push_back(8);
    equation.m_operands.push_back(6);
    equation.m_operands.push_back(15);

    bool isValid{ validateEquationEx(equation) };

    std::println("isValid: {}", isValid);
}

static void test_04_01()
{
    Equation equation;

    // 156: 15 6 3

    equation.m_value = 156;
    equation.m_operands.push_back(15);
    equation.m_operands.push_back(6);
    equation.m_operands.push_back(3);

    bool isValid{ validateEquationEx(equation) };

    std::println("isValid: {}", isValid);
}

static void puzzle_07_part_two()
{
    //readPuzzleFromFile(g_filenameTestData);
    readPuzzleFromFile(g_filenameRealData);

    size_t totalCalibrationResult{};
    validateAllEquationsEx(totalCalibrationResult);
    std::println("Result: {}", totalCalibrationResult);
}

// ===========================================================================
// main

void puzzle_07()
{
    //test_01();
    //test_02();
    //test_03();

    // puzzle_07_part_one();      // expected 267566105056

    //test_04();

    puzzle_07_part_two();   // expected       116094961956019
}

// ===========================================================================
// End-of-File
// ===========================================================================
