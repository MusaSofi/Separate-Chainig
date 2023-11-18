#include <iostream>
#include "ADS_set.h"
#include <vector>

using namespace std;

int main()
{
    ADS_set<int> test;
    test.dump();
    ADS_set<int> test1{0, 30, 62, 1, 2, 8, 3, 4, 5, 9, 16, 31};
    test1.dump();
    ADS_set<int> test2(test1);
    test2.dump();
    test1.erase(9);

    auto firstElem = test1.begin();
    cout << "first Element: " << *firstElem << endl;

    auto second = firstElem++;
    cout << "second Element: " << *second << endl;
    auto third = ++second;
    cout << "third Element: " << *third << endl;
    auto fours = ++third;
    cout << "fours Element: " << *fours << endl;
    auto five = ++fours;
    cout << "five Element: " << *five << endl;
    auto six = ++five;
    cout << "six Element: " << *six << endl;
    auto seven = ++six;
    cout << "seven Element: " << *seven << endl;
    auto eight = ++seven;
    cout << "eight Element: " << *eight << endl;
    auto nine = ++eight;
    cout << "nine Element: " << *nine << endl;
    auto ten = ++nine;
    cout << "ten Element: " << *ten << endl;
    auto eleven = ++ten;
    cout << "eleven Element: " << *eleven << endl;

    ADS_set<std::string> testS;
    testS.insert("c-666");
    testS.dump();

    cout << "_____________ITBEGIN_____________________" << endl;
    auto itbegin = testS.begin();
    auto afterBegin = ++itbegin;
    auto itend = testS.end();
    cout << (afterBegin == itend) << endl;
    cout << "_______________Iterator__________________" << endl;

    for (auto elem : testS)
        cout << elem << endl;

    ADS_set<int> test1_1{1, 16, 31, 2, 3, 4, 5, 6, 7, 8};
    test1_1.dump();
    auto it = test1_1.begin();
    auto end = test1_1.end();
    cout << "size: " << test1_1.size() << endl;
    for (auto i = it; i != end; i++)
    {
        cout << *i << endl;
    }
    ADS_set<int> test2_1{1, 2, 3};
    auto it2 = test2_1.begin();
    auto end2 = test2_1.end();
    cout << "size: " << test2_1.size() << endl;
    for (auto i = it2; i != end2; i++)
    {
        cout << *i << endl;
    }

    ADS_set<int> test3_1{11, 21, 31, 44, 55, 66, 77, 99};
    auto it3 = test3_1.begin();
    auto end3 = test3_1.end();
    cout << "size: " << test3_1.size() << endl;
    for (auto i = it3; i != end3; i++)
    {
        cout << *i << endl;
    }
    return 0;
}