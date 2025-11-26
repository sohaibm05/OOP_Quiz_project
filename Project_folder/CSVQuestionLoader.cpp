#include "CSVQuestionLoader.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

vector<string> CSVQuestionLoader::splitCSV(const string& line) {
    vector<string> result;
    string current;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            result.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    
    result.push_back(current); // add last field
    return result;
}

string CSVQuestionLoader::trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return str.substr(start, end - start + 1);
}

bool CSVQuestionLoader::tryParseInt(const string& str, int& out) {
    try {
        stringstream ss(str);
        return (ss >> out) && ss.eof();
    } catch (...) {
        return false;
    }
}

bool CSVQuestionLoader::tryParseFloat(const string& str, float& out) {
    try {
        stringstream ss(str);
        return (ss >> out) && ss.eof();
    } catch (...) {
        return false;
    }
}

bool CSVQuestionLoader::parseBool(const string& str) {
    string lower = str;
    for (char& c : lower) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return lower == "true" || lower == "1" || lower == "yes";
}

vector<Question*> CSVQuestionLoader::loadFromCSV(const string& filePath) {
    vector<Question*> questions;
    ifstream file(filePath);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open CSV file: " << filePath << endl;
        return questions;
    }

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        
        // Skip empty lines and comments
        string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#') {
            continue;
        }

        vector<string> fields = splitCSV(line);
        
        // Need at least: Type, QuestionID, QuestionText, Marks, CorrectAnswer
        if (fields.size() < 5) {
            cerr << "Warning: Line " << lineNum << " has too few fields. Skipping." << endl;
            continue;
        }

        string type = trim(fields[0]);
        string questionID = trim(fields[1]);
        string questionText = trim(fields[2]);
        
        float marks = 1.0f;
        if (!tryParseFloat(trim(fields[3]), marks)) {
            cerr << "Warning: Line " << lineNum << " has invalid marks. Using default 1.0." << endl;
            marks = 1.0f;
        }

        Question* question = nullptr;

        if (type == "MCQ") {
            // Format: MCQ,ID,Text,Marks,CorrectOptionIndex,Option1,Option2,Option3,Option4,...
            if (fields.size() < 6) {
                cerr << "Warning: Line " << lineNum << " MCQ missing correct option. Skipping." << endl;
                continue;
            }

            int correctIdx = 0;
            if (!tryParseInt(trim(fields[4]), correctIdx)) {
                cerr << "Warning: Line " << lineNum << " MCQ has invalid correct option index. Using 0." << endl;
                correctIdx = 0;
            }

            MCQ* mcq = new MCQ(questionID, questionText, marks);
            
            // Add options (fields 5 onwards)
            for (size_t i = 5; i < fields.size(); ++i) {
                string option = trim(fields[i]);
                if (!option.empty()) {
                    mcq->addOption(option);
                }
            }

            if (correctIdx >= 0 && correctIdx < static_cast<int>(fields.size() - 5)) {
                mcq->setCorrectOption(correctIdx);
            } else {
                cerr << "Warning: Line " << lineNum << " MCQ correct option index out of range. Setting to 0." << endl;
                mcq->setCorrectOption(0);
            }

            question = mcq;

        } else if (type == "TrueFalse") {
            // Format: TrueFalse,ID,Text,Marks,CorrectAnswer(true/false)
            TrueFalse* tf = new TrueFalse(questionID, questionText, marks);
            bool correctAnswer = parseBool(trim(fields[4]));
            tf->setCorrectAnswer(correctAnswer);
            question = tf;

        } else if (type == "FillInTheBlank") {
            // Format: FillInTheBlank,ID,Text,Marks,CorrectAnswer
            FillInTheBlank* fib = new FillInTheBlank(questionID, questionText, marks);
            string correctAnswer = trim(fields[4]);
            fib->setCorrectAnswer(correctAnswer);
            question = fib;

        } else if (type == "Numerical") {
            // Format: Numerical,ID,Text,Marks,CorrectAnswer,Tolerance
            float correctValue = 0.0f;
            if (!tryParseFloat(trim(fields[4]), correctValue)) {
                cerr << "Warning: Line " << lineNum << " Numerical has invalid correct value. Using 0.0." << endl;
                correctValue = 0.0f;
            }

            float tolerance = 0.01f; // default tolerance
            if (fields.size() > 5) {
                if (!tryParseFloat(trim(fields[5]), tolerance)) {
                    cerr << "Warning: Line " << lineNum << " Numerical has invalid tolerance. Using 0.01." << endl;
                    tolerance = 0.01f;
                }
            }

            NumericalQuestion* num = new NumericalQuestion(questionID, questionText, marks);
            num->setCorrectAnswer(correctValue, tolerance);
            question = num;

        } else {
            cerr << "Warning: Line " << lineNum << " unknown question type: " << type << ". Skipping." << endl;
            continue;
        }

        if (question) {
            questions.push_back(question);
        }
    }

    file.close();
    cout << "Loaded " << questions.size() << " questions from CSV file." << endl;
    return questions;
}
