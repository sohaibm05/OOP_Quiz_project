#ifndef QUIZATTEMPT_HPP
#define QUIZATTEMPT_HPP

#include <map>
#include <string>
#include <ctime>
#include <iostream> 

using namespace std;

class Question;   // Forward declarations
class Student;
class Quiz;

class QuizAttempt 
{
    private:
        string attemptID;
        time_t startTime;
        time_t endTime;
        float score;
        map<Question*, string> answers; // Question -> student's answer
        Student* student;
        Quiz* quiz;

    public:
        QuizAttempt(string id, Student* student, Quiz* quiz);
        
        void setAnswer(Question* question, string answer);
        void submit(); // Calculate score and set endTime
        float getScore() const; // Get the score
        string getAttemptID() const;
        void display() const; // Display attempt details
};

#endif // QUIZATTEMPT_HPP