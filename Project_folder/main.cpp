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

using namespace std;

int main() {
    cout << "=== Quiz Library Test Harness ===" << endl << endl;

    cout << "Choose mode:\n 1) Use demo quiz\n 2) Create quiz questions interactively" << endl;
    cout << "Enter choice (1 or 2): ";
    string modeLine;
    std::getline(cin, modeLine);
    int mode = 1;
    try { mode = stoi(modeLine); } catch (...) { mode = 1; }

    Quiz* quiz = new Quiz("Quiz1", "User Quiz", "Generated quiz.", 30);

    if (mode == 1) {
        // Demo questions
        MCQ* mcq = new MCQ("Q1", "What is 2 + 2?", 5.0f);
        mcq->addOption("3");
        mcq->addOption("4");
        mcq->addOption("5");
        mcq->setCorrectOption(1); // option index 1 -> "4"

        TrueFalse* tf = new TrueFalse("Q2", "The earth is round.", 2.0f);
        tf->setCorrectAnswer(true);

        FillInTheBlank* fib = new FillInTheBlank("Q3", "Capital of France?", 3.0f);
        fib->setCorrectAnswer("Paris");

        NumericalQuestion* num = new NumericalQuestion("Q4", "Value of pi (approx)?", 4.0f);
        num->setCorrectAnswer(3.14159, 0.01); // 1% relative tolerance

        NumericalQuestion* num2 = new NumericalQuestion("Q5", "A measurement (absolute tol)", 2.0f);
        num2->setCorrectAnswer(10.0, 0.5);

        quiz->addQuestion(mcq);
        quiz->addQuestion(tf);
        quiz->addQuestion(fib);
        quiz->addQuestion(num);
        quiz->addQuestion(num2);

        cout << "Displaying entire quiz:" << endl;
        quiz->displayQuiz();
        cout << endl;
    } else {
        cout << "Entering interactive question creation. Type '0' as question type to finish." << endl;
        int qcount = 0;
        while (true) {
            cout << "Select question type:\n 1) MCQ\n 2) True/False\n 3) FillInTheBlank\n 4) Numerical\n 0) Done" << endl;
            cout << "Your choice: ";
            string choiceLine;
            std::getline(cin, choiceLine);
            int choice = -1;
            try { choice = stoi(choiceLine); } catch (...) { choice = -1; }
            if (choice == 0) break;

            cout << "Enter question ID: ";
            string qid; std::getline(cin, qid);
            if (qid.empty()) qid = "Q" + to_string(++qcount);
            cout << "Enter question text: ";
            string qtext; std::getline(cin, qtext);
            cout << "Enter marks (e.g. 2.5): ";
            string marksLine; std::getline(cin, marksLine);
            float marks = 1.0f;
            try { marks = stof(marksLine); } catch (...) { marks = 1.0f; }

            if (choice == 1) {
                MCQ* m = new MCQ(qid, qtext, marks);
                cout << "How many options? ";
                string optCountLine; std::getline(cin, optCountLine);
                int optCount = 0; try { optCount = stoi(optCountLine); } catch (...) { optCount = 0; }
                for (int i = 0; i < optCount; ++i) {
                    cout << "Option " << (i+1) << ": ";
                    string opt; std::getline(cin, opt);
                    m->addOption(opt);
                }
                cout << "Enter correct option number (1-based): ";
                string corr; std::getline(cin, corr);
                try { int idx = stoi(corr) - 1; m->setCorrectOption(idx); } catch (...) {}
                quiz->addQuestion(m);
            } else if (choice == 2) {
                TrueFalse* t = new TrueFalse(qid, qtext, marks);
                cout << "Correct answer (true/false): ";
                string ans; std::getline(cin, ans);
                // SAFE transform (critical fix):
                std::transform(ans.begin(), ans.end(), ans.begin(),
                    [](unsigned char c) { return std::tolower(c); });
                t->setCorrectAnswer(ans == "true" || ans == "t" || ans == "1" || ans == "yes");
                quiz->addQuestion(t);
            } else if (choice == 3) {
                FillInTheBlank* f = new FillInTheBlank(qid, qtext, marks);
                cout << "Correct answer: ";
                string can; std::getline(cin, can);
                f->setCorrectAnswer(can);
                quiz->addQuestion(f);
            } else if (choice == 4) {
                NumericalQuestion* n = new NumericalQuestion(qid, qtext, marks);
                cout << "Correct numeric answer (e.g. 3.14): ";
                string ans; std::getline(cin, ans);
                double c = 0.0; try { c = stod(ans); } catch (...) { c = 0.0; }
                cout << "Tolerance (e.g. 0.01 for 1% or absolute value): ";
                string tol; std::getline(cin, tol);
                double tval = 0.0; try { tval = stod(tol); } catch (...) { tval = 0.0; }
                n->setCorrectAnswer(c, tval);
                quiz->addQuestion(n);
            } else {
                cout << "Invalid choice, try again." << endl;
            }
        }
        cout << "Finished creating questions. Quiz contains " << quiz->getQuestions().size() << " questions." << endl;
        cout << endl;
    }

    // Create a student and attempt the quiz
    Student* s = new Student("S1", "Alice", "alice@example.com");
    QuizAttempt* attempt = new QuizAttempt("A1", s, quiz);

    // Interactive: prompt the user for answers to each question
    cout << "Please answer the following questions (type the option number or your answer text):" << endl;
    for (Question* q : quiz->getQuestions()) {
        q->displayQuestion(); 
        cout << "Your answer: ";
        string userAns;
        std::getline(cin, userAns);
        attempt->setAnswer(q, userAns);
    }

    s->addAttempt(attempt);

    // Submit and display results
    attempt->submit();
    cout << "After submitting attempt:" << endl;
    attempt->display();
    cout << endl;

    // Cleanup: deleting quiz will delete contained questions; delete attempt and student
    delete attempt;
    delete s;
    delete quiz; // this deletes mcq, tf, fib, num, num2

    cout << "Test harness finished." << endl;
    return 0;
}