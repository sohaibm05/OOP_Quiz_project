#ifndef FILLINTHEBLANK_HPP
#define FILLINTHEBLANK_HPP

#include "Question.hpp"
#include <string>

class FillInTheBlank : public Question 
{
    private:
        std::string correctAnswer;
    public:
        FillInTheBlank(std::string id, std::string text, float marks); // Constructor

        void setCorrectAnswer(std::string answer); //   Set the correct answer
        bool checkAnswer(std::string answer) const; // Check if the provided answer is correct
        void displayQuestion() const; // Display the question text
};

#endif // FILLINTHEBLANK_HPP