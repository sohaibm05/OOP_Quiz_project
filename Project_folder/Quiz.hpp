#ifndef QUIZ_HPP
#define QUIZ_HPP

#include "Question.hpp"
#include <vector>
#include <string>

class Quiz 
{
private:
    string quizID;
    string title;
    string description;
    int timeLimit; // minutes
    vector<Question*> questions; // Quiz owns these questions

public:
    Quiz(string id, string title, string desc, int timeLimit); // Constructor  
    ~Quiz(); // Must delete all questions

    void addQuestion(Question* question);
    void displayQuiz() const; //Display entire quiz

    string getQuizID() const;
    string getTitle() const;
    int getTimeLimit() const;
    const vector<Question*>& getQuestions() const;
};

#endif // QUIZ_HPP