#include <SFML/Graphics.hpp>

int main()
{
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Game", sf::Style::Default);
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	// set framerate
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}