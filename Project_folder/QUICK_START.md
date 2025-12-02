# Quick Start - Install SFML

## The Problem
You're getting this error:
```
fatal error: SFML/Graphics.hpp: No such file or directory
```

This means SFML is not installed yet.

## Solution - 3 Simple Steps

### Step 1: Download SFML
**Download this exact file:**
- **For MinGW (your compiler):** https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip
- Or go to: https://www.sfml-dev.org/download.php
- Look for: **SFML 2.5.1** → **MinGW (GCC) - 64-bit**

### Step 2: Extract to C:\
1. Right-click the downloaded ZIP file
2. Select "Extract All..."
3. Extract to: `C:\`
4. This should create: `C:\SFML-2.5.1\`

**Verify:** After extraction, check that this file exists:
```
C:\SFML-2.5.1\include\SFML\Graphics.hpp
```

### Step 3: Build
Run this command in PowerShell (from Project_folder directory):
```powershell
.\build_sfml.ps1
```

Or manually:
```powershell
g++ -std=c++17 mainsfml.cpp Question.cpp MCQ.cpp TrueFalse.cpp FillInTheBlank.cpp NumericalQuestion.cpp Student.cpp Quiz.cpp QuizAttempt.cpp CreateQuestion.cpp QuizManager.cpp StudentManager.cpp CSVQuestionLoader.cpp -I"C:\SFML-2.5.1\include" -L"C:\SFML-2.5.1\lib" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe
```

## Still Having Issues?

If SFML is installed in a different location, edit `build_sfml.ps1` and change this line:
```powershell
$SFML_PATH = "C:\SFML-2.5.1"
```
to point to your SFML installation folder.


