//
// Created by omar on 12.04.2023.
//

#ifndef SWORDSMAN_SCOREBOARD_H
#define SWORDSMAN_SCOREBOARD_H

#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/Audio.hpp>
#include <iostream>

class ScoreBoard {
    std::string marioHead; // the path to the mario head image used to display number of lives left
    std::string score = "00000"; // the score string
    sf::RenderWindow *window;
    sf::Texture texture[3];  // the texture object used to load images into the sprite
    sf::Sprite sprite[3]; // the sprite object used to display images on the window
    sf::Font font; // the font object used to display the score
    sf::Text text;
    int lives = 3; // the number of lives left
public:
    ScoreBoard(sf::RenderWindow *window, const std::string& marioHeadFile, const std::string& fontFile);
    void setScore(int score); // sets the score and draws it on the window
    void setLives(int lives); // sets the number of lives left and draws them on the window
    int getLives(); // returns the number of lives left

};


#endif //SWORDSMAN_SCOREBOARD_H
