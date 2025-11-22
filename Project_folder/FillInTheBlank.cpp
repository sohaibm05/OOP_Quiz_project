#include "FillInTheBlank.hpp"
#include <algorithm>
#include <iostream>

FillInTheBlank::FillInTheBlank(std::string id, std::string text, float marks)
    : Question(id, text, marks), correctAnswer("") {}

void FillInTheBlank::setCorrectAnswer(std::string answer) { correctAnswer = answer; }

static std::string normalize(const std::string &s) {
    std::string r = s;
    // trim simple spaces (front/back)
    size_t start = r.find_first_not_of(" \t\n\r");
    size_t end = r.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    r = r.substr(start, end - start + 1);
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

bool FillInTheBlank::checkAnswer(std::string answer) const {
    return normalize(answer) == normalize(correctAnswer);
}

void FillInTheBlank::displayQuestion() const { std::cout << questionText << std::endl; }
