#pragma once
#include <SFML/Graphics.hpp>
#include "Solver.hpp"

class Renderer
{
public:
	Renderer(sf::RenderTarget& target) : target(target) {}

	void render(Solver& solver);
private:
	sf::RenderTarget& target;
};