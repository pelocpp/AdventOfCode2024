// ===========================================================================
// Puzzle07.cpp
// ===========================================================================

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <print>
#include <stack>
#include <queue>
#include <string>
#include <regex>

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

// ===========================================================================
// forward declarations

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

            std::println("Line: {} !", line);
        }

        file.close();
    }
    else {

        std::println("Unable to open file {} !", filename);
    }
}

static void puzzle_07_parseLine_regex(const std::string& line)
{
    // simple example - counting the number of all matches

    std::regex expression{ "([1-9][0-9]+):(\\s[1-9][0-9]+)*" };

  //  std::string text = { "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))" };

    int result{};

    auto first = std::sregex_iterator(
        line.begin(),
        line.end(),
        expression
    );

    auto iterator{ first };

    while (iterator != std::sregex_iterator{}) {

        const auto& token = *iterator;
        std::println("Found Token: {} - Size: {}", token.str(), token.size());

        //for (size_t i{}; i < token.size(); ++i) {
        //    std::println("{}: '{}'", i, token[i].str());
        //}

        ++iterator;
    }

    //std::ptrdiff_t const count{
    //    std::distance(first, std::sregex_iterator{})
    //};

    //std::println("Total Result: {}", result);

    //std::println("Count: {}", count);
}


static void puzzle_07_parseLine(const std::string& line)
{
  //  std::string s = "scott>=tiger";
    std::string delimiter = ":";
    size_t pos = line.find(delimiter);
    std::string token = line.substr(0, pos); 

    std::println("First Token: {}", token);

    // std::string tail = line.substr(line.size() - pos);
    std::string tail = line.substr(pos+2);
    delimiter = " ";

    while ((pos = tail.find(delimiter)) != std::string::npos) {
        token = tail.substr(0, pos);

        std::println("Another Token: {}", token);

        //tokens.push_back(token);
        tail.erase(0, pos + 1);
    }

    std::println("Last Token: {}", tail);
}

// ===========================================================================
// types / logic

class Node {
public:
    size_t m_data;
    Node*  m_left;
    Node*  m_right;

    Node(size_t value) : m_data{ value }, m_left{ nullptr }, m_right{ nullptr } {}
};

class BinaryTree {
private:
    Node* m_root;

public:
    BinaryTree() : m_root{ nullptr } {}

    // Function to insert a node in the binary tree 
    void insertNode(size_t value) {

        Node* node = new Node(value);

        if (m_root == nullptr) {
            m_root = node;
            return;
        }

        std::queue<Node*> q;
        q.push(m_root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (current->m_left == nullptr) {
                current->m_left = node;
                return;
            }
            else {
                q.push(current->m_left);
            }

            if (current->m_right == nullptr) {
                current->m_right = node;
                return;
            }
            else {
                q.push(current->m_right);
            }
        }
    }

    void inorderRecursive(Node* node)
    {
        if (node != nullptr) {
            inorderRecursive(node->m_left);
            std::print("{} ", node->m_data);
            inorderRecursive(node->m_right);
        }
    }

    void inorder() {
        inorderRecursive(m_root);
        std::println();
    }
};

class SimpleBinaryTree
{
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

            if (node->m_left == nullptr && node->m_right == nullptr) {

                std::println("Found leave: {}", node->m_data);
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

// ===========================================================================
// test

static void test_01()
{
  //  readPuzzleFromFile(g_filenameTestData);

    puzzle_07_parseLine("28883667: 640 6 1 9 5 2 7 59 1 6 93");
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

// ===========================================================================
// main

void puzzle_07()
{
    test_01();
  //  test_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
