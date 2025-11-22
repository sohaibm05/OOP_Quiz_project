#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <iostream>
#include <string>
using namespace std;

class Question 
{
protected:
    string questionID;
    string questionText;
    float marks;

public:
    Question(string id, string text, float marks); // Constructor
    virtual ~Question() = default; // Virtual destructor

    string getQuestionID() const;
    string getQuestionText() const;
    float getMarks() const;

    virtual bool checkAnswer(string answer) const = 0; // Pure virtual function for checking answer
    virtual void displayQuestion() const = 0; // Pure virtual function for displaying question
};

#endif // QUESTION_HPP