#ifndef MESSAGES_H
#define MESSAGES_H

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

#include <string>

// Global variable for storing messages
extern std::vector<sf::Text> messages;

// Function prototypes
void displayMessage(const std::string& message);
void drawMessages(sf::RenderWindow& window);

#endif

