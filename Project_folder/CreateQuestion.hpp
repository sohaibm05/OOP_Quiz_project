#ifndef CREATEQUESTION_HPP
#define CREATEQUESTION_HPP

#include "Question.hpp"
#include "MCQ.hpp"
#include "TrueFalse.hpp"
#include "FillInTheBlank.hpp"
#include "NumericalQuestion.hpp"

namespace CreateQuestion {
    // Prompts the user to create a single question interactively.
    // Returns a newly allocated Question* (caller owns it) or nullptr if the user chose to finish/cancel.
    Question* createQuestionInteractive();
}

#endif // CREATEQUESTION_HPP
