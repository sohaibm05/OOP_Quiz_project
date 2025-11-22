#include "Student.hpp"
#include <iostream>
using namespace std;

Student::Student(string id, string name, string email) : studentID(id), name(name), email(email) {}

void Student::addAttempt(QuizAttempt* attempt) 
{ 
    attempts.push_back(attempt); 
}

string Student::getStudentID() const
{ 
    return studentID; 
}

string Student::getEmail() const 
{ 
    return email; 
}

string Student::getName() const 
{ 
    return name; 
}

void Student::display() const 
{
    cout << "Student ID: " << studentID << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Attempts: " << attempts.size() << endl;
}
