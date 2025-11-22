#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector>
#include <iostream> // Added for display
using namespace std;

class QuizAttempt; // Forward declaration

class Student {
private:
    string studentID;
    string name;
    string email;
    vector<QuizAttempt*> attempts; // Student references attempts

public:
    Student(string id, string name, string email);
    
    void addAttempt(QuizAttempt* attempt);
    string getName() const;
    
    // NEW: Display student info and attempts
    void display() const;
};

#endif // STUDENT_HPP