#include "Renderer.hpp"

Renderer::Renderer(sf::RenderTarget& target, Solver& solver)
	: target(target), solver(solver), worldBox(sf::TriangleStrip, 4)
{
	initWorldBox();

	// load texture
	texture.loadFromFile("assets/circle.png");
	texture.generateMipmap();
	texture.setSmooth(true);
}

void Renderer::initWorldBox()
{
	sf::Vector3f worldInfo = solver.getWorld();
	// start drawing counterclockwise from bottom left
	worldBox[0].position = { 0.0f, 0.0f };
	worldBox[1].position = { worldInfo.x, 0.0f };
	worldBox[2].position = { worldInfo.x, worldInfo.y };
	worldBox[3].position = { 0.0f, worldInfo.y };

	/*worldBox[0].texCoords = { 0.0f, 0.0f };
	worldBox[1].texCoords = { 1.0f, 0.0f };
	worldBox[2].texCoords = { 1.0f, 1.0f };
	worldBox[3].texCoords = { 0.0f, 1.0f };*/

	sf::Color worldBoxColor = { 50, 50, 50 };
	for (int i = 0; i < 4; i++)
	{
		worldBox[i].color = worldBoxColor;
	}
}

void Renderer::render()
{
	// draw constraint
	/*const sf::Vector3f constraintInfo = solver.getConstraint();
	sf::CircleShape constraint(constraintInfo.z);
	constraint.setOrigin({ constraintInfo.z,constraintInfo.z });
	constraint.setFillColor(sf::Color::Black);
	constraint.setPosition({ constraintInfo.x, constraintInfo.y });
	constraint.setPointCount(128);
	target.draw(constraint);*/

	// render state to store transform and texture
	sf::RenderStates states;
	states.texture = &texture;

	// draw world box
	target.draw(worldBox, states);

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