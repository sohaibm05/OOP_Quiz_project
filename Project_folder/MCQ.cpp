#include "MCQ.hpp"
#include <iostream>
#include <algorithm>

MCQ::MCQ(std::string id, std::string text, float marks)
    : Question(id, text, marks), correctOption(-1) {}

void MCQ::addOption(std::string option) { options.push_back(option); }

void MCQ::setCorrectOption(int index) {
    if (index >= 0 && index < static_cast<int>(options.size()))
        correctOption = index;
}

static std::string toLowerCopy(const std::string &s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

bool MCQ::checkAnswer(std::string answer) const {
    // try numeric answer (1-based index)
    try {
        int idx = std::stoi(answer) - 1;
        if (idx == correctOption) return true;
    } catch (...) {}

    // compare text of the answer (case-insensitive)
    if (correctOption >= 0 && correctOption < static_cast<int>(options.size())) {
        if (toLowerCopy(options[correctOption]) == toLowerCopy(answer)) return true;
    }

    return false;
}

void MCQ::displayQuestion() const {
    std::cout << questionText << std::endl;
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << (i + 1) << ". " << options[i] << std::endl;
    }
}
