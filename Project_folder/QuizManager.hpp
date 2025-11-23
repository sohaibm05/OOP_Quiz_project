#ifndef QUIZMANAGER_HPP
#define QUIZMANAGER_HPP

#include "Quiz.hpp"
#include <vector>
#include <string>

class QuizManager {
private:
    std::vector<Quiz*> quizzes; // manager owns quizzes

public:
    QuizManager() = default;
    ~QuizManager();

    void addQuiz(Quiz* q);
    const std::vector<Quiz*>& listQuizzes() const;
    Quiz* getQuiz(size_t index) const; // returns nullptr if out of range
    size_t count() const;
};

#endif // QUIZMANAGER_HPP
