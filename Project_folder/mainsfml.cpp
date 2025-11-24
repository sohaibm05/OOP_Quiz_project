#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <map>
#include <ctime>

#include "MCQ.hpp"
#include "TrueFalse.hpp"
#include "FillInTheBlank.hpp"
#include "NumericalQuestion.hpp"
#include "Quiz.hpp"
#include "Student.hpp"
#include "QuizAttempt.hpp"
#include "CreateQuestion.hpp"
#include "QuizManager.hpp"
#include "StudentManager.hpp"

using namespace std;

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered = false;
    int id = 0;

    Button(sf::Vector2f pos, sf::Vector2f size, const string& label, const sf::Font& font, int buttonId = 0) {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(sf::Color(50, 50, 50));
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color(100, 100, 100));
        id = buttonId;
        
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(
            pos.x + size.x / 2 - bounds.width / 2,
            pos.y + size.y / 2 - bounds.height / 2 - 5
        );
    }

    void update(const sf::Vector2f& mousePos) {
        isHovered = shape.getGlobalBounds().contains(mousePos);
        shape.setFillColor(isHovered ? sf::Color(70, 70, 70) : sf::Color(50, 50, 50));
    }

    bool isClicked(sf::Vector2f mousePos) {
        return shape.getGlobalBounds().contains(mousePos);
    }
    
    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(
            pos.x + shape.getSize().x / 2 - bounds.width / 2,
            pos.y + shape.getSize().y / 2 - bounds.height / 2 - 5
        );
    }
};

struct TextInput {
    sf::RectangleShape background;
    sf::Text text;
    string content = "";
    bool isActive = false;

    TextInput(sf::Vector2f pos, sf::Vector2f size, const sf::Font& font) {
        background.setPosition(pos);
        background.setSize(size);
        background.setFillColor(sf::Color(40, 40, 40));
        background.setOutlineThickness(2);
        background.setOutlineColor(sf::Color(80, 80, 80));

        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setPosition(pos.x + 10, pos.y + 8);
    }

    void update(const sf::Vector2f& mousePos, bool clicked) {
        if (clicked && background.getGlobalBounds().contains(mousePos)) {
            isActive = true;
        } else if (clicked && !background.getGlobalBounds().contains(mousePos)) {
            isActive = false;
        }
    }

    void handleInput(char c) {
        if (isActive) {
            if (c == '\b' && !content.empty()) {
                content.pop_back();
            } else if (c != '\b') {
                content += c;
            }
        }
    }

    void clear() {
        content = "";
    }

    string getValue() const {
        return content;
    }

    void setPosition(sf::Vector2f pos) {
        background.setPosition(pos);
        text.setPosition(pos.x + 10, pos.y + 8);
    }

    void draw(sf::RenderWindow& window) {
        text.setString(content);
        window.draw(background);
        window.draw(text);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 1100), "Quiz App", sf::Style::Default);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        return -1;
    }

    sf::Color bgColor(15, 15, 15);
    sf::Color panelColor(30, 30, 30, 220);
    sf::Color textColor(220, 220, 220);
    sf::Color highlightColor(100, 150, 255);

    QuizManager manager;
    StudentManager studentManager;
    
    // State management
    enum AppState {
        MAIN_MENU,
        CREATE_QUIZ,
        LIST_QUIZZES,
        ATTEMPT_QUIZ,
        LIST_STUDENTS,
        FIND_STUDENTS,
        CREATE_QUESTION,
        QUIZ_DETAILS
    };
    
    int state = MAIN_MENU;
    vector<Button> mainButtons;
    vector<string> menuOptions = {"Create quiz", "List quizzes", "Attempt a quiz", "Exit", "List students", "Find students by id"};
    
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        mainButtons.emplace_back(
            sf::Vector2f(50, 150 + i * 120),
            sf::Vector2f(350, 60),
            to_string(i+1) + ". " + menuOptions[i],
            font,
            static_cast<int>(i + 1)
        );
    }

    sf::Text title("QUIZ APP", font, 42);
    title.setFillColor(textColor);
    title.setPosition(50, 50);
    title.setStyle(sf::Text::Bold);

    // Create quiz state variables
    TextInput quizIdInput(sf::Vector2f(50, 280), sf::Vector2f(300, 50), font);
    TextInput quizTitleInput(sf::Vector2f(50, 370), sf::Vector2f(300, 50), font);
    TextInput quizDescInput(sf::Vector2f(50, 460), sf::Vector2f(300, 50), font);
    TextInput timeLimitInput(sf::Vector2f(50, 550), sf::Vector2f(300, 50), font);
    Button createDemoButton(sf::Vector2f(50, 640), sf::Vector2f(300, 50), "Add Demo Questions", font, 100);
    Button createInteractiveButton(sf::Vector2f(370, 640), sf::Vector2f(300, 50), "Create Interactively", font, 101);
    Button saveQuizButton(sf::Vector2f(50, 710), sf::Vector2f(300, 50), "Save Quiz", font, 102);
    Button backButton(sf::Vector2f(370, 710), sf::Vector2f(300, 50), "Back to Menu", font, 103);

    // List quizzes state variables
    vector<Button> quizButtons;
    vector<Quiz*> currentQuizzes;

    // Attempt quiz state variables
    vector<Button> attemptQuizButtons;
    vector<Quiz*> availableQuizzes;
    TextInput studentIdInput(sf::Vector2f(50, 250), sf::Vector2f(300, 50), font);
    TextInput studentNameInput(sf::Vector2f(50, 340), sf::Vector2f(300, 50), font);
    TextInput studentEmailInput(sf::Vector2f(50, 430), sf::Vector2f(300, 50), font);
    Button startAttemptButton(sf::Vector2f(50, 520), sf::Vector2f(300, 50), "Start Attempt", font, 200);
    Button backToMenuButton(sf::Vector2f(370, 520), sf::Vector2f(300, 50), "Back to Menu", font, 201);

    // Question creation state variables
    vector<Button> questionTypeButtons;
    vector<string> questionTypes = {"MCQ", "True/False", "Fill in the Blank", "Numerical", "Done/Cancel"};
    for (size_t i = 0; i < questionTypes.size(); ++i) {
        questionTypeButtons.emplace_back(
            sf::Vector2f(50, 150 + i * 70),
            sf::Vector2f(350, 50),
            to_string(i+1) + ". " + questionTypes[i],
            font,
            static_cast<int>(i + 1)
        );
    }
    TextInput questionIdInput(sf::Vector2f(50, 550), sf::Vector2f(300, 50), font);
    TextInput questionTextInput(sf::Vector2f(50, 640), sf::Vector2f(300, 50), font);
    TextInput marksInput(sf::Vector2f(50, 730), sf::Vector2f(300, 50), font);
    Button addQuestionButton(sf::Vector2f(50, 820), sf::Vector2f(300, 50), "Add Question", font, 300);
    Button backToCreateQuizButton(sf::Vector2f(370, 820), sf::Vector2f(300, 50), "Back to Create Quiz", font, 301);

    // Current quiz being created or attempted
    Quiz* currentQuiz = nullptr;
    Student* currentStudent = nullptr;
    QuizAttempt* currentAttempt = nullptr;
    vector<Question*> currentQuestions;

    while (window.isOpen()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::TextEntered) {
                char c = static_cast<char>(event.text.unicode);
                if (state == CREATE_QUIZ) {
                    quizIdInput.handleInput(c);
                    quizTitleInput.handleInput(c);
                    quizDescInput.handleInput(c);
                    timeLimitInput.handleInput(c);
                } else if (state == ATTEMPT_QUIZ) {
                    studentIdInput.handleInput(c);
                    studentNameInput.handleInput(c);
                    studentEmailInput.handleInput(c);
                } else if (state == CREATE_QUESTION) {
                    questionIdInput.handleInput(c);
                    questionTextInput.handleInput(c);
                    marksInput.handleInput(c);
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (state == MAIN_MENU) {
                        for (auto& button : mainButtons) {
                            if (button.isClicked(mousePos)) {
                                state = button.id;
                                if (button.id == 4) {
                                    window.close();
                                }
                                break;
                            }
                        }
                    } else if (state == CREATE_QUIZ) {
                        createDemoButton.update(mousePos);
                        createInteractiveButton.update(mousePos);
                        saveQuizButton.update(mousePos);
                        backButton.update(mousePos);
                        
                        if (createDemoButton.isClicked(mousePos)) {
                            // Create demo questions
                            MCQ* mcq = new MCQ("", "What is 2 + 2?", 5.0f);
                            mcq->addOption("3");
                            mcq->addOption("4");
                            mcq->addOption("5"); 
                            mcq->setCorrectOption(1);
                            
                            TrueFalse* tf = new TrueFalse("", "The earth is round.", 2.0f);
                            tf->setCorrectAnswer(true);
                            
                            FillInTheBlank* fib = new FillInTheBlank("", "Capital of France?", 3.0f); 
                            fib->setCorrectAnswer("Paris");
                            
                            NumericalQuestion* num = new NumericalQuestion("", "Value of pi (approx)?", 4.0f);
                            num->setCorrectAnswer(3.14159, 0.01);
                            
                            currentQuestions.push_back(mcq);
                            currentQuestions.push_back(tf);
                            currentQuestions.push_back(fib);
                            currentQuestions.push_back(num);
                        } else if (createInteractiveButton.isClicked(mousePos)) {
                            state = CREATE_QUESTION;
                            currentQuestions.clear();
                        } else if (saveQuizButton.isClicked(mousePos)) {
                            string qid = quizIdInput.getValue();
                            if (qid.empty()) {
                                qid = "Quiz" + to_string(manager.count() + 1);
                            }
                            string qtitle = quizTitleInput.getValue();
                            string qdesc = quizDescInput.getValue();
                            int tlim = 30;
                            try {
                                tlim = stoi(timeLimitInput.getValue());
                            } catch (...) {}
                            
                            Quiz* quiz = new Quiz(qid, qtitle, qdesc, tlim);
                            for (Question* q : currentQuestions) {
                                quiz->addQuestion(q);
                            }
                            manager.addQuiz(quiz);
                            currentQuiz = quiz;
                            state = QUIZ_DETAILS;
                        } else if (backButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                            quizIdInput.clear();
                            quizTitleInput.clear();
                            quizDescInput.clear();
                            timeLimitInput.clear();
                            currentQuestions.clear();
                        }
                    } else if (state == LIST_QUIZZES) {
                        for (auto& button : quizButtons) {
                            if (button.isClicked(mousePos)) {
                                state = QUIZ_DETAILS;
                                currentQuiz = manager.getQuiz(button.id - 1);
                                break;
                            }
                        }
                    } else if (state == ATTEMPT_QUIZ) {
                        startAttemptButton.update(mousePos);
                        backToMenuButton.update(mousePos);
                        
                        if (startAttemptButton.isClicked(mousePos)) {
                            string sid = studentIdInput.getValue();
                            if (sid.empty()) {
                                sid = "S1";
                            }
                            string sname = studentNameInput.getValue();
                            if (sname.empty()) {
                                sname = "Student";
                            }
                            string sem = studentEmailInput.getValue();
                            
                            Student* s = studentManager.findStudentByID(sid);
                            if (!s) {
                                s = new Student(sid, sname, sem);
                                s = studentManager.addOrGetStudent(s);
                            }
                            currentStudent = s;
                            
                            // Start quiz attempt
                            currentAttempt = new QuizAttempt("A1", s, currentQuiz);
                            state = QUIZ_DETAILS; // Will show quiz questions
                        } else if (backToMenuButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                            studentIdInput.clear();
                            studentNameInput.clear();
                            studentEmailInput.clear();
                        }
                    } else if (state == CREATE_QUESTION) {
                        for (auto& button : questionTypeButtons) {
                            if (button.isClicked(mousePos)) {
                                int type = button.id;
                                if (type == 5) { // Done/Cancel
                                    state = CREATE_QUIZ;
                                    break;
                                }
                                
                                string qid = questionIdInput.getValue();
                                string qtext = questionTextInput.getValue();
                                float marks = 1.0f;
                                try {
                                    marks = stof(marksInput.getValue());
                                } catch (...) {}
                                
                                Question* q = nullptr;
                                
                                if (type == 1) { // MCQ
                                    MCQ* m = new MCQ(qid.empty() ? "" : qid, qtext, marks);
                                    // For demo purposes, add some options
                                    m->addOption("Option 1");
                                    m->addOption("Option 2");
                                    m->addOption("Option 3");
                                    m->setCorrectOption(0);
                                    q = m;
                                } else if (type == 2) { // True/False
                                    TrueFalse* t = new TrueFalse(qid.empty() ? "" : qid, qtext, marks);
                                    t->setCorrectAnswer(true);
                                    q = t;
                                } else if (type == 3) { // Fill in the Blank
                                    FillInTheBlank* f = new FillInTheBlank(qid.empty() ? "" : qid, qtext, marks);
                                    f->setCorrectAnswer("Answer");
                                    q = f;
                                } else if (type == 4) { // Numerical
                                    NumericalQuestion* n = new NumericalQuestion(qid.empty() ? "" : qid, qtext, marks);
                                    n->setCorrectAnswer(0.0, 0.0);
                                    q = n;
                                }
                                
                                if (q) {
                                    currentQuestions.push_back(q);
                                }
                                state = CREATE_QUIZ;
                                break;
                            }
                        }
                    } else if (state == QUIZ_DETAILS) {
                        // Handle quiz question answers
                        if (currentAttempt && currentQuiz) {
                            // This would handle answering questions
                            state = MAIN_MENU;
                        }
                    }
                }
                
                // Update text inputs for all states
                if (state == CREATE_QUIZ) {
                    quizIdInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    quizTitleInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    quizDescInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    timeLimitInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                } else if (state == ATTEMPT_QUIZ) {
                    studentIdInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    studentNameInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    studentEmailInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                } else if (state == CREATE_QUESTION) {
                    questionIdInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    questionTextInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    marksInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                }
            }
        }

        window.clear(bgColor);

        if (state == MAIN_MENU) {
            for (auto& button : mainButtons) {
                button.update(mousePos);
                window.draw(button.shape);
                window.draw(button.text);
            }

            sf::Text prompt("Select an option:", font, 20);
            prompt.setFillColor(textColor);
            prompt.setPosition(50, 100);
            window.draw(prompt);
        } else if (state == CREATE_QUIZ) {
            sf::Text header("Create New Quiz", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 100);
            window.draw(header);
            
            sf::Text idLabel("Quiz ID:", font, 18);
            idLabel.setFillColor(textColor);
            idLabel.setPosition(50, 230);
            window.draw(idLabel);
            quizIdInput.setPosition(sf::Vector2f(50, 250));
            quizIdInput.draw(window);
            
            sf::Text titleLabel("Title:", font, 18);
            titleLabel.setFillColor(textColor);
            titleLabel.setPosition(50, 320);
            window.draw(titleLabel);
            quizTitleInput.setPosition(sf::Vector2f(50, 340));
            quizTitleInput.draw(window);
            
            sf::Text descLabel("Description:", font, 18);
            descLabel.setFillColor(textColor);
            descLabel.setPosition(50, 410);
            window.draw(descLabel);
            quizDescInput.setPosition(sf::Vector2f(50, 430));
            quizDescInput.draw(window);
            
            sf::Text timeLabel("Time Limit (minutes):", font, 18);
            timeLabel.setFillColor(textColor);
            timeLabel.setPosition(50, 500);
            window.draw(timeLabel);
            timeLimitInput.setPosition(sf::Vector2f(50, 520));
            timeLimitInput.draw(window);
            
            createDemoButton.setPosition(sf::Vector2f(50, 640));
            createInteractiveButton.setPosition(sf::Vector2f(370, 640));
            saveQuizButton.setPosition(sf::Vector2f(50, 710));
            backButton.setPosition(sf::Vector2f(370, 710));
            
            createDemoButton.update(mousePos);
            createInteractiveButton.update(mousePos);
            saveQuizButton.update(mousePos);
            backButton.update(mousePos);
            
            window.draw(createDemoButton.shape);
            window.draw(createDemoButton.text);
            window.draw(createInteractiveButton.shape);
            window.draw(createInteractiveButton.text);
            window.draw(saveQuizButton.shape);
            window.draw(saveQuizButton.text);
            window.draw(backButton.shape);
            window.draw(backButton.text);
            
            // Show current questions count
            sf::Text questionCount("Questions: " + to_string(currentQuestions.size()), font, 18);
            questionCount.setFillColor(textColor);
            questionCount.setPosition(50, 780);
            window.draw(questionCount);
            
        } else if (state == LIST_QUIZZES) {
            sf::Text header("Available Quizzes", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 100);
            window.draw(header);
            
            currentQuizzes = manager.listQuizzes();
            quizButtons.clear();
            
            for (size_t i = 0; i < currentQuizzes.size(); ++i) {
                Quiz* q = currentQuizzes[i];
                quizButtons.emplace_back(
                    sf::Vector2f(50, 150 + i * 100),
                    sf::Vector2f(900, 60),
                    to_string(i + 1) + ") " + q->getTitle() + " (" + q->getQuizID() + ") - " + to_string(q->getQuestions().size()) + " questions",
                    font,
                    static_cast<int>(i + 1)
                );
            }
            
            for (auto& button : quizButtons) {
                button.update(mousePos);
                window.draw(button.shape);
                window.draw(button.text);
            }
            
            if (currentQuizzes.empty()) {
                sf::Text noQuizzes("No quizzes available.", font, 22);
                noQuizzes.setFillColor(textColor);
                noQuizzes.setPosition(50, 150);
                window.draw(noQuizzes);
            }
            
        } else if (state == ATTEMPT_QUIZ) {
            sf::Text header("Attempt a Quiz", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 100);
            window.draw(header);
            
            sf::Text selectLabel("Select a quiz to attempt:", font, 22);
            selectLabel.setFillColor(textColor);
            selectLabel.setPosition(50, 150);
            window.draw(selectLabel);
            
            availableQuizzes = manager.listQuizzes();
            attemptQuizButtons.clear();
            
            for (size_t i = 0; i < availableQuizzes.size(); ++i) {
                Quiz* q = availableQuizzes[i];
                attemptQuizButtons.emplace_back(
                    sf::Vector2f(50, 200 + i * 100),
                    sf::Vector2f(900, 60),
                    to_string(i + 1) + ") " + q->getTitle() + " (" + q->getQuizID() + ") - " + to_string(q->getQuestions().size()) + " questions",
                    font,
                    static_cast<int>(i + 1)
                );
            }
            
            for (auto& button : attemptQuizButtons) {
                button.update(mousePos);
                window.draw(button.shape);
                window.draw(button.text);
            }
            
            if (availableQuizzes.empty()) {
                sf::Text noQuizzes("No quizzes available. Create one first.", font, 22);
                noQuizzes.setFillColor(textColor);
                noQuizzes.setPosition(50, 200);
                window.draw(noQuizzes);
            } else {
                sf::Text studentLabel("Student Information:", font, 22);
                studentLabel.setFillColor(textColor);
                studentLabel.setPosition(50, 400);
                window.draw(studentLabel);
                
                sf::Text idLabel("Student ID:", font, 18);
                idLabel.setFillColor(textColor);
                idLabel.setPosition(50, 450);
                window.draw(idLabel);
                studentIdInput.setPosition(sf::Vector2f(50, 470));
                studentIdInput.draw(window);
                
                sf::Text nameLabel("Name:", font, 18);
                nameLabel.setFillColor(textColor);
                nameLabel.setPosition(50, 540);
                window.draw(nameLabel);
                studentNameInput.setPosition(sf::Vector2f(50, 560));
                studentNameInput.draw(window);
                
                sf::Text emailLabel("Email:", font, 18);
                emailLabel.setFillColor(textColor);
                emailLabel.setPosition(50, 630);
                window.draw(emailLabel);
                studentEmailInput.setPosition(sf::Vector2f(50, 650));
                studentEmailInput.draw(window);
                
                startAttemptButton.setPosition(sf::Vector2f(50, 740));
                backToMenuButton.setPosition(sf::Vector2f(370, 740));
                
                startAttemptButton.update(mousePos);
                backToMenuButton.update(mousePos);
                
                window.draw(startAttemptButton.shape);
                window.draw(startAttemptButton.text);
                window.draw(backToMenuButton.shape);
                window.draw(backToMenuButton.text);
            }
            
        } else if (state == CREATE_QUESTION) {
            sf::Text header("Create Question", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 100);
            window.draw(header);
            
            sf::Text typeLabel("Select question type:", font, 22);
            typeLabel.setFillColor(textColor);
            typeLabel.setPosition(50, 150);
            window.draw(typeLabel);
            
            for (auto& button : questionTypeButtons) {
                button.update(mousePos);
                window.draw(button.shape);
                window.draw(button.text);
            }
            
            sf::Text idLabel("Question ID:", font, 18);
            idLabel.setFillColor(textColor);
            idLabel.setPosition(50, 500);
            window.draw(idLabel);
            questionIdInput.setPosition(sf::Vector2f(50, 520));
            questionIdInput.draw(window);
            
            sf::Text textLabel("Question Text:", font, 18);
            textLabel.setFillColor(textColor);
            textLabel.setPosition(50, 610);
            window.draw(textLabel);
            questionTextInput.setPosition(sf::Vector2f(50, 630));
            questionTextInput.draw(window);
            
            sf::Text marksLabel("Marks:", font, 18);
            marksLabel.setFillColor(textColor);
            marksLabel.setPosition(50, 700);
            window.draw(marksLabel);
            marksInput.setPosition(sf::Vector2f(50, 720));
            marksInput.draw(window);
            
            addQuestionButton.setPosition(sf::Vector2f(50, 820));
            backToCreateQuizButton.setPosition(sf::Vector2f(370, 820));
            
            addQuestionButton.update(mousePos);
            backToCreateQuizButton.update(mousePos);
            
            window.draw(addQuestionButton.shape);
            window.draw(addQuestionButton.text);
            window.draw(backToCreateQuizButton.shape);
            window.draw(backToCreateQuizButton.text);
            
        } else if (state == QUIZ_DETAILS) {
            if (currentQuiz) {
                sf::Text header("Quiz Details", font, 32);
                header.setFillColor(textColor);
                header.setPosition(50, 100);
                window.draw(header);
                
                sf::Text titleText("Title: " + currentQuiz->getTitle(), font, 22);
                titleText.setFillColor(textColor);
                titleText.setPosition(50, 150);
                window.draw(titleText);
                
                sf::Text idText("ID: " + currentQuiz->getQuizID(), font, 22);
                idText.setFillColor(textColor);
                idText.setPosition(50, 200);
                window.draw(idText);
                
                sf::Text timeText("Time Limit: " + to_string(currentQuiz->getTimeLimit()) + " minutes", font, 22);
                timeText.setFillColor(textColor);
                timeText.setPosition(50, 250);
                window.draw(timeText);
                
                sf::Text questionsHeader("Questions:", font, 22);
                questionsHeader.setFillColor(textColor);
                questionsHeader.setPosition(50, 300);
                window.draw(questionsHeader);
                
                const auto& questions = currentQuiz->getQuestions();
                for (size_t i = 0; i < questions.size(); ++i) {
                    sf::Text questionText(to_string(i + 1) + ". " + questions[i]->getQuestionText(), font, 18);
                    questionText.setFillColor(textColor);
                    questionText.setPosition(50, 350 + i * 50);
                    window.draw(questionText);
                }
                
                if (currentAttempt) {
                    sf::Text attemptHeader("Quiz Attempt", font, 22);
                    attemptHeader.setFillColor(textColor);
                    attemptHeader.setPosition(50, 600);
                    window.draw(attemptHeader);
                    
                    sf::Text studentText("Student: " + currentStudent->getName() + " (" + currentStudent->getStudentID() + ")", font, 18);
                    studentText.setFillColor(textColor);
                    studentText.setPosition(50, 650);
                    window.draw(studentText);
                    
                    sf::Text scoreText("Score: " + to_string(currentAttempt->getScore()), font, 18);
                    scoreText.setFillColor(textColor);
                    scoreText.setPosition(50, 700);
                    window.draw(scoreText);
                }
            }
        }

        window.draw(title);
        window.display();
    }

    // Clean up
    if (currentQuiz) delete currentQuiz;
    if (currentStudent) delete currentStudent;
    if (currentAttempt) delete currentAttempt;

    return 0;
}


// g++ -std=c++17 *.cpp -I"path\to\SFML\include" -L"path\to\SFML\lib" -lsfml-graphics -lsfml-window -lsfml-system -o quiz_sfml.exe
// .\quiz_sfml.exe