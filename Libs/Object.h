//
// Created by omar on 12.04.2023.
//
#ifndef SWORDSMAN_OBJECT_H
#define SWORDSMAN_OBJECT_H

#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include <cstdlib>
#define INIT_VX 10 // the initial velocity of the object in the x direction
#define JUMP_VY 14 // the initial velocity of the object in the y direction
#define GRAVITY 9.8

using namespace std::chrono; // for the timer

class Object {
protected:
    sf::RenderWindow* window; // the window object used to display the map
    sf::Texture texture[8];  // the texture object used to load images into the sprite
    sf::Sprite sprite;  // the sprite object used to display images on the window
    sf::Vector2f current_position; // the position of the object
    int heading = 1; // heading is used to change between sprites
    bool read_keyboard = false; // if the object is controlled by the keyboard
    bool second_jump = false; // if the object is allowed to jump again
    int smooth_factor = 0;
    milliseconds timer; // the timer, start count and check smooth factor
public:
    float floor_height = 0; // the height of the floor
    int state = 0; // the state of the object taken from game_state
    bool is_dying = false; // if the object is dying
    microseconds start_time;  // the start time of the object when killed
    bool on_floor = true; // if the object is on the floor
    int current_floor = 0; // the current floor of the object
    float distance_factor = 0.1; // a factor used to add increment or decrement to the y position
    float off_floor_pos = 0; // a feedback from the onFloor func which alert the object with the closest floor in order to not pass it
    float vx, vy; // the velocity of the object in the x and y direction
    float step_down = 0; // the distance taken by the object when falling
    explicit Object(sf::RenderWindow *window);
    virtual void init(sf::Vector2f pos, int smooth_factor); // initialize the object
    void setPosition(sf::Vector2f pos); // set the position of the object
    sf::Vector2f getPosition(); // get the position of the object
    sf::IntRect boundingBox(); // get the bounding box of the object
    float* get_dimensions(); // get the dimensions of the object
    void draw(sf::RenderWindow &window); // draw the object on the window
    void update(int evenState, bool on_floor); // update the object, sprite, position, etc. in an iteration.
    virtual void move();  // move the object
    virtual void fall(); // make the object fall
    virtual void jump(bool down); // make the object jump , i always use down = false
    virtual void sprite_set(); // change the sprite of the object depending on the state and the header

};

#endif //SWORDSMAN_OBJECT_H






