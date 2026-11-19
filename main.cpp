#include<SFML/Graphics.hpp>
#include <random>
#include <vector>
#include<cmath>
#include<string>


enum class Gameevents {
	Starting,Playing,Pause,GameOver
};

enum class block {
	I, J, L, Z, S, O, T
};


std::vector<std::vector<int>> BlockPosition(sf::RectangleShape& rect, block& MinoBlock, int& length,int& id)  {
	id = (int)MinoBlock+1;
	switch (MinoBlock) {
	case::block::I:
		rect.setFillColor(sf::Color::Cyan);
		length = 4;
		return{
		{0, 0, id, 0},
		{0, 0, id, 0},
		{0, 0, id, 0},
		{0, 0, id, 0}
		};
		break;
	case::block::J:
		rect.setFillColor(sf::Color(247, 150, 46));
		length = 3;
		return {
			{0, id, 0},
			{0, id, 0},
			{id, id, 0}
		};
		break;
	case::block::L:
		rect.setFillColor(sf::Color::Blue);
		length = 3;
		return {
			{0, id, 0},
			{0, id, 0},
			{0, id, id}
		};
		break;
	case::block::O:
		rect.setFillColor(sf::Color::Yellow);
		length = 2;
		return {
			{id, id},
			{id, id}
		};
		break;
	case::block::S:
		rect.setFillColor(sf::Color::Green);
		length = 3;
		return {
			{0, id, id},
			{id, id, 0},
			{0, 0, 0}
		};
		break;
	case::block::T:
		rect.setFillColor(sf::Color(135, 35, 134));
		length = 3;
		return {
			{0, id, 0},
			{id, id, 0},
			{0, id, 0}
		};
		break;
	case::block::Z:
		rect.setFillColor(sf::Color::Red);
		length = 3;
		return {
			{id, id, 0},
			{0, id, id},
			{0, 0, 0}
		};
		break;
	}
}

bool HorizontalCollisionCheck(std::vector<std::vector<int>> Currentgrid, std::vector<std::vector<int>> GameGrid, int length, int BlockX, int BlockY, int row, int coloumn,int offSet) {
		for (int i = 0;i < length;i++) {
			for (int j = 0;j < length;j++){
				if (Currentgrid[i][j] != 0) {
					int nextX = BlockX + j + offSet;
					int currentY = BlockY + i;
					if (nextX < 0 || nextX >= coloumn) {
						return true;
					}
					if (currentY >= 0 && currentY < row && GameGrid[currentY][nextX] != 0) {
						return true;
					}
				}
			}
		}
		return false;
}

bool VerticalCollisionCheck(std::vector<std::vector<int>> Currentgrid, std::vector<std::vector<int>> GameGrid, int length, int BlockX, int BlockY, int row, int coloumn,int offset) {
	for (int i = 0;i < length;i++) {
		for (int j = 0;j < length;j++) {
			if (Currentgrid[i][j] != 0) {
				int nextY = BlockY + i + offset;
				int currentX = BlockX + j;
				if (nextY >= row) {
					return true;
				}
				if (nextY >= 0 && currentX >= 0 && currentX < coloumn) {
					if (GameGrid[nextY][currentX] != 0) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Drop(int& blockY, int level, float DeltaTime, float& fallTimer, std::vector<std::vector<int>>& Currentgrid, int length, bool& reachedBottom, int blockX, int column, int row,std::vector<std::vector<int>>& GameGrid,int& id) {
	double fallSpeed = 0.8 - ((level - 1) * 0.007);
	float fallDelay = pow(fallSpeed, (level - 1));
	fallTimer += DeltaTime;
	if (fallTimer >= fallDelay) {
		int offset = 1;
		if (!(VerticalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column,offset))) {
			blockY++;
		}
		else {
			reachedBottom = 1;
		}
		fallTimer = 0.0f;
	}
	
}

void CollisionResult(std::vector<std::vector<int>> Currentgrid, std::vector<std::vector<int>>& GameGrid, int length, int BlockX, int BlockY, int row, int coloumn,bool reachedBottom) {
	if (reachedBottom) {
		for (int i = 0;i < length;i++) {
			for (int j = 0;j < length;j++) {
				if (Currentgrid[i][j] != 0) {
					int lastX = BlockX + j;
					int lastY = BlockY + i;
					if (lastY >= 0 && lastY < row && lastX >= 0 && lastX < coloumn) {
						GameGrid[lastY][lastX] = Currentgrid[i][j];
					}
				}
			}
		}
	}
}

void clearance(std::vector<std::vector<int>>& GameGrid, int row, int column, int& clearline, int& level, int& score) {
	int lines = 0;
	for (int i = row - 1;i >= 0;i--) {
		bool RowComplete = true;
		for (int j = 0;j < column;j++) {
			if (GameGrid[i][j] == 0) {
				RowComplete = false;
				break;
			}
		}
		if (RowComplete == true) {
			clearline++;
			lines++;
			for (int k = i;k > 0;k--) {
				for (int j = 0;j < column;j++) {
					GameGrid[k][j] = GameGrid[k - 1][j];
				}
			}
			for (int j = 0;j < column;j++) {
				GameGrid[0][j] = 0;
			}
			i++;
		}
	}
	if (lines == 1) {
		score += 100 * level;
	}
	else if (lines == 2) {
		score += 300 * level;
	}
	else if (lines == 3) {
		score += 500 * level;
	}
	else if (lines == 4) {
		score += 800 * level;
	}

	level = (clearline / 7) + 1;
}

void GridShapeColour(sf::RectangleShape& rect,int colour) {
	if (colour == 1) {
		rect.setFillColor(sf::Color::Cyan);
	}
	else if (colour == 2) {
		rect.setFillColor(sf::Color(247, 150, 46));
	}
	else if (colour == 3) {
		rect.setFillColor(sf::Color::Blue);
	}
	else if (colour == 4) {
		rect.setFillColor(sf::Color::Red);
	}
	else if (colour == 5) {
		rect.setFillColor(sf::Color::Green);
	}
	else if (colour == 6) {
		rect.setFillColor(sf::Color::Yellow);
	}
	else if (colour == 7) {
		rect.setFillColor(sf::Color(135, 35, 134));
	}
}

void ShadowRotate(std::vector<std::vector<int>>& Currentgrid, std::vector<std::vector<int>> GameGrid, int length, int& BlockX, int BlockY, int row, int coloumn){
	std::vector<std::vector<int>> tempGrid(length, std::vector<int>(length, 0));
	for (int i = 0;i < length;i++) {
		for (int j = 0;j < length;j++) {
			tempGrid[j][length - 1 - i] = Currentgrid[i][j];
		}
	}
	int offset[5] = { 0,1,-1,2,-2 };
	for (int i = 0;i < 5;i++) {
		if (!(HorizontalCollisionCheck(tempGrid, GameGrid, length, BlockX, BlockY, row, coloumn, offset[i]))) {
			BlockX += offset[i];
				if (!(VerticalCollisionCheck(tempGrid, GameGrid, length, BlockX, BlockY, row, coloumn, offset[0]))) {
					Currentgrid = tempGrid;
					break;
				}
				else {
					BlockX -= offset[i];
				}
		}
	}
}

int main() {
	//----------------------------------Screen Dimensions-----------------------------------------------
	sf::RenderWindow window(sf::VideoMode({ 600,800 }), "Tetris Game");
	window.setIcon(sf::Image(std::filesystem::path("Tetris-logo.PNG")));
	window.setFramerateLimit(60);
	int viewRow = 20;
	int bufferRow = 2;
	int row = viewRow + bufferRow;
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
	sf::RectangleShape Gridshape(sf::Vector2f(39, 39));
	sf::RectangleShape Previewshape(sf::Vector2f(29, 29));
	sf::RectangleShape Previewrect(sf::Vector2f(120, 120));
	sf::RectangleShape Scorerect(sf::Vector2f(120, 180));
	sf::Texture img("Tetris-logo.PNG");
	sf::Sprite Image(img);
	sf::Time movementTime;
	sf::RectangleShape start(sf::Vector2f(200, 50));
	sf::RectangleShape pause(sf::Vector2f(window.getSize()));
	sf::RectangleShape GameOver(sf::Vector2f(window.getSize()));
	int lineCleared = 0;
	int level = 1;
	int score = 0;
	float fallTimer = 0.0f;
	sf::Clock Timer;
	int id = 1;
	std::vector<std::vector<int>> Currentgrid = BlockPosition(shape, Current, length, id);
	Gameevents gevent = Gameevents::Starting;
	//-----------------------------------Customization------------------------------------------------------
	background.setFillColor(sf::Color(22, 25, 37));
	background.setOutlineThickness(1);
	background.setOutlineColor(sf::Color::White);
	Previewshape.setOutlineThickness(1);
	Previewshape.setOutlineColor(sf::Color::White);
	Previewrect.setOutlineThickness(1);
	Previewrect.setOutlineColor(sf::Color::White);
	Previewrect.setFillColor(sf::Color::Black);
	Previewrect.setPosition(sf::Vector2f(440, 80));
	Scorerect.setOutlineThickness(1);
	Scorerect.setOutlineColor(sf::Color::White);
	Scorerect.setFillColor(sf::Color::Black);
	Scorerect.setPosition(sf::Vector2f(440, 230));
	Image.setPosition(sf::Vector2f(120, 50));
	Image.setScale(sf::Vector2f(1.25, 1.25));
	start.setFillColor(sf::Color::Cyan);
	start.setPosition(sf::Vector2f(200, 400));
	pause.setFillColor(sf::Color(0, 0, 0, 160));
	GameOver.setFillColor(sf::Color(0, 0, 0, 200));
	sf::Font font;
	if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
		return -1;
	}
	sf::Text startGame(font);
	sf::Text scoreText(font);
	sf::Text pauseText(font, "PAUSED", 48);
	sf::Text gameOver(font, "Game Over", 48);
	sf::Text returnToMenue(font, "Main Menue", 28);
	pauseText.setFillColor(sf::Color::White);
	gameOver.setFillColor(sf::Color::White);
	pauseText.setPosition({ 180,50 });
	gameOver.setPosition({ 180,50 });
	scoreText.setCharacterSize(28);
	startGame.setCharacterSize(28);
	scoreText.setFillColor(sf::Color::White);
	startGame.setFillColor(sf::Color::Black);
	startGame.setOutlineThickness(1);
	startGame.setOutlineColor(sf::Color::White);
	returnToMenue.setFillColor(sf::Color::Black);
	returnToMenue.setOutlineThickness(1);
	returnToMenue.setOutlineColor(sf::Color::White);
	scoreText.setPosition(sf::Vector2f(450, 240));
	startGame.setPosition(sf::Vector2f(225, 412));
	returnToMenue.setPosition(sf::Vector2f(225, 412));
	startGame.setString("Start Game");
	//--------------------------------Game Loop-------------------------------------------------------------
	bool hardDrop = true;
	while (window.isOpen()) {
		bool reachedBottom = false;
		//------------------------------------Event Check & Movement----------------------------------------
		while (const auto event = window.pollEvent()) {
			if (event->is < sf::Event::Closed >()) {
				window.close();
			}

			if (gevent == Gameevents::Starting) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = start.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + start.getSize().x && mousePos.y >= pos.y && mousePos.y <= pos.y + start.getSize().y) {
						blockX = 3;
						blockY = 0;
						gevent = Gameevents::Playing;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
					blockX = 3;
					blockY = 0;
					gevent = Gameevents::Playing;
				}
			}
			if (event->is < sf::Event::KeyPressed >()) {
				auto key = event->getIf < sf::Event::KeyPressed >();
				if (gevent == Gameevents::Playing) {

					if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) {
						int offset = -1;
						if (!(HorizontalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column, offset))) {
							blockX--;
						}
					}

					else if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D) {
						int offset = 1;
						if (!(HorizontalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column, offset))) {
							blockX++;
						}
					}
					else if ((key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S) && hardDrop) {
						int droppedLines = 0;
						while (!VerticalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column, 1)) {
							blockY++;
							droppedLines++;
						}
						score += 2 * droppedLines;
						reachedBottom = true;
						fallTimer = 0.0f;
						hardDrop = false;
					}
					else if (key->code == sf::Keyboard::Key::Space) {
						ShadowRotate(Currentgrid, GameGrid, length, blockX, blockY, row, column);
					}

				}

				if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S) {
					hardDrop = true;
				}
				if (key->code == sf::Keyboard::Key::P) {
					if (gevent == Gameevents::Playing) {
						gevent = Gameevents::Pause;
					}
					else if (gevent == Gameevents::Pause) {
						gevent = Gameevents::Playing;
					}
				}
			}
		}
		std::vector<std::vector<int>> Nextgrid = BlockPosition(Previewshape, Next, Nextlength, id);
		if (gevent == Gameevents::Playing) {
			//--------------------------Constant Drop and Locking Positon------------------------------------------------------------
			float deltaTime = Timer.restart().asSeconds();
			if (reachedBottom == false) {
				Drop(blockY, level, deltaTime, fallTimer, Currentgrid, length, reachedBottom, blockX, column, row, GameGrid, id);
			}
			CollisionResult(Currentgrid, GameGrid, length, blockX, blockY, row, column, reachedBottom);
			clearance(GameGrid, row, column, lineCleared, level, score);
			for (int i = 0; i < bufferRow; i++) {
				for (int j = 0; j < column; j++) {
					if (GameGrid[i][j] != 0) {
						gevent = Gameevents::GameOver;
					}
				}
			}
			if (reachedBottom == true) {
				Current = Next;
				Next = static_cast<block>(rand() % 7);
				blockX = 3;
				blockY = 0;
				id = 1;
				Currentgrid = BlockPosition(shape, Current, length, id);
				reachedBottom = false;
				if (VerticalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column, 0)) {
					gevent = Gameevents::GameOver;
				}
			}
		}
		//----------------------------------------Drawing------------------------------------------------------
		if (gevent == Gameevents::Playing || gevent == Gameevents::Pause || gevent == Gameevents::GameOver) {
			window.clear(sf::Color(15, 17, 26));
				for (int i = bufferRow;i < row;i++) {
					for (int j = 0;j < column;j++) {
						background.setPosition(sf::Vector2f(20 + (j * 40), (i - bufferRow) * 40));
						window.draw(background);

						if (GameGrid[i][j] != 0) {
							int colour = GameGrid[i][j];
							Gridshape.setPosition(sf::Vector2f(20 + (j * 40), (i - bufferRow) * 40));
							GridShapeColour(Gridshape, colour);
							window.draw(Gridshape);
						}
					}
				}
				window.draw(Previewrect);
			if (gevent == Gameevents::Playing) {
				window.draw(Scorerect);
				scoreText.setString(
					"Score:\n" + std::to_string(score));
				window.draw(scoreText);

				for (int i = 0;i < length;i++) {
					for (int j = 0;j < length;j++) {
						if (Currentgrid[i][j] != 0) {
							float posX = 20 + ((j + blockX) * 40);
							float posY = (i + blockY - bufferRow) * 40;
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
						if (Nextgrid[i][j] != 0) {
							float nextX = previewStartX + (j * previewCellSize);
							float nextY = previewStartY + (i * previewCellSize);
							Previewshape.setPosition(sf::Vector2f(nextX, nextY));
							window.draw(Previewshape);
						}
					}

				}
			}
			if (gevent == Gameevents::Pause) {
				window.draw(pause);
				window.draw(pauseText);
			}
			if (gevent == Gameevents::GameOver) {
				window.draw(GameOver);
				for (int i = bufferRow;i < row;i++) {
					for (int j = 0;j < column;j++) {
						if (GameGrid[i][j] != 0) {
							Gridshape.setPosition(sf::Vector2f(20 + (j * 40), (i - bufferRow) * 40));
							Gridshape.setFillColor(sf::Color(53, 56, 57));
							window.draw(Gridshape);
						}
					}
				}
				window.draw(gameOver);
				window.draw(start);
				window.draw(returnToMenue);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = start.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + start.getSize().x && mousePos.y >= pos.y && mousePos.y <= pos.y + start.getSize().y) {
						gevent = Gameevents::Starting;
					}
				}

			}
			window.display();
		}
		else if (gevent == Gameevents::Starting) {
			window.clear(sf::Color(10, 12, 18, 200));
			window.draw(Image);
			window.draw(start);
			window.draw(startGame);
			window.display();
		}
	}
	return 0;
}
