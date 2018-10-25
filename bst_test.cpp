//
//  CS263-Trees
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "BinaryTree.hpp"

struct TreeTestListener : Catch::TestEventListenerBase {
    using TestEventListenerBase::TestEventListenerBase;

    void testCaseStarting(Catch::TestCaseInfo const& info) override {
        cout << "Start of " << info.name << endl;
        if (gv::BinaryTree<string>::hasLeak()) {
            fprintf(stderr, "%s has %d nodes leaked\n", info.name.c_str(),
                    gv::BinaryTree<string>::allocatedNodes());
        }
    }

    void testCaseEnded(Catch::TestCaseStats const& stat) override {
        if (gv::BinaryTree<string>::hasLeak()) {
            fprintf(stderr, "%s has %d nodes leaked\n", stat.testInfo.name.c_str(),
                    gv::BinaryTree<string>::allocatedNodes());
        }
    }

};

CATCH_REGISTER_LISTENER(TreeTestListener)

class TreeTest {
public:
    TreeTest() {
        for (auto& x : refData)
            items.insert(x);
    }

    vector<string> refData {
        "ram",
        "memory",
        "keyboard",
        "cpu",
        "gpu",
        "mouse",
        "monitor",
        "usb",
        "wireless",
        "ssd",
        "dvd",
        "lcd",
        "vga",
        "bluetooth",
        "hdmi",
        "speakers",
        "videocam",
        "microphone",
        "power",
        "firewire"};
    gv::BinaryTree<string> items;
};


TEST_CASE_METHOD(TreeTest, "All functions shall work on empty trees") {
    gv::BinaryTree<int> nothing;

    CHECK(nothing.isEmpty());
    CHECK_FALSE(nothing.contains(-99));
    CHECK_THROWS_AS(nothing.remove(87), length_error);
    CHECK_THROWS_AS(nothing.findMin(), length_error);
    CHECK_THROWS_AS(nothing.findMax(), length_error);
}

TEST_CASE_METHOD(TreeTest, "singleton") {
    gv::BinaryTree<int> onlyOne;
    onlyOne.insert(57);
    CHECK_FALSE(onlyOne.isEmpty());
    CHECK(onlyOne.contains(57));
    CHECK(onlyOne.findMax() == 57);
    CHECK(onlyOne.findMin() == 57);
}

TEST_CASE_METHOD (TreeTest, "contains")
{
    CHECK_NOTHROW(items.contains("power"));
    for (auto& p : refData)
        CHECK (items.contains(p));
    CHECK_FALSE (items.contains("POWER"));
}

TEST_CASE_METHOD (TreeTest, "insert should not accept duplicate items")
{
    for (auto& p : refData)
        CHECK_FALSE(items.insert(p));
}

TEST_CASE_METHOD(TreeTest, "minimum/max element") {
    vector<string> tmp {refData};
    sort(tmp.begin(), tmp.end());

    CHECK (items.findMin() == tmp[0]);
    CHECK (items.findMax() == tmp[tmp.size() - 1]);
};

TEST_CASE_METHOD (TreeTest, "remove should discard items")
{
    for (auto& p : refData) {
        items.remove(p);
        CHECK_FALSE (items.contains(p));
    }
}

TEST_CASE_METHOD(TreeTest, "makeEmpty must clear all elements") {
    items.makeEmpty();
    CHECK (items.numberOfNodes() == 0);
    CHECK (items.isEmpty());
}

TEST_CASE_METHOD(TreeTest, "printTree") {
    stringstream out;
    items.printTree(out);
    string s;
    vector<string> aref { refData };

    sort (aref.begin(), aref.end());
    int k = 0;
    while (out >> s) {
        CHECK(s == aref[k]);
        k++;
    }
    CHECK(k == refData.size());
}

TEST_CASE_METHOD(TreeTest, "printTree non recursive") {
    stringstream out;
    items.printTree(out);
    string s;
    vector<string> aref { refData };

    sort (aref.begin(), aref.end());
    int k = 0;
    while (out >> s) {
        CHECK(s == aref[k]);
        k++;
    }
    CHECK(k == refData.size());
}


TEST_CASE_METHOD(TreeTest, "Tree counters ")
{
    /* Tester function for Question 4.13 a-c */
    gv::BinaryTree<string> ctest;

    CHECK (ctest.numberOfNodes() == 0);
    CHECK (ctest.numberOfLeaves() == 0);
    CHECK (ctest.numberOfFullNodes() == 0);
    int k = 0;
    for (auto& x : refData)
    {
        k++;
        ctest.insert (x);
        int F = ctest.numberOfFullNodes();
        int L = ctest.numberOfLeaves();
        CHECK (ctest.numberOfNodes() == k);
        CHECK (L == F + 1);
    }
}

TEST_CASE_METHOD(TreeTest, "Leaves are removed") {
    vector<vector<string>> refData;
    refData.push_back({"bluetooth", "firewire", "hdmi", "lcd", "microphone", "power", "speakers", "videocam"});
    refData.push_back({"dvd", "monitor", "ssd", "vga"});
    refData.push_back({"gpu", "mouse", "wireless"});
    refData.push_back({"cpu", "usb"});
    refData.push_back({"keyboard"});
    refData.push_back({"memory"});
    refData.push_back({"ram"});

    int k = 0;
    while (!items.isEmpty() && k < refData.size()) {
        items.remove_leaves();
        for (auto &s : refData[k]) {
            CHECK_FALSE(items.contains(s));
        }
        k++;
    }
    CHECK(items.isEmpty());
}


TEST_CASE_METHOD(TreeTest, "level order")
{
    /* Tester function for Question 4.40 */
    vector<string> level = items.levelOrder();
    CHECK (!level.empty());
    vector<string> expected {"ram", "memory", "usb", "keyboard", "mouse",
        "ssd", "wireless", "cpu", "lcd", "monitor", "power", "speakers", "vga",
        "bluetooth", "gpu", "microphone", "videocam", "dvd", "hdmi",
        "firewire"};
    CHECK (level == expected);
}
