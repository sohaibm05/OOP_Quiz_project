# PowerShell build script for SFML Quiz Application

# Update this path to match your SFML installation location
$SFML_PATH = "C:\Users\LENOVO\OneDrive\Desktop\parhai\OOP\SFML-2.5.0"

Write-Host "Checking SFML installation..." -ForegroundColor Yellow

if (-not (Test-Path "$SFML_PATH\include\SFML\Graphics.hpp")) {
    Write-Host "ERROR: SFML not found at $SFML_PATH" -ForegroundColor Red
    Write-Host "Please download SFML from https://www.sfml-dev.org/download.php" -ForegroundColor Yellow
    Write-Host "Extract it to $SFML_PATH" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Or modify the SFML_PATH variable in this script to point to your SFML installation." -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host "SFML found! Compiling..." -ForegroundColor Green

$sourceFiles = @(
    "mainsfml.cpp",
    "Question.cpp",
    "MCQ.cpp",
    "TrueFalse.cpp",
    "FillInTheBlank.cpp",
    "NumericalQuestion.cpp",
    "Student.cpp",
    "Quiz.cpp",
    "QuizAttempt.cpp",
    "CreateQuestion.cpp",
    "QuizManager.cpp",
    "StudentManager.cpp",
    "CSVQuestionLoader.cpp"
)

$compileCommand = "g++ -std=c++17 " + ($sourceFiles -join " ") + " -I`"$SFML_PATH\include`" -L`"$SFML_PATH\lib`" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe"

Write-Host "Running: $compileCommand" -ForegroundColor Cyan
Invoke-Expression $compileCommand

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "Compilation successful!" -ForegroundColor Green
    Write-Host ""
    
    Write-Host "Copying DLL files..." -ForegroundColor Yellow
    $dllFiles = Get-ChildItem "$SFML_PATH\bin\*.dll" -ErrorAction SilentlyContinue
    if ($dllFiles) {
        Copy-Item "$SFML_PATH\bin\*.dll" -Destination "." -Force
        Write-Host "DLL files copied." -ForegroundColor Green
    } else {
        Write-Host "Warning: No DLL files found in $SFML_PATH\bin" -ForegroundColor Yellow
    }
    
    Write-Host ""
    Write-Host "You can now run: .\quiz_sfml.exe" -ForegroundColor Green
} else {
    Write-Host ""
    Write-Host "Compilation failed! Check the error messages above." -ForegroundColor Red
    Write-Host ""
    Write-Host "Common issues:" -ForegroundColor Yellow
    Write-Host "1. Make sure g++ is installed and in your PATH" -ForegroundColor Yellow
    Write-Host "2. Verify SFML is installed at $SFML_PATH" -ForegroundColor Yellow
    Write-Host "3. Check that you downloaded the correct SFML version for your compiler" -ForegroundColor Yellow
}

Read-Host "Press Enter to exit"

