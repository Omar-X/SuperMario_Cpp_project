//
// Created by omar on 12.04.2023.
//

#ifndef SWORDSMAN_MARIO_H
#define SWORDSMAN_MARIO_H
#include "Object.h" // the base class


class Mario : public Object {
public:

    explicit Mario(sf::RenderWindow *window);
    void killed();  // when mario is killed, change the sprite to the dying sprite
    void init(sf::Vector2f, int smooth_factor) override;
    void move() override;  // move mario according to the state
    void jump(bool down) override;  // make mario jump
    void fall() override; // make mario fall down if he is not on the floor
    void sprite_set() override;  // set the sprite according to the state and the heading
};



#endif //SWORDSMAN_MARIO_H
