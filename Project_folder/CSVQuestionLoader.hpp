#ifndef CSVQUESTIONLOADER_HPP
#define CSVQUESTIONLOADER_HPP

#include <string>
#include <vector>
#include <sstream>
#include "Question.hpp"
#include "MCQ.hpp"
#include "TrueFalse.hpp"
#include "FillInTheBlank.hpp"
#include "NumericalQuestion.hpp"

using namespace std;

class CSVQuestionLoader {
public:
    // Load questions from CSV file
    // CSV Format (comma-separated):
    // Type,QuestionID,QuestionText,Marks,CorrectAnswer,Option1,Option2,Option3,Option4
    //
    // Examples:
    // MCQ,Q1,What is 2+2?,5,2,3,4,5,6
    //   (Type=MCQ, ID=Q1, Text=What is 2+2?, Marks=5, CorrectAnswer=2 (0-based index), Options=3,4,5,6)
    //
    // TrueFalse,Q2,The earth is round,2,true
    //   (Type=TrueFalse, ID=Q2, Text=The earth is round, Marks=2, CorrectAnswer=true)
    //
    // FillInTheBlank,Q3,Capital of France,3,Paris
    //   (Type=FillInTheBlank, ID=Q3, Text=Capital of France, Marks=3, CorrectAnswer=Paris)
    //
    // Numerical,Q4,Value of pi,4,3.14159,0.01
    //   (Type=Numerical, ID=Q4, Text=Value of pi, Marks=4, CorrectAnswer=3.14159, Tolerance=0.01)
    
    static vector<Question*> loadFromCSV(const string& filePath);

private:
    // Helper: split a CSV line by comma (simple, handles basic cases)
    static vector<string> splitCSV(const string& line);
    
    // Helper: trim whitespace
    static string trim(const string& str);
    
    // Helper: parse int safely
    static bool tryParseInt(const string& str, int& out);
    
    // Helper: parse float safely
    static bool tryParseFloat(const string& str, float& out);
    
    // Helper: parse bool (true/false)
    static bool parseBool(const string& str);
};

#endif // CSVQUESTIONLOADER_HPP
