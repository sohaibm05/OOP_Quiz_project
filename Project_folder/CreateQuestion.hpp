#ifndef CREATEQUESTION_HPP
#define CREATEQUESTION_HPP

#include "Question.hpp"
#include "MCQ.hpp"
#include "TrueFalse.hpp"
#include "FillInTheBlank.hpp"
#include "NumericalQuestion.hpp"

namespace CreateQuestion 
{
    Question* createQuestionInteractive(); // made this just so that main function is smaler and cleaner
}

#endif // CREATEQUESTION_HPP
