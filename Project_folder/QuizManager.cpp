#include "QuizManager.hpp"
#include <algorithm>

QuizManager::~QuizManager() {
    for (Quiz* q : quizzes) delete q;
    quizzes.clear();
}

void QuizManager::addQuiz(Quiz* q) {
    if (q) quizzes.push_back(q);
}

const std::vector<Quiz*>& QuizManager::listQuizzes() const { return quizzes; }

Quiz* QuizManager::getQuiz(size_t index) const {
    if (index < quizzes.size()) return quizzes[index];
    return nullptr;
}

size_t QuizManager::count() const { return quizzes.size(); }
