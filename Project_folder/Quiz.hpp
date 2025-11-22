#ifndef QUIZ_HPP
#define QUIZ_HPP

#include "Question.hpp"
#include <vector>
#include <string>

class Quiz 
{
private:
    std::string quizID;
    std::string title;
    std::string description;
    int timeLimit; // minutes
    std::vector<Question*> questions; // Quiz owns these questions

public:
    Quiz(std::string id, std::string title, std::string desc, int timeLimit); // Constructor  
    ~Quiz(); // Must delete all questions

    void addQuestion(Question* question);
    void displayQuiz() const; //Display entire quiz

    std::string getQuizID() const;
    std::string getTitle() const;
    int getTimeLimit() const;
    const std::vector<Question*>& getQuestions() const;
};

#endif // QUIZ_HPP