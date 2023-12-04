#pragma once
#include <SFML/Graphics.hpp>
#include "Solver.hpp"

class Renderer
{
public:
	Renderer(sf::RenderTarget& target, Solver& solver);

	void initWorldBox();

	void render();
private:
	// target for rendering
	sf::RenderTarget& target;
	// physics solver
	Solver& solver;
	// vertex array of world box (draw with triangle strip)
	sf::VertexArray worldBox;
	// texture
	sf::Texture texture;
};