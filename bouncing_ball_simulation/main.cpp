#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>

// gravitational constant
const float g = -1000.f;

// coefficient of restitution
const float e = 0.5f;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

struct Ball {
	sf::Vector2f position;
	sf::Vector2f velocity;
};

// Update the velocity of the ball (delta v = u + at)
sf::Vector2f update_velocity(sf::Vector2f vel, sf::Time timeElapsed) {
	float newVelY = vel.y + g * timeElapsed.asSeconds();
	
	return {vel.x, newVelY};
}

// Update the position of the ball (delta y = ut + 0.5ut^2)
sf::Vector2f update_position(Ball ball, sf::Time timeElapsed) {
	float posX = ball.position.x + ball.velocity.x * timeElapsed.asSeconds();
	float posY = ball.position.y + (ball.velocity.y * timeElapsed.asSeconds() + 0.5 * g * timeElapsed.asSeconds() * timeElapsed.asSeconds());

	return { posX, posY };
}

// Detects the collision of the ball with the y-axis. If y <= 0, switch the direction of velocity and multiply it by "e". Also return the updated entity of the ball
Ball detect_collision(Ball ball) {
	if (ball.position.y <= 0) {
		ball.position.y = 0;
		ball.velocity.y = -ball.velocity.y * e;		
	}
	return ball;
}

int main() {
	// Initial Position
	sf::Vector2f iPos{ 0, SCREEN_HEIGHT-100 };
	
	// Initial Velocity
	sf::Vector2f iVel{ 200, 0 };

	// Instance of the ball
	Ball ball{ iPos, iVel };

	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "Hello World");
	window.setFramerateLimit(60);

	sf::View view;
	view.setSize({ SCREEN_WIDTH, -SCREEN_HEIGHT});
	view.setCenter({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	window.setView(view);

	sf::CircleShape circle(50);
	circle.setFillColor(sf::Color::Blue);

	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();
		
		while (const std::optional event = window.pollEvent()) {

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		// Updates the velocity of the ball after 1 ms
		sf::Vector2f vel = update_velocity(ball.velocity, elapsed);

		// Updates the position of the ball after 1 ms
		sf::Vector2f pos = update_position(ball, elapsed);

		// Check if the ball went below y-axis and assign a new value for the velocity and position of the ball
		ball = detect_collision({ pos, vel });

		circle.setPosition(ball.position);

		window.clear(sf::Color::White);

		window.draw(circle);

		window.display();

	}

	return 0;
}