#include <SFML/Graphics.hpp>
#include <iostream>

#include "WindowManager.h"

// Define the global RenderWindow object
sf::RenderWindow window;


void displayInstructions(sf::RenderWindow &window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font.");
    }

    // Title Text
    sf::Text title("INSTRUCTIONS", font, 30);
    title.setPosition(400, 20);
    title.setFillColor(sf::Color::Black);

    // Instructions Content
    std::string instructionsContent =
        "1) Each player will roll the dice by using the spacebar key.\n"
        "2) If dice rolls 6, the player will be given an extra roll, which will also\n"
        "   be done using the spacebar key.\n"
        "3) A player will select the token to be moved based on its number using\n"
        "   '1', '2', '3', or '4' keys.\n"
        "4) If there is no possible move, the player may press 'n' to move the turn\n"
        "   to the next player.\n"
        "5) Press Esc to return to the main menu.";

    sf::Text instructionsText(instructionsContent, font, 20);
    instructionsText.setPosition(50, 80);
    instructionsText.setFillColor(sf::Color::Black);

    // Instruction to go back
    sf::Text backText("Press Esc to return to the main menu", font, 18);
    backText.setPosition(300, 500);
    backText.setFillColor(sf::Color::Black);

    // Instructions Page Loop
    while (window.isOpen()) {
        window.clear(sf::Color::White);

        // Draw instructions and back instruction
        window.draw(title);
        window.draw(instructionsText);
        window.draw(backText);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                return; // Go back to the main menu
            }
        }
    }
}



int displayMainMenu(sf::RenderWindow &window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font.");
    }

    // Title text
    sf::Text titleText("LUDO", font, 50);
    titleText.setPosition(400, 100);
    titleText.setFillColor(sf::Color::Black);

    sf::Text subtitleText("MAIN MENU", font, 30);
    subtitleText.setPosition(380, 160);
    subtitleText.setFillColor(sf::Color::Black);

    // Menu options
    sf::Text option1("1) Play Game", font, 20);
    option1.setPosition(380, 250);
    option1.setFillColor(sf::Color::Black);

    sf::Text option2("2) Game Rules", font, 20);
    option2.setPosition(380, 290);
    option2.setFillColor(sf::Color::Black);

    sf::Text option3("3) Instructions", font, 20);
    option3.setPosition(380, 330);
    option3.setFillColor(sf::Color::Black);

    sf::Text option4("4) Exit", font, 20);
    option4.setPosition(380, 370);
    option4.setFillColor(sf::Color::Black);

    // Instruction text
    sf::Text instructionText("Press the respective numeric key", font, 18);
    instructionText.setPosition(310, 450);
    instructionText.setFillColor(sf::Color::Black);

    sf::Text exitText("Press Esc to exit", font, 18);
    exitText.setPosition(340, 500);
    exitText.setFillColor(sf::Color::Black);

    // Main menu loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return -1; // Exit game
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) return 1; // Play Game
                if (event.key.code == sf::Keyboard::Num2) return 2; // Leaderboard
                if (event.key.code == sf::Keyboard::Num3) return 3; // Rules
                if (event.key.code == sf::Keyboard::Num4) return 4; // Instructions
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return -1; // Exit game
                }
            }
        }

        // Draw the menu
        window.clear(sf::Color::White);
        window.draw(titleText);
        window.draw(subtitleText);
        window.draw(option1);
        window.draw(option2);
        window.draw(option3);
        window.draw(option4);
        window.draw(instructionText);
        window.draw(exitText);
        window.display();
    }

    return -1; // Fallback, should never reach here
}

void displayRules(sf::RenderWindow &window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font.");
    }

    // Title Text
    sf::Text title("Game Rules", font, 30);
    title.setPosition(400, 20);
    title.setFillColor(sf::Color::Black);

    // Rules Content
    std::string rulesContent =
        "1. Each player rolls the die.\n"
        "2. Players alternate turns randomly, but all players must play each round.\n"
        "3. To enter a token from yard to starting square, roll a 6.\n"
        "   - If no tokens are in play and 6 isn't rolled, turn passes to next player.\n"
        "4. A player must move according to the die roll. No passes are allowed.\n"
        "5. Rolling 6 gives a bonus roll but three consecutive 6s lose the turn.\n"
        "6. Landing on an opponent's square sends their token back to their yard.\n"
        "7. Safe squares are marked with colors and cannot be attacked.\n"
        "8. To enter the home column, a player must knock out at least one opponent.\n"
        "9. Pieces can only enter the home triangle with an exact roll.\n"
        "10. The first player to move all 4 tokens to the home triangle wins.\n";

    sf::Text rulesText(rulesContent, font, 20);
    rulesText.setPosition(50, 80);
    rulesText.setFillColor(sf::Color::Black);

    // Instruction to go back
    sf::Text backText("Press Esc to return to the Main Menu", font, 18);
    backText.setPosition(300, 500);
    backText.setFillColor(sf::Color::Black);

    // Rules Page Loop
    while (window.isOpen()) {
        window.clear(sf::Color::White);

        // Draw rules and back instruction
        window.draw(title);
        window.draw(rulesText);
        window.draw(backText);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                return; // Go back to the main menu
            }
        }
    }
}



// Function to initialize the window
void initializeWindow() {
    window.create(sf::VideoMode(1000, 1000), "Ludo Game");
}

// Function to capture number of tokens through the window
int getNumberOfTokens(sf::RenderWindow &window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font.");
    }

    // Instruction text
    sf::Text instructionText("Enter number of tokens per player (1-4):", font, 20);
    instructionText.setPosition(50, 50);
    instructionText.setFillColor(sf::Color::Black);

    // Input text
    sf::Text inputText("", font, 20);
    inputText.setPosition(50, 100);
    inputText.setFillColor(sf::Color::Black);

    // Error message text
    sf::Text errorText("You must enter a number between 1 and 4!", font, 20);
    errorText.setPosition(50, 150);
    errorText.setFillColor(sf::Color::Red);

    std::string userInput = "";
    bool showError = false;

    while (window.isOpen()) {
        window.clear(sf::Color::White);
        window.draw(instructionText);
        window.draw(inputText);

        if (showError) {
            window.draw(errorText);
        }

        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }

            if (event.type == sf::Event::TextEntered) {
                if (std::isdigit(event.text.unicode)) {
                    userInput += static_cast<char>(event.text.unicode);
                    inputText.setString(userInput);
                    showError = false; // Clear error when typing valid input
                } else if (event.text.unicode == '\b' && !userInput.empty()) {
                    userInput.pop_back();
                    inputText.setString(userInput);
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (!userInput.empty()) {
                    int tokens = std::stoi(userInput);
                    if (tokens >= 1 && tokens <= 4) {
                        return tokens; // Valid input
                    } else {
                        showError = true; // Invalid range
                    }
                } else {
                    showError = true; // Empty input
                }
            }
        }
    }

    return 1; // Default fallback
}

