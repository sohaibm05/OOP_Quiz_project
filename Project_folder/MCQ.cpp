#include "MCQ.hpp"
#include <iostream>
#include <algorithm>
using namespace std;
MCQ::MCQ(string id, string text, float marks): Question(id, text, marks), correctOption(-1) {}

void MCQ::addOption(string option)
{ 
    options.push_back(option); 
}

void MCQ::setCorrectOption(int index) 
{
    if (index >= 0 && index < static_cast<int>(options.size()))
    {
        correctOption = index;
    }
}

static string toLowerCopy(const string &s) // not neccesary will remove in final version keeping just incase i need for future 
{
    string r = s;
    transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

bool MCQ::checkAnswer(string answer) const 
{
    int idx = stoi(answer) - 1; // string to integer  the issue is this gives error if it fails to convert so we need to handle that 
    if (idx == correctOption) // will think about using try and catch
    {
        return true;
    }
    return false;
}

void MCQ::displayQuestion() const 
{
    cout << questionText << endl;
    for (size_t i = 0; i < options.size(); ++i) 
    {
        cout << (i + 1) << ") " << options[i] << endl;
    }
}
