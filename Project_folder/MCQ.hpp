#ifndef MCQ_HPP
#define MCQ_HPP

#include "Question.hpp"
#include <vector>
#include <string>

class MCQ : public Question 
{
    private:
        std::vector<std::string> options;
        int correctOption; // 0-based index

    public:
        MCQ(std::string id, std::string text, float marks); // constructor taking id, text, and marks
        
        void addOption(std::string option);
        void setCorrectOption(int index);
        bool checkAnswer(std::string answer) const; // overridden method to check if the provided answer is correct
        void displayQuestion() const; // overriden method to display the question and options
        const std::vector<std::string>& getOptions() const; // getter for options
};

#endif // MCQ_HPP