#pragma once
#include <SFML/Graphics.hpp>
#include "Solver.hpp"
#include "RenderContext.hpp"

class Renderer
{
public:
	Renderer(Solver& solver);

	void initWorldBox();

	void render(RenderContext& context);

	void drawParticles(RenderContext& context, sf::RenderStates& states);
	void drawLinks(RenderContext& context, sf::RenderStates& states);
	void drawThickLine(sf::VertexArray& va, const sf::Vector2f& start, const sf::Vector2f& end, float width, sf::Color color);

private:
	// target for rendering
	//sf::RenderTarget& target;
	// physics solver
	Solver& solver;
	// vertex array of world box (draw with triangle strip)
	sf::VertexArray worldBox;
	// texture
	sf::Texture texture;
};