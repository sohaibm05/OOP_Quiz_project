#include "NumericalQuestion.hpp"
#include <iostream>

NumericalQuestion::NumericalQuestion(string id, string text, float marks)
    : Question(id, text, marks), correctAnswer(0.0), tolerance(0.0) {}

void NumericalQuestion::setCorrectAnswer(double answer, double tol) {
    correctAnswer = answer;
    tolerance = tol;
}

bool NumericalQuestion::checkAnswer(string answer) const {
    try {
        double val = std::stod(answer);
        double diff = std::abs(val - correctAnswer);
        if (tolerance > 0.0 && tolerance < 1.0) {
            return diff <= std::abs(correctAnswer) * tolerance;
        }
        return diff <= tolerance;
    } catch (...) {
        return false;
    }
}

void NumericalQuestion::displayQuestion() const { std::cout << questionText << std::endl; }
