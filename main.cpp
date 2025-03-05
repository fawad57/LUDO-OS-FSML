#include <SFML/Graphics.hpp>
#include <iostream>
#include "WindowManager.h"
#include "GameLogic.h"


// Main function
int main() {
    // Initialize the SFML window
    initializeWindow();

    while (true) {
        // Display Main Menu and get user choice
        int choice = displayMainMenu(window);

        if (choice == 1) {
            startGame(); // Play Game
        } else if (choice == 2) {
            displayRules(window); // Show Rules Page
        } else if (choice == 3) {
            displayInstructions(window); // Show Instructions
        } else if (choice == -1 || choice == 4) {
            std::cout << "Exiting game..." << std::endl;
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}
