//
// Created by omar on 12.04.2023.
//

#include "Game.h"  // Load the game class
#define WIDTH 1000 // Define the window width
#define HEIGHT 800 // Define the window height
#define SMOOTH_FACTOR 120 // Decrease this value to increase the speed of the game but it may cause some bugs with the floors
// You can change the following maze file to build your own maze but make sure that the maze is 35 in each floor and 8 floors
// or change the constants in the Game.h file
#define MazeFile "../TheMaze.txt" // The file that contains the maze "B" for bricks and " " for empty space
#define StartupImage "../Media/StartUpScreen.jpg"  // The image that will be displayed at the start menu
#define FontFile "../Fonts/Ceria.ttf"  // The font file that will be used in the game
#define MarioHeadImage "../Media/mariohead.png"

bool isNewGameButtonPressed(sf::RenderWindow&);  // Check if the new game button is pressed or exit button is pressed

void displayMessage(sf::RenderWindow&, Game&, const std::string&, bool); // Display a message at the center of the screen


int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SwordsMan", sf::Style::Close | sf::Style::Titlebar);
    bool won; // True if the player won the game, false otherwise
    while (true) {
        bool newGame = isNewGameButtonPressed(window);
        if (newGame) {
            Game* game = new Game(&window);
            game->init(SMOOTH_FACTOR, MazeFile, MarioHeadImage, FontFile);  // Initialize the game
            won = game->run();  // Run the game
            if (won) {
                displayMessage(window,*game, "You Won!", true);
            } else {
                displayMessage(window,*game, "You Lost!", false);
            }
            delete game;
        }
        else {
            window.close();
            break;
        }
    }
    return 0;
}

    bool isNewGameButtonPressed(sf::RenderWindow& window)
{
    // Load the font for the button text
    int counter = 0;  // Used to make the button blink
    sf::Font font;
    if (!font.loadFromFile(FontFile))

    {
        // Error handling if the font fails to load
        std::cout << "Error loading font" << std::endl;
        return false;
    }

    // Create the button shape
    sf::RectangleShape button(sf::Vector2f(WIDTH/3.f, HEIGHT/7.f));
    button.setFillColor(sf::Color::Blue);
    button.setPosition(WIDTH/2.f - WIDTH/6.f, HEIGHT/1.5);

    // Create the button text
    sf::Text buttonText("New Game", font, WIDTH/15);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(WIDTH/2.f - WIDTH/6.f + WIDTH/15.f, HEIGHT/1.5);

    // Create Exit button
    sf::RectangleShape exitButton(sf::Vector2f(WIDTH/5.f, HEIGHT/10.f));
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setPosition(WIDTH/2.f - WIDTH/6.f +  WIDTH/15.f , HEIGHT/1.5 + HEIGHT/7.f + 15);

    // Create the button text
    sf::Text exitButtonText("Exit", font, WIDTH/20);
    exitButtonText.setFillColor(sf::Color::White);
    exitButtonText.setPosition(WIDTH/2.f  - WIDTH/6.f +  WIDTH/7.5f, HEIGHT/1.5f + HEIGHT/7.f + 15);


    // Load the background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(StartupImage))
    {
        // Error handling if the image fails to load
        std::cout << "Error loading image" << std::endl;
        return false;
    }
    // Create the background sprite
    sf::Sprite background(backgroundTexture);

    background.setPosition(0, 50);
    background.setTextureRect(sf::IntRect(0, 0, WIDTH, HEIGHT/2));


    // Main event loop
    while (window.isOpen())
    {
        counter++;
        sf::Event event{};  // Create an event object to store the events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // Close the window if the close button is clicked
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)  // Check if the mouse button is pressed
            {
                // Check if the mouse button is pressed within the button's bounds
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                if (button.getGlobalBounds().contains(mousePos))
                {
                    // Return true if the button is clicked
                    return true;
                }
                else if (exitButton.getGlobalBounds().contains(mousePos))
                {
                    // Return false if the exit button is clicked
                    return false;
                }
            }
        }

        // Clear the window
        window.clear();

        // Draw the buttons and the background
        window.draw(button);
        window.draw(exitButton);
        window.draw(exitButtonText);
        if(counter % 1000 <= 500)window.draw(buttonText);
        window.draw(background);

        // Display everything on the screen
        window.display();
    }
    // Return false if the window is closed without the button being pressed
    return false;
}

void displayMessage(sf::RenderWindow& window, Game& game, const std::string& message, bool shining_colors) {
    int counter = 0; // Used to make the Message blink
    // Load the font for the button text
    sf::Font font;
    if (!font.loadFromFile(FontFile)) {
        // Error handling if the font fails to load
        std::cout << "Error loading font" << std::endl;
        return;
    }
    std::time_t waitTime = std::time(nullptr) + 3; // The time to wait before closing the message window and returning to the main menu

    // Create the message text
    sf::Text MessageText(message, font, WIDTH / 5);
    MessageText.setPosition(WIDTH / 2.f  - WIDTH/3.f, HEIGHT / 2.7f);

    while (waitTime - std::time(nullptr) > 0) {
        counter++;
        window.clear();
        if (shining_colors) { // If the player won the game, make the message text shine
            MessageText.setFillColor(sf::Color::Green);
            MessageText.setOutlineColor(sf::Color::Blue);
            MessageText.setOutlineThickness(5);
        }
        else MessageText.setFillColor(sf::Color::Red); // If the player lost the game, make the message text red
        game.drawBackground(window);
        if(counter % 1000 <= 500)window.draw(MessageText); // Make the message text blink
        window.display();
    }

}