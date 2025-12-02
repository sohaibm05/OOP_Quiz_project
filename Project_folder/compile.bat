@echo off
REM Simple compilation script - no PowerShell needed

set SFML_PATH=C:\Users\LENOVO\OneDrive\Desktop\parhai\OOP\SFML-2.5.0

echo Compiling...
g++ -std=c++17 mainsfml.cpp Question.cpp MCQ.cpp TrueFalse.cpp FillInTheBlank.cpp NumericalQuestion.cpp Student.cpp Quiz.cpp QuizAttempt.cpp CreateQuestion.cpp QuizManager.cpp StudentManager.cpp CSVQuestionLoader.cpp -I"%SFML_PATH%\include" -L"%SFML_PATH%\lib" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilation successful!
    echo.
    echo Copying DLL files...
    copy /Y "%SFML_PATH%\bin\*.dll" . >nul 2>&1
    echo Done! Run quiz_sfml.exe
) else (
    echo.
    echo Compilation failed!
)

pause

