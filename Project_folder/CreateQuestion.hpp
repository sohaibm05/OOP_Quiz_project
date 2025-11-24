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
    // Ownership: the returned Question* is newly allocated and ownership is
    // transferred to the caller (usually added to a Quiz via addQuestion).
    // Caller must delete it if not added to a Quiz.
    Question* createQuestionInteractive();
}

#endif // CREATEQUESTION_HPP
