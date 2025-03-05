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

#include "grid.h"
#include "utility.h"

sf::RectangleShape grid[GRID_SIZE][GRID_SIZE];
sf::ConvexShape stars[GRID_SIZE][GRID_SIZE];

// Function to initialize the grid
void initializeGrid() {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            grid[row][col] = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            grid[row][col].setPosition(col * CELL_SIZE, row * CELL_SIZE);
            grid[row][col].setFillColor(getCellColor(row, col));
            grid[row][col].setOutlineThickness(1);
            grid[row][col].setOutlineColor(sf::Color::Black);

            // Add stars to specific cells
            if ((row == 8 && col == 2) || (row == 2 && col == 6) || 
                (row == 6 && col == 12) || (row == 12 && col == 8)) {
                float centerX = col * CELL_SIZE + CELL_SIZE / 2.0f;
                float centerY = row * CELL_SIZE + CELL_SIZE / 2.0f;
                stars[row][col] = createStar(centerX, centerY, 15.0f);
            } else {
                stars[row][col].setPointCount(0);
            }
        }
    }
}


void drawGrid(sf::RenderWindow& window) {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            window.draw(grid[row][col]);
            if (stars[row][col].getPointCount() > 0) {
                window.draw(stars[row][col]);
            }
        }
    }
}

