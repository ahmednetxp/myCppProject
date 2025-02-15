#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

const float paddleWidth = 25.f;
const float paddleHeight = 100.f;
const float ballRadius = 10.f;
const float paddleSpeed = 400.f;
const float ballSpeed = 300.f;
const int windowWidth = 800;
const int windowHeight = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pong Game");
    window.setFramerateLimit(60);

    sf::RectangleShape leftPaddle(sf::Vector2f(paddleWidth, paddleHeight));
    leftPaddle.setFillColor(sf::Color::Blue);
    leftPaddle.setPosition(50.f, windowHeight / 2 - paddleHeight / 2);

    sf::RectangleShape rightPaddle(sf::Vector2f(paddleWidth, paddleHeight));
    rightPaddle.setFillColor(sf::Color::Red);
    rightPaddle.setPosition(windowWidth - 50.f - paddleWidth, windowHeight / 2 - paddleHeight / 2);

    sf::CircleShape ball(ballRadius);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);

    sf::Vector2f ballVelocity(-ballSpeed, -ballSpeed);

    int leftScore = 0;
    int rightScore = 0;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }
    sf::Text leftScoreText(std::to_string(leftScore), font, 30);
    leftScoreText.setPosition(windowWidth / 4, 20.f);
    sf::Text rightScoreText(std::to_string(rightScore), font, 30);
    rightScoreText.setPosition(3 * windowWidth / 4, 20.f);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && leftPaddle.getPosition().y > 0)
            leftPaddle.move(0.f, -paddleSpeed * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && leftPaddle.getPosition().y + paddleHeight < windowHeight)
            leftPaddle.move(0.f, paddleSpeed * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && rightPaddle.getPosition().y > 0)
            rightPaddle.move(0.f, -paddleSpeed * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && rightPaddle.getPosition().y + paddleHeight < windowHeight)
            rightPaddle.move(0.f, paddleSpeed * dt);

        ball.move(ballVelocity * dt);

        if (ball.getPosition().y <= 0 || ball.getPosition().y + 2 * ballRadius >= windowHeight)
            ballVelocity.y = -ballVelocity.y;

        if (ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()) && ballVelocity.x < 0) {
            ballVelocity.x = -ballVelocity.x;
            ball.setPosition(leftPaddle.getPosition().x + paddleWidth, ball.getPosition().y);
        }
        if (ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds()) && ballVelocity.x > 0) {
            ballVelocity.x = -ballVelocity.x;
            ball.setPosition(rightPaddle.getPosition().x - 2 * ballRadius, ball.getPosition().y);
        }

        if (ball.getPosition().x <= 0) {
            rightScore++;
            rightScoreText.setString(std::to_string(rightScore));
            ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
            ballVelocity = sf::Vector2f(-ballSpeed, -ballSpeed);
        }
        if (ball.getPosition().x + 2 * ballRadius >= windowWidth) {
            leftScore++;
            leftScoreText.setString(std::to_string(leftScore));
            ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
            ballVelocity = sf::Vector2f(ballSpeed, ballSpeed);
        }

        window.clear(sf::Color::Black);
        window.draw(leftPaddle);
        window.draw(rightPaddle);
        window.draw(ball);
        window.draw(leftScoreText);
        window.draw(rightScoreText);
        window.display();
    }

    return 0;
}
