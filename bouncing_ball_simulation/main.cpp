#include <iostream>
#include <thread>
#include <chrono>

// gravitational constant
const float g = -9.81f;

// coefficient of restitution
const float e = 0.5f;

// time interval between iterations of the simulation (1 ms)
const float timeInterval = 0.001f;

struct Vector2D {
	float x;
	float y;
};

struct Ball {
	Vector2D position;
	Vector2D velocity;
};

// Update the velocity of the ball (delta v = u + at)
Vector2D update_velocity(Vector2D vel) {
	float newVelY = vel.y + g * timeInterval;
	
	return {vel.x, newVelY};
}

// Update the position of the ball (delta y = ut + 0.5ut^2)
Vector2D update_position(Ball ball) {
	float posX = ball.position.x + ball.position.x * timeInterval;
	float posY = ball.position.y + (ball.velocity.y * timeInterval + 0.5 * g * timeInterval * timeInterval);

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
	Vector2D iPos{ 0, 2 };
	
	// Initial Velocity
	Vector2D iVel{ 10, 2 };

	// Instance of the ball
	Ball ball{ iPos, iVel };
	
	// main simulation loop
	while (1) {
		// Updates the velocity of the ball after 1 ms
		Vector2D vel = update_velocity(ball.velocity);

		// Updates the position of the ball after 1 ms
		Vector2D pos = update_position(ball);

		// Check if the ball went below y-axis and assign a new value for the velocity and position of the ball
		ball = detect_collision({ pos, vel });

		// Waits for 1 ms before the start of a new cycle
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}