#include "Student.hpp"
#include <iostream>

Student::Student(string id, string name, string email)
    : studentID(id), name(name), email(email) {}

void Student::addAttempt(QuizAttempt* attempt) { attempts.push_back(attempt); }

string Student::getStudentID() const { return studentID; }
string Student::getEmail() const { return email; }
string Student::getName() const { return name; }

void Student::display() const {
    std::cout << "Student ID: " << studentID << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Attempts: " << attempts.size() << std::endl;
}
