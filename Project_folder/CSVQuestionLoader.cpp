#include "CSVQuestionLoader.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <system_error>

using namespace std;

vector<string> CSVQuestionLoader::splitCSV(const string& line) 
{
    vector<string> result;
    string current;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i) 
    {
        char c = line[i];
        if (c == '\"') 
        {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) 
        {
            result.push_back(current);
            current.clear();
        } else 
        {
            current += c;
        }
    }
    result.push_back(current); // add last field
    return result;
}

string CSVQuestionLoader::trim(const string& str) 
{
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return str.substr(start, end - start + 1);
}

bool CSVQuestionLoader::tryParseInt(const string& str, int& out) 
{
    // Trim whitespace first so from_chars doesn't fail on leading/trailing spaces
    string trimmedStr = trim(str);
    if (trimmedStr.empty()) 
        return false;

    const char* begin = trimmedStr.data();
    const char* end = begin + trimmedStr.size();
    auto result = std::from_chars(begin, end, out);
    
    // Success if: no error occurred AND we consumed the entire string
    return (result.ec == std::errc()) && (result.ptr == end);
}

bool CSVQuestionLoader::tryParseFloat(const string& str, float& out)
{
    string trimmedStr = trim(str);
    if (trimmedStr.empty())
        return false;

    try {
        size_t idx = 0;
        float value = std::stof(trimmedStr, &idx);
        if (idx != trimmedStr.size())
            return false;      // extra junk after the number

        out = value;
        return true;
    }
    catch (const std::exception&) {
        return false;          // invalid or out-of-range
    }
}

bool CSVQuestionLoader::parseBool(const string& str) 
{
    string lowerStr = str;
    for (char& character : lowerStr) 
    {
        character = tolower(static_cast<unsigned char>(character));
    }
    
    // Accept: "true", "1", or "yes" (case-insensitive)
    return lowerStr == "true" || lowerStr == "1" || lowerStr == "yes";
}

vector<Question*> CSVQuestionLoader::loadFromCSV(const string& filePath) 
{
    vector<Question*> questions;
    ifstream file(filePath);
    if (!file.is_open()) 
    {
        cerr << "Error: Could not open CSV file: " << filePath << endl;
        return questions;
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line)) 
    {
        lineNumber++;
        // Skip empty lines and lines starting with '#' (comments)
        string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#') 
            continue;

        vector<string> fields = splitCSV(line);

        // Minimum fields: Type, QuestionID, QuestionText, Marks, CorrectAnswer
        if (fields.size() < 5) 
        {
            cerr << "Warning: Line " << lineNumber << " has too few fields. Skipping." << endl;
            continue;
        }

        string questionType = trim(fields[0]);
        string questionID = trim(fields[1]);
        string questionText = trim(fields[2]);
        float marks = 1.0f;
        if (!tryParseFloat(trim(fields[3]), marks)) 
        {
            cerr << "Warning: Line " << lineNumber << " has invalid marks. Using default 1.0." << endl;
            marks = 1.0f;
        }

        Question* question = nullptr;

        if (questionType == "MCQ") {
            // Format: MCQ, ID, Text, Marks, CorrectOptionIndex, Option1, Option2, ...
            if (fields.size() < 6) 
            {
                cerr << "Warning: Line " << lineNumber << " MCQ missing correct option. Skipping." << endl;
                continue;
            }

            int correctOptionIndex = 0;
            if (!tryParseInt(trim(fields[4]), correctOptionIndex)) 
            {
                cerr << "Warning: Line " << lineNumber << " MCQ has invalid correct option index. Using 0." << endl;
                correctOptionIndex = 0;
            }

            MCQ* mcqQuestion = new MCQ(questionID, questionText, marks);

            // Add all answer options (starting from field index 5)
            for (size_t i = 5; i < fields.size(); ++i) 
            {
                string option = trim(fields[i]);
                if (!option.empty()) 
                    mcqQuestion->addOption(option);
            }

            // Set the correct option if index is valid
            int numOptions = fields.size() - 5;
            if (correctOptionIndex >= 0 && correctOptionIndex < numOptions) 
            {
                mcqQuestion->setCorrectOption(correctOptionIndex);
            } 
            else 
            {
                cerr << "Warning: Line " << lineNumber << " MCQ correct option index out of range. Setting to 0." << endl;
                mcqQuestion->setCorrectOption(0);
            }

            question = mcqQuestion;

        } 
        else if (questionType == "TrueFalse") 
        {
            // Format: TrueFalse, ID, Text, Marks, CorrectAnswer (true/false)
            TrueFalse* tfQuestion = new TrueFalse(questionID, questionText, marks);
            bool correctAnswer = parseBool(trim(fields[4]));
            tfQuestion->setCorrectAnswer(correctAnswer);
            question = tfQuestion;

        } 
        else if (questionType == "FillInTheBlank") 
        {
            // Format: FillInTheBlank, ID, Text, Marks, CorrectAnswer
            FillInTheBlank* fibQuestion = new FillInTheBlank(questionID, questionText, marks);
            string correctAnswer = trim(fields[4]);
            fibQuestion->setCorrectAnswer(correctAnswer);
            question = fibQuestion;

        } 
        else if (questionType == "Numerical") 
        {
            // Format: Numerical, ID, Text, Marks, CorrectAnswer, Tolerance
            float correctValue = 0.0f;
            if (!tryParseFloat(trim(fields[4]), correctValue)) 
            {
                cerr << "Warning: Line " << lineNumber << " Numerical has invalid correct value. Using 0.0." << endl;
                correctValue = 0.0f;
            }

            // Default tolerance for numerical answers
            float tolerance = 0.01f;
            if (fields.size() > 5) 
            {
                if (!tryParseFloat(trim(fields[5]), tolerance)) 
                {
                    cerr << "Warning: Line " << lineNumber << " Numerical has invalid tolerance. Using 0.01." << endl;
                    tolerance = 0.01f;
                }
            }

            NumericalQuestion* numQuestion = new NumericalQuestion(questionID, questionText, marks);
            numQuestion->setCorrectAnswer(correctValue, tolerance);
            question = numQuestion;

        } 
        else 
        {
            cerr << "Warning: Line " << lineNumber << " unknown question type: " << questionType << ". Skipping." << endl;
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

