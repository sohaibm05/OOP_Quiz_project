#include "Question.hpp"

Question::Question(string id, string text, float m) : questionID(id), questionText(text), marks(m) {}

string Question::getQuestionID() const
{
     return questionID;
}
string Question::getQuestionText() const
{ 
    return questionText; 
}
float Question::getMarks() const 
{ 
    return marks; 
}
