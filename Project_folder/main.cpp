#include <iostream>
#include <algorithm>  // For std::transform
#include <cctype>     // For ::tolower
#include <string>     // For string operations
#include <vector>     // If needed

#include "MCQ.hpp"
#include "TrueFalse.hpp"
#include "FillInTheBlank.hpp"
#include "NumericalQuestion.hpp"
#include "Quiz.hpp"
#include "Student.hpp"
#include "QuizAttempt.hpp"
#include "CreateQuestion.hpp"
#include "QuizManager.hpp"
#include "StudentManager.hpp"

using namespace std;

int main() {
    cout << "=== Quiz Library Test Harness ===" << endl << endl;

    QuizManager manager;
    StudentManager studentManager;

    while (true) {
        cout << "Main menu:\n 1) Create quiz\n 2) List quizzes\n 3) Attempt a quiz\n 4) Exit\n 5) List students & attempts\n 6) Find students by id" << endl;
        cout << "Your choice: ";
        string mainChoice; std::getline(cin, mainChoice);
        int m = -1; try { m = stoi(mainChoice); } catch (...) { m = -1; }
        if (m == 4) break;

        if (m == 1) {
            // Create a new quiz
            cout << "Create quiz: Enter quiz ID: ";
            string qid; std::getline(cin, qid);
            if (qid.empty()) qid = "Quiz" + to_string(manager.count() + 1);
            cout << "Enter title: "; string qtitle; std::getline(cin, qtitle);
            cout << "Enter description: "; string qdesc; std::getline(cin, qdesc);
            cout << "Enter time limit (minutes): "; string tline; std::getline(cin, tline);
            int tlim = 30; try { tlim = stoi(tline); } catch (...) { tlim = 30; }

            Quiz* quiz = new Quiz(qid, qtitle, qdesc, tlim);

            cout << "Create mode for this quiz:\n 1) Add demo questions\n 2) Create interactively" << endl;
            cout << "Choice: "; string cm; std::getline(cin, cm);
            int cmode = 1; try { cmode = stoi(cm); } catch (...) { cmode = 1; }
            if (cmode == 1) {
                MCQ* mcq = new MCQ("Q1", "What is 2 + 2?", 5.0f);
                mcq->addOption("3"); mcq->addOption("4"); mcq->addOption("5"); mcq->setCorrectOption(1);
                TrueFalse* tf = new TrueFalse("Q2", "The earth is round.", 2.0f); tf->setCorrectAnswer(true);
                FillInTheBlank* fib = new FillInTheBlank("Q3", "Capital of France?", 3.0f); fib->setCorrectAnswer("Paris");
                NumericalQuestion* num = new NumericalQuestion("Q4", "Value of pi (approx)?", 4.0f); num->setCorrectAnswer(3.14159, 0.01);
                quiz->addQuestion(mcq); quiz->addQuestion(tf); quiz->addQuestion(fib); quiz->addQuestion(num);
            } else {
                cout << "Entering interactive question creation for quiz '" << qid << "' (type 0 to finish)" << endl;
                while (true) {
                    Question* q = CreateQuestion::createQuestionInteractive();
                    if (!q) break;
                    quiz->addQuestion(q);
                    cout << "Question added. Current count: " << quiz->getQuestions().size() << "\n";
                }
            }

            manager.addQuiz(quiz);
            cout << "Quiz '" << qtitle << "' created and added. Total quizzes: " << manager.count() << "\n";
        }
        else if (m == 2) {
            const auto &qs = manager.listQuizzes();
            if (qs.empty()) { cout << "No quizzes available." << endl; continue; }
            cout << "Available quizzes:" << endl;
            for (size_t i = 0; i < qs.size(); ++i) {
                cout << i + 1 << ") " << qs[i]->getTitle() << " (" << qs[i]->getQuizID() << ") - " << qs[i]->getQuestions().size() << " questions" << endl;
            }
        }
        else if (m == 3) {
            const auto &qs = manager.listQuizzes();
            if (qs.empty()) { cout << "No quizzes to attempt. Create one first." << endl; continue; }
            cout << "Select quiz to attempt (number): "; string sel; std::getline(cin, sel);
            int idx = -1; try { idx = stoi(sel) - 1; } catch (...) { idx = -1; }
            Quiz* chosen = manager.getQuiz(idx < 0 ? SIZE_MAX : static_cast<size_t>(idx));
            if (!chosen) { cout << "Invalid selection." << endl; continue; }

            cout << "Attempting quiz: " << chosen->getTitle() << "\n";
            cout << "Enter your student ID: "; string sid; std::getline(cin, sid);
            if (sid.empty()) sid = "S1";
            cout << "Enter your name: "; string sname; std::getline(cin, sname);
            if (sname.empty()) sname = "Student";
            cout << "Enter your email: "; string sem; std::getline(cin, sem);

            Student* s = studentManager.findStudentByID(sid);
            if (!s) {
                s = new Student(sid, sname, sem);
                s = studentManager.addOrGetStudent(s);
            }

            // Create attempt and attach to student; student will own the attempt
            QuizAttempt* attempt = new QuizAttempt("A1", s, chosen);

            cout << "Please answer the following questions (type the option number or your answer text):" << endl;
            for (Question* q : chosen->getQuestions()) {
                q->displayQuestion();
                cout << "Your answer: ";
                string userAns; std::getline(cin, userAns);
                attempt->setAnswer(q, userAns);
            }

            s->addAttempt(attempt);
            attempt->submit();
            cout << "After submitting attempt:" << endl; attempt->display(); cout << endl;
        }
        else if (m == 5) {
            const auto &studs = studentManager.listStudents();
            if (studs.empty()) { cout << "No students recorded." << endl; continue; }
            cout << "Students and attempts:" << endl;
            for (Student* st : studs) {
                st->display();
                const auto &atts = st->getAttempts();
                if (atts.empty()) {
                    cout << "  (no attempts)" << endl;
                } else {
                    cout << "  Attempts:" << endl;
                    for (const QuizAttempt* a : atts) {
                        cout << "  - "; a->display(); // attempts being displayed here 
                    }
                }
                cout << "---" << endl;
            }
        }
        else if (m == 6) {
            cout << "Enter id to search:";
            string id;
            std::getline(cin, id);
            auto matches = studentManager.findStudentsByID(id);
            if (matches.empty()) { cout << "No students match '" << id << "'." << endl; continue; }
            cout << "Found " << matches.size() << " student(s):" << endl;
            for (Student* st : matches) 
            {
                st->display();
                const auto &atts = st->getAttempts();
                if (atts.empty()) cout << "  (no attempts)" << endl;
                else {
                    cout << "  Attempts:" << endl;
                    for (const QuizAttempt* a : atts) {
                        cout << "  - "; a->display();
                    }
                }
                cout << "---" << endl;
            }
        }
        else if (m == 0) {
            // Exit
            break;
        }
        else 
        {
            cout << "Invalid menu choice." << endl;
        }
    }

    cout << "Exiting. Goodbye." << endl;
    return 0;
}

// cd 'c:\Users\dell\Documents\GitHub\OOP_Quiz_project\Project_folder' basixly cding
// >> g++ -std=c++17 -c Question.cpp MCQ.cpp TrueFalse.cpp FillInTheBlank.cpp NumericalQuestion.cpp Student.cpp Quiz.cpp QuizAttempt.cpp CreateQuestion.cpp QuizManager.cpp StudentManager.cpp  main.cpp
// >> g++ *.o -o quiz_app.exe
// >> .\quiz_app.exe