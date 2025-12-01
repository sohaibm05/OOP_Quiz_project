#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector>
#include <iostream> // Added for display
#include "QuizAttempt.hpp" // Include QuizAttempt header
using namespace std;

class QuizAttempt; // Forward declaration

class Student 
{
    private:
        string studentID;
        string name;
        string email;
        // Student owns QuizAttempt pointers added via `addAttempt`.
        // Attempts are deleted in the Student destructor.
        vector<QuizAttempt*> attempts;

    public:
        Student(string id, string name, string email); // Constructor
        ~Student();

        // Takes ownership of `attempt`. The Student will delete it in the
        // destructor. Do not delete the attempt after calling this.
        void addAttempt(QuizAttempt* attempt); // Add a quiz attempt
        string getStudentID() const; // Get student ID
        string getEmail() const; // Get student email
        string getName() const; // Get student name
        void display() const; // Display student details
        const std::vector<QuizAttempt*>& getAttempts() const;
    
};

#endif // STUDENT_HPP