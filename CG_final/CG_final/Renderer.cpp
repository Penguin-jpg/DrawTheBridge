#include "Renderer.hpp"

Renderer::Renderer(Solver& solver) : solver(solver), worldBox(sf::Quads, 4), numOfTexture(19), circleTexture(numOfTexture)
{
	initWorldBox();

	// load texture
	texture.loadFromFile("assets/circle.png");
	texture.generateMipmap();
	//texture.setSmooth(true);
	particleTexture.loadFromFile("assets/circle.png");
	particleTexture.generateMipmap();
	cubeTexture.loadFromFile("assets/cube.png");
	cubeTexture.generateMipmap();
	std::string texturePath = "assets/CircleColor/Circ_Deg";

	for (int i = 0; i < numOfTexture; i++) {

		circleTexture[i].loadFromFile(texturePath + std::to_string(i + 1) + ".png");
		circleTexture[i].generateMipmap();
	}



	textureMap[ParticleColor::Deg1] = &circleTexture[0];
	textureMap[ParticleColor::Deg2] = &circleTexture[1];
	textureMap[ParticleColor::Deg3] = &circleTexture[2];
	textureMap[ParticleColor::Deg4] = &circleTexture[3];
	textureMap[ParticleColor::Deg5] = &circleTexture[4];
	textureMap[ParticleColor::Deg6] = &circleTexture[5];
	textureMap[ParticleColor::Deg7] = &circleTexture[6];
	textureMap[ParticleColor::Deg8] = &circleTexture[7];
	textureMap[ParticleColor::Deg9] = &circleTexture[8];
	textureMap[ParticleColor::Deg10] = &circleTexture[9];
	textureMap[ParticleColor::Deg11] = &circleTexture[10];
	textureMap[ParticleColor::Deg12] = &circleTexture[11];
	textureMap[ParticleColor::Deg13] = &circleTexture[12];
	textureMap[ParticleColor::Deg14] = &circleTexture[13];
	textureMap[ParticleColor::Deg15] = &circleTexture[14];
	textureMap[ParticleColor::Deg16] = &circleTexture[15];
	textureMap[ParticleColor::Deg17] = &circleTexture[16];
	textureMap[ParticleColor::Deg18] = &circleTexture[17];
	textureMap[ParticleColor::Deg19] = &circleTexture[18];
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

void Renderer::render(RenderContext& context, const sf::Vector2f& mousePosition, int type, bool showGrid)
{
	// render state to store transform and texture
	sf::RenderStates states;

	// draw world box
	// context.draw(worldBox, states);
	// draw particles
	drawParticles(context, states);
	// draw links
	drawConstraints(context, states);
	// draw collision grid
	if (showGrid)
		drawGrid(context, states);
	// draw object type
	drawType(context, states, mousePosition, type);
}

void Renderer::drawParticles(RenderContext& context, sf::RenderStates& states)
{
	const sf::Vector2i textureSize = (sf::Vector2i)texture.getSize();
	// draw a cirlce to represent an object
	sf::CircleShape circle(1.0f);
	//circle.setPointCount(32);
	circle.setOrigin(1.0f, 1.0f);
	circle.setTextureRect({ 0, 0, textureSize.x, textureSize.y });
	// circle.setTexture(&texture);
	const civ::IndexVector<Particle>& particles = solver.getParticles();
	for (const Particle& particle : particles)
	{
		circle.setPosition(particle.currentPosition);
		circle.setScale(particle.radius, particle.radius);

		auto it = textureMap.find(particle.color);
		if (it != textureMap.end()) {
			circle.setTexture(it->second);
		}
		else { // Handle the case where the texture is not found
			circle.setTexture(textureMap[ParticleColor::Deg5]);
		}
		context.draw(circle, states);
	}
}

void Renderer::drawConstraints(RenderContext& context, sf::RenderStates& states)
{
	// vertex array of links (draw with line)
	const civ::IndexVector<Constraint>& constraints = solver.getConstraints();
	// width of line
	const float width = 2.0f;
	sf::VertexArray linkVertices(sf::Quads);
	for (int i = 0; i < constraints.size(); i++)
	{
		const Constraint& constraint = constraints[i];
		drawThickLine(linkVertices, constraint.p1->currentPosition, constraint.p2->currentPosition, width, sf::Color::Red);
	}
	context.draw(linkVertices, states);
}

void Renderer::drawGrid(RenderContext& context, sf::RenderStates& states)
{
	CollisionGrid& grid = solver.getGrid();
	int cellSize = grid.cellSize;
	// draw a rectangle to represent a cell
	sf::RectangleShape rect({ (float)cellSize, (float)cellSize });
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(0.3f);
	for (int row = 0; row < grid.numRows; row++)
	{
		for (int col = 0; col < grid.numCols; col++)
		{
			rect.setPosition(col * cellSize, row * cellSize);
			// if there are objects inside the cell, turn its outline color to green
			if (grid.getCell(row, col).numObjects > 0)
			{
				rect.setOutlineColor(sf::Color::Green);
			}
			else
			{
				rect.setOutlineColor(sf::Color::Red);
			}
			context.draw(rect, states);
		}
	}
}

void Renderer::drawType(RenderContext& context, sf::RenderStates& states, const sf::Vector2f& position, int type)
{
	float offset = 0.0f;

	if (type == 1)
	{
		states.texture = &cubeTexture;
		offset = 15.0f;
	}
	else
	{
		states.texture = &particleTexture;
		offset = 5.0f;
	}

	sf::VertexArray va(sf::Quads, 4);
	const sf::Vector2f textureSize = (sf::Vector2f)states.texture->getSize();
	va[0] = sf::Vertex({ position.x - offset, position.y - offset }, { 0.0f, 0.0f });
	va[1] = sf::Vertex({ position.x - offset, position.y + offset }, { 0.0f, textureSize.y });
	va[2] = sf::Vertex({ position.x + offset, position.y + offset }, { textureSize.x, textureSize.y });
	va[3] = sf::Vertex({ position.x + offset, position.y - offset }, { textureSize.x, 0.0f });
	context.draw(va, states);
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