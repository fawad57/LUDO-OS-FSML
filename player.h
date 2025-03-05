#ifndef PLAYER_H
#define PLAYER_H

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

#include <utility>
#include <string>
#include "utility.h"

// Global variables
extern sf::Font font;
extern std::vector<std::string> playerColors;

// Token structure
struct Token {
    int pathIndex;
    int row, col;
    bool inYard;
    bool inHome;
};

// External variables for player management
extern std::pair<int, int> homePosition[4]; // Declare home positions
extern std::vector<Token> playerTokens[4];
extern std::vector<int> consecutiveSixes;
extern std::vector<bool> playerIsActive;
extern std::vector<std::pair<int, int>> playerPaths[4];
extern std::vector<int> playerHitRates;
extern std::mutex playerTokensMutex[4];
extern sf::Font font;       // Declare the font
extern sf::Text turnText;   // Declare the turn text

extern sem_t playerSemaphores[4]; 

extern std::atomic<int> currentPlayer;
extern std::atomic<bool> diceRolled;
extern std::vector<int> playersCompletedTurn;
extern std::vector<int> consecutiveNoSixOrHit;
extern std::vector<int> finishingOrder;
extern std::atomic<int> selectedToken;
extern std::mutex tokenSelectionMutex;


void destroySemaphores();
void initializeSemaphores();


// Function prototypes
void initializePlayerPaths();
void initializeTokens();
void initializeTurnText();
void initializeHomePositions();
void updateTurnText();

void moveToken(int playerId, int tokenId);

int getPlayerTokenChoice(int playerId, sf::RenderWindow &window);

//int getPlayerTokenChoice(int playerId);
//void moveToken(int playerId);
void checkForCaptures(int playerId, Token& movingToken);
bool isSafeSquare(int row, int col);
void resetTokenToYard(int playerId, Token& token);
void drawTokens(sf::RenderWindow& window);
void initializeDice();
int rollDice();
void updateDiceDots();
void drawDice(sf::RenderWindow& window);
bool playerHasFinished(int playerId);


// Function prototypes
void* playerThread(void* arg);
void* masterThreadFunction(void* arg);
void* gridCheckerThreadFunction(void* arg);
int getNextPlayer();
void displayFinalStandings(sf::RenderWindow& window, const std::vector<std::string>& playerColors);




#endif

