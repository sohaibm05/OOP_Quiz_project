#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
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
#include "CSVQuestionLoader.hpp"

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
    
    void setText(const string& newText, const sf::Font& font) {
        text.setString(newText);
        text.setFont(font);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(
            shape.getPosition().x + shape.getSize().x / 2 - bounds.width / 2,
            shape.getPosition().y + shape.getSize().y / 2 - bounds.height / 2 - 5
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
            background.setOutlineColor(sf::Color(100, 150, 255));
        } else if (clicked && !background.getGlobalBounds().contains(mousePos)) {
            isActive = false;
            background.setOutlineColor(sf::Color(80, 80, 80));
        }
    }

    void handleInput(char c) {
        if (isActive) {
            if (c == '\b' && !content.empty()) {
                content.pop_back();
            } else if (c >= 32 && c < 127) { // Printable ASCII characters
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
        if (isActive) {
            background.setOutlineColor(sf::Color(100, 150, 255));
        }
        text.setString(content);
        window.draw(background);
        window.draw(text);
    }
};

// Helper function to load font (tries multiple paths)
bool loadFont(sf::Font& font) {
    // Try Windows font paths
    if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) return true;
    if (font.loadFromFile("C:/Windows/Fonts/calibri.ttf")) return true;
    if (font.loadFromFile("C:/Windows/Fonts/times.ttf")) return true;
    
    // Try Linux font paths
    if (font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) return true;
    if (font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) return true;
    
    // Try macOS font paths
    if (font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) return true;
    if (font.loadFromFile("/Library/Fonts/Arial.ttf")) return true;
    
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 1100), "Quiz App", sf::Style::Default);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!loadFont(font)) {
        cerr << "Error: Could not load font. The app may not display correctly." << endl;
        // Continue anyway - SFML will use a default font
    }

    sf::Color bgColor(15, 15, 15);
    sf::Color panelColor(30, 30, 30, 220);
    sf::Color textColor(220, 220, 220);
    sf::Color highlightColor(100, 150, 255);

    QuizManager manager;
    StudentManager studentManager;
    
    // Parsing helpers
    auto parseInt = [](const string &s, int def)->int {
        if (s.empty()) return def;
        stringstream ss(s);
        int v; if (ss >> v) return v; return def;
    };
    auto parseFloat = [](const string &s, float def)->float {
        if (s.empty()) return def;
        stringstream ss(s);
        float v; if (ss >> v) return v; return def;
    };
    
    // State management
    enum AppState {
        MAIN_MENU = 0,
        CREATE_QUIZ = 1,
        LIST_QUIZZES = 2,
        ATTEMPT_QUIZ = 3,
        LIST_STUDENTS = 5,
        FIND_STUDENTS = 6,
        LOAD_CSV = 7,
        CREATE_QUESTION = 10,
        QUIZ_DETAILS = 11,
        ANSWERING_QUIZ = 12,
        CREATE_MCQ_OPTIONS = 13
    };
    
    int state = MAIN_MENU;
    int scrollOffset = 0;
    
    // Main menu buttons
    vector<Button> mainButtons;
    vector<string> menuOptions = {"Create quiz", "List quizzes", "Attempt a quiz", "Exit", "List students", "Find students by id", "Load questions from CSV"};
    
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        mainButtons.emplace_back(
            sf::Vector2f(50, 150 + i * 90),
            sf::Vector2f(400, 50),
            to_string(i+1) + ". " + menuOptions[i],
            font,
            static_cast<int>(i == 3 ? 4 : (i == 6 ? 7 : i + 1))
        );
    }

    sf::Text title("QUIZ APP", font, 42);
    title.setFillColor(textColor);
    title.setPosition(50, 50);
    title.setStyle(sf::Text::Bold);

    // Create quiz state variables
    TextInput quizIdInput(sf::Vector2f(50, 300), sf::Vector2f(300, 50), font);
    TextInput quizTitleInput(sf::Vector2f(50, 400), sf::Vector2f(300, 50), font);
    TextInput quizDescInput(sf::Vector2f(50, 500), sf::Vector2f(300, 50), font);
    TextInput timeLimitInput(sf::Vector2f(50, 600), sf::Vector2f(300, 50), font);
    Button createDemoButton(sf::Vector2f(50, 700), sf::Vector2f(300, 50), "Add Demo Questions", font, 100);
    Button createInteractiveButton(sf::Vector2f(370, 700), sf::Vector2f(300, 50), "Create Interactively", font, 101);
    Button loadCSVButton(sf::Vector2f(690, 700), sf::Vector2f(300, 50), "Load from CSV", font, 102);
    Button saveQuizButton(sf::Vector2f(50, 780), sf::Vector2f(300, 50), "Save Quiz", font, 103);
    Button backButton(sf::Vector2f(370, 780), sf::Vector2f(300, 50), "Back to Menu", font, 104);

    // List quizzes state variables
    vector<Button> quizButtons;
    vector<Quiz*> currentQuizzes;
    Button backToListButton(sf::Vector2f(50, 1000), sf::Vector2f(200, 50), "Back", font, 200);

    // Attempt quiz state variables
    vector<Button> attemptQuizButtons;
    vector<Quiz*> availableQuizzes;
    int selectedAttemptQuiz = -1;
    TextInput studentIdInput(sf::Vector2f(50, 700), sf::Vector2f(300, 50), font);
    TextInput studentNameInput(sf::Vector2f(50, 790), sf::Vector2f(300, 50), font);
    TextInput studentEmailInput(sf::Vector2f(50, 880), sf::Vector2f(300, 50), font);
    Button startAttemptButton(sf::Vector2f(50, 970), sf::Vector2f(300, 50), "Start Attempt", font, 201);
    Button backToMenuButton(sf::Vector2f(370, 970), sf::Vector2f(300, 50), "Back to Menu", font, 202);

    // Question creation state variables
    int currentQuestionType = 0;
    vector<Button> questionTypeButtons;
    vector<string> questionTypes = {"MCQ", "True/False", "Fill in the Blank", "Numerical", "Done/Cancel"};
    for (size_t i = 0; i < questionTypes.size(); ++i) {
        questionTypeButtons.emplace_back(
            sf::Vector2f(50, 150 + i * 75),
            sf::Vector2f(350, 50),
            to_string(i+1) + ". " + questionTypes[i],
            font,
            static_cast<int>(i + 1)
        );
    }
    TextInput questionIdInput(sf::Vector2f(50, 600), sf::Vector2f(300, 50), font);
    TextInput questionTextInput(sf::Vector2f(50, 700), sf::Vector2f(300, 50), font);
    TextInput marksInput(sf::Vector2f(50, 800), sf::Vector2f(300, 50), font);
    Button addQuestionButton(sf::Vector2f(50, 900), sf::Vector2f(300, 50), "Add Question", font, 300);
    Button backToCreateQuizButton(sf::Vector2f(370, 900), sf::Vector2f(300, 50), "Back to Create Quiz", font, 301);

    // MCQ options creation
    vector<TextInput> mcqOptionInputs;
    TextInput correctOptionInput(sf::Vector2f(50, 900), sf::Vector2f(300, 50), font);
    Button addMCQButton(sf::Vector2f(50, 980), sf::Vector2f(300, 50), "Add MCQ Question", font, 400);
    Button backFromMCQButton(sf::Vector2f(370, 980), sf::Vector2f(300, 50), "Back", font, 401);
    for (int i = 0; i < 4; ++i) {
        mcqOptionInputs.emplace_back(sf::Vector2f(50, 600 + i * 75), sf::Vector2f(400, 50), font);
    }

    // Quiz answering state variables
    int currentQuestionIndex = 0;
    TextInput answerInput(sf::Vector2f(50, 600), sf::Vector2f(500, 50), font);
    Button nextQuestionButton(sf::Vector2f(50, 700), sf::Vector2f(200, 50), "Next Question", font, 500);
    Button submitQuizButton(sf::Vector2f(270, 700), sf::Vector2f(200, 50), "Submit Quiz", font, 501);
    Button backFromQuizButton(sf::Vector2f(50, 1000), sf::Vector2f(200, 50), "Back to Menu", font, 502);

    // CSV loading state variables
    TextInput csvPathInput(sf::Vector2f(50, 250), sf::Vector2f(600, 50), font);
    TextInput csvQuizIdInput(sf::Vector2f(50, 340), sf::Vector2f(300, 50), font);
    TextInput csvQuizTitleInput(sf::Vector2f(370, 340), sf::Vector2f(300, 50), font);
    TextInput csvQuizDescInput(sf::Vector2f(50, 430), sf::Vector2f(300, 50), font);
    TextInput csvTimeLimitInput(sf::Vector2f(370, 430), sf::Vector2f(300, 50), font);
    Button loadCSVButton2(sf::Vector2f(50, 520), sf::Vector2f(300, 50), "Load CSV", font, 600);
    Button backFromCSVButton(sf::Vector2f(370, 520), sf::Vector2f(300, 50), "Back to Menu", font, 601);

    // Find students state variables
    TextInput searchIdInput(sf::Vector2f(50, 250), sf::Vector2f(400, 50), font);
    Button searchButton(sf::Vector2f(470, 250), sf::Vector2f(200, 50), "Search", font, 700);
    Button backFromSearchButton(sf::Vector2f(50, 1000), sf::Vector2f(200, 50), "Back to Menu", font, 701);
    vector<Button> studentResultButtons;
    vector<Student*> searchResults;

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
            
            // Handle scrolling
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (state == LIST_QUIZZES || state == LIST_STUDENTS || state == FIND_STUDENTS) {
                    scrollOffset += static_cast<int>(event.mouseWheelScroll.delta * 30);
                    if (scrollOffset < 0) scrollOffset = 0;
                }
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
                } else if (state == CREATE_MCQ_OPTIONS) {
                    for (auto& input : mcqOptionInputs) {
                        input.handleInput(c);
                    }
                    correctOptionInput.handleInput(c);
                } else if (state == ANSWERING_QUIZ) {
                    answerInput.handleInput(c);
                } else if (state == LOAD_CSV) {
                    csvPathInput.handleInput(c);
                    csvQuizIdInput.handleInput(c);
                    csvQuizTitleInput.handleInput(c);
                    csvQuizDescInput.handleInput(c);
                    csvTimeLimitInput.handleInput(c);
                } else if (state == FIND_STUDENTS) {
                    searchIdInput.handleInput(c);
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (state == MAIN_MENU) {
                        for (auto& button : mainButtons) {
                            if (button.isClicked(mousePos)) {
                                if (button.id == 4) {
                                    window.close();
                                } else {
                                    state = button.id;
                                }
                                break;
                            }
                        }
                    } else if (state == CREATE_QUIZ) {
                        createDemoButton.update(mousePos);
                        createInteractiveButton.update(mousePos);
                        loadCSVButton.update(mousePos);
                        saveQuizButton.update(mousePos);
                        backButton.update(mousePos);
                        
                        if (createDemoButton.isClicked(mousePos)) {
                            MCQ* mcq = new MCQ("Q1", "What is 2 + 2?", 5.0f);
                            mcq->addOption("3");
                            mcq->addOption("4");
                            mcq->addOption("5");
                            mcq->setCorrectOption(1);
                            
                            TrueFalse* tf = new TrueFalse("Q2", "The earth is round.", 2.0f);
                            tf->setCorrectAnswer(true);
                            
                            FillInTheBlank* fib = new FillInTheBlank("Q3", "Capital of France?", 3.0f);
                            fib->setCorrectAnswer("Paris");
                            
                            NumericalQuestion* num = new NumericalQuestion("Q4", "Value of pi (approx)?", 4.0f);
                            num->setCorrectAnswer(3.14159, 0.01);
                            
                            currentQuestions.push_back(mcq);
                            currentQuestions.push_back(tf);
                            currentQuestions.push_back(fib);
                            currentQuestions.push_back(num);
                        } else if (createInteractiveButton.isClicked(mousePos)) {
                            state = CREATE_QUESTION;
                            questionIdInput.clear();
                            questionTextInput.clear();
                            marksInput.clear();
                        } else if (loadCSVButton.isClicked(mousePos)) {
                            state = LOAD_CSV;
                        } else if (saveQuizButton.isClicked(mousePos)) {
                            string qid = quizIdInput.getValue();
                            if (qid.empty()) {
                                qid = "Quiz" + to_string(manager.count() + 1);
                            }
                            string qtitle = quizTitleInput.getValue();
                            string qdesc = quizDescInput.getValue();
                            int tlim = parseInt(timeLimitInput.getValue(), 30);
                            
                            if (!currentQuestions.empty()) {
                                Quiz* quiz = new Quiz(qid, qtitle, qdesc, tlim);
                                for (Question* q : currentQuestions) {
                                    quiz->addQuestion(q);
                                }
                                manager.addQuiz(quiz);
                                currentQuestions.clear();
                                quizIdInput.clear();
                                quizTitleInput.clear();
                                quizDescInput.clear();
                                timeLimitInput.clear();
                                state = MAIN_MENU;
                            }
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
                        if (backToListButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                        }
                    } else if (state == ATTEMPT_QUIZ) {
                        for (size_t bi = 0; bi < attemptQuizButtons.size(); ++bi) {
                            Button &b = attemptQuizButtons[bi];
                            if (b.isClicked(mousePos)) {
                                selectedAttemptQuiz = static_cast<int>(bi);
                                currentQuiz = availableQuizzes[selectedAttemptQuiz];
                                break;
                            }
                        }

                        startAttemptButton.update(mousePos);
                        backToMenuButton.update(mousePos);
                        
                        if (startAttemptButton.isClicked(mousePos)) {
                            if (selectedAttemptQuiz >= 0 && selectedAttemptQuiz < static_cast<int>(availableQuizzes.size())) {
                                string sid = studentIdInput.getValue();
                                if (sid.empty()) {
                                    sid = "S" + to_string(studentManager.count() + 1);
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

                                currentQuiz = availableQuizzes[selectedAttemptQuiz];
                                currentAttempt = new QuizAttempt("A" + to_string(s->getAttempts().size() + 1), s, currentQuiz);
                                s->addAttempt(currentAttempt);
                                currentQuestionIndex = 0;
                                state = ANSWERING_QUIZ;
                                answerInput.clear();
                            }
                        } else if (backToMenuButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                            studentIdInput.clear();
                            studentNameInput.clear();
                            studentEmailInput.clear();
                            selectedAttemptQuiz = -1;
                        }
                    } else if (state == CREATE_QUESTION) {
                        for (auto& button : questionTypeButtons) {
                            if (button.isClicked(mousePos)) {
                                int type = button.id;
                                if (type == 5) {
                                    state = CREATE_QUIZ;
                                    break;
                                } else if (type == 1) {
                                    // MCQ - go to MCQ options screen
                                    currentQuestionType = 1;
                                    state = CREATE_MCQ_OPTIONS;
                                    for (auto& input : mcqOptionInputs) {
                                        input.clear();
                                    }
                                    correctOptionInput.clear();
                                    break;
                                } else {
                                    currentQuestionType = type;
                                    // For other types, we can create directly
                                    string qid = questionIdInput.getValue();
                                    string qtext = questionTextInput.getValue();
                                    float marks = parseFloat(marksInput.getValue(), 1.0f);
                                    
                                    Question* q = nullptr;
                                    
                                    if (type == 2) {
                                        TrueFalse* t = new TrueFalse(qid, qtext, marks);
                                        t->setCorrectAnswer(true);
                                        q = t;
                                    } else if (type == 3) {
                                        FillInTheBlank* f = new FillInTheBlank(qid, qtext, marks);
                                        f->setCorrectAnswer("Answer");
                                        q = f;
                                    } else if (type == 4) {
                                        NumericalQuestion* n = new NumericalQuestion(qid, qtext, marks);
                                        n->setCorrectAnswer(0.0, 0.01);
                                        q = n;
                                    }
                                    
                                    if (q) {
                                        currentQuestions.push_back(q);
                                    }
                                    state = CREATE_QUIZ;
                                    questionIdInput.clear();
                                    questionTextInput.clear();
                                    marksInput.clear();
                                    break;
                                }
                            }
                        }
                    } else if (state == CREATE_MCQ_OPTIONS) {
                        addMCQButton.update(mousePos);
                        backFromMCQButton.update(mousePos);
                        
                        if (addMCQButton.isClicked(mousePos)) {
                            string qid = questionIdInput.getValue();
                            string qtext = questionTextInput.getValue();
                            float marks = parseFloat(marksInput.getValue(), 1.0f);
                            
                            MCQ* m = new MCQ(qid, qtext, marks);
                            for (size_t i = 0; i < mcqOptionInputs.size(); ++i) {
                                string opt = mcqOptionInputs[i].getValue();
                                if (!opt.empty()) {
                                    m->addOption(opt);
                                }
                            }
                            int correctIdx = parseInt(correctOptionInput.getValue(), 1) - 1;
                            if (correctIdx >= 0 && correctIdx < static_cast<int>(m->getOptions().size())) {
                                m->setCorrectOption(correctIdx);
                            }
                            
                            currentQuestions.push_back(m);
                            state = CREATE_QUIZ;
                            questionIdInput.clear();
                            questionTextInput.clear();
                            marksInput.clear();
                            for (auto& input : mcqOptionInputs) {
                                input.clear();
                            }
                            correctOptionInput.clear();
                        } else if (backFromMCQButton.isClicked(mousePos)) {
                            state = CREATE_QUESTION;
                        }
                    } else if (state == ANSWERING_QUIZ) {
                        nextQuestionButton.update(mousePos);
                        submitQuizButton.update(mousePos);
                        backFromQuizButton.update(mousePos);
                        
                        if (nextQuestionButton.isClicked(mousePos) || submitQuizButton.isClicked(mousePos)) {
                            if (currentQuiz && currentAttempt) {
                                const auto& questions = currentQuiz->getQuestions();
                                if (currentQuestionIndex < static_cast<int>(questions.size())) {
                                    string answer = answerInput.getValue();
                                    currentAttempt->setAnswer(questions[currentQuestionIndex], answer);
                                }
                                
                                currentQuestionIndex++;
                                answerInput.clear();
                                
                                if (currentQuestionIndex >= static_cast<int>(questions.size()) || submitQuizButton.isClicked(mousePos)) {
                                    currentAttempt->submit();
                                    state = QUIZ_DETAILS;
                                }
                            }
                        } else if (backFromQuizButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                        }
                    } else if (state == QUIZ_DETAILS) {
                        if (backToListButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                            currentQuiz = nullptr;
                            currentAttempt = nullptr;
                            currentStudent = nullptr;
                        }
                    } else if (state == LOAD_CSV) {
                        loadCSVButton2.update(mousePos);
                        backFromCSVButton.update(mousePos);
                        
                        if (loadCSVButton2.isClicked(mousePos)) {
                            string csvPath = csvPathInput.getValue();
                            if (csvPath.empty()) {
                                csvPath = "sample_questions.csv";
                            }
                            
                            string qid = csvQuizIdInput.getValue();
                            if (qid.empty()) {
                                qid = "Quiz" + to_string(manager.count() + 1);
                            }
                            
                            string qtitle = csvQuizTitleInput.getValue();
                            if (qtitle.empty()) {
                                qtitle = "Quiz from CSV";
                            }
                            
                            string qdesc = csvQuizDescInput.getValue();
                            if (qdesc.empty()) {
                                qdesc = "Imported from CSV";
                            }
                            
                            int tlim = parseInt(csvTimeLimitInput.getValue(), 30);
                            
                            vector<Question*> questions = CSVQuestionLoader::loadFromCSV(csvPath);
                            if (!questions.empty()) {
                                Quiz* quiz = new Quiz(qid, qtitle, qdesc, tlim);
                                for (Question* q : questions) {
                                    quiz->addQuestion(q);
                                }
                                manager.addQuiz(quiz);
                                state = MAIN_MENU;
                                csvPathInput.clear();
                                csvQuizIdInput.clear();
                                csvQuizTitleInput.clear();
                                csvQuizDescInput.clear();
                                csvTimeLimitInput.clear();
                            }
                        } else if (backFromCSVButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                        }
                    } else if (state == LIST_STUDENTS) {
                        if (backToListButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                        }
                    } else if (state == FIND_STUDENTS) {
                        searchButton.update(mousePos);
                        backFromSearchButton.update(mousePos);
                        
                        if (searchButton.isClicked(mousePos)) {
                            string searchId = searchIdInput.getValue();
                            if (!searchId.empty()) {
                                searchResults = studentManager.findStudentsByID(searchId);
                            }
                        } else if (backFromSearchButton.isClicked(mousePos)) {
                            state = MAIN_MENU;
                            searchIdInput.clear();
                            searchResults.clear();
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
                    for (auto& b : attemptQuizButtons) b.update(mousePos);
                    studentIdInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    studentNameInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    studentEmailInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                } else if (state == CREATE_QUESTION) {
                    questionIdInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    questionTextInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    marksInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                } else if (state == CREATE_MCQ_OPTIONS) {
                    for (auto& input : mcqOptionInputs) {
                        input.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    }
                    correctOptionInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                } else if (state == ANSWERING_QUIZ) {
                    answerInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                } else if (state == LOAD_CSV) {
                    csvPathInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    csvQuizIdInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    csvQuizTitleInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    csvQuizDescInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                    csvTimeLimitInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
                } else if (state == FIND_STUDENTS) {
                    searchIdInput.update(mousePos, event.mouseButton.button == sf::Mouse::Left);
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
        } else if (state == CREATE_QUIZ) {
            sf::Text header("Create New Quiz", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 100);
            window.draw(header);
            
            sf::Text idLabel("Quiz ID:", font, 18);
            idLabel.setFillColor(textColor);
            idLabel.setPosition(50, 250);
            window.draw(idLabel);
            quizIdInput.setPosition(sf::Vector2f(50, 280));
            quizIdInput.draw(window);
            
            sf::Text titleLabel("Title:", font, 18);
            titleLabel.setFillColor(textColor);
            titleLabel.setPosition(50, 350);
            window.draw(titleLabel);
            quizTitleInput.setPosition(sf::Vector2f(50, 380));
            quizTitleInput.draw(window);
            
            sf::Text descLabel("Description:", font, 18);
            descLabel.setFillColor(textColor);
            descLabel.setPosition(50, 450);
            window.draw(descLabel);
            quizDescInput.setPosition(sf::Vector2f(50, 480));
            quizDescInput.draw(window);
            
            sf::Text timeLabel("Time Limit (minutes):", font, 18);
            timeLabel.setFillColor(textColor);
            timeLabel.setPosition(50, 550);
            window.draw(timeLabel);
            timeLimitInput.setPosition(sf::Vector2f(50, 580));
            timeLimitInput.draw(window);
            
            createDemoButton.update(mousePos);
            createInteractiveButton.update(mousePos);
            loadCSVButton.update(mousePos);
            saveQuizButton.update(mousePos);
            backButton.update(mousePos);
            
            window.draw(createDemoButton.shape);
            window.draw(createDemoButton.text);
            window.draw(createInteractiveButton.shape);
            window.draw(createInteractiveButton.text);
            window.draw(loadCSVButton.shape);
            window.draw(loadCSVButton.text);
            window.draw(saveQuizButton.shape);
            window.draw(saveQuizButton.text);
            window.draw(backButton.shape);
            window.draw(backButton.text);
            
            sf::Text questionCount("Questions: " + to_string(currentQuestions.size()), font, 18);
            questionCount.setFillColor(textColor);
            questionCount.setPosition(50, 830);
            window.draw(questionCount);
            
        } else if (state == LIST_QUIZZES) {
            sf::Text header("Available Quizzes", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 110);
            window.draw(header);
            
            currentQuizzes = manager.listQuizzes();
            quizButtons.clear();
            
            float yPos = 160 - scrollOffset;
            for (size_t i = 0; i < currentQuizzes.size(); ++i) {
                Quiz* q = currentQuizzes[i];
                if (yPos > -100 && yPos < 1100) {
                    quizButtons.emplace_back(
                        sf::Vector2f(50, yPos),
                        sf::Vector2f(900, 60),
                        to_string(i + 1) + ") " + q->getTitle() + " (" + q->getQuizID() + ") - " + to_string(q->getQuestions().size()) + " questions",
                        font,
                        static_cast<int>(i + 1)
                    );
                }
                yPos += 80;
            }
            
            for (auto& button : quizButtons) {
                button.update(mousePos);
                window.draw(button.shape);
                window.draw(button.text);
            }
            
            if (currentQuizzes.empty()) {
                sf::Text noQuizzes("No quizzes available.", font, 22);
                noQuizzes.setFillColor(textColor);
                noQuizzes.setPosition(50, 210);
                window.draw(noQuizzes);
            }
            
            backToListButton.update(mousePos);
            window.draw(backToListButton.shape);
            window.draw(backToListButton.text);
            
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
            
            for (size_t i = 0; i < availableQuizzes.size() && i < 5; ++i) {
                Quiz* q = availableQuizzes[i];
                attemptQuizButtons.emplace_back(
                    sf::Vector2f(50, 200 + i * 90),
                    sf::Vector2f(900, 50),
                    to_string(i + 1) + ") " + q->getTitle() + " (" + q->getQuizID() + ") - " + to_string(q->getQuestions().size()) + " questions",
                    font,
                    static_cast<int>(i + 1)
                );
            }
            
            for (size_t bi = 0; bi < attemptQuizButtons.size(); ++bi) {
                Button &button = attemptQuizButtons[bi];
                button.update(mousePos);
                if (static_cast<int>(bi) == selectedAttemptQuiz) {
                    button.shape.setFillColor(highlightColor);
                }
                window.draw(button.shape);
                window.draw(button.text);
                if (static_cast<int>(bi) == selectedAttemptQuiz) {
                    button.shape.setFillColor(sf::Color(50,50,50));
                }
            }
            
            if (availableQuizzes.empty()) {
                sf::Text noQuizzes("No quizzes available. Create one first.", font, 22);
                noQuizzes.setFillColor(textColor);
                noQuizzes.setPosition(50, 200);
                window.draw(noQuizzes);
            } else {
                sf::Text studentLabel("Student Information:", font, 22);
                studentLabel.setFillColor(textColor);
                studentLabel.setPosition(50, 650);
                window.draw(studentLabel);
                
                sf::Text idLabel("Student ID:", font, 18);
                idLabel.setFillColor(textColor);
                idLabel.setPosition(50, 670);
                window.draw(idLabel);
                studentIdInput.setPosition(sf::Vector2f(50, 700));
                studentIdInput.draw(window);
                
                sf::Text nameLabel("Name:", font, 18);
                nameLabel.setFillColor(textColor);
                nameLabel.setPosition(50, 760);
                window.draw(nameLabel);
                studentNameInput.setPosition(sf::Vector2f(50, 790));
                studentNameInput.draw(window);
                
                sf::Text emailLabel("Email:", font, 18);
                emailLabel.setFillColor(textColor);
                emailLabel.setPosition(50, 850);
                window.draw(emailLabel);
                studentEmailInput.setPosition(sf::Vector2f(50, 880));
                studentEmailInput.draw(window);
                
                startAttemptButton.setPosition(sf::Vector2f(50, 970));
                backToMenuButton.setPosition(sf::Vector2f(370, 970));
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
            header.setPosition(50, 50);
            window.draw(header);
            
            sf::Text typeLabel("Select question type:", font, 22);
            typeLabel.setFillColor(textColor);
            typeLabel.setPosition(50, 100);
            window.draw(typeLabel);
            
            for (auto& button : questionTypeButtons) {
                button.update(mousePos);
                window.draw(button.shape);
                window.draw(button.text);
            }
            
            sf::Text idLabel("Question ID:", font, 18);
            idLabel.setFillColor(textColor);
            idLabel.setPosition(50, 550);
            window.draw(idLabel);
            questionIdInput.setPosition(sf::Vector2f(50, 580));
            questionIdInput.draw(window);
            
            sf::Text textLabel("Question Text:", font, 18);
            textLabel.setFillColor(textColor);
            textLabel.setPosition(50, 650);
            window.draw(textLabel);
            questionTextInput.setPosition(sf::Vector2f(50, 680));
            questionTextInput.draw(window);
            
            sf::Text marksLabel("Marks:", font, 18);
            marksLabel.setFillColor(textColor);
            marksLabel.setPosition(50, 750);
            window.draw(marksLabel);
            marksInput.setPosition(sf::Vector2f(50, 780));
            marksInput.draw(window);
            
            addQuestionButton.setPosition(sf::Vector2f(50, 900));
            backToCreateQuizButton.setPosition(sf::Vector2f(370, 900));
            
            addQuestionButton.update(mousePos);
            backToCreateQuizButton.update(mousePos);
            
            window.draw(addQuestionButton.shape);
            window.draw(addQuestionButton.text);
            window.draw(backToCreateQuizButton.shape);
            window.draw(backToCreateQuizButton.text);
            
        } else if (state == CREATE_MCQ_OPTIONS) {
            sf::Text header("Create MCQ Question", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 50);
            window.draw(header);
            
            sf::Text idLabel("Question ID:", font, 18);
            idLabel.setFillColor(textColor);
            idLabel.setPosition(50, 120);
            window.draw(idLabel);
            questionIdInput.setPosition(sf::Vector2f(50, 150));
            questionIdInput.draw(window);
            
            sf::Text textLabel("Question Text:", font, 18);
            textLabel.setFillColor(textColor);
            textLabel.setPosition(50, 220);
            window.draw(textLabel);
            questionTextInput.setPosition(sf::Vector2f(50, 250));
            questionTextInput.draw(window);
            
            sf::Text marksLabel("Marks:", font, 18);
            marksLabel.setFillColor(textColor);
            marksLabel.setPosition(50, 320);
            window.draw(marksLabel);
            marksInput.setPosition(sf::Vector2f(50, 350));
            marksInput.draw(window);
            
            sf::Text optionsLabel("Options (enter up to 4):", font, 18);
            optionsLabel.setFillColor(textColor);
            optionsLabel.setPosition(50, 550);
            window.draw(optionsLabel);
            
            for (size_t i = 0; i < mcqOptionInputs.size(); ++i) {
                sf::Text optLabel(to_string(i + 1) + ":", font, 16);
                optLabel.setFillColor(textColor);
                optLabel.setPosition(50, 580 + i * 80);
                window.draw(optLabel);
                mcqOptionInputs[i].setPosition(sf::Vector2f(50, 600 + i * 75));
                mcqOptionInputs[i].draw(window);
            }
            
            sf::Text correctLabel("Correct Option (1-4):", font, 18);
            correctLabel.setFillColor(textColor);
            correctLabel.setPosition(50, 860);
            window.draw(correctLabel);
            correctOptionInput.setPosition(sf::Vector2f(50, 890));
            correctOptionInput.draw(window);
            
            addMCQButton.update(mousePos);
            backFromMCQButton.update(mousePos);
            
            window.draw(addMCQButton.shape);
            window.draw(addMCQButton.text);
            window.draw(backFromMCQButton.shape);
            window.draw(backFromMCQButton.text);
            
        } else if (state == ANSWERING_QUIZ) {
            if (currentQuiz && currentAttempt) {
                const auto& questions = currentQuiz->getQuestions();
                if (currentQuestionIndex < static_cast<int>(questions.size())) {
                    Question* q = questions[currentQuestionIndex];
                    
                    sf::Text header("Answering Quiz: " + currentQuiz->getTitle(), font, 28);
                    header.setFillColor(textColor);
                    header.setPosition(50, 50);
                    window.draw(header);
                    
                    sf::Text progress("Question " + to_string(currentQuestionIndex + 1) + " of " + to_string(questions.size()), font, 20);
                    progress.setFillColor(textColor);
                    progress.setPosition(50, 100);
                    window.draw(progress);
                    
                    sf::Text questionText("Q: " + q->getQuestionText(), font, 22);
                    questionText.setFillColor(textColor);
                    questionText.setPosition(50, 150);
                    window.draw(questionText);
                    
                    // Display options for MCQ
                    MCQ* mcq = dynamic_cast<MCQ*>(q);
                    float yPos = 200;
                    if (mcq) {
                        const auto& options = mcq->getOptions();
                        sf::Text optionsHeader("Options:", font, 18);
                        optionsHeader.setFillColor(textColor);
                        optionsHeader.setPosition(50, yPos);
                        window.draw(optionsHeader);
                        yPos += 35;
                        
                        for (size_t i = 0; i < options.size(); ++i) {
                            sf::Text optionText(to_string(i + 1) + ") " + options[i], font, 16);
                            optionText.setFillColor(sf::Color(200, 200, 200));
                            optionText.setPosition(70, yPos);
                            window.draw(optionText);
                            yPos += 30;
                        }
                        
                        sf::Text optionsLabel("Enter option number (1-" + to_string(options.size()) + "):", font, 18);
                        optionsLabel.setFillColor(textColor);
                        optionsLabel.setPosition(50, yPos + 20);
                        window.draw(optionsLabel);
                        answerInput.setPosition(sf::Vector2f(50, yPos + 50));
                    } else {
                        sf::Text answerLabel("Your answer:", font, 18);
                        answerLabel.setFillColor(textColor);
                        answerLabel.setPosition(50, yPos);
                        window.draw(answerLabel);
                        answerInput.setPosition(sf::Vector2f(50, yPos + 40));
                    }
                    
                    answerInput.draw(window);
                    
                    float buttonY = 700;
                    if (mcq) {
                        buttonY = yPos + 100;
                    }
                    
                    nextQuestionButton.setPosition(sf::Vector2f(50, buttonY));
                    submitQuizButton.setPosition(sf::Vector2f(270, buttonY));
                    backFromQuizButton.setPosition(sf::Vector2f(50, buttonY + 70));
                    
                    nextQuestionButton.update(mousePos);
                    submitQuizButton.update(mousePos);
                    backFromQuizButton.update(mousePos);
                    
                    if (currentQuestionIndex < static_cast<int>(questions.size()) - 1) {
                        window.draw(nextQuestionButton.shape);
                        window.draw(nextQuestionButton.text);
                    } else {
                        window.draw(submitQuizButton.shape);
                        window.draw(submitQuizButton.text);
                    }
                    window.draw(backFromQuizButton.shape);
                    window.draw(backFromQuizButton.text);
                }
            }
            
        } else if (state == QUIZ_DETAILS) {
            if (currentQuiz) {
                sf::Text header("Quiz Details", font, 32);
                header.setFillColor(textColor);
                header.setPosition(50, 50);
                window.draw(header);
                
                sf::Text titleText("Title: " + currentQuiz->getTitle(), font, 22);
                titleText.setFillColor(textColor);
                titleText.setPosition(50, 100);
                window.draw(titleText);
                
                sf::Text idText("ID: " + currentQuiz->getQuizID(), font, 22);
                idText.setFillColor(textColor);
                idText.setPosition(50, 140);
                window.draw(idText);
                
                sf::Text timeText("Time Limit: " + to_string(currentQuiz->getTimeLimit()) + " minutes", font, 22);
                timeText.setFillColor(textColor);
                timeText.setPosition(50, 180);
                window.draw(timeText);
                
                sf::Text questionsHeader("Questions:", font, 22);
                questionsHeader.setFillColor(textColor);
                questionsHeader.setPosition(50, 220);
                window.draw(questionsHeader);
                
                const auto& questions = currentQuiz->getQuestions();
                float yPos = 260;
                for (size_t i = 0; i < questions.size() && yPos < 900; ++i) {
                    sf::Text questionText(to_string(i + 1) + ". " + questions[i]->getQuestionText(), font, 18);
                    questionText.setFillColor(textColor);
                    questionText.setPosition(50, yPos);
                    window.draw(questionText);
                    yPos += 40;
                }
                
                if (currentAttempt) {
                    sf::Text attemptHeader("Quiz Attempt Results", font, 22);
                    attemptHeader.setFillColor(highlightColor);
                    attemptHeader.setPosition(50, 900);
                    window.draw(attemptHeader);
                    
                    sf::Text studentText("Student: " + currentStudent->getName() + " (" + currentStudent->getStudentID() + ")", font, 18);
                    studentText.setFillColor(textColor);
                    studentText.setPosition(50, 940);
                    window.draw(studentText);
                    
                    float totalMarks = 0.0f;
                    for (Question* q : questions) {
                        totalMarks += q->getMarks();
                    }
                    
                    sf::Text scoreText("Score: " + to_string(currentAttempt->getScore()) + " / " + to_string(totalMarks), font, 18);
                    scoreText.setFillColor(textColor);
                    scoreText.setPosition(50, 980);
                    window.draw(scoreText);
                }
                
                backToListButton.update(mousePos);
                window.draw(backToListButton.shape);
                window.draw(backToListButton.text);
            }
            
        } else if (state == LOAD_CSV) {
            sf::Text header("Load Questions from CSV", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 100);
            window.draw(header);
            
            sf::Text pathLabel("CSV File Path:", font, 18);
            pathLabel.setFillColor(textColor);
            pathLabel.setPosition(50, 200);
            window.draw(pathLabel);
            csvPathInput.draw(window);
            
            sf::Text idLabel("Quiz ID:", font, 18);
            idLabel.setFillColor(textColor);
            idLabel.setPosition(50, 290);
            window.draw(idLabel);
            csvQuizIdInput.draw(window);
            
            sf::Text titleLabel("Quiz Title:", font, 18);
            titleLabel.setFillColor(textColor);
            titleLabel.setPosition(370, 290);
            window.draw(titleLabel);
            csvQuizTitleInput.draw(window);
            
            sf::Text descLabel("Description:", font, 18);
            descLabel.setFillColor(textColor);
            descLabel.setPosition(50, 380);
            window.draw(descLabel);
            csvQuizDescInput.draw(window);
            
            sf::Text timeLabel("Time Limit (minutes):", font, 18);
            timeLabel.setFillColor(textColor);
            timeLabel.setPosition(370, 380);
            window.draw(timeLabel);
            csvTimeLimitInput.draw(window);
            
            loadCSVButton2.update(mousePos);
            backFromCSVButton.update(mousePos);
            
            window.draw(loadCSVButton2.shape);
            window.draw(loadCSVButton2.text);
            window.draw(backFromCSVButton.shape);
            window.draw(backFromCSVButton.text);
            
        } else if (state == LIST_STUDENTS) {
            sf::Text header("Students and Attempts", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 110);
            window.draw(header);
            
            const auto& students = studentManager.listStudents();
            float yPos = 160 - scrollOffset;
            
            for (size_t i = 0; i < students.size(); ++i) {
                Student* st = students[i];
                if (yPos > -100 && yPos < 1000) {
                    sf::Text studentText(st->getName() + " (" + st->getStudentID() + ") - " + st->getEmail(), font, 18);
                    studentText.setFillColor(textColor);
                    studentText.setPosition(50, yPos);
                    window.draw(studentText);
                    
                    const auto& attempts = st->getAttempts();
                    sf::Text attemptsText("Attempts: " + to_string(attempts.size()), font, 16);
                    attemptsText.setFillColor(sf::Color(180, 180, 180));
                    attemptsText.setPosition(50, yPos + 30);
                    window.draw(attemptsText);
                    
                    yPos += 80;
                }
            }
            
            if (students.empty()) {
                sf::Text noStudents("No students recorded.", font, 22);
                noStudents.setFillColor(textColor);
                noStudents.setPosition(50, 210);
                window.draw(noStudents);
            }
            
            backToListButton.update(mousePos);
            window.draw(backToListButton.shape);
            window.draw(backToListButton.text);
            
        } else if (state == FIND_STUDENTS) {
            sf::Text header("Find Students by ID", font, 32);
            header.setFillColor(textColor);
            header.setPosition(50, 100);
            window.draw(header);
            
            sf::Text searchLabel("Enter ID to search:", font, 18);
            searchLabel.setFillColor(textColor);
            searchLabel.setPosition(50, 200);
            window.draw(searchLabel);
            searchIdInput.draw(window);
            
            searchButton.update(mousePos);
            window.draw(searchButton.shape);
            window.draw(searchButton.text);
            
            float yPos = 350;
            for (size_t i = 0; i < searchResults.size() && yPos < 1000; ++i) {
                Student* st = searchResults[i];
                
                // Student header
                sf::Text studentHeader("Student ID: " + st->getStudentID(), font, 20);
                studentHeader.setFillColor(highlightColor);
                studentHeader.setPosition(50, yPos);
                window.draw(studentHeader);
                yPos += 30;
                
                sf::Text nameText("Name: " + st->getName(), font, 18);
                nameText.setFillColor(textColor);
                nameText.setPosition(50, yPos);
                window.draw(nameText);
                yPos += 30;
                
                sf::Text emailText("Email: " + st->getEmail(), font, 18);
                emailText.setFillColor(textColor);
                emailText.setPosition(50, yPos);
                window.draw(emailText);
                yPos += 30;
                
                const auto& attempts = st->getAttempts();
                sf::Text attemptsCount("Attempts: " + to_string(attempts.size()), font, 18);
                attemptsCount.setFillColor(textColor);
                attemptsCount.setPosition(50, yPos);
                window.draw(attemptsCount);
                yPos += 40;
                
                // Display each attempt's details
                if (!attempts.empty()) {
                    sf::Text attemptsHeader("  Attempts:", font, 18);
                    attemptsHeader.setFillColor(sf::Color(200, 200, 200));
                    attemptsHeader.setPosition(50, yPos);
                    window.draw(attemptsHeader);
                    yPos += 30;
                    
                    for (size_t j = 0; j < attempts.size() && yPos < 1000; ++j) {
                        QuizAttempt* attempt = attempts[j];
                        
                        // Attempt ID
                        sf::Text attemptIdText("  - Attempt ID: " + attempt->getAttemptID(), font, 16);
                        attemptIdText.setFillColor(sf::Color(180, 180, 180));
                        attemptIdText.setPosition(50, yPos);
                        window.draw(attemptIdText);
                        yPos += 25;
                        
                        // Student info
                        sf::Text studentInfo("    Student: " + st->getName() + " (" + st->getStudentID() + ")", font, 16);
                        studentInfo.setFillColor(sf::Color(180, 180, 180));
                        studentInfo.setPosition(50, yPos);
                        window.draw(studentInfo);
                        yPos += 25;
                        
                        // Quiz info
                        if (attempt->getQuiz()) {
                            sf::Text quizInfo("    Quiz: " + attempt->getQuiz()->getTitle() + " (" + attempt->getQuiz()->getQuizID() + ")", font, 16);
                            quizInfo.setFillColor(sf::Color(180, 180, 180));
                            quizInfo.setPosition(50, yPos);
                            window.draw(quizInfo);
                            yPos += 25;
                        }
                        
                        // Score
                        sf::Text scoreText("    Score: " + to_string(static_cast<int>(attempt->getScore())), font, 16);
                        scoreText.setFillColor(sf::Color(180, 180, 180));
                        scoreText.setPosition(50, yPos);
                        window.draw(scoreText);
                        yPos += 25;
                        
                        // Start time
                        time_t startTime = attempt->getStartTime();
                        char startTimeStr[100];
                        struct tm timeinfo;
                        #ifdef _WIN32
                            localtime_s(&timeinfo, &startTime);
                        #else
                            localtime_r(&startTime, &timeinfo);
                        #endif
                        strftime(startTimeStr, sizeof(startTimeStr), "%a %b %d %H:%M:%S %Y", &timeinfo);
                        sf::Text startText("    Start: " + string(startTimeStr), font, 16);
                        startText.setFillColor(sf::Color(180, 180, 180));
                        startText.setPosition(50, yPos);
                        window.draw(startText);
                        yPos += 25;
                        
                        // End time
                        time_t endTime = attempt->getEndTime();
                        if (endTime != 0) {
                            char endTimeStr[100];
                            struct tm endTimeinfo;
                            #ifdef _WIN32
                                localtime_s(&endTimeinfo, &endTime);
                            #else
                                localtime_r(&endTime, &endTimeinfo);
                            #endif
                            strftime(endTimeStr, sizeof(endTimeStr), "%a %b %d %H:%M:%S %Y", &endTimeinfo);
                            sf::Text endText("    End: " + string(endTimeStr), font, 16);
                            endText.setFillColor(sf::Color(180, 180, 180));
                            endText.setPosition(50, yPos);
                            window.draw(endText);
                            yPos += 25;
                        }
                        
                        // Answers provided
                        sf::Text answersText("    Answers provided: " + to_string(attempt->getAnswersCount()), font, 16);
                        answersText.setFillColor(sf::Color(180, 180, 180));
                        answersText.setPosition(50, yPos);
                        window.draw(answersText);
                        yPos += 40;
                    }
                }
                
                yPos += 20; // Extra space between students
            }
            
            if (!searchIdInput.getValue().empty() && searchResults.empty()) {
                sf::Text noResults("No students found.", font, 18);
                noResults.setFillColor(textColor);
                noResults.setPosition(50, 350);
                window.draw(noResults);
            }
            
            backFromSearchButton.update(mousePos);
            window.draw(backFromSearchButton.shape);
            window.draw(backFromSearchButton.text);
        }

        window.draw(title);
        window.display();
    }

    // Managers own quizzes and students; Student owns attempts.
    // No manual deletion here to avoid double-free. Let destructors run.
    return 0;
}
