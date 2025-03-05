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

#include "messages.h"
#include "utility.h"

std::vector<sf::Text> messages;

void displayMessage(const std::string& message) {
    messages.clear(); // Clear any previous messages
    sf::Text messageText;
    messageText.setFont(font);
    messageText.setString(message);
    messageText.setCharacterSize(20);
    messageText.setFillColor(sf::Color::Black);
    messageText.setPosition(10, GRID_WINDOW_SIZE + 50 + 30 * messages.size());
    messages.push_back(messageText);
}

void drawMessages(sf::RenderWindow& window) {
    float messageY = 10.0f;
    for (const auto& message : messages) {
        sf::Text messageText = message;
        messageText.setPosition(GRID_WINDOW_SIZE - messageText.getLocalBounds().width - 10, messageY);
        window.draw(messageText);
        messageY += messageText.getLocalBounds().height + 5;
    }
}

