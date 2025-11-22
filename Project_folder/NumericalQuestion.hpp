#ifndef NUMERICALQUESTION_HPP
#define NUMERICALQUESTION_HPP

#include "Question.hpp"
#include <string>
#include <cmath>

using namespace std;

class NumericalQuestion : public Question 
{
private:
    double correctAnswer;
    double tolerance; // e.g., 0.05 for ±5%

public:
    NumericalQuestion(string id, string text, float marks);

    void setCorrectAnswer(double answer, double tolerance);
    bool checkAnswer(string answer) const;
    void displayQuestion() const; 
};

#endif // NUMERICALQUESTION_HPP