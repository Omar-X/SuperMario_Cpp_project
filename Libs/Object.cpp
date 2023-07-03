//
// Created by omar on 12.04.2023.
//
/*
 * This is the base class for all the objects in the game (Mario, Turtle)
 * It contains the common attributes and methods for all the objects
 */

#include "Object.h"

Object::Object(sf::RenderWindow *window) { // the constructor, takes the window
    std::cout << "Object created" << std::endl;
    this->window = window;
    this->state = 0;
    this->heading = 1;

}

void Object::init(sf::Vector2f pos, int sf) {  // initialize the object, takes the position and the smooth factor
    std::cout << "Object init" << std::endl;
    current_position = pos;
    this->smooth_factor = sf;
    setPosition(current_position);
}

void Object::setPosition(sf::Vector2f pos) { // set the position of the object
    if (pos.x < 0.f + float(sprite.getTextureRect().width)/2.f) {  // if the object is out of the window, left
        pos.x = 0.f + float(sprite.getTextureRect().width) / 2.f;
        vy = 0;
    }
    else if (pos.x > float(window->getSize().x - sprite.getTextureRect().width/2.)) pos.x =
            float(window->getSize().x - sprite.getTextureRect().width/2.);

    if (pos.y < 0.f) pos.y = 0.f ; // if the object is out of the window, top
    else if (pos.y > (float(window->getSize().y) - floor_height) && !is_dying) {
        pos.y = float(window->getSize().y)  - floor_height;
        current_floor = 0;
    }
    current_position = pos;
    sprite.setPosition(pos);
}

sf::Vector2f Object::getPosition() { // get the position of the object since it's private
    return sprite.getPosition();
}

sf::IntRect Object::boundingBox() { // get the bounding box of the object
    return sprite.getTextureRect();
}

void Object::draw(sf::RenderWindow &window) { // draw the object on the window
    window.draw(sprite);
}

 void Object::move() {  // this is a virtual function, it will be overridden in the derived classes
} // move the object

void Object::fall() { // this is a virtual function, it will be overridden in the derived classes
} // make the object fall

void Object::jump(bool down) { // this is a virtual function, it will be overridden in the derived classes
} // make the object jump

void Object::update(int evenState, bool get_on_floor) { // update the object, takes the state and if the object is on the floor or not
    timer = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    this->on_floor = get_on_floor;
    if(this->state == 4) return;
    if (timer.count() % smooth_factor <= 1){
        if(on_floor){
            second_jump = false; //allow the object to jump again
            distance_factor = 0;
            vy = 0;
        if (evenState == 3)  jump(false);
        }
        else if (!on_floor && evenState == 3) {
            jump(false);
            second_jump = true;
        }
        else fall();
        move();
        this->state = (on_floor || evenState) ? evenState : 3; // check if the object is on the floor or not
        sprite_set();
    }


    draw(*this->window);

} // update the object

void Object::sprite_set() {  // this is a virtual function, it will be overridden in the derived classes
}



float* Object::get_dimensions() { // get the dimensions of the object
    auto* dimensions = new float[2];
    dimensions[0] = sprite.getGlobalBounds().width;
    dimensions[1] = sprite.getGlobalBounds().height;
    return dimensions;
}










