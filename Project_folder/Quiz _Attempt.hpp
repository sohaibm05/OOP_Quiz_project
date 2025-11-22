#ifndef QUIZ_ATTEMPT_HPP
#define QUIZ_ATTEMPT_HPP

#include <iostream>
#include <vector>
#include <string>
#include "Quiz.hpp"
#include "Student.hpp"

using namespace std;

class Quiz_Attempt
{
    private:
        Student* student;
        Quiz* quiz;
        vector<string> studentAnswers;
        int score;
    public:
        Quiz_Attempt(Student* s, Quiz* q);
        void recordAnswer(const string& answer);
        void calculateScore();
        void displayAttemptDetails() const;
};


#endif