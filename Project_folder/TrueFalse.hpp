#ifndef TRUEFALSE_HPP
#define TRUEFALSE_HPP

#include "Question.hpp"
#include <string>

class TrueFalse : public Question 
{
private:
    bool correctAnswer; // true = "True", false = "False"

public:
    TrueFalse(std::string id, std::string text, float marks);
    void setCorrectAnswer(bool isTrue);
    bool checkAnswer(std::string answer) const;
    void displayQuesion() const; // NEW
};

#endif // TRUEFALSE_HPP