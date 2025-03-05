#ifndef DICE_H
#define DICE_H

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

extern sf::RectangleShape diceShape;
extern std::vector<sf::CircleShape> diceDots;
extern int diceValue;

void initializeDice();
int rollDice();
void updateDiceDots();
void drawDice(sf::RenderWindow& window);

#endif

