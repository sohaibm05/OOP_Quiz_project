# SFML Setup Guide for Windows

## Step 1: Download SFML

1. Go to https://www.sfml-dev.org/download.php
2. Download **SFML 2.5.1** for **MinGW (GCC) - 64-bit** (since you're using MinGW g++ 8.1.0)
3. Extract the ZIP file to `C:\SFML-2.5.1` (or any location you prefer)

**Important:** Make sure to download the MinGW version, NOT the Visual C++ version!

## Step 2: Verify Installation

After extraction, you should have this structure:
```
C:\SFML-2.5.1\
├── include\
│   └── SFML\
│       ├── Graphics.hpp
│       ├── Window.hpp
│       └── ...
├── lib\
│   └── (library files)
└── bin\
    └── (DLL files)
```

## Step 3: Compile the Application

Use the provided `build_sfml.bat` script, or manually run:

```powershell
cd Project_folder
g++ -std=c++17 mainsfml.cpp Question.cpp MCQ.cpp TrueFalse.cpp FillInTheBlank.cpp NumericalQuestion.cpp Student.cpp Quiz.cpp QuizAttempt.cpp CreateQuestion.cpp QuizManager.cpp StudentManager.cpp CSVQuestionLoader.cpp -I"C:\SFML-2.5.1\include" -L"C:\SFML-2.5.1\lib" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe
```

## Step 4: Copy DLL Files

After compilation, copy the DLL files to your project folder:
```powershell
Copy-Item "C:\SFML-2.5.1\bin\*.dll" -Destination "Project_folder\"
```

## Alternative: Using vcpkg (Recommended for Development)

If you have vcpkg installed:
```powershell
vcpkg install sfml:x64-windows
```

Then compile with:
```powershell
g++ -std=c++17 mainsfml.cpp *.cpp -I"path\to\vcpkg\installed\x64-windows\include" -L"path\to\vcpkg\installed\x64-windows\lib" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe
```

