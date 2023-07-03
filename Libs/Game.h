//
// Created by omar on 12.04.2023.
//

#ifndef SWORDSMAN_GAME_H
#define SWORDSMAN_GAME_H
#include "Object.h"
#include "Mario.h"
#include "Turtle.h"
#include "ScoreBoard.h"
#include<fstream>
#include <string>

#define TOTAL_LAYERS 15  // the number of layers in the background floor + pipes + bricks
#define BRICK_FLOORS 8  // the number of floors for the bricks
#define MAX_BRICKS_PER_FLOOR 35  // the maximum number of bricks per floor
#define WAIT_TO_ESCALATE 15 // the number of seconds to wait before escalating the speed of the turtles and number of turtles
#define MAX_TURTLES 7 // the maximum number of turtles
#define MAX_TURTLE_AT_ONCE 5 // the maximum number of turtles to be displayed at once
#define BRICK_SYMBOL 'B'  // the symbol for the bricks in the maze file

class Game {
    sf::RenderWindow* window;
    Mario* mario; // the mario object
    Turtle* turtle[MAX_TURTLES]; // the turtle objects
    ScoreBoard* scoreBoard; // the score board object
    sf::Texture floor_texture; // the floor texture object used to load the floor image into the sprite
    sf::Texture brick_texture; // the brick texture object used to load the brick images into the sprite
    sf::Texture straight_pipe_texture; // the straight pipe texture object used to load the straight pipe image into the sprite
    sf::Texture curved_pipe_texture; // the curved pipe texture object used to load the curved pipe image into the sprite
    sf::Sprite floor_sprite; // the floor sprite object used to display the floor image on the window
    sf::Sprite brick_sprite[100]; // the brick sprite objects used to display the brick images on the window
    sf::Sprite straight_pipe_sprite[2]; // the straight pipe sprite objects used to display the straight pipe images on the window
    sf::Sprite curved_pipe_sprite[2]; // the curved pipe sprite objects used to display the curved pipe images on the window
    std::ifstream maze_file; // the maze file object used to read the maze file
    float dimensions[2]; // the dimensions of game window
    float floors_height[BRICK_FLOORS]; // the height of each floor of bricks
    int floor_num_segments[BRICK_FLOORS] = {0}; // the number of segments in each floor, a segment is bricks adjacent to each other
    int mario_score = 0; // +100 for each turtle killed
    float floors_segments_pos_width[BRICK_FLOORS][MAX_BRICKS_PER_FLOOR/2][2] = {0};
    int brick_counter = 0;  // the number of bricks in the maze file
    int game_state = 0; // when an arrow key is pressed, the game state changes like mario takes 0 none, 1 left arrow, 2 right arrow, 3 up arrow
    int counter = 0;  // if counter reaches 200, the game state changes to 0
    int smooth_factor = 0;
    void marioKilled();
    void turtleKilled(int turtle_num);
    void gameOver();
    void setTurtle(int turtle_num); // create a new turtle object with some different properties
public:
    int killed_turtles = 0; // the number of turtles killed
    int lives_left = 3; // the number of lives left
    explicit Game(sf::RenderWindow* window);
    void init(int, const std::string& MazeFile, const std::string& marioHeadFile, const std::string& fontFile);
    static std::ifstream read_maze(const std::string&);
    ~Game();
    void setBackgroundStructure(); // set the background structure only once
    void drawBackground(sf::RenderWindow& window); // draw the background structure in each iteration
    bool hitBrick(Object *obj);
    bool onFloor(Object *obj); // check if the object is on the floor ether mario or turtle
    bool checkCollusion(Turtle *t, Mario *m, int& side); // check if the turtle and mario are colliding
    bool run(); // run the game, return true if mario won and killed 5 turtles, false if mario killed three times
};


#endif //SWORDSMAN_GAME_H
