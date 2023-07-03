//
// Created by omar on 12.04.2023.
//

#include "Game.h"

#include <utility>

Game::Game(sf::RenderWindow *window) { // the constructor
    this->window = window;
    srand(time(nullptr)); // initialize the random seed to make the random numbers different each time
    this->dimensions[0] = window->getSize().x; // the width of the window
    this->dimensions[1] = window->getSize().y; // the height of the window
    std::cout << "Game created" << std::endl;

}

void Game::init(int smooth, const std::string& maze_file_name, const std::string& marioHeadFile, const std::string& fontFile) {
    this->smooth_factor = smooth;

    this->maze_file = read_maze(maze_file_name); // read the maze from the file
    floor_texture.loadFromFile("../Media/floor.png"); // load the floor texture
    brick_texture.loadFromFile("../Media/brick.png"); // load the brick texture
    straight_pipe_texture.loadFromFile("../Media/pipe.png"); // load the straight pipe texture
    curved_pipe_texture.loadFromFile("../Media/pipeS.png"); // load the curved pipe texture

    floor_sprite.setTexture(floor_texture); // set the floor sprite to the floor texture
    scoreBoard = new ScoreBoard(window, marioHeadFile, fontFile); // create the scoreboard object
    mario = new Mario(window); // create the mario object
    // create the turtle objects
    mario->init(sf::Vector2f(dimensions[0] / 2.f, dimensions[1] - floor_sprite.getGlobalBounds().height - float(mario->boundingBox().height)), smooth_factor);


    this->setBackgroundStructure(); // set the background structure along with the floor heights, segments. only once
    // set the turtles objects
    for (int i = 0; i < MAX_TURTLES; i++) {
        setTurtle(i);
    }

}

Game::~Game() { // the destructor
    // delete all the objects created, mari, turtles, scoreboard
    std::cout << "Game destroyed" << std::endl;
    delete mario;
    for(auto & i : turtle)
        delete i;
    delete scoreBoard;
}

void Game::setTurtle(int turtle_num) {
    static sf::Vector2f pos;
    turtle[turtle_num] = new Turtle(window);
    // pick 1 or -1 randomly, using srand and rand
    int sign = rand() % 2; // 0 or 1 for the state and initial position
    if (sign == 0) sign = -1; // make switching between 1 and -1 easier
    turtle[turtle_num]->state = sign; // set the state of the turtle
    if(sign == 1) pos.x = 0.f + float(turtle[turtle_num]->boundingBox().width) + curved_pipe_sprite[0].getGlobalBounds().width/2.f; // on the left side
    else pos.x = dimensions[0] - float(turtle[turtle_num]->boundingBox().width) - curved_pipe_sprite[0].getGlobalBounds().width/2.f; // on the right side
    pos.y = floors_height[BRICK_FLOORS - 1]  - float(turtle[turtle_num]->boundingBox().height) - float(curved_pipe_sprite[0].getGlobalBounds().height)/2.f;
    turtle[turtle_num]->init(pos, smooth_factor);
    turtle[turtle_num]->floor_height = floor_sprite.getGlobalBounds().height;

}

bool Game::run() {
    static int collision_side = 0; // mario hits the turtle >
    // 2 from the top, 3 from the bottom, 1 from the right (left of the turtle), 0 from the left (right of the turtle)
    bool collision = false; // is there a collision between mario and the turtle
    std::time_t action_time = std::time(nullptr);
    int current_turtles = 1; // the number of turtles at the beginning
    int turtle_num = 0; // make sure that we don't have more than "MAX_TURTLE_AT_ONCE" turtles at once
    float speed_up = 0; // the  add up speed to the turtles
    float max_speed_up = 7; // the maximum speed up
    while (this->window->isOpen()) {
        this->window->clear();

        sf::Event event{};
        while (this->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) { this->window->close(); }

            if (event.type == sf::Event::KeyPressed) { // if a key is pressed
                if (event.key.code == sf::Keyboard::Left) { game_state = 1; }
                else if (event.key.code == sf::Keyboard::Right) { game_state = 2; }
                else if (event.key.code == sf::Keyboard::Up) { game_state = 3; }
                else if (event.key.code == sf::Keyboard::Down) { continue; }
                counter = 0;
            }
        }

        counter++;
        if (counter % 200 == 0) {
            game_state = 0; // when mario stops moving set the first sprite
            counter = 0; // reset the counter
        }

        if((std::time(nullptr) - action_time) > WAIT_TO_ESCALATE * current_turtles && current_turtles < MAX_TURTLE_AT_ONCE ) {
            current_turtles++; // increase the number of turtles
            if(speed_up < max_speed_up) speed_up += 0.5; // increase the speed of the turtles
        }

        for (int i = 0; i < MAX_TURTLES; i++){ // draw the turtles
            if(turtle[i]->is_dying) {  // if the turtle is dying don't update it
                if(std::time(nullptr) - turtle[i]->wait_time < 4){  // wait for 4 seconds before removing the turtle
                    turtle[i]->fall(); // make the turtle fall
                    turtle[i]->setPosition(turtle[i]->getPosition());
                    turtle[i]->draw(*this->window); // draw the turtle
                }
                continue;
            }
            turtle_num++; // increase the number of turtles
            if(turtle_num > current_turtles) break; // if we have more than "MAX_TURTLE_AT_ONCE" turtles at once break (don't draw more turtles

            collision = this->checkCollusion(turtle[i], mario, collision_side); // check for collision
            if (collision && !mario->is_dying) { // if there is a collision
                if(collision_side != 2) { marioKilled(); } // if mario killed a turtle
                else { turtleKilled(i); this->mario_score += 100; scoreBoard->setScore(this->mario_score); //turtle_num--;
                 current_turtles --;} // if the turtle killed mario
            }
            turtle[i]->speed_up = speed_up; // set the speed up
            if(turtle_num <= current_turtles) { turtle[i]->update(turtle[i]->state, onFloor(turtle[i]));}
        }
        turtle_num = 0; // reset the number of turtles

        if(mario->is_dying)mario->update(5, onFloor(mario)); // if mario is dying don't update it
        else mario->update(game_state, onFloor(mario)); // update mario


        this->drawBackground(*this->window); // draw the background. floor, bricks, pipes, score board
        this->window->display();

        // for main.cpp to display the message
        if(lives_left == 0) { return false; }
        else if (killed_turtles == 5) { return true; }
    }
    return false;
}

std::ifstream Game::read_maze(const std::string& maze_file_name) { // read the maze file and return the ifstream object
    std::ifstream maze_file;
    maze_file.open(maze_file_name);
    return maze_file;
}

void Game::setBackgroundStructure() {
    // first floor
    floor_sprite.setPosition(0, dimensions[1] - floor_sprite.getGlobalBounds().height); // separate function

    // 2nd 3rd 4th 5th 6th 7th floors
    static int count_bricks; // count the all bricks in the maze
    count_bricks = 0;
    static float brick_x; // x position of the brick
    static int bricks_per_segment; // number of bricks in each segment
    for(int floor_number = BRICK_FLOORS; floor_number > 0; floor_number--) { // draw floors from top to bottom
        static std::string line;
        std::getline(maze_file, line); // read a line from the maze file
        // 8 layers of bricks out of 15 layers, calculate the height of each floor
        floors_height[floor_number - 1] = dimensions[1] - (dimensions[1]/TOTAL_LAYERS) *
                                                          float(floor_number + (TOTAL_LAYERS - BRICK_FLOORS)/1.8);
        bricks_per_segment = 0; // reset the number of bricks in each segment
        floor_num_segments[floor_number - 1] = 0; // reset the number of segments in each floor

        for (int block = 0; block <= MAX_BRICKS_PER_FLOOR; block++) {  // draw bricks for each floor from left to right
            if (line[block] == BRICK_SYMBOL) { // if the block is a brick "B"
                brick_sprite[count_bricks].setTexture(brick_texture); // set the texture of the brick
                brick_x = dimensions[0] * float(block) / MAX_BRICKS_PER_FLOOR; // calculate the x position of the brick

                brick_sprite[count_bricks].setPosition(brick_x, floors_height[floor_number - 1]); // set the position of the brick

                if (!bricks_per_segment) { // if it is the first brick in the segment
                    // set the position of the segment
                    floors_segments_pos_width[floor_number - 1][floor_num_segments[floor_number - 1]][0] = brick_sprite[count_bricks].getPosition().x;
                    floor_num_segments[floor_number - 1]++; // increase the number of segments in the floor
                }
                bricks_per_segment++; // increase the number of bricks in the segment
                count_bricks++; // increase the number of bricks in the maze

            }
            else {
                if (bricks_per_segment > 0) { // if it is the last brick in the segment
                    // set the width of the segment
                    floors_segments_pos_width[floor_number - 1][floor_num_segments[floor_number - 1] - 1][1] =
                            brick_sprite[0].getGlobalBounds().width * float(bricks_per_segment);
                    bricks_per_segment = 0; // reset the number of bricks in the segment
                }
            }
        }
    }
    // draw straight pipes on the ground floor.
    // 2 pipes on the left and right
    straight_pipe_sprite[0].setTexture(straight_pipe_texture);
    straight_pipe_sprite[1].setTexture(straight_pipe_texture);
    straight_pipe_sprite[0].setOrigin(straight_pipe_sprite[0].getGlobalBounds().width / 2.f, straight_pipe_sprite[0].getGlobalBounds().height );
    straight_pipe_sprite[1].setOrigin(straight_pipe_sprite[1].getGlobalBounds().width / 2.f, straight_pipe_sprite[1].getGlobalBounds().height );
    straight_pipe_sprite[0].setPosition(straight_pipe_sprite[0].getGlobalBounds().width / 2.f, floor_sprite.getPosition().y );
    straight_pipe_sprite[1].setPosition(dimensions[0] - straight_pipe_sprite[1].getGlobalBounds().width/2, floor_sprite.getPosition().y );
    straight_pipe_sprite[1].setScale(-1.f, 1.f); // flip the pipe horizontally

    // draw curved pipes on the top floor
    // 2 pipes on the left and right
    curved_pipe_sprite[0].setTexture(curved_pipe_texture);
    curved_pipe_sprite[1].setTexture(curved_pipe_texture);
    curved_pipe_sprite[0].setOrigin(curved_pipe_sprite[0].getGlobalBounds().width / 2.f, curved_pipe_sprite[0].getGlobalBounds().height );
    curved_pipe_sprite[1].setOrigin(curved_pipe_sprite[1].getGlobalBounds().width / 2.f, curved_pipe_sprite[1].getGlobalBounds().height );
    curved_pipe_sprite[0].setPosition(curved_pipe_sprite[0].getGlobalBounds().width / 2.f, floors_height[BRICK_FLOORS - 1] );
    curved_pipe_sprite[1].setPosition(dimensions[0] - curved_pipe_sprite[1].getGlobalBounds().width/2, floors_height[7] );
    curved_pipe_sprite[1].setScale(-1.f, 1.f);

    brick_counter = count_bricks; // set the number of bricks in the maze
    maze_file.close(); // close the maze file


    // set the score board
    scoreBoard->setLives(this->lives_left);
    scoreBoard->setScore(this->mario_score);
}

void Game::drawBackground(sf::RenderWindow &window) { // draw the background in each iteration

    // first floor
    window.draw(floor_sprite);

    // 2nd 3rd 4th 5th 6th 7th floors
    for(int brick = 0; brick < brick_counter; brick++) {
        window.draw(brick_sprite[brick]);
    }

    // draw straight pipes
    window.draw(straight_pipe_sprite[0]);
    window.draw(straight_pipe_sprite[1]);

    // draw curved pipes
    window.draw(curved_pipe_sprite[0]);
    window.draw(curved_pipe_sprite[1]);

    // draw the score board
    scoreBoard->setLives(this->lives_left);
    scoreBoard->setScore(this->mario_score);

}

//bool Game::hitBrick(Object *obj) {
////    static sf::IntRect obj_bounds;
////    static sf::Vector2f obj_pos;
////    static int current_floor;
////    current_floor = obj->current_floor; // did not use it yet
////    obj_bounds = obj->boundingBox();
////    obj_pos = obj->getPosition();
////    for(int brick = 0; brick < brick_counter; brick++) {
////        if (brick_sprite[brick].getGlobalBounds().intersects(obj_bounds)) {
////            if (obj_pos.x + float(obj_bounds.width) >= brick_sprite[brick].getPosition().x &&
////                obj_pos.x <= brick_sprite[brick].getPosition().x + brick_sprite[brick].getGlobalBounds().width) {
////                if (obj_pos.y + float(obj_bounds.height) >= brick_sprite[brick].getPosition().y &&
////                    obj_pos.y <= brick_sprite[brick].getPosition().y + brick_sprite[brick].getGlobalBounds().height) {
////                    return true;
////                }
////            }
////        }
////    }
//    return false;
//}

bool Game::onFloor(Object *obj) {
    static sf::IntRect obj_bounds;
    static sf::Vector2f obj_pos;
    static int current_floor;
    current_floor = obj->current_floor;  // did not use it yet
    // set the object's position and bounding box
    obj_bounds = obj->boundingBox(); obj_pos = obj->getPosition();

    // check if object is on the ground floor
    if (obj_pos.y + float(obj_bounds.height) >= dimensions[1] - floor_sprite.getGlobalBounds().height) return true;

    // check if object is on a brick floor
    static float relative_pos_x;  // the relative position of the object to the floor
    static float diff; // the height difference between the object and a brick floor
    static int floor_number; // the floor number
    // start from the top floor
    for(floor_number = BRICK_FLOORS - 1; floor_number >= 0; floor_number--) {
        diff = (obj_pos.y + float(obj_bounds.height) - floors_height[floor_number]);
        if (diff >= -40 && diff <= 2 ) { // to see the closest floor
            for(int segment = 0; segment < floor_num_segments[floor_number]; segment++) {
                relative_pos_x = obj_pos.x - floors_segments_pos_width[floor_number][segment][0];
                if (relative_pos_x >= 0 && relative_pos_x <= floors_segments_pos_width[floor_number][segment][1]) {
                    obj->off_floor_pos = diff; // a reference to the object's position off the floor
                    obj->current_floor = floor_number; // didn't use it yet
                    if(std::abs(diff) <= 2) return true; // if the object is on the floor
                }
            }
        }
    }
    return false;

}

bool Game::checkCollusion(Turtle *t, Mario *m, int &side) {
    /* check if turtle and mario collide
     * for int side: 0: left, 1: right, 2: top, 3: bottom
     * */
    static sf::IntRect turtle_bounds;
    static sf::IntRect mario_bounds;
    static sf::Vector2f turtle_pos;
    static sf::Vector2f mario_pos;
    static sf::Vector2f relative_pos;
    // get the turtle and mario bounding boxes and positions
    turtle_bounds = t->boundingBox(); turtle_pos = t->getPosition();
    mario_bounds = m->boundingBox(); mario_pos = m->getPosition();
    relative_pos = turtle_pos - mario_pos;

    if (std::abs(relative_pos.x) <= float(turtle_bounds.width + mario_bounds.width) / 2.f && std::abs(relative_pos.y) <= float(turtle_bounds.height + mario_bounds.height) / 2.f) {

        // check which side of the turtle is hit
         if (relative_pos.y + 10 >= turtle_bounds.height) side = 2;  // top

         else if (relative_pos.y <= -turtle_bounds.height) side = 3; // bottom

         else if (relative_pos.x > 0 ) side = 0; // left

         else if (relative_pos.x < 0 ) side = 1; // right
        return true;

    } return false;
}

void Game::marioKilled() {
    static int current_turtle_number;
    current_turtle_number = 0;
    this->lives_left--;  // decrease the number of lives left
    scoreBoard->setLives(this->lives_left);  // update the score board
    if(lives_left) {  // add return false if not killed
        this->mario->is_dying = true;  // set mario to dying state
        this->mario->killed(); // set the dying sprite
        sf::Vector2f mario_pos = mario->getPosition(); // get mario's position
        mario->vy = 10;

        std::time_t action_time = std::time(nullptr);
        while(std::time(nullptr) - action_time < 3)  // wait for 3 seconds
        {
            if((std::time(nullptr) - action_time)*100 % 4 != 0) continue;
            this->window->clear();

            // dying animation
            if(std::time(nullptr) - action_time < 1) {mario_pos.y -= mario->vy * 0.05f; mario->setPosition(mario_pos);}
            else {mario_pos.y += mario->vy * 0.3f; mario->setPosition(mario_pos);}
            mario->setPosition(mario->getPosition());
            mario->draw(*this->window);

            drawBackground(*this->window);
            this->window->display();


        }
        this->mario->is_dying = false; // get back mario to normal state
        mario_pos.x = dimensions[0] / 2.f;
        mario_pos.y = dimensions[1] - floor_sprite.getGlobalBounds().height - float(mario->boundingBox().height);
        mario->setPosition(mario_pos);

        for(int turtle_num = 0; turtle_num < MAX_TURTLES; turtle_num++) { // restart all turtles that are alive
            if(current_turtle_number >= MAX_TURTLE_AT_ONCE) continue;
            current_turtle_number++;
            this->turtle[turtle_num]->restart();
        }
    }
    else gameOver();
}

void Game::turtleKilled(int turtle_num) {
    this->turtle[turtle_num]->is_dying = true; // set turtle to dying state
    this->turtle[turtle_num]->wait_time = std::time(nullptr); // in the run function, the turtle will wait for 3 seconds before disappearing
    this->turtle[turtle_num]->killed(); // set the dying sprite

    this->mario->jump(false);

    this->killed_turtles++;
}

void Game::gameOver() {
    return;
}