#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Graphics.hpp>
#include <pthread.h>
#include <mutex>
#include <random>
#include <vector>
#include <iostream>
#include <atomic>
#include <algorithm>
#include <chrono>
#include <thread>

// Constants
const int GRID_SIZE = 15;

// Global variables
extern sf::Font font; 
extern int CELL_SIZE;
extern int GRID_WINDOW_SIZE;
extern int WINDOW_HEIGHT;
extern int numberOfTokens; // Declared as extern for global use

// Function prototypes
void calculateDynamicSizes();
sf::Color getCellColor(int row, int col);
sf::ConvexShape createStar(float x, float y, float size);
bool isStarCell(int row, int col);

#endif

