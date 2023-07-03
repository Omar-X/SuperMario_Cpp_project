//
// Created by omar on 12.04.2023.
//

#include "Mario.h"

Mario::Mario(sf::RenderWindow *window) : Object(window) {
    std::cout << "Mario created" << std::endl;
    for (int i = 0; i < 7; i++) {  // 7 is the number of mario sprites
        texture[i].loadFromFile("../Media/mario" + std::to_string(i + 1) + ".png");
    }
    texture[7].loadFromFile("../Media/mariohead.png");

    this->read_keyboard = true;
    sprite.setTexture(texture[0]);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2, 0); // set the origin to the center of the sprite make it easy to flip the sprite
    vx = 0;
    vy = 0;
}

void Mario::init(sf::Vector2f pos, int sf) {
    smooth_factor = sf; // how fast the transition between the sprites
    current_position = pos;
    floor_height = window->getSize().y - pos.y;
    setPosition(current_position);
}

void Mario::move() {
    if (state == 0) { if (abs(heading) == 1) vx = 0; }
    else if (state == 1) vx = -INIT_VX; // move left
    else if (state == 2) vx = INIT_VX; // move right
    current_position.x += vx;

    setPosition(current_position);
}

void Mario::jump(bool down) {
    distance_factor = 0.13; // a parameter to control the jump height and speed
    vy = JUMP_VY;
    if(!down) current_position.y += - vy * distance_factor + GRAVITY * distance_factor* distance_factor / 2 ;
    setPosition(current_position);
}

void Mario::fall() {
    distance_factor += 0.13;
    step_down = - vy * distance_factor + GRAVITY * distance_factor* distance_factor / 2 ;

    if(bool(off_floor_pos)) {  // a reference parameter that indicate the closest floor and the distance to it
        // if the distance to the floor is less than the step down, then the step down will be the distance to the floor
//        step_down = -off_floor_pos;
//        std::cout << "step_down: " << step_down << " | diff" << off_floor_pos << std::endl;
        if(step_down > -off_floor_pos && off_floor_pos < -4) step_down = -off_floor_pos;
    }

    current_position.y += step_down;
//    setPosition(current_position);
}

void Mario::sprite_set() {
    switch (this->state) {

        case 0: // idle
            if (heading > 1 ) heading = 1;
            else if (heading < -1) heading = -1;
            sprite.setScale(float(heading), 1);
            sprite.setTexture(texture[abs(heading) - 1]);
            break;

        case 1: // move left
            if (heading >= 1){if (on_floor) heading = heading % 4 + 1;else if (!on_floor && heading != 6)heading = 6;}
            else {if (on_floor)heading = 1;else if (!on_floor)heading = 6;}

            sprite.setScale(1, 1);
            sprite.setTexture(texture[heading - 1]);
            break;

        case 2: // move right
            if (heading <= -1){heading = abs(heading);if (on_floor) heading = heading % 4 + 1;else if (!on_floor && heading != 6)heading = 6;}
            else {if (on_floor)heading = 1;else if (!on_floor)heading = 6;}

            sprite.setScale(-1, 1);
            sprite.setTexture(texture[heading - 1]);
            heading = -heading;
            break;

        case 3:  // jump
            if(on_floor){
                if (heading >= 1) heading = 6;
                else heading = -6;
                sprite.setScale((heading > 0) ? 1 : -1, 1);
                sprite.setTexture(texture[abs(heading) - 1]);
            }
            break;

    }
}

void Mario::killed() {
    // killed motion
    vx = 0;
    vy = float(this->window->getSize().y) / 10.f;
    sprite.setScale(1, 1);
    sprite.setTexture(texture[6]);
    jump(false);

}








