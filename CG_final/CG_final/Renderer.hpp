#pragma once
#include <SFML/Graphics.hpp>
#include "Solver.hpp"
#include "RenderContext.hpp"

class Renderer
{
public:
	Renderer(Solver& solver);

	void initWorldBox();

	void render(RenderContext& context, const sf::Vector2f& mousePosition, int type, bool showGrid);

	void drawParticles(RenderContext& context, sf::RenderStates& states);
	void drawConstraints(RenderContext& context, sf::RenderStates& states);
	void drawGrid(RenderContext& context, sf::RenderStates& states);

	void drawType(RenderContext& context, sf::RenderStates& states, const sf::Vector2f& position, int type);
	void drawThickLine(sf::VertexArray& va, const sf::Vector2f& start, const sf::Vector2f& end, float width, sf::Color color);

private:
	// physics solver
	Solver& solver;
	// vertex array of world box (draw with triangle strip)
	sf::VertexArray worldBox;
	// texture
	sf::Texture texture;
	// current type of object to build
	sf::Texture particleTexture;
	sf::Texture cubeTexture;
};