#include "QuizAttempt.hpp"
#include "Question.hpp"
#include "Student.hpp"
#include "Quiz.hpp"
#include <ctime>
#include <iostream>

QuizAttempt::QuizAttempt(string id, Student* studentPtr, Quiz* quizPtr)
    : attemptID(id), startTime(std::time(nullptr)), endTime(0), score(0.0f), student(studentPtr), quiz(quizPtr) {}

void QuizAttempt::setAnswer(Question* question, string answer) { answers[question] = answer; }

void QuizAttempt::submit() {
    float total = 0.0f;
    if (quiz) {
        const auto &qs = quiz->getQuestions();
        for (Question* q : qs) {
            auto it = answers.find(q);
            if (it != answers.end()) {
                if (q->checkAnswer(it->second)) {
                    total += q->getMarks();
                }
            }
        }
    }
    score = total;
    endTime = std::time(nullptr);
}

float QuizAttempt::getScore() const { return score; }

string QuizAttempt::getAttemptID() const { return attemptID; }

void QuizAttempt::display() const {
    std::cout << "Attempt ID: " << attemptID << std::endl;
    if (student) std::cout << "Student: " << student->getName() << " (" << student->getStudentID() << ")" << std::endl;
    if (quiz) std::cout << "Quiz: " << quiz->getTitle() << " (" << quiz->getQuizID() << ")" << std::endl;
    std::cout << "Score: " << score << std::endl;
    std::cout << "Start: " << std::ctime(&startTime);
    if (endTime != 0) std::cout << "End: " << std::ctime(&endTime);
    std::cout << "Answers provided: " << answers.size() << std::endl;
}
