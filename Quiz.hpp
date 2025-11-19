#ifndef QUIZ_HPP
#define QUIZ_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;
class Quiz
{
    protected:
        int quizID;
        string  title;
        int time_limit;
        vector<Question> questions;
    public:
        Quiz(int id, string t, int limit, vector<Question> q);
        int getQuizID() const;
        string getTitle() const;
        int getTimeLimit() const;
        vector<Question> getQuiz() const; // gets all the questions
        Question getQuestion(int index) const; 
        void displayQuizInfo() const;
};

class Question
{
    protected:
        int questionID;
        string questionText;
        int correctAnswer;
        int marks; // this means number of marks the question is worth
    public:
        Question(int id, string text, int correctIndex, int m);
        // int getQuestionID() const;
        // string getQuestionText() const;
        // int getMarks() const;
        // int getCorrectAsnwer() const;
        virtual void displayQuestion() const = 0; // pure virtual function
        virtual bool checkAnswer(int answer) const = 0; // pure virtual function
        virtual ~Question() {}
};

#endif