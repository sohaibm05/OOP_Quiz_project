#include "Quiz.hpp"
#include <iostream>

Quiz::Quiz(string id, string t, string desc, int timeLimit)
    : quizID(id), title(t), description(desc), timeLimit(timeLimit) {}

Quiz::~Quiz() {
    for (Question* q : questions) {
        delete q;
    }
    questions.clear();
}

void Quiz::addQuestion(Question* question) { questions.push_back(question); }

void Quiz::displayQuiz() const {
    std::cout << "Quiz: " << title << " (" << quizID << ")" << std::endl;
    std::cout << description << std::endl;
    std::cout << "Time limit: " << timeLimit << " minutes" << std::endl;
    std::cout << "Questions:" << std::endl;
    for (const Question* q : questions) {
        q->displayQuestion();
        std::cout << std::endl;
    }
}

string Quiz::getQuizID() const { return quizID; }
string Quiz::getTitle() const { return title; }
int Quiz::getTimeLimit() const { return timeLimit; }
const vector<Question*>& Quiz::getQuestions() const { return questions; }
