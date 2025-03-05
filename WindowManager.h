#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <SFML/Graphics.hpp>

// Declare a global RenderWindow object
extern sf::RenderWindow window;

// Function to initialize the window
void initializeWindow();
int getNumberOfTokens(sf::RenderWindow &window);
int displayMainMenu(sf::RenderWindow &window);
void displayRules(sf::RenderWindow &window);
void displayInstructions(sf::RenderWindow &window);

#endif

