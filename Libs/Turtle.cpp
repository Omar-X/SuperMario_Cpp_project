//
// Created by omar on 12.04.2023.
//

#include "Turtle.h"

Turtle::Turtle(sf::RenderWindow *window) : Object(window) { // call the base class constructor
    std::cout << "Turtle created" << std::endl;
    for (int i = 0; i < 5; ++i) { // load the 5 sprites of the turtle
        texture[i].loadFromFile("../Media/turtle" + std::to_string(i + 1) + ".png");
    }
    this->read_keyboard = false;
    sprite.setTexture(texture[0]);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2, 0);
    vx = 0;
    vy = 0;

}

void Turtle::init(sf::Vector2f pos, int sf) { // initialize the turtle
    smooth_factor = sf;
    start_pos = pos;
    current_position = pos;

    turtle_unique_factor = rand() % 300 / 100.0f;

    setPosition(current_position);
    // floor height will be set in the game class
}

void Turtle::move() { // move the turtle (Automatically)
    if (state == 0) { if (abs(heading) == 1) vx = 0; } // speed up the turtle with time "speed_up"
    else if (state == 1) vx = INIT_VX/1.3 + speed_up + turtle_unique_factor;
    else if (state == -1) vx = -INIT_VX/1.3f - speed_up - turtle_unique_factor;
    current_position.x += vx;
    setPosition(current_position);
}

void Turtle::jump(bool down) { // this function is only used when the turtle is kicked by mario
    // No jump for turtle
    distance_factor = 0.13;
    vy = JUMP_VY;
    if(!down) current_position.y += - vy * distance_factor + GRAVITY * distance_factor* distance_factor / 2 ;
    setPosition(current_position);
}

void Turtle::fall() { // fall down, similar to mario
    distance_factor += 0.11;
    step_down = - vy * distance_factor + GRAVITY * distance_factor* distance_factor / 2 ;

    if(bool(off_floor_pos)) {
        if(step_down > -off_floor_pos && off_floor_pos < -4) step_down = -off_floor_pos;
    }

    current_position.y += step_down;
    if (current_position.y > this->window->getSize().y - this->floor_height - this->get_dimensions()[1]) {
        current_position.y = this->window->getSize().y - this->floor_height - this->get_dimensions()[1];
    }
}

void Turtle::sprite_set() {  // set the sprite according to the heading automatically
    if(!on_floor)return;

    // flip turtle if it is at the edge of the screen or move it back to the start position with different heading
    if(this->current_position.x <= this->get_dimensions()[0] / 2.f || this->current_position.x >= this->window->getSize().x - this->get_dimensions()[0] / 2.f) {
        if(this->current_position.y + this->get_dimensions()[1] >= this->window->getSize().y - this->floor_height) {
         restart();
        } else this->state *= -1;

    }


    switch (this->state) {

        case 0: // idle
        if (heading > 1 ) heading = 1;
        else if (heading < -1) heading = -1;
        sprite.setScale(float(heading), 1);
        sprite.setTexture(texture[abs(heading) - 1]);
        break;

        case 1: // move right
        if (heading >= 1) heading = heading % 3 + 1;
        else heading = 1;

        sprite.setScale(1, 1);
        sprite.setTexture(texture[heading - 1]);
        break;

        case -1: // move left
        if (heading <= -1){heading = abs(heading); heading = heading % 3 + 1;}
        else heading = 1;

        sprite.setScale(-1, 1);
        sprite.setTexture(texture[heading - 1]);
        heading = -heading;
        break;
    }
}

void Turtle::restart() { // move the turtle back to the start position
    start_pos.x  = float(this->window->getSize().x) - start_pos.x;
    this->setPosition(start_pos);
}

void Turtle::killed() { // this function is called when the turtle is killed, set the flipped sprite.
    vx = 0;
    vy = float(this->window->getSize().y) / 10.f;
    sprite.setScale(1, 1);
    sprite.setTexture(texture[4]);
    jump(true);
//    for(int i = 0; i < 12000; i++)
//        if(i % 1000 == 0) {
//            if (i <= 1000) sprite.setPosition(current_position.x, current_position.y - vy);
//            else sprite.setPosition(current_position.x, current_position.y + vy );
//            std::cout << "Restart " << 10 - i / 1000 << std::endl;
//        }
}
