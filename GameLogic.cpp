#include "GameLogic.h"
#include <SFML/Graphics.hpp>
#include <pthread.h>
#include <mutex>
#include <random>
#include <vector>
#include <iostream>
#include <atomic>
#include <algorithm>
#include <semaphore.h>
#include <chrono>
#include <thread>

#include "grid.h"
#include "dice.h"
#include "player.h"
#include "messages.h"
#include "utility.h"
#include "WindowManager.h"



void startGame() {
        // Get the number of tokens using graphical input
    numberOfTokens = getNumberOfTokens(window);

    calculateDynamicSizes();

    initializePlayerPaths();
    initializeGrid();
    initializeDice();
    initializeTokens(); // Now initializes tokens based on user input
    initializeTurnText();
    displayMessage("Press SPACE to roll the dice.");
    updateDiceDots();
    initializeHomePositions(); 

    pthread_t playerThreads[4];
    int playerIds[4] = {0, 1, 2, 3};
    
        // Initialize semaphores
    initializeSemaphores();

    for (int i = 0; i < 4; ++i) {
        pthread_create(&playerThreads[i], nullptr, playerThread, &playerIds[i]);
    }
   

    pthread_t masterThread;
    pthread_create(&masterThread, nullptr, masterThreadFunction, nullptr);

    pthread_t gridCheckerThread;
    pthread_create(&gridCheckerThread, nullptr, gridCheckerThreadFunction, nullptr);

    bool clearDiceDots = false;

    // Initialize the first player randomly
    currentPlayer = getNextPlayer();

    while (window.isOpen()) {
        if (currentPlayer == -1) {
            displayFinalStandings(window, playerColors);

            // Wait for the user to close the game
            sf::Text exitText("Game Over! Close the window to exit.", sf::Font(), 20);
            exitText.setPosition(200, 200);
            exitText.setFillColor(sf::Color::Black);

            while (window.isOpen()) {
                window.clear(sf::Color::White);
                window.draw(exitText);
                window.display();

                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
            }
            break;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (clearDiceDots) {
                    diceDots.clear();
                    clearDiceDots = false;
                } else {
                    rollDice();
                    diceRolled = true; // Notify player threads that dice has been rolled
                    clearDiceDots = true;
                    
                }
            }
            
                       // Handle token selection
            if (event.type == sf::Event::KeyPressed) {
                int tokenChoice = -1;
                if (event.key.code == sf::Keyboard::Num1) tokenChoice = 0;
                if (event.key.code == sf::Keyboard::Num2) tokenChoice = 1;
                if (event.key.code == sf::Keyboard::Num3) tokenChoice = 2;
                if (event.key.code == sf::Keyboard::Num4) tokenChoice = 3;

                if (tokenChoice != -1) {
                    std::lock_guard<std::mutex> lock(tokenSelectionMutex);
                    selectedToken = tokenChoice; // Update selected token
                }
            } 
        }

        window.clear(sf::Color::White);

        // Draw grid
        for (int row = 0; row < GRID_SIZE; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                window.draw(grid[row][col]);
                if (stars[row][col].getPointCount() > 0) {
                    window.draw(stars[row][col]);
                }
            }
        }

        drawTokens(window);

        // Draw dice
        window.draw(diceShape);
        for (const auto& dot : diceDots) {
            window.draw(dot);
        }

        // Draw current turn text
        window.draw(turnText);

        // Draw messages
        float messageY = 10.0f;
        for (const auto& message : messages) {
            sf::Text messageText = message;
            messageText.setPosition(GRID_WINDOW_SIZE - messageText.getLocalBounds().width - 10, messageY);
            window.draw(messageText);
            messageY += messageText.getLocalBounds().height + 5;
        }
        window.display();
    }

    // Cancel all threads
    for (int i = 0; i < 4; ++i) {
        pthread_cancel(playerThreads[i]);
    }
    pthread_cancel(masterThread);
    pthread_cancel(gridCheckerThread);

    // Join all threads
    for (int i = 0; i < 4; ++i) {
        pthread_join(playerThreads[i], nullptr);
    }
    pthread_join(masterThread, nullptr);
    pthread_join(gridCheckerThread, nullptr);
    
     // Destroy semaphores before exiting
    destroySemaphores();

}

