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

#include "player.h"
#include "grid.h"
#include "utility.h"
#include "dice.h"
#include "messages.h"
#include "WindowManager.h"

sem_t playerSemaphores[4]; // Array of semaphores for players

std::vector<Token> playerTokens[4];
std::vector<int> consecutiveSixes(4, 0);
std::vector<bool> playerIsActive(4, true);
std::vector<std::pair<int, int>> playerPaths[4];
std::vector<int> playerHitRates(4, 0);
std::mutex playerTokensMutex[4];
std::pair<int, int> homePosition[4];
std::vector<std::string> playerColors = {"Red", "Green", "Yellow", "Blue"}; // Define player colors
sf::Font font;             // Define the font
sf::Text turnText;         // Define the turn text

bool playerHasWon[4] = {false, false, false, false};
std::vector<int> finishingOrder;// Stores player IDs in the order they finish



std::atomic<bool> diceRolled(false);
std::vector<int> playersCompletedTurn;
std::vector<int> consecutiveNoSixOrHit(4, 0);
std::mutex consecutiveMutex;
std::mutex playerHitRatesMutex;
std::atomic<int> currentPlayer(0);
std::vector<bool> playerHitOpponent(4, false);
std::mutex turnMutex;
std::mutex gridMutex;


std::atomic<int> selectedToken(-1);
std::mutex tokenSelectionMutex;
sf::Text gameMessage; 

void initializeGameMessage() {
    gameMessage.setFont(font); // Use the loaded font
    gameMessage.setCharacterSize(24); // Set font size
    gameMessage.setFillColor(sf::Color::Black); // Set text color
    gameMessage.setPosition(10, GRID_WINDOW_SIZE + 10); // Position below the grid
    gameMessage.setString("Press SPACE to roll the dice."); // Initial message
}


int getPlayerTokenChoice(int playerId) {
    int tokenId = -1;
    std::vector<int> movableTokens;

    // Determine movable tokens
    for (int i = 0; i < playerTokens[playerId].size(); ++i) {
        const Token &token = playerTokens[playerId][i];
        if ((!token.inYard || diceValue == 6) && !token.inHome) {
            movableTokens.push_back(i);
        }
    }

    // If no tokens are movable
    if (movableTokens.empty()) {
        return -1;
    }

    // Wait for the main thread to update the selected token
    while (tokenId == -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Poll every 100ms
        std::lock_guard<std::mutex> lock(tokenSelectionMutex);
        if (selectedToken >= 0 && std::find(movableTokens.begin(), movableTokens.end(), selectedToken) != movableTokens.end()) {
            tokenId = selectedToken;
            selectedToken = -1; // Reset for the next selection
        }
    }

    return tokenId;
}



// Function to set the initial home positions for each player
void initializePlayerPaths() {
    playerPaths[0] = { // Red player path
        {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
        {5, 6}, {4, 6}, {3, 6}, {2, 6},
        {1, 6}, {0, 6}, {0, 7}, {0, 8},
        {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8},
        {6, 9}, {6, 10}, {6, 11}, {6, 12},
        {6, 13}, {6, 14}, 
        {7, 14}, {8, 14},
        {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9},
        {9, 8}, {10, 8}, {11, 8}, {12, 8},
        {13, 8}, {14, 8}, {14, 7}, {14, 6},
        {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
        {8, 5}, {8, 4}, {8, 3}, {8, 2},
        {8, 1}, {8, 0}, {7, 0},
        // Final stretch (home column)
        {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}
    };

    // Green player path
    playerPaths[1] = {
        // Starting position after leaving the yard
        {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8},
        {6, 9}, {6, 10}, {6, 11}, {6, 12},
        {6, 13}, {6, 14}, {7, 14}, {8, 14},
        {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9},
        {9, 8}, {10, 8}, {11, 8}, {12, 8},
        {13, 8}, {14, 8}, {14, 7}, {14, 6},
        {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
        {8, 5}, {8, 4}, {8, 3}, {8, 2},
        {8, 1}, {8, 0}, {7, 0}, {6, 0},
        {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
        {5, 6}, {4, 6}, {3, 6}, {2, 6},
        {1, 6}, {0, 6}, {0, 7},
        // Final stretch (home column)
        {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7}
    };

    // Yellow player path
    playerPaths[3] = {
        // Starting position after leaving the yard
        {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
        {8, 5}, {8, 4}, {8, 3}, {8, 2},
        {8, 1}, {8, 0}, {7, 0}, {6, 0},
        {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
        {5, 6}, {4, 6}, {3, 6}, {2, 6},
        {1, 6}, {0, 6}, {0, 7}, {0, 8},
        {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8},
        {6, 9}, {6, 10}, {6, 11}, {6, 12},
        {6, 13}, {6, 14}, {7, 14}, {8, 14},
        {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9},
        {9, 8}, {10, 8}, {11, 8}, {12, 8}, {13, 8}, {14, 8},
        // Final stretch (home column)
        {14, 7}, {13, 7}, {12, 7}, {11, 7}, {10, 7}, {9, 7}, {8, 7}
    };

    // Blue player path
    playerPaths[2] = {
        // Starting position after leaving the yard
        {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9},
        {9, 8}, {10, 8}, {11, 8}, {12, 8},
        {13, 8}, {14, 8}, {14, 7}, {14, 6},
        {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
        {8, 5}, {8, 4}, {8, 3}, {8, 2},
        {8, 1}, {8, 0}, {7, 0}, {6, 0},
        {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
        {5, 6}, {4, 6}, {3, 6}, {2, 6},
        {1, 6}, {0, 6}, {0, 7}, {0, 8},
        {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8},
        {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13},
        {6, 14}, {7, 14},
        // Turning into home column
        // Home column for blue
        {7, 13}, {7, 12}, {7, 11}, {7, 10}, {7, 9}, {7, 8}

    };
}

void initializeTokens() {
    int positions[4][4][2] = {
        {{2, 2}, {2, 3}, {3, 2}, {3, 3}}, // Red
        {{2, 11}, {2, 12}, {3, 11}, {3, 12}}, // Green
        {{11, 11}, {11, 12}, {12, 11}, {12, 12}},// Blue
        {{11, 2}, {11, 3}, {12, 2}, {12, 3}} // Yellow
    };

    for (int player = 0; player < 4; ++player) {
        for (int i = 0; i < numberOfTokens; ++i) { // Use numberOfTokens
            Token token;
            token.row = positions[player][i][0];
            token.col = positions[player][i][1];
            token.inYard = true;
            token.inHome = false;
            token.pathIndex = -1;
            playerTokens[player].push_back(token);
        }
    }
}

void initializeHomePositions() {
    homePosition[0] = {7, 7}; // Red's home position
    homePosition[1] = {7, 7}; // Green's home position
    homePosition[3] = {7, 7}; // Yellow's home position
    homePosition[2] = {7, 7}; // Blue's home position
}



// Function to initialize the turn text
void initializeTurnText() {
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        exit(EXIT_FAILURE);
    }

    turnText.setFont(font);
    turnText.setCharacterSize(24);
    turnText.setPosition(10, GRID_WINDOW_SIZE + 10);
    updateTurnText();
}

// Function to update the turn text based on the current player
void updateTurnText() {
    turnText.setString("Player " + playerColors[currentPlayer] + "'s Turn");
    turnText.setFillColor(
        (currentPlayer == 0) ? sf::Color::Red :
        (currentPlayer == 1) ? sf::Color::Green :
        (currentPlayer == 2) ? sf::Color::Yellow : sf::Color::Blue
    );
}


void moveToken(int playerId, int tokenId) {
    std::lock_guard<std::mutex> lock(gridMutex);

    // Validate the selected token
    if (tokenId < 0 || tokenId >= playerTokens[playerId].size()) {
        gameMessage.setString("Invalid token choice. Try again.");
        return;
    }

    Token& token = playerTokens[playerId][tokenId];

    // Move token out of the yard if dice roll is 6
    if (token.inYard && diceValue == 6) {
        token.inYard = false;
        token.pathIndex = 0;
        token.row = playerPaths[playerId][0].first;
        token.col = playerPaths[playerId][0].second;
        gameMessage.setString("Token " + std::to_string(tokenId + 1) + " moved out of the yard.");
        return;
    }

    // Move token along the path
    if (!token.inYard && !token.inHome) {
        int newPathIndex = token.pathIndex + diceValue;

        if (newPathIndex == playerPaths[playerId].size()) {
            token.inHome = true;
            token.row = homePosition[playerId].first;
            token.col = homePosition[playerId].second;
            gameMessage.setString("Token " + std::to_string(tokenId + 1) + " reached home!");
            
            // Check if the player has finished the game
            if (playerHasFinished(playerId)) {
                playerHasWon[playerId] = true; // Mark the player as a winner
    if (std::find(finishingOrder.begin(), finishingOrder.end(), playerId) == finishingOrder.end()) {
        finishingOrder.push_back(playerId);
        gameMessage.setString("Player " + std::to_string(playerId + 1) + " has finished!");
    }
            }
        } else if (newPathIndex < playerPaths[playerId].size()) {
            token.pathIndex = newPathIndex;
            token.row = playerPaths[playerId][newPathIndex].first;
            token.col = playerPaths[playerId][newPathIndex].second;
            gameMessage.setString("Token " + std::to_string(tokenId + 1) + " moved to (" + 
                                  std::to_string(token.row) + ", " + std::to_string(token.col) + ").");

            // Check for capturing opponent tokens
            checkForCaptures(playerId, token);
        } else {
            gameMessage.setString("Token " + std::to_string(tokenId + 1) + " cannot move without an exact roll to home.");
        }
    }
}


void checkForCaptures(int playerId, Token& movingToken) {
    for (int otherPlayer = 0; otherPlayer < 4; ++otherPlayer) {
        if (otherPlayer == playerId) continue;

        std::lock_guard<std::mutex> lock(playerTokensMutex[otherPlayer]);
        for (auto& token : playerTokens[otherPlayer]) {
            if (!token.inYard && !token.inHome &&
                token.row == movingToken.row && token.col == movingToken.col) {
                if (!isSafeSquare(token.row, token.col)) {
                    // Capture the opponent's token
                    token.inYard = true;
                    token.pathIndex = -1; // Reset path index
                    resetTokenToYard(otherPlayer, token);

                    // Increment the player's hit rate
                    {
                        std::lock_guard<std::mutex> lock(playerHitRatesMutex);
                        playerHitRates[playerId]++;
                    }

                    // Set playerHitOpponent to true
                    {
                        std::lock_guard<std::mutex> lock(consecutiveMutex);
                        playerHitOpponent[playerId] = true;
                    }

                    std::string message = "Player " + std::to_string(playerId) + " captured a token of Player " + std::to_string(otherPlayer) + "! Hit rate: " + std::to_string(playerHitRates[playerId]);
                    displayMessage(message);
                    std::cout << message << std::endl;

                    return; // Exit after capturing one token
                }
            }
        }
    }
}



bool isSafeSquare(int row, int col) {
    // Define the positions of safe squares
    // For example, the starting squares, home columns, and any specially marked squares
    std::vector<std::pair<int, int>> safeSquares = {
        // Add the coordinates of all safe squares
        {6, 1}, {2, 6}, {8, 2}, {1, 8}, {13, 6}, {8, 13},
        {6,12}, {12, 8}, {13, 6},
        // Add star positions as safe squares
        {8, 2}, {2, 6}, {6, 12}, {12, 8}
        // Add more safe squares as per the Ludo board
    };

    return std::find(safeSquares.begin(), safeSquares.end(), std::make_pair(row, col)) != safeSquares.end();
}

void resetTokenToYard(int playerId, Token& token) {
    // Define the yard positions for each player's tokens
    std::vector<std::pair<int, int>> yardPositions[4] = {
        {{2, 2}, {2, 3}, {3, 2}, {3, 3}}, // Red
        {{2, 11}, {2, 12}, {3, 11}, {3, 12}}, // Green
                {{11, 11}, {11, 12}, {12, 11}, {12, 12}}, // Blue
        {{11, 2}, {11, 3}, {12, 2}, {12, 3}} // Yellow
    };

    // Find the index of the token to reset its position
    int tokenIndex = &token - &playerTokens[playerId][0]; // Pointer arithmetic to get index

    token.row = yardPositions[playerId][tokenIndex].first;
    token.col = yardPositions[playerId][tokenIndex].second;
}


// Function to draw tokens
void drawTokens(sf::RenderWindow& window) {
    for (int player = 0; player < 4; ++player) {
        for (int i = 0; i < playerTokens[player].size(); ++i) {
            const auto& token = playerTokens[player][i];
            sf::CircleShape tokenShape(10.0f);
            tokenShape.setFillColor((player == 0) ? sf::Color::Red :
                                    (player == 1) ? sf::Color::Green :
                                    (player == 2) ? sf::Color::Yellow : sf::Color::Blue);
            tokenShape.setOutlineThickness(2.0f);
            tokenShape.setOutlineColor(sf::Color::Black);
            tokenShape.setPosition(token.col * CELL_SIZE + 15, token.row * CELL_SIZE + 15);
            window.draw(tokenShape);

            // Draw token number
            sf::Text tokenNumber;
            tokenNumber.setFont(font);
            tokenNumber.setString(std::to_string(i + 1));
            tokenNumber.setCharacterSize(12);
            tokenNumber.setFillColor(sf::Color::Black);
            tokenNumber.setPosition(token.col * CELL_SIZE + 21, token.row * CELL_SIZE + 16);
            window.draw(tokenNumber);
        }
    }
}



void* playerThread(void* arg) {
    int playerId = *(int*)arg;

    while (playerIsActive[playerId]) {
        // Wait for the semaphore to allow this player's turn
        sem_wait(&playerSemaphores[playerId]);

        // Player's turn logic
        if (diceRolled) {
            updateTurnText(); // Update turn text
            diceRolled = false; // Reset dice state

            rollDice();
            std::cout << "Player " << playerColors[playerId] << " rolled a " << diceValue << "." << std::endl;

            int tokenId = getPlayerTokenChoice(playerId);
            if (tokenId != -1) {
                moveToken(playerId, tokenId); // Move the selected token
            } else {
                std::cout << "Player " << playerColors[playerId] << " has no valid tokens to move." << std::endl;
            }

            if (diceValue == 6) {
                consecutiveSixes[playerId]++;
                if (consecutiveSixes[playerId] >= 3) {
                    std::cout << "Player " << playerColors[playerId] << " rolled three consecutive sixes. Turn skipped." << std::endl;
                    consecutiveSixes[playerId] = 0;
                    currentPlayer = getNextPlayer();
                } else {
                    std::cout << "Player " << playerColors[playerId] << " rolled a six and gets another turn!" << std::endl;
                    sem_post(&playerSemaphores[playerId]); // Allow the same player to play again
                    continue;
                }
            } else {
                consecutiveSixes[playerId] = 0; // Reset consecutive sixes count
                currentPlayer = getNextPlayer(); // Move to next player
            }
        }

        // Signal the next player's semaphore
        sem_post(&playerSemaphores[currentPlayer]);
    }

    return nullptr;
}


// Semaphore initialization and cleanup in main.cpp
void initializeSemaphores() {
    for (int i = 0; i < 4; ++i) {
        sem_init(&playerSemaphores[i], 0, (i == 0 ? 1 : 0)); // Start with the first player
    }
}

void destroySemaphores() {
    for (int i = 0; i < 4; ++i) {
        sem_destroy(&playerSemaphores[i]);
    }
}

// Master thread function
void* masterThreadFunction(void* arg) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for a short while

        for (int playerId = 0; playerId < 4; ++playerId) {
            if (playerIsActive[playerId]) {
                int count;
                {
                    std::lock_guard<std::mutex> lock(consecutiveMutex);
                    count = consecutiveNoSixOrHit[playerId];
                }
                if (count >= 20) {
                    // Cancel the player's thread
                    playerIsActive[playerId] = false;
                    std::string message = "Player " + playerColors[playerId] + " has been kicked out of the game due to 20 consecutive turns without a 6 or a hit.";
                    displayMessage(message);
                    std::cout << message << std::endl;
                }
            }
        }

        // Check if game is over (all players finished or kicked out)
        int activePlayers = 0;
        for (int i = 0; i < 4; ++i) {
            if (playerIsActive[i]) activePlayers++;
        }
        if (activePlayers == 0) {
            std::string message = "Game Over.";
            displayMessage(message);
            std::cout << message << std::endl;
            break;
        }
    }
    return nullptr;
}


// Grid checker thread function
void* gridCheckerThreadFunction(void* arg) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Sleep for a short while

        // Randomly pick a position on the grid to check
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disRow(0, GRID_SIZE - 1);
        std::uniform_int_distribution<> disCol(0, GRID_SIZE - 1);

        int row = disRow(gen);
        int col = disCol(gen);

        // Check if there are multiple tokens on this position (excluding safe squares)
        if (!isSafeSquare(row, col)) {
            std::vector<std::pair<int, int>> tokensAtPosition; // pair of playerId and tokenIndex

            for (int playerId = 0; playerId < 4; ++playerId) {
                std::lock_guard<std::mutex> lock(playerTokensMutex[playerId]);
                for (int i = 0; i < playerTokens[playerId].size(); ++i) {
                    const auto& token = playerTokens[playerId][i];
                    if (!token.inYard && !token.inHome && token.row == row && token.col == col) {
                        tokensAtPosition.push_back({playerId, i});
                    }
                }
            }

            if (tokensAtPosition.size() > 1) {
                // There are multiple tokens on this position
                // The last token to arrive captures the others
                // For simplicity, let's pick the first token as capturer, and the rest as victims

                int capturerPlayerId = tokensAtPosition[0].first;

                for (size_t idx = 1; idx < tokensAtPosition.size(); ++idx) {
                    int victimPlayerId = tokensAtPosition[idx].first;
                    int victimTokenIndex = tokensAtPosition[idx].second;

                    // Reset the victim token to yard
                    resetTokenToYard(victimPlayerId, playerTokens[victimPlayerId][victimTokenIndex]);
                    playerTokens[victimPlayerId][victimTokenIndex].inYard = true;
                    playerTokens[victimPlayerId][victimTokenIndex].pathIndex = -1; // Reset path index

                    // Increment the capturer's hit rate
                    {
                        std::lock_guard<std::mutex> lock(playerHitRatesMutex);
                        playerHitRates[capturerPlayerId]++;
                    }

                    // Set playerHitOpponent to true
                    {
                        std::lock_guard<std::mutex> lock(consecutiveMutex);
                        playerHitOpponent[capturerPlayerId] = true;
                    }

                    std::string message = "Player " + std::to_string(capturerPlayerId) + " captured a token of Player " + std::to_string(victimPlayerId) + "! Hit rate: " + std::to_string(playerHitRates[capturerPlayerId]);
                    displayMessage(message);
                    std::cout << message << std::endl;
                }
            }
        }
    }
    return nullptr;
}



bool playerHasFinished(int playerId) {
    int tokensInGoal = 0;

    for (const auto& token : playerTokens[playerId]) {
        if (token.inHome) {
            tokensInGoal++;
        }
    }

    return tokensInGoal == playerTokens[playerId].size();
}



int getNextPlayer() {
    std::lock_guard<std::mutex> lock(turnMutex);
    static int currentTurn = 0;

    if (playersCompletedTurn.size() == 4) {
        playersCompletedTurn.clear();
    }

    for (int i = 0; i < 4; ++i) {
        int nextPlayer = (currentTurn + i) % 4;
        if (playerIsActive[nextPlayer] && 
            std::find(playersCompletedTurn.begin(), playersCompletedTurn.end(), nextPlayer) == playersCompletedTurn.end() && 
            !playerHasWon[nextPlayer]) {
            playersCompletedTurn.push_back(nextPlayer);
            currentTurn = (nextPlayer + 1) % 4;
            return nextPlayer;
        }
    }

    return -1;
}


// Define a mapping between color names and sf::Color
sf::Color getColor(const std::string& colorName) {
    static const std::unordered_map<std::string, sf::Color> colorMap = {
        {"Red", sf::Color::Red},
        {"Green", sf::Color::Green},
        {"Yellow", sf::Color(255, 255, 0)}, // Yellow (not predefined in SFML)
        {"Blue", sf::Color::Blue}
    };

    auto it = colorMap.find(colorName);
    if (it != colorMap.end()) {
        return it->second;
    }

    return sf::Color::White; // Default color if the name is not found
}




void displayFinalStandings(sf::RenderWindow& window, const std::vector<std::string>& playerColors) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Failed to load font!\n";
        return;
    }

    sf::Text standingsText[4];

    // Loop through finishing order to display rankings
    for (size_t i = 0; i < finishingOrder.size(); ++i) {
        int playerId = finishingOrder[i];

        // Create text for the player's ranking
        standingsText[i].setFont(font);
        standingsText[i].setString(std::to_string(i + 1) + " - Player " + std::to_string(playerId + 1));
        standingsText[i].setFillColor(getColor(playerColors[playerId])); // Use mapped sf::Color
        standingsText[i].setCharacterSize(30);
        standingsText[i].setPosition(100, 100 + i * 50); // Adjust vertical spacing
    }

    // Check for players who didn't finish
    size_t index = finishingOrder.size();
    for (int playerId = 0; playerId < 4; ++playerId) {
        if (std::find(finishingOrder.begin(), finishingOrder.end(), playerId) == finishingOrder.end()) {
            standingsText[index].setFont(font);
            standingsText[index].setString("Last - Player " + std::to_string(playerId + 1));
            standingsText[index].setFillColor(getColor(playerColors[playerId])); // Use mapped sf::Color
            standingsText[index].setCharacterSize(30);
            standingsText[index].setPosition(100, 100 + index * 50); // Adjust vertical spacing
            index++;
        }
    }

    // Render the standings
    window.clear(sf::Color::White);
    for (size_t i = 0; i < 4; ++i) {
        window.draw(standingsText[i]);
    }
    window.display();
}

