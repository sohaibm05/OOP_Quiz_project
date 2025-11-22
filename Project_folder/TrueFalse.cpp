#include "TrueFalse.hpp"
#include <algorithm>
#include <iostream>
using namespace std;

TrueFalse::TrueFalse(string id, string text, float marks): Question(id, text, marks), correctAnswer(false) {}

void TrueFalse::setCorrectAnswer(bool isTrue) 
{ 
    correctAnswer = isTrue; 
}

bool TrueFalse::checkAnswer(string answer) const 
{
    string a = answer;
    transform(a.begin(), a.end(), a.begin(), ::tolower); // Convert to lowercase for case-insensitive comparison

    if (a == "true" || a == "t" || a == "1" || a == "yes")
    { 
        return correctAnswer == true;
    }
    if (a == "false" || a == "f" || a == "0" || a == "no")
    { 
        return correctAnswer == false;
    }
    return false;
}

void TrueFalse::displayQuestion() const 
{
    cout << questionText << " (True/False)" << endl;
}
