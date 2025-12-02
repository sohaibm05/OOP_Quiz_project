# Script to check if SFML is installed correctly

Write-Host "`n=== Checking SFML Installation ===`n" -ForegroundColor Cyan

$SFML_PATHS = @(
    "C:\SFML-2.5.1",
    "C:\SFML",
    "$env:USERPROFILE\SFML-2.5.1",
    "$env:USERPROFILE\SFML"
)

$found = $false

foreach ($path in $SFML_PATHS) {
    $graphicsHeader = Join-Path $path "include\SFML\Graphics.hpp"
    if (Test-Path $graphicsHeader) {
        Write-Host "✓ SFML found at: $path" -ForegroundColor Green
        Write-Host "  Graphics.hpp location: $graphicsHeader`n" -ForegroundColor Gray
        
        # Check for lib files
        $libPath = Join-Path $path "lib"
        if (Test-Path $libPath) {
            $libFiles = Get-ChildItem "$libPath\*.a" -ErrorAction SilentlyContinue
            if ($libFiles) {
                Write-Host "  ✓ Library files found: $($libFiles.Count) files" -ForegroundColor Green
            } else {
                Write-Host "  ⚠ Warning: No .a library files found in $libPath" -ForegroundColor Yellow
            }
        }
        
        # Check for DLL files
        $binPath = Join-Path $path "bin"
        if (Test-Path $binPath) {
            $dllFiles = Get-ChildItem "$binPath\*.dll" -ErrorAction SilentlyContinue
            if ($dllFiles) {
                Write-Host "  ✓ DLL files found: $($dllFiles.Count) files" -ForegroundColor Green
            }
        }
        
        Write-Host "`nTo use this installation, update build_sfml.ps1:" -ForegroundColor Yellow
        Write-Host "  Change: `$SFML_PATH = `"C:\SFML-2.5.1`"" -ForegroundColor White
        Write-Host "  To:     `$SFML_PATH = `"$path`"`n" -ForegroundColor White
        
        $found = $true
        break
    }
}

if (-not $found) {
    Write-Host "✗ SFML NOT FOUND" -ForegroundColor Red
    Write-Host "`nSFML is not installed. Please:" -ForegroundColor Yellow
    Write-Host "1. Download from: https://www.sfml-dev.org/download.php" -ForegroundColor White
    Write-Host "2. Choose: SFML 2.5.1 for MinGW (GCC) - 64-bit" -ForegroundColor White
    Write-Host "3. Extract to: C:\SFML-2.5.1`n" -ForegroundColor White
    Write-Host "Direct download link:" -ForegroundColor Cyan
    Write-Host "https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip`n" -ForegroundColor White
}

Write-Host "Press Enter to exit..."
Read-Host


