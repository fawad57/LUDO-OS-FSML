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

#include "dice.h"
#include "utility.h"

int diceValue = 1;

sf::RectangleShape diceShape;
std::vector<sf::CircleShape> diceDots;
std::mutex diceMutex;

// Function to initialize the dice
void initializeDice() {
    diceShape.setSize(sf::Vector2f(100.0f, 100.0f));
    diceShape.setFillColor(sf::Color(135,206,235));
    diceShape.setOutlineThickness(2);
    diceShape.setOutlineColor(sf::Color(70, 130, 180));
    diceShape.setPosition(GRID_WINDOW_SIZE / 2 - 50, GRID_WINDOW_SIZE + 25);
}

int rollDice() {
    std::lock_guard<std::mutex> lock(diceMutex);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 6);
    diceValue = dis(gen);
    updateDiceDots();

    return diceValue;
}

// Function to update dice dots
void updateDiceDots() {
    diceDots.clear();
    std::vector<sf::Vector2f> positions = {{50, 50}, {25, 25}, {75, 75}, {25, 75}, {75, 25}, {25, 50}, {75, 50}};
    std::vector<std::vector<int>> patterns = {
        {0}, {1, 2}, {0, 1, 2}, {1, 2, 3, 4}, {0, 1, 2, 3, 4}, {1, 2, 3, 4, 5, 6}};
    for (int i : patterns[diceValue - 1]) {
        sf::CircleShape dot(7.0f);
        dot.setFillColor(sf::Color::Black);
        dot.setPosition(diceShape.getPosition().x + positions[i].x - dot.getRadius(),
                        diceShape.getPosition().y + positions[i].y - dot.getRadius());
        diceDots.push_back(dot);
    }
}

void drawDice(sf::RenderWindow& window) {
    window.draw(diceShape);
    for (const auto& dot : diceDots) {
        window.draw(dot);
    }
}

