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
#include <cmath>

int CELL_SIZE;
int GRID_WINDOW_SIZE;
int WINDOW_HEIGHT;
int numberOfTokens; // Define the global variable here

#ifndef M_PI
#define M_PI 3.14159265358979323846 // Define PI if not already defined
#endif


// Function to get the screen size and calculate dynamic sizes
void calculateDynamicSizes() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int screenWidth = desktop.width;
    int screenHeight = desktop.height;

    // Calculate sizes based on screen dimensions
    CELL_SIZE = std::min(screenWidth, screenHeight) / (GRID_SIZE + 4); // Adding extra space for UI
    GRID_WINDOW_SIZE = GRID_SIZE * CELL_SIZE;
    WINDOW_HEIGHT = GRID_WINDOW_SIZE + 250;
}


// Function to determine the color of each cell
sf::Color getCellColor(int row, int col) {
    // Player yards
    if (row < 6 && col < 6) {
        // Inner white squares
        if ((row == 2 || row == 3) && (col == 2 || col == 3)) {
            return sf::Color::White;
        }
        return sf::Color::Red;
    }
    if (row < 6 && col > 8) {
        // Inner white squares
        if ((row == 2 || row == 3) && (col == 11 || col == 12)) {
            return sf::Color::White;
        }
        return sf::Color::Green; // Top-right (Green)
    }
    if (row > 8 && col < 6) {
        // Inner white squares
        if ((row == 11 || row == 12) && (col == 2 || col == 3)) {
            return sf::Color::White;
        }
        return sf::Color::Blue; // Bottom-left (Yellow)
    }
    if (row > 8 && col > 8) {
        // Inner white squares
        if ((row == 11 || row == 12) && (col == 11 || col == 12)) {
            return sf::Color::White;
        }
        return sf::Color::Yellow; // Bottom-right (Blue)
    }

    // Make the center squares black
    if ((row == 6 && col == 6) || (row == 6 && col == 8) ||
        (row == 8 && col == 6) || (row == 8 && col == 8) || (row == 7 && col == 7)) {
        return sf::Color::Black;
    }

    // Safe paths
    if (col == 7 && row <= 6 && row > 0) return sf::Color::Green;   // Vertical path
    if (col == 8 && row == 1) return sf::Color::Green;             // Green path start
    if (row == 7 && col <= 6 && col > 0) return sf::Color::Red;    // Horizontal path
    if (col == 1 && row == 6) return sf::Color::Red;               // Red path start
    if (col > 7 && row == 7 && col < 14) return sf::Color::Yellow;   // Horizontal path
    if (row > 7 && col == 7 && row < 14) return sf::Color::Blue; // Vertical path
    if (col == 6 && row == 13) return sf::Color::Blue;           // Yellow path start
    if (col == 13 && row == 8) return sf::Color::Yellow;             // Blue path start

    // Default grid color
    return sf::Color::Transparent; // Leave other areas transparent
}

// Function to create a star shape
sf::ConvexShape createStar(float x, float y, float size) {
    sf::ConvexShape star;
    star.setPointCount(10);
    for (int i = 0; i < 10; ++i) {
        float angle = i * 36 * (M_PI / 180.0f);
        float radius = (i % 2 == 0) ? size : size / 2.0f;
        float px = x + radius * std::cos(angle);
        float py = y + radius * std::sin(angle);
        star.setPoint(i, sf::Vector2f(px, py));
    }
    star.setFillColor(sf::Color::Black);
    return star;
}

bool isStarCell(int row, int col) {
    return (row == 8 && col == 2) || (row == 2 && col == 6) || (row == 6 && col == 12) || (row == 12 && col == 8);
}

