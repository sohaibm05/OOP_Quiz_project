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
        // Question(int id, string text, int correctIndex, int m); // i dont think i need this 
        virtual void displayQuestion() const = 0; // pure virtual function
        virtual bool checkAnswer(int answer) const = 0; // pure virtual function
        virtual ~Question() {}
};


class MCQ: public Question
{
    private:
        vector<string> options;
    public:
        MCQ(int id, string text, vector<string> opts, int correctIndex, int m);
        void displayQuestion() const override;
        bool checkAnswer(int answer) const override;
        void addOption(const string& option);
};

class TrueFalse: public Question
{
    private:
        bool correctAnswer;
    public:
        TrueFalse(int id, string text, int correctIndex, int m);
        void displayQuestion() const;
        bool checkAnswer(int answer) const;
};

class fillInTheBlank: public Question
{
    private:
        string correctAnswer;
    public:
        fillInTheBlank(int id, string text, string correctAns, int m);
        void displayQuestion() const;
        bool checkAnswer(int answer) const;
};

class Numerical_Question: public Question
{
    private:
        double correctAnswer;
    public:
        Numerical_Question(int id, string text, double correctAns, int m);
        void displayQuestion() const;
        bool checkAnswer(int answer) const;
};



#endif