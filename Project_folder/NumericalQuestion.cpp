#include "NumericalQuestion.hpp"
#include <iostream>
using namespace std;

NumericalQuestion::NumericalQuestion(string id, string text, float marks): Question(id, text, marks), correctAnswer(0.0), tolerance(0.0) {}

void NumericalQuestion::setCorrectAnswer(double answer, double tol) 
{
    correctAnswer = answer;
    tolerance = tol;
}

bool NumericalQuestion::checkAnswer(string answer) const // override base virtual
{
    try 
    {
        double val = stod(answer);
        double diff = abs(val - correctAnswer); // absolute difference
        if (tolerance > 0.0 && tolerance < 1.0) 
        {
            return diff <= abs(correctAnswer) * tolerance; //  relative tolerance
        }
        return diff <= tolerance;
    } catch (...) {return false;}
}

void NumericalQuestion::displayQuestion() const // override base virtual
{ 
    cout << questionText << endl; 
}
