#ifndef STUDENTMANAGER_HPP
#define STUDENTMANAGER_HPP

#include "Student.hpp"
#include <vector>
#include <string>
#include <algorithm>

class StudentManager {
private:
    std::vector<Student*> students; // manager owns student pointers

public:
    StudentManager() = default;
    ~StudentManager();

    // Add a student if ID not present; if a student with same ID exists,
    // the existing student pointer is returned and the provided pointer is deleted by the manager.
    // Returns the stored Student* (existing or newly-added) or nullptr if input was nullptr.
    Student* addOrGetStudent(Student* s);
    Student* findStudentByID(const std::string& id) const;
    // Return all students whose name matches (case-insensitive substring match)
    // Return students whose ID matches (case-insensitive substring match)
    std::vector<Student*> findStudentsByID(const std::string& id) const;
    const std::vector<Student*>& listStudents() const;
    size_t count() const;
};

#endif // STUDENTMANAGER_HPP
