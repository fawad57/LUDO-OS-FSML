#ifndef GRID_H
#define GRID_H

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

#include "utility.h"

extern sf::RectangleShape grid[GRID_SIZE][GRID_SIZE];
extern sf::ConvexShape stars[GRID_SIZE][GRID_SIZE];

void initializeGrid();
void drawGrid(sf::RenderWindow& window);

#endif

