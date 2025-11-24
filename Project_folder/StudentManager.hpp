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
    // Note: Manager takes ownership of the stored Student pointers.
    // On duplicate ID, the manager will delete the provided pointer and
    // return the existing stored Student*.
    // Callers should not delete a Student after passing it here.
    Student* addOrGetStudent(Student* s);
    Student* findStudentByID(const std::string& id) const;
    // Return all students whose name matches (case-insensitive substring match)
    // Return students whose ID matches (case-insensitive substring match)
    // Returned Student* pointers are non-owning references into the
    // manager's storage. Do not delete them; the manager retains ownership.
    std::vector<Student*> findStudentsByID(const std::string& id) const;
    const std::vector<Student*>& listStudents() const;
    size_t count() const;
};

#endif // STUDENTMANAGER_HPP
