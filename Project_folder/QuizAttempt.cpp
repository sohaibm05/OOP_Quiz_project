#include "QuizAttempt.hpp"
#include "Question.hpp"
#include "Student.hpp"
#include "Quiz.hpp"
#include <ctime>
#include <iostream>
using namespace std;

QuizAttempt::QuizAttempt(string id, Student* studentPtr, Quiz* quizPtr): attemptID(id), startTime(time(nullptr)), endTime(0), score(0.0f), student(studentPtr), quiz(quizPtr) {}

void QuizAttempt::setAnswer(Question* question, string answer) 
{ 
    answers[question] = answer; 
}

void QuizAttempt::submit() 
{
    float total = 0.0f;
    if (quiz) {
        const auto &qs = quiz->getQuestions();
        for (Question* q : qs)
        {
            auto it = answers.find(q);
            if (it != answers.end()) 
            {
                if (q->checkAnswer(it->second)) 
                {
                    total += q->getMarks();
                }
            }
        }
    }
    score = total;
    endTime = time(nullptr);
}

float QuizAttempt::getScore() const 
{ 
    return score; 
}

string QuizAttempt::getAttemptID() const 
{ 
    return attemptID; 
}

void QuizAttempt::display() const 
{
    cout << "Attempt ID: " << attemptID << endl;
    if (student)
    { 
        cout << "Student: " << student->getName() << " (" << student->getStudentID() << ")" << endl;
    }
    if (quiz) 
    {
        cout << "Quiz: " << quiz->getTitle() << " (" << quiz->getQuizID() << ")" << endl;
    }
    cout << "Score: " << score << endl;
    cout << "Start: " << ctime(&startTime);
    if (endTime != 0)
    { 
        cout << "End: " << ctime(&endTime);
    }
    cout << "Answers provided: " << answers.size() << endl;
}
