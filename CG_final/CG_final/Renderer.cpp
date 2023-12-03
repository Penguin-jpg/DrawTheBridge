#include "Renderer.hpp"

void Renderer::render(Solver& solver)
{
	// draw constraint
	const sf::Vector3f constraintInfo = solver.getConstraint();
	sf::CircleShape constraint(constraintInfo.z);
	constraint.setOrigin({ constraintInfo.z,constraintInfo.z });
	constraint.setFillColor(sf::Color::Black);
	constraint.setPosition({ constraintInfo.x, constraintInfo.y });
	constraint.setPointCount(128);
	target.draw(constraint);

	// draw a cirlce to represent an object
	sf::CircleShape circle(1.0f);
	circle.setPointCount(32);
	circle.setOrigin(1.0f, 1.0f);
	const std::vector<Object>& objects = solver.getObjects();
	for (const Object& object : objects)
	{
		circle.setPosition(object.currentPosition);
		circle.setScale(object.radius, object.radius);
		circle.setFillColor(sf::Color::Green);
		target.draw(circle);
	}
}