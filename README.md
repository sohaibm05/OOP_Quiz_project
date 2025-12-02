                                Welcome

What this project is
- A compact quiz library and a tiny app that demonstrates it. You can run a
  console-based menu to create quizzes, attempt them, view student attempts,
  and import questions from a CSV file. There's also an SFML-based GUI file
  ( mainsfml.cpp ) if you want a graphical front end.

Quick starter (what to run)
- From PowerShell, change into  Project_folder  and compile the sources:

  cd "c:\Users\dell\Documents\GitHub\OOP_Quiz_project\Project_folder"
  g++ -std=c++17 -c *.cpp
  g++ *.o -o quiz_app.exe
  .\quiz_app.exe

  Note: Don't compile both  main.cpp  and  mainsfml.cpp  into the same
  executable — you'll get a "multiple definition of main" error. Build the
  console app (above) or the SFML app separately and link SFML libraries
  when building the GUI version.

What's in this folder 
-  Question  and its subclasses: MCQ, TrueFalse, FillInTheBlank, NumericalQuestion
-  Quiz : a collection of questions (Quiz owns the questions it contains)
-  QuizAttempt : stores a student's answers for a quiz and computes a score
-  Student  +  StudentManager : students and the manager that owns them
-  QuizManager : stores quizzes and owns them
-  CSVQuestionLoader : a small helper that reads CSV lines and builds
   Question*  objects for supported types
-  main.cpp : the console demo and simple menu
-  mainsfml.cpp : a graphical front-end (work-in-progress)

How the pieces fit together 
-  QuizManager  keeps quizzes. Each  Quiz  keeps the questions it contains.
-  StudentManager  keeps students. Each  Student  keeps the attempts they made.
- When a student takes a quiz, we create a  QuizAttempt  that references the
   Quiz  (non-owning) and records answers keyed by  Question* .
- To grade an attempt we loop through the quiz's  Question*  list and call the
  virtual  checkAnswer  method on each question. Because each question type
  implements  checkAnswer  differently, the grading works without the
  caller needing to know the concrete question type — that is runtime
  polymorphism in action.

CSV import — formats to use
- MCQ:       MCQ,ID,Text,Marks,CorrectOptionIndex,Option1,Option2,...
- TrueFalse:  TrueFalse,ID,Text,Marks,CorrectAnswer
- FillInTheBlank: FillInTheBlank,ID,Text,Marks,CorrectAnswer
- Numerical:  Numerical,ID,Text,Marks,CorrectValue,Tolerance





