#include<SFML/graphics.hpp>
using namespace sf;
int main() {
	RenderWindow window(VideoMode({ 800,600 }), "Tetris Game");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		while (const auto event = window.pollEvent()) {
			if (event->is < Event::Closed >()) {
				window.close();
			}
		}
			window.clear(Color::Black);
			window.display();
	}
	return 0;
}