#include<SFML/graphics.hpp>
using namespace sf;

enum class block {
	I, J, L, Z, S, O, T
};

int main() {
	RenderWindow window(VideoMode({ 600,800 }), "Tetris Game");
	window.setFramerateLimit(60);
	int row = 20;
	int coloumn = 10;
	sf::RectangleShape background(sf::Vector2f(60-1, 40-1));
	background.setFillColor(sf::Color::Black);
	background.setOutlineThickness(1);
	background.setOutlineColor(sf::Color::White);
	block Tetramino = block::I;


	while (window.isOpen()) {
		while (const auto event = window.pollEvent()) {
			if (event->is < Event::Closed >()) {
				window.close();
			}
		}
		std::vector<std::vector<int>> grid(4, std::vector<int>(4, 0));

		if (Tetramino == block::I) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{1, 1, 1, 1}
			};
		}
		else if (Tetramino == block::J) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 1},
				{0, 1, 1, 1}
			};
		}else if (Tetramino == block::L) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 1, 1}
			};
		}else if (Tetramino == block::S) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 1, 1},
				{0, 1, 1, 0}
			};
		}else if (Tetramino == block::Z) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{1, 1, 0, 0},
				{0, 1, 1, 0}
			};
		}else if (Tetramino == block::T) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 1, 0},
				{0, 1, 1, 1}
			};
		}
	
			window.clear(Color::Black);

			for (int i = 0;i < row;i++) {
				for (int j = 0;j < coloumn;j++) {
					background.setPosition(sf::Vector2f(j * 60, i * 40));
					window.draw(background);
				}
			}
			
			window.display();
	}
	return 0;
}