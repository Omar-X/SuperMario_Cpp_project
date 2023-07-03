//
// Created by omar on 12.04.2023.
//

#include "ScoreBoard.h"


ScoreBoard::ScoreBoard(sf::RenderWindow *window, const std::string& MH, const std::string& fontFile) {  // constructor, takes the window, the path to the mario head image and the path to the font file
    std::cout << "ScoreBoard created" << std::endl;
    this->window = window;
    this->marioHead = MH;
    this->lives = 3;
    for(int i = 0; i < 3; i++) {  // load the 3 sprites of the mario head (for 3 lives)
        if (!texture[i].loadFromFile(this->marioHead)) {
            std::cout << "Error loading ScoreBoard.png" << std::endl;
        }
        sprite[i].setTexture(texture[i]);
//        sprite[i].setTextureRect(sf::IntRect(0, 0, 10, 10));
        sprite[i].setPosition(i * 30, 40);

    }

    if(!font.loadFromFile(fontFile)) { // load the font
        std::cout << "Error loading font" << std::endl;
    }
    text.setFont(font);
    text.setString(this->score);
    text.setCharacterSize(25);
    text.setFillColor(sf::Color::White);
    text.setPosition(0, 10);
}

void ScoreBoard::setScore(int score) {  // the score is displayed on the top right corner of the window
    this->score = std::to_string(score);
    // make sure there's always 5 digits
    while (this->score.length() < 5) {
        this->score = "0" + this->score;
        text.setString(this->score);
    }
    text.setString(this->score);
    (*this->window).draw(text);


}

void ScoreBoard::setLives(int lives) {  // three lives are displayed on the top left corner of the window
    this->lives = lives;
    for (int live = 0; live < this->lives; ++live) {
        (*this->window).draw(sprite[live]);
    }

}

int ScoreBoard::getLives() { // returns the number of lives left
    return this->lives;
}
