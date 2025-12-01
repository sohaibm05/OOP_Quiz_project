#ifndef CSVQUESTIONLOADER_HPP
#define CSVQUESTIONLOADER_HPP

#include "Question.hpp"
#include "MCQ.hpp"
#include "TrueFalse.hpp"
#include "FillInTheBlank.hpp"
#include "NumericalQuestion.hpp"
#include <vector>
#include <string>

class CSVQuestionLoader {
public:
    // Load questions from a CSV file
    static std::vector<Question*> loadFromCSV(const std::string& filePath);
    
private:
    // Helper functions for CSV parsing
    static std::vector<std::string> splitCSV(const std::string& line);
    static std::string trim(const std::string& str);
    static bool tryParseInt(const std::string& str, int& out);
    static bool tryParseFloat(const std::string& str, float& out);
    static bool parseBool(const std::string& str);
};

#endif // CSVQUESTIONLOADER_HPP

