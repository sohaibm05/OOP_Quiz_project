#include "FillInTheBlank.hpp"
#include <algorithm>
#include <iostream>
using namespace std;

FillInTheBlank::FillInTheBlank(string id, string text, float marks): Question(id, text, marks), correctAnswer("") {}

void FillInTheBlank::setCorrectAnswer(string answer) 
{ 
    correctAnswer = answer; 
}

static string normalize(const string& s) 
{
    int n = static_cast<int>(s.length()); // static cast here is unessary cuz our number will never be that big the reason s.length() give type size_t is to handle very large strings on 64 bit systems but here we dont need that
    int start = 0; // Find start index (first non-whitespace)
    while (start < n && ::isspace(static_cast< char>(s[start])))   // why use static_cast< char>? it is becaseuse the standard library functions like isspace and tolower are defined for int values that are either EOF or representable as  char. This prevents potential issues with negative char values.
    {
        ++start;
    }
    if (start >= n) // If we reached the end, string is all whitespace
    {
        return "";
    }
    int end = n - 1; // Find end index (last non-whitespace)
    while (end >= 0 && ::isspace(static_cast< char>(s[end]))) 
    {
        --end;
    }

    string result; // Now [start, end] is the trimmed range (inclusive)
    for (int i = start; i <= end; ++i)   // Step 3: Copy and convert to lowercase
    {
        result += (::tolower(static_cast<char>(s[i])));
    }
    return result;
}

bool FillInTheBlank::checkAnswer(string answer) const 
{
    return normalize(answer) == normalize(correctAnswer);
}

void FillInTheBlank::displayQuestion() const
{
    cout << questionText << endl; 
}
