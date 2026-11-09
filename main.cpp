#include<SFML/Graphics.hpp>
#include <random>
#include <vector>
#include<cmath>

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

void clearance(std::vector<std::vector<int>>& GameGrid, int row, int column, int& clearline) {
	for (int i = row - 1;i >= 0;i--) {
		bool RowComplete = true;
		for (int j = 0;j < column;j++) {
			if (GameGrid[i][j] == 0) {
				RowComplete = false;
				break;
			}
		}
		if (RowComplete == true) {
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
	sf::RectangleShape Gridshape(sf::Vector2f(39, 39));
	sf::RectangleShape Previewshape(sf::Vector2f(29, 29));
	sf::RectangleShape Previewrect(sf::Vector2f(120, 120));
	sf::Time movementTime;
	int lineCleared = 0;
	int level = 1;
	float fallTimer = 0.0f;
	sf::Clock Timer;
	int id = 1;
	std::vector<std::vector<int>> Currentgrid = BlockPosition(shape, Current, length,id);
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
	while (window.isOpen()) {
		//------------------------------------Event Check & Movement----------------------------------------
		while (const auto event = window.pollEvent()) {
			if (event->is < sf::Event::Closed >()) {
				window.close();
			}
			if (event->is < sf::Event::KeyPressed >()) {
				auto key = event->getIf < sf::Event::KeyPressed >();

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
				// spacebar
				if (key->code == sf::Keyboard::Key::Space) {
					ShadowRotate(Currentgrid, GameGrid, length, blockX, blockY, row, column);
				}
			}
		}
		bool reachedBottom = false;

		std::vector<std::vector<int>> Nextgrid = BlockPosition(Previewshape, Next, Nextlength,id);
		//--------------------------Constant Drop and Locking Positon------------------------------------------------------------
		float deltaTime = Timer.restart().asSeconds();
		Drop(blockY, level, deltaTime, fallTimer, Currentgrid, length, reachedBottom, blockX, column, row, GameGrid,id);
		CollisionResult(Currentgrid, GameGrid, length, blockX, blockY, row, column, reachedBottom);
		clearance(GameGrid, row, column, lineCleared);
		if (reachedBottom == true) {
			Current = Next;
			Next = static_cast<block>(rand() % 7);
			blockX = 3;
			blockY = 0;
			id = 1;
			Currentgrid = BlockPosition(shape, Current, length,id);
			reachedBottom = false;
		}
		//----------------------------------------Drawing------------------------------------------------------
		window.clear(sf::Color::Black);

		for (int i = 0;i < row;i++) {
			for (int j = 0;j < column;j++) {
				background.setPosition(sf::Vector2f(20 + (j * 40), i * 40));
				window.draw(background);

				if (GameGrid[i][j] != 0) {
					int colour = GameGrid[i][j];
					Gridshape.setPosition(sf::Vector2f(20 + (j * 40), i * 40));
					GridShapeColour(Gridshape, colour);
					window.draw(Gridshape);
				}
			}
		}
		window.draw(Previewrect);
		for (int i = 0;i < length;i++) {
			for (int j = 0;j < length;j++) {
				if (Currentgrid[i][j] != 0) {
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
				if (Nextgrid[i][j] != 0) {
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
