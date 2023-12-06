#include "Renderer.hpp"

Renderer::Renderer(Solver& solver) : solver(solver), worldBox(sf::Quads, 4)
{
	initWorldBox();

	// load texture
	texture.loadFromFile("assets/circle.png");
	texture.generateMipmap();
	//texture.setSmooth(true);
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
	// render state to store transform and texture
	sf::RenderStates states;

	// draw world box
	context.draw(worldBox, states);
	// draw particles
	drawParticles(context, states);
	// draw links
	//drawLinks(context, states);
}

void Renderer::drawParticles(RenderContext& context, sf::RenderStates& states)
{
	const sf::Vector2i textureSize = (sf::Vector2i)texture.getSize();
	// draw a cirlce to represent an object
	sf::CircleShape circle(1.0f);
	//circle.setPointCount(32);
	circle.setOrigin(1.0f, 1.0f);
	circle.setTextureRect({ 0, 0, textureSize.x, textureSize.y });
	circle.setTexture(&texture);
	const civ::IndexVector<Particle>& particles = solver.getParticles();
	for (const Particle& particle : particles)
	{
		circle.setPosition(particle.currentPosition);
		circle.setScale(particle.radius, particle.radius);
		//circle.setFillColor(sf::Color::Green);
		context.draw(circle, states);
	}
}

void Renderer::drawLinks(RenderContext& context, sf::RenderStates& states)
{
	// vertex array of links (draw with line)
	const civ::IndexVector<Link>& links = solver.getLinks();
	// width of line
	const float width = 2.0f;
	sf::VertexArray linkVertices(sf::Quads);
	for (int i = 0; i < links.size(); i++)
	{
		const Link& link = links[i];
		drawThickLine(linkVertices, link.p1->currentPosition, link.p2->currentPosition, width, sf::Color::Red);
	}
	context.draw(linkVertices, states);
}

// because SFML doesn't have line with width, draw a rectangle instead
void Renderer::drawThickLine(sf::VertexArray& va, const sf::Vector2f& start, const sf::Vector2f& end, float width, sf::Color color)
{
	sf::Vector2f direction = end - start;
	sf::Vector2f unit = direction / Math::getLength(direction);

	// extend the point along direction
	va.append(sf::Vertex({ start.x - width * unit.y, start.y + width * unit.x }, color));
	va.append(sf::Vertex({ start.x + width * unit.y, start.y - width * unit.x }, color));
	va.append(sf::Vertex({ end.x + width * unit.y, end.y - width * unit.x }, color));
	va.append(sf::Vertex({ end.x - width * unit.y, end.y + width * unit.x }, color));
}