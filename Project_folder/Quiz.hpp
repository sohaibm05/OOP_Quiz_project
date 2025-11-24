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
    // Quiz owns the Question* pointers stored here. Questions added via
    // `addQuestion` become owned by the Quiz and will be deleted in the
    // Quiz destructor.
    vector<Question*> questions;

public:
    Quiz(string id, string title, string desc, int timeLimit); // Constructor  
    ~Quiz(); // Must delete all questions

    // Takes ownership of `question`. Caller must not delete `question`
    // after passing it to this method.
    void addQuestion(Question* question);
    void displayQuiz() const; //Display entire quiz

    string getQuizID() const;
    string getTitle() const;
    int getTimeLimit() const;
    const vector<Question*>& getQuestions() const;
};

#endif // QUIZ_HPP