#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector>
#include <iostream> // Added for display
using namespace std;

class QuizAttempt; // Forward declaration

class Student 
{
private:
    string studentID;
    string name;
    string email;
    vector<QuizAttempt*> attempts; // Student references attempts

public:
    Student(string id, string name, string email); // Constructor
    
    void addAttempt(QuizAttempt* attempt); // Add a quiz attempt
    string getStudentID() const; // Get student ID
    string getEmail() const; // Get student email
    string getName() const; // Get student name
    void display() const; // Display student details
    
};

#endif // STUDENT_HPP