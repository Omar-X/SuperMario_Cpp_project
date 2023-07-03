//
// Created by omar on 12.04.2023.
//


#ifndef SWORDSMAN_TURTLE_H
#define SWORDSMAN_TURTLE_H
#include "Object.h" // the base class

class Turtle : public Object {
public:
    float vx, vy;
    float gravity = 9.8;
    sf::Vector2f start_pos;
    std::time_t wait_time;  // the time to wait when the turtle is killed 3 seconds
    float speed_up = 0;  // the speed to add to the turtle with time
    float turtle_unique_factor; // assigning a random value to each turtle to add it to the speed


    explicit Turtle(sf::RenderWindow *window);
    void init(sf::Vector2f, int smooth_factor) override;
    void move() override; // move the turtle automatically
    void jump(bool down) override; // make the turtle jump only when its killed
    void fall() override; // make the turtle fall down if it is not on the floor
    void sprite_set() override; // set the sprite according to the heading automatically
    void restart();  // restart the turtle to its initial position
    void killed(); // when the turtle is killed, change the sprite to the dying sprite
};
#endif //SWORDSMAN_TURTLE_H
