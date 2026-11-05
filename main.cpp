#include<SFML/Graphics.hpp>
#include <random>

enum class block {
	I, J, L, Z, S, O, T
};


int main() {
	sf::RenderWindow window(sf::VideoMode({ 600,800 }), "Tetris Game");
	window.setFramerateLimit(60);
	int row = 20;
	int coloumn = 10;
	sf::RectangleShape background(sf::Vector2f(60 - 1, 40 - 1));
	background.setFillColor(sf::Color::Black);
	background.setOutlineThickness(1);
	background.setOutlineColor(sf::Color::White);
	block Current = block::I;

	int blockX = 0;
	int blockY = 0;
	int rotate = 0;

	sf::RectangleShape shape(sf::Vector2f(60 - 1, 40 - 1));

	while (window.isOpen()) {
		while (const auto event = window.pollEvent()) {
			if (event->is < sf::Event::Closed >()) {
				window.close();
			}
			if (event->is < sf::Event::KeyPressed >()) {
				auto key = event->getIf < sf::Event::KeyPressed >();
				// left
				if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) {
					if (blockX > -1) {
						blockX--;
					}
				}
				// right 
				if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D) {
					if (blockX < 5) {
						blockX++;
					}
				}
				// down 
				if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S) {
					if (blockY < 18) {
						blockY++;
					}
				}
				// spacebar
				if (key->code == sf::Keyboard::Key::Space) {
					rotate++;
					if (rotate == 4) {
						rotate = 0;
					}
				}
			}
		}
		std::vector<std::vector<int>> grid(4, std::vector<int>(4, 0));

		if (Current == block::I) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{1, 1, 1, 1}
			};
			shape.setFillColor(sf::Color(0, 161, 228));
		}
		else if (Current == block::J) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 1},
				{0, 1, 1, 1}
			};
			shape.setFillColor(sf::Color(247, 150, 46));
		}
		else if (Current == block::L) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 1, 1}
			};
			shape.setFillColor(sf::Color(0, 110, 175));
		}
		else if (Current == block::S) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 1, 1},
				{0, 1, 1, 0}
			};
			shape.setFillColor(sf::Color(116, 181, 64));
		}
		else if (Current == block::Z) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{1, 1, 0, 0},
				{0, 1, 1, 0}
			};
			shape.setFillColor(sf::Color(234, 31, 34));
		}
		else if (Current == block::T) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 1, 0},
				{0, 1, 1, 1}
			};
			shape.setFillColor(sf::Color(135, 35, 134));
		}
		else if (Current == block::O) {
			grid = {
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 1, 0},
				{0, 1, 1, 0}
			};
			shape.setFillColor(sf::Color(255, 198, 11));
		}

		// rotate
		for (int r = 0;r < rotate;r++) {
			std::vector<std::vector<int>>rotate(4, std::vector<int>(4, 0));

			for (int i = 0;i < 4;i++) {
				for (int j = 0;j < 4;j++) {
					rotate[j][3 - i] = grid[i][j];
				}
			}
			grid = rotate;
		}
		block next = block(rand() % 7);

		window.clear(sf::Color::Black);

		for (int i = 0;i < row;i++) {
			for (int j = 0;j < coloumn;j++) {
				background.setPosition(sf::Vector2f(j * 60, i * 40));
				window.draw(background);
			}
		}
		for (int i = 0;i < 4;i++) {
			for (int j = 0;j < 4;j++) {
				if (grid[i][j] == 1) {
					float posX = (1 + j + blockX) * 60;
					float posY = (-2 + i + blockY) * 40;
					shape.setPosition(sf::Vector2f(posX, posY));
					window.draw(shape);
				}
			}
		}

		window.display();
	}
	return 0;
}
