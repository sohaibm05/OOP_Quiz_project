#include "TrueFalse.hpp"
#include <algorithm>
#include <iostream>

TrueFalse::TrueFalse(std::string id, std::string text, float marks): Question(id, text, marks), correctAnswer(false) {}

void TrueFalse::setCorrectAnswer(bool isTrue) 
{ 
    correctAnswer = isTrue; 
}

bool TrueFalse::checkAnswer(std::string answer) const 
{
    std::string a = answer;
    std::transform(a.begin(), a.end(), a.begin(), ::tolower);

    if (a == "true" || a == "t" || a == "1" || a == "yes") 
        return correctAnswer == true;
    if (a == "false" || a == "f" || a == "0" || a == "no") 
        return correctAnswer == false;

    return false;
}

void TrueFalse::displayQuestion() const 
{
    std::cout << questionText << " (True/False)" << std::endl;
}
