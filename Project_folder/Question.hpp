#ifndef QUESTION_HPP
#define QUESTION_HPP
#include <iostream>
#include <string>
using namespace std;

class Question {
protected:
    string questionID;
    string questionText;
    float marks;

public:
    Question(string id, string text, float marks);
    virtual ~Question() = default;

    string getQuestionID() const;
    string getQuestionText() const;
    float getMarks() const;

    virtual bool checkAnswer(string answer) const = 0;
};

#endif // QUESTION_HPP