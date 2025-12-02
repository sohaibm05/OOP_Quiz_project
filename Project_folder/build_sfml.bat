@echo off
REM Build script for SFML Quiz Application
REM Make sure SFML is installed at the path below

set SFML_PATH=C:\Users\LENOVO\OneDrive\Desktop\parhai\OOP\SFML-2.5.0

echo Checking SFML installation...
if not exist "%SFML_PATH%\include\SFML\Graphics.hpp" (
    echo ERROR: SFML not found at %SFML_PATH%
    echo Please download SFML from https://www.sfml-dev.org/download.php
    echo Extract it to %SFML_PATH%
    pause
    exit /b 1
)

echo SFML found! Compiling...
g++ -std=c++17 mainsfml.cpp Question.cpp MCQ.cpp TrueFalse.cpp FillInTheBlank.cpp NumericalQuestion.cpp Student.cpp Quiz.cpp QuizAttempt.cpp CreateQuestion.cpp QuizManager.cpp StudentManager.cpp CSVQuestionLoader.cpp -I"%SFML_PATH%\include" -L"%SFML_PATH%\lib" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilation successful!
    echo.
    echo Copying DLL files...
    if exist "%SFML_PATH%\bin\*.dll" (
        copy /Y "%SFML_PATH%\bin\*.dll" . >nul 2>&1
        echo DLL files copied.
    )
    echo.
    echo You can now run: quiz_sfml.exe
) else (
    echo.
    echo Compilation failed! Check the error messages above.
)

pause

