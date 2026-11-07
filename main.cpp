#include<SFML/Graphics.hpp>
#include <random>
#include <vector>
#include<cmath>

enum class block {
	I, J, L, Z, S, O, T
};


std::vector<std::vector<int>> BlockPosition(sf::RectangleShape& rect, block& MinoBlock, int& length) {
	switch (MinoBlock) {
	case::block::I:
		rect.setFillColor(sf::Color::Cyan);
		length = 4;
		return{
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 0}
		};
		break;
	case::block::J:
		rect.setFillColor(sf::Color(247, 150, 46));
		length = 3;
		return {
			{0, 1, 0},
			{0, 1, 0},
			{1, 1, 0}
		};
		break;
	case::block::L:
		rect.setFillColor(sf::Color::Blue);
		length = 3;
		return {
			{0, 1, 0},
			{0, 1, 0},
			{0, 1, 1}
		};
		break;
	case::block::O:
		rect.setFillColor(sf::Color::Yellow);
		length = 2;
		return {
			{1, 1},
			{1, 1}
		};
		break;
	case::block::S:
		rect.setFillColor(sf::Color::Green);
		length = 3;
		return {
			{0, 1, 1},
			{1, 1, 0},
			{0, 0, 0}
		};
		break;
	case::block::T:
		rect.setFillColor(sf::Color(135, 35, 134));
		length = 3;
		return {
			{0, 1, 0},
			{1, 1, 0},
			{0, 1, 0}
		};
		break;
	case::block::Z:
		rect.setFillColor(sf::Color::Red);
		length = 3;
		return {
			{1, 1, 0},
			{0, 1, 1},
			{0, 0, 0}
		};
		break;
	}
}

void Drop(int& blockY, int level, float DeltaTime, float& fallTimer, std::vector<std::vector<int>>& Currentgird, int length, bool& reachedBottom, int blockX, int column, int row,std::vector<std::vector<int>>& GameGrid) {
	double fallSpeed = 0.8 - ((level - 1) * 0.007);
	float fallDelay = pow(fallSpeed, (level - 1));
	fallTimer += DeltaTime;
	// bottom and block by block collision
	if (fallTimer >= fallDelay) {
		bool collision = false;
		for (int i = 0;i < length;i++) {
			for (int j = 0;j < length;j++) {
				if (Currentgird[i][j] == 1) {
					int nextX = blockX + j;
					int nextY = blockY + i + 1;
					if (nextY >= row||nextX<0||nextX>=column) {
						collision = true;
					}
					else if (nextY>=0 && GameGrid[nextY][nextX] == 1) {
						collision = true;
					}
				}
			}
		}
		if (collision != true) {
			blockY++;
		}
		else {
			reachedBottom = true;
			for (int i = 0;i < length;i++) {
				for (int j = 0;j < length;j++) {
					if (Currentgird[i][j] == 1) {
						int lastX = blockX + j;
						int lastY = blockY + i;
						if (lastY >= 0 && lastY < row && lastX >= 0 && lastX < column) {
							GameGrid[lastY][lastX] = 1;
						}
					}
				}
			}
		}
		fallTimer = 0.0f;
	}
}


int main() {
	//----------------------------------Screen Dimensions-----------------------------------------------
	sf::RenderWindow window(sf::VideoMode({ 600,800 }), "Tetris Game");
	window.setFramerateLimit(60);
	int row = 20;
	int column = 10;
	std::vector<std::vector<int>>GameGrid(row, std::vector<int>(column, 0));
	//-----------------------------------Definition-----------------------------------------------------
	sf::RectangleShape background(sf::Vector2f(39, 39));
	int blockX = 3;
	int blockY = 0;
	int rotate = 0;
	int length;
	int Nextlength;
	block Current = block(rand() % 7);
	block Next = block(rand() % 7);
	sf::RectangleShape shape(sf::Vector2f(39, 39));
	sf::RectangleShape Previewshape(sf::Vector2f(29, 29));
	sf::RectangleShape Previewrect(sf::Vector2f(120, 120));
	sf::Time movementTime;
	int lineCleared = 0;
	int level = 1;
	float fallTimer = 0.0f;
	sf::Clock Timer;

	//-----------------------------------Customization------------------------------------------------------
	background.setFillColor(sf::Color::Black);
	background.setOutlineThickness(1);
	background.setOutlineColor(sf::Color::White);
	Previewshape.setOutlineThickness(1);
	Previewshape.setOutlineColor(sf::Color::White);
	Previewrect.setOutlineThickness(1);
	Previewrect.setOutlineColor(sf::Color::White);
	Previewrect.setFillColor(sf::Color::Black);
	Previewrect.setPosition(sf::Vector2f(440, 80));

	//--------------------------------Game Loop-------------------------------------------------------------
	std::vector<std::vector<int>> Currentgrid = BlockPosition(shape, Current, length);
	while (window.isOpen()) {
		//------------------------------------Event Check & Movement----------------------------------------
		while (const auto event = window.pollEvent()) {
			if (event->is < sf::Event::Closed >()) {
				window.close();
			}
			if (event->is < sf::Event::KeyPressed >()) {
				auto key = event->getIf < sf::Event::KeyPressed >();

				if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) {
					bool collision = false;        //left collision
					for (int i = 0;i < length;i++) {
						for (int j = 0;j < length;j++) {
							if (Currentgrid[i][j] == 1) {
								if (blockX + j - 1 < 0) {
									collision = true;
								}
							}
						}
					}
					if (collision != true) {
						blockX--;
					}
				}

				else if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D) {
					bool collision = false;               //right collision
					for (int i = 0;i < length;i++) {
						for (int j = 0;j < length;j++) {
							if (Currentgrid[i][j] == 1) {
								if (blockX + j + 1 >= column) {
									collision = true;
								}
							}
						}
					}
					if (collision != true) {
						blockX++;
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
		bool reachedBottom = false;

		std::vector<std::vector<int>> Nextgrid = BlockPosition(Previewshape, Next, Nextlength);
		//--------------------------Constant Drop and Locking Positon------------------------------------------------------------
		float deltaTime = Timer.restart().asSeconds();
		Drop(blockY, level, deltaTime, fallTimer, Currentgrid, length, reachedBottom, blockX, column, row, GameGrid);
		if (reachedBottom == true) {
			Current = Next;
			Next = static_cast<block>(rand() % 7);
			blockX = 3;
			blockY = 0;
			Currentgrid = BlockPosition(shape, Current, length);

			reachedBottom = false;
		}
		//-----------------------------Rotation--------------------------------------------------------------
		Currentgrid = BlockPosition(shape, Current, length);
		for (int r = 0;r < rotate;r++) {
			std::vector<std::vector<int>>rotate(4, std::vector<int>(4, 0));

			for (int i = 0;i < length;i++) {
				for (int j = 0;j < length;j++) {
					rotate[j][length - 1 - i] = Currentgrid[i][j];
				}
			}
			Currentgrid = rotate;
		}
		//----------------------------------------Drawing------------------------------------------------------
		window.clear(sf::Color::Black);

		for (int i = 0;i < row;i++) {
			for (int j = 0;j < column;j++) {
				background.setPosition(sf::Vector2f(20 + (j * 40), i * 40));
				window.draw(background);

				if (GameGrid[i][j] == 1) {
					shape.setPosition(sf::Vector2f(20 + (j * 40), i * 40));
					window.draw(shape);
				}
			}
		}
		window.draw(Previewrect);
		for (int i = 0;i < length;i++) {
			for (int j = 0;j < length;j++) {
				if (Currentgrid[i][j] == 1) {
					float posX = 20 + ((j + blockX) * 40);
					float posY = (i + blockY) * 40;
					shape.setPosition(sf::Vector2f(posX, posY));
					window.draw(shape);
				}
			}
		}
		float previewCellSize = 25.0f;
		float previewStartX = 500.0f - ((Nextlength * previewCellSize) / 2.0f);
		float previewStartY = 140.0f - ((Nextlength * previewCellSize) / 2.0f);

		for (int i = 0; i < Nextlength; i++) {
			for (int j = 0; j < Nextlength; j++) {
				if (Nextgrid[i][j] == 1) {
					float nextX = previewStartX + (j * previewCellSize);
					float nextY = previewStartY + (i * previewCellSize);
					Previewshape.setPosition(sf::Vector2f(nextX, nextY));
					window.draw(Previewshape);
				}
			}

		}

		window.display();
	}
	return 0;
}
