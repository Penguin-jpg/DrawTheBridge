#include "Renderer.hpp"

Renderer::Renderer(Solver& solver) : solver(solver), worldBox(sf::Quads, 4)
{
	initWorldBox();

	// load texture
	texture.loadFromFile("assets/circle.png");
	texture.generateMipmap();
	texture.setSmooth(true);
}

void Renderer::initWorldBox()
{
	const sf::Vector3f worldInfo = solver.getWorld();
	const sf::Vector2f textureSize = (sf::Vector2f)texture.getSize();
	const sf::Color worldBoxColor = { 50, 50, 50 };

	// start drawing counterclockwise from bottom left
	worldBox[0] = sf::Vertex({ 0.0f, 0.0f }, worldBoxColor, { 0.0f, 0.0f });
	worldBox[1] = sf::Vertex({ worldInfo.x, 0.0f }, worldBoxColor, { textureSize.x, 0.0f });
	worldBox[2] = sf::Vertex({ worldInfo.x, worldInfo.y }, worldBoxColor, { textureSize.x ,textureSize.y });
	worldBox[3] = sf::Vertex({ 0.0f, worldInfo.y }, worldBoxColor, { 0.0f, textureSize.y });
}

void Renderer::render(RenderContext& context)
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
	//states.texture = &texture;

	// draw world box
	context.draw(worldBox, states);

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
		context.draw(circle);
	}
}

//void Renderer::render()
//{
//	// draw constraint
//	/*const sf::Vector3f constraintInfo = solver.getConstraint();
//	sf::CircleShape constraint(constraintInfo.z);
//	constraint.setOrigin({ constraintInfo.z,constraintInfo.z });
//	constraint.setFillColor(sf::Color::Black);
//	constraint.setPosition({ constraintInfo.x, constraintInfo.y });
//	constraint.setPointCount(128);
//	target.draw(constraint);*/
//
//	// render state to store transform and texture
//	sf::RenderStates states;
//	//states.texture = &texture;
//
//	// draw world box
//	target.draw(worldBox, states);
//
//	// draw a cirlce to represent an object
//	sf::CircleShape circle(1.0f);
//	circle.setPointCount(32);
//	circle.setOrigin(1.0f, 1.0f);
//	const std::vector<Object>& objects = solver.getObjects();
//	for (const Object& object : objects)
//	{
//		circle.setPosition(object.currentPosition);
//		circle.setScale(object.radius, object.radius);
//		circle.setFillColor(sf::Color::Green);
//		target.draw(circle);
//	}
//}