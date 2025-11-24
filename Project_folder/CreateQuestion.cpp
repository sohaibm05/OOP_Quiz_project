#include "CreateQuestion.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

static string readLineTrimmed() // same logic as normalize in FillInTheBlank.cpp but returns trimmed string instead of lowercased
{
    string s;
    getline(cin, s);
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start])))
    { 
        ++start;
    }
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1])))
    { 
        --end;
    }
    return s.substr(start, end - start);
}

Question* CreateQuestion::createQuestionInteractive()
{
    cout << "Select question type:\n 1) MCQ\n 2) True/False\n 3) FillInTheBlank\n 4) Numerical\n 0) Done/Cancel" << endl;
    cout << "Your choice: ";
    string choiceLine = readLineTrimmed();
    int choice = -1;
    try 
    { 
        choice = stoi(choiceLine); 
    } catch (...) { choice = -1; }
    if (choice == 0)
    {
        return nullptr;
    }

    cout << "Enter question ID (leave blank to auto-generate): ";
    string qid = readLineTrimmed();
    cout << "Enter question text: ";
    string qtext = readLineTrimmed();
    cout << "Enter marks (e.g. 2.5): ";
    string marksLine = readLineTrimmed();
    float marks = 1.0f; // default marks
    try 
    { 
        marks = stof(marksLine); 
    } catch (...) { marks = 1.0f;}

    if (choice == 1) 
    {
        MCQ* m = new MCQ(qid, qtext, marks);
        cout << "How many options? ";
        string optCountLine = readLineTrimmed();
        int optCount = 0; 
        try 
        { 
            optCount = stoi(optCountLine); 
        } catch (...) { optCount = 0; }

        for (int i = 0; i < optCount; ++i) 
        {
            cout << "Option " << (i+1) << ": ";
            string opt = readLineTrimmed();
            m->addOption(opt);
        }
        cout << "Enter correct option number (1-based): ";
        string corr = readLineTrimmed();
        try 
        { 
            int idx = stoi(corr) - 1;
            m->setCorrectOption(idx); //idx 
        } catch (...) {}
        return m;
    }

    if (choice == 2) 
    {
        TrueFalse* t = new TrueFalse(qid, qtext, marks);
        cout << "Correct answer (true/false): ";
        string ans = readLineTrimmed();
        for (size_t i = 0; i < ans.size(); ++i) 
        {
            ans[i] = static_cast<char>(tolower(static_cast<unsigned char>(ans[i])));
        }
        t->setCorrectAnswer(ans == "true" || ans == "t" || ans == "1" || ans == "yes");
        return t;
    }

    if (choice == 3) 
    {
        FillInTheBlank* f = new FillInTheBlank(qid, qtext, marks);
        cout << "Correct answer: ";
        string can = readLineTrimmed();
        f->setCorrectAnswer(can);
        return f;
    }

    if (choice == 4) 
    {
        NumericalQuestion* n = new NumericalQuestion(qid, qtext, marks);
        cout << "Correct numeric answer (e.g. 3.14): ";
        string ans = readLineTrimmed();
        double c = 0.0; 
        try 
        { 
            c = stod(ans); 
        } catch (...) { c = 0.0; }

        cout << "Tolerance (e.g. 0.01 for 1% or absolute value): ";
        string tol = readLineTrimmed();
        double tval = 0.0; 
        try 
        { 
            tval = stod(tol); 
        } catch (...) { tval = 0.0; }
        n->setCorrectAnswer(c, tval);
        return n;
    }

    cout << "Invalid choice." << endl;
    return nullptr;
}
