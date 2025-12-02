# Compilation Instructions

## Method 1: Use the Batch File (Easiest)
Double-click `compile.bat` or run:
```cmd
compile.bat
```

## Method 2: Direct Command (Copy & Paste)
Open PowerShell or Command Prompt in the Project_folder directory and run:

```powershell
$SFML_PATH = "C:\Users\LENOVO\OneDrive\Desktop\parhai\OOP\SFML-2.5.0"
g++ -std=c++17 mainsfml.cpp Question.cpp MCQ.cpp TrueFalse.cpp FillInTheBlank.cpp NumericalQuestion.cpp Student.cpp Quiz.cpp QuizAttempt.cpp CreateQuestion.cpp QuizManager.cpp StudentManager.cpp CSVQuestionLoader.cpp -I"$SFML_PATH\include" -L"$SFML_PATH\lib" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe
```

Then copy DLL files:
```powershell
Copy-Item "$SFML_PATH\bin\*.dll" -Destination "." -Force
```

## Method 3: Command Prompt (CMD)
```cmd
set SFML_PATH=C:\Users\LENOVO\OneDrive\Desktop\parhai\OOP\SFML-2.5.0
g++ -std=c++17 mainsfml.cpp Question.cpp MCQ.cpp TrueFalse.cpp FillInTheBlank.cpp NumericalQuestion.cpp Student.cpp Quiz.cpp QuizAttempt.cpp CreateQuestion.cpp QuizManager.cpp StudentManager.cpp CSVQuestionLoader.cpp -I"%SFML_PATH%\include" -L"%SFML_PATH%\lib" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe
copy /Y "%SFML_PATH%\bin\*.dll" .
```

## After Compilation
Run the executable:
```cmd
quiz_sfml.exe
```

