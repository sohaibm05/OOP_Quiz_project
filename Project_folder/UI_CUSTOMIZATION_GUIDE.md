# UI Customization Guide

This guide shows you where to adjust texts, boxes, colors, and sizes in `mainsfml.cpp`.

## Quick Reference

### 1. **Window Size** (Line ~160)
```cpp
sf::RenderWindow window(sf::VideoMode(1400, 1100), "Quiz App", sf::Style::Default);
// Change: 1400 (width), 1100 (height)
```

### 2. **Color Scheme** (Lines ~169-172)
```cpp
sf::Color bgColor(15, 15, 15);           // Background color (dark gray)
sf::Color panelColor(30, 30, 30, 220);   // Panel color (semi-transparent)
sf::Color textColor(220, 220, 220);     // Default text color (light gray)
sf::Color highlightColor(100, 150, 255); // Highlight color (blue)
// Format: sf::Color(Red, Green, Blue) - values 0-255
```

### 3. **Button Styling** (Lines 25-77)

#### Button Colors:
```cpp
// Line 34: Normal button color
shape.setFillColor(sf::Color(50, 50, 50));

// Line 36: Button border color
shape.setOutlineColor(sf::Color(100, 100, 100));

// Line 52: Hover button color
shape.setFillColor(isHovered ? sf::Color(70, 70, 70) : sf::Color(50, 50, 50));
```

#### Button Text Size:
```cpp
// Line 41: Button text size
text.setCharacterSize(18);  // Change this number
```

#### Button Size & Position:
When creating buttons, adjust the `sf::Vector2f` parameters:
```cpp
// Example from line ~213:
mainButtons.emplace_back(
    sf::Vector2f(50, 150 + i * 80),  // Position (x, y)
    sf::Vector2f(400, 50),            // Size (width, height)
    ...
);
```

### 4. **TextInput Styling** (Lines 79-139)

#### TextInput Colors:
```cpp
// Line 88: Background color
background.setFillColor(sf::Color(40, 40, 40));

// Line 90: Border color (inactive)
background.setOutlineColor(sf::Color(80, 80, 80));

// Line 101: Border color (active/focused)
background.setOutlineColor(sf::Color(100, 150, 255));
```

#### TextInput Text Size:
```cpp
// Line 93: Input text size
text.setCharacterSize(18);  // Change this number
```

#### TextInput Size & Position:
```cpp
// Example from line ~185:
TextInput quizIdInput(sf::Vector2f(50, 280), sf::Vector2f(300, 50), font);
//                      Position (x, y)      Size (width, height)
```

### 5. **Text Styling** (Throughout the code)

#### Title Text (Line ~221):
```cpp
sf::Text title("QUIZ APP", font, 42);  // Size: 42
title.setFillColor(textColor);
title.setPosition(50, 50);  // Position (x, y)
```

#### Header Text (Example):
```cpp
sf::Text header("Create New Quiz", font, 32);  // Size: 32
header.setFillColor(textColor);
header.setPosition(50, 100);  // Position (x, y)
```

#### Label Text (Example):
```cpp
sf::Text idLabel("Quiz ID:", font, 18);  // Size: 18
idLabel.setFillColor(textColor);
idLabel.setPosition(50, 230);  // Position (x, y)
```

## Common Adjustments

### Make Buttons Bigger:
1. Find where the button is created (search for `Button(` or `emplace_back`)
2. Change the size parameter: `sf::Vector2f(width, height)`
3. Increase text size: `text.setCharacterSize(20)` (in Button constructor)

### Change Text Colors:
1. Find the text object: `sf::Text variableName(...)`
2. Change color: `variableName.setFillColor(sf::Color(R, G, B))`

### Adjust Spacing Between Elements:
1. Find the position parameter: `sf::Vector2f(x, y)`
2. Change the `y` value to move items up/down
3. Change the `x` value to move items left/right

### Make Input Boxes Taller:
1. Find the TextInput creation
2. Change size: `sf::Vector2f(width, height)` - increase height value

## Example: Customizing Main Menu Buttons

To make main menu buttons bigger with larger text:

```cpp
// Around line 212-218, change:
mainButtons.emplace_back(
    sf::Vector2f(50, 150 + i * 100),  // Increased spacing (was 80)
    sf::Vector2f(450, 70),             // Bigger size (was 400, 50)
    to_string(i+1) + ". " + menuOptions[i],
    font,
    static_cast<int>(i == 3 ? 4 : (i == 6 ? 7 : i + 1))
);

// And in Button constructor (line 41), change:
text.setCharacterSize(22);  // Larger text (was 18)
```

## Color Reference

Common colors you might want:
- **White**: `sf::Color::White` or `sf::Color(255, 255, 255)`
- **Black**: `sf::Color::Black` or `sf::Color(0, 0, 0)`
- **Red**: `sf::Color(255, 0, 0)`
- **Green**: `sf::Color(0, 255, 0)`
- **Blue**: `sf::Color(0, 0, 255)`
- **Light Gray**: `sf::Color(200, 200, 200)`
- **Dark Gray**: `sf::Color(50, 50, 50)`

## Tips

1. **After making changes**, recompile: `.\build_sfml.ps1`
2. **Test incrementally** - change one thing at a time
3. **Use consistent sizes** - keep similar elements the same size
4. **Check positioning** - make sure elements don't overlap


