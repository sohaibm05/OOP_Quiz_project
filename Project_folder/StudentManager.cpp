#include "StudentManager.hpp"
#include "Student.hpp"
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

StudentManager::~StudentManager()
{
    for (Student* s : students) delete s;
    students.clear();
}

Student* StudentManager::addOrGetStudent(Student* s)
{
    if (!s) return nullptr;
    const string id = s->getStudentID();
    if (!id.empty()) {
        for (Student* existing : students) {
            if (existing->getStudentID() == id) {
                delete s; // avoid leak
                return existing;
            }
        }
    }
    students.push_back(s);
    return s;
}

Student* StudentManager::findStudentByID(const string& id) const
{
    if (id.empty()) return nullptr;
    for (Student* s : students) {
        if (s->getStudentID() == id) return s;
    }
    return nullptr;
}

static string normalize(const string& s)
{
    size_t start = 0;
    while (start < s.size() && ::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && ::isspace(static_cast<unsigned char>(s[end - 1]))) --end;
    string r = s.substr(start, end - start);
    transform(r.begin(), r.end(), r.begin(), [](unsigned char c){ return static_cast<char>(::tolower(c)); });
    return r;
}


vector<Student*> StudentManager::findStudentsByID(const string& id) const
{
    vector<Student*> matches;
    if (id.empty()) return matches;
    string term = normalize(id);
    for (Student* s : students) {
        string sid = normalize(s->getStudentID());
        if (sid.find(term) != string::npos) matches.push_back(s);
    }
    return matches;
}

const vector<Student*>& StudentManager::listStudents() const 
{ 
    return students; 
}

size_t StudentManager::count() const 
{ 
    return students.size(); 
}

