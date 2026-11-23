#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <random>
#include <vector>
#include<fstream>
#include<filesystem>
#include<cmath>
#include<string>


enum class Gameevents {
	Starting, Playing, Pause, GameOver,Credits , Controls 
};
enum class block {
	I, J, L, Z, S, O, T
};
enum class GameModes {
	Marathon,Timer,Endless
};


std::vector<std::vector<int>> BlockPosition(sf::RectangleShape& rect, block& MinoBlock, int& length)  {
	int id = (int)MinoBlock+1;
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
	return { { 0 } };
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

void Drop(int& blockY, int level, float DeltaTime, float& fallTimer, std::vector<std::vector<int>>& Currentgrid, int length, bool& reachedBottom, int blockX, int column, int row,std::vector<std::vector<int>>& GameGrid,block Current,bool& lastButtonSpin,bool softDrop =false) {
	double fallSpeed = 0.8 - ((level - 1) * 0.007);
	double fallDelay = pow(fallSpeed, (level - 1));
	if (softDrop) {
		fallDelay = 0.3f;
	}
	fallTimer += DeltaTime;
	if (fallTimer >= fallDelay) {
		int offset = 1;
		if (!(VerticalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column,offset))) {
			blockY++;
			lastButtonSpin = false;
		}
		else {
			reachedBottom = 1;
		}
		fallTimer = 0.0f;
	}
	
}

bool Tspin(std::vector<std::vector<int>> Currentgrid, std::vector<std::vector<int>>& GameGrid, int BlockX, int BlockY,int length,int row,int coloumn) {
	int blocked = 0;
	for (int i = 0;i < length;i++) {
		for (int j = 0;j < length;j++) {
			if (Currentgrid[i][j] == 0&&i+j!=1&&i+j!=3) {
				int lastX = BlockX + j;
				int lastY = BlockY + i;
				if(lastX < 0 || lastX >= coloumn||lastY>=row) {
					blocked++;
				}
				else if (lastY >= 0 && GameGrid[lastY][lastX] != 0) {
					blocked++;
				}
				if (blocked == 3) {
					return true;
				}
			}
		}
	}
	return false;
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

void clearance(std::vector<std::vector<int>>& GameGrid, int row, int column, int& clearline, int& level, int& score,bool& isTspin) {
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
	if (isTspin) {
		if (lines == 0) score += 400 * level;
		else if (lines == 1) score += 800 * level;
		else if (lines == 2) score += 1200 * level;
		else if (lines == 3) score += 1600 * level;
	}
	else {
		if (lines == 1) score += 100 * level;
		else if (lines == 2) score += 300 * level;
		else if (lines == 3) score += 500 * level;
		else if (lines == 4) score += 800 * level;
	}

	level = (clearline / 7) + 1;
	isTspin = false;
}
std::vector<int> FindCompletedRow(std::vector<std::vector<int>>& GameGrid, int row, int column) {
	std::vector<int> completedRows;
	for (int i = row - 1;i >= 0;i--) {
		bool RowCompleted = true;
		for (int j = 0;j < column;j++) {
			if (GameGrid[i][j] == 0) {
				RowCompleted = false;
				break;
			}
		}
		if (RowCompleted == true) {
			completedRows.push_back(i);
		}
	}
	return completedRows;
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

void ShadowRotate(std::vector<std::vector<int>>& Currentgrid, std::vector<std::vector<int>> GameGrid, int length, int& BlockX, int& BlockY, int row, int coloumn,block Current,bool& lastButtonSpin,bool clockWise){
	std::vector<std::vector<int>> tempGrid(length, std::vector<int>(length, 0));
	if (clockWise) {
		for (int i = 0;i < length;i++) {
			for (int j = 0;j < length;j++) {
				tempGrid[j][length - 1 - i] = Currentgrid[i][j];
			}
		}
	}
	else {
		for (int i = 0;i < length;i++) {
			for (int j = 0;j < length;j++) {
				tempGrid[length - 1 - j][i] = Currentgrid[i][j];
			}
		}
	}
	if (Current == block::T) {
		int offset[7][2] = { { 0, 0 }, { 1, 0 }, { -1, 0 }, { 0, 1 }, { 1, 1 }, { -1, 1 }, { 0, 2 } };
		lastButtonSpin = true;
		for (int i = 0; i < 7; i++) {
			int dx =offset[i][0];
			int dy = offset[i][1];
			if (!(HorizontalCollisionCheck(tempGrid, GameGrid, length, BlockX, BlockY+dy, row, coloumn, dx))) {
					if (!(VerticalCollisionCheck(tempGrid, GameGrid, length, BlockX+dx, BlockY, row, coloumn, dy))) {
						BlockY += dy;
						BlockX += dx;
						Currentgrid = tempGrid;
						break;
					}
			}
		}
	}
	else {
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
}

void reset(int& blockX, int& blockY, std::vector<std::vector<int>>& GameGrid, int row, int coloumn, int& level, int& score, bool& canHold, float&fallTimer,bool& hasHold,int& lineCleared,float& modeTimer,bool& isVictory,bool& lastButtonSpin,GameModes Mode,bool& isNewHighScore,sf::RectangleShape& Gridshape) {
	GameGrid.assign(row, std::vector<int>(coloumn, 0));
	blockX = 3;
	blockY = 1;
	level = 1;
	canHold = true;
	fallTimer = 0.0f;
	score = 0;
	hasHold = false;
	lineCleared = 0;
	isVictory = false;
	lastButtonSpin = false;
	Gridshape.setOutlineThickness(0.0f);
	isNewHighScore = false;
	if (Mode == GameModes::Timer) {
		modeTimer = 300.0f;
	}
	else {
		modeTimer = 0.0f;
	}
}

std::string getSaveFileName(GameModes mode) {
	if (mode == GameModes::Marathon) return "save_marathon.txt";
	if (mode == GameModes::Timer) return "save_timer.txt";
	return "save_endless.txt";
}

void saveGameState(const std::vector<std::vector<int>>& GameGrid, int row, int col, GameModes mode, int score, int lineCleared, int level, float modeTimer, block Current, block Next, block Hold, bool hasHold, bool canHold, int blockX, int blockY) {
	std::ofstream out(getSaveFileName(mode));
	if (!out.is_open()) return;
	out << static_cast<int>(mode) << " " << score << " " << lineCleared << " " << level << " " << modeTimer << "\n";
	out << static_cast<int>(Current) << " " << static_cast<int>(Next) << " " << static_cast<int>(Hold) << " "
		<< hasHold << " " << canHold << " " << blockX << " " << blockY << "\n";
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			out << GameGrid[i][j] << " ";
		}
		out << "\n";
	}
}

bool loadGameState(std::vector<std::vector<int>>& GameGrid, int row, int col,GameModes mode, int& score, int& lineCleared, int& level, float& modeTimer,block& Current, block& Next, block& Hold, bool& hasHold, bool& canHold, int& blockX, int& blockY,bool& hasLocked,bool& isClearingAnimation,float& clearingAnimationTimer,bool& isTspin, std::vector<int>& clearingRows) {
	std::ifstream in(getSaveFileName(mode));
	if (!in.is_open()) return false;
	int m, c, n, h;
	if (!(in >> m >> score >> lineCleared >> level >> modeTimer >> c >> n >> h >> hasHold >> canHold >> blockX >> blockY))
		return false;
	mode = static_cast<GameModes>(m);
	Current = static_cast<block>(c);
	Next = static_cast<block>(n);
	Hold = static_cast<block>(h);
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++) {
			in >> GameGrid[i][j];
		}
}
	hasLocked = false;
	isClearingAnimation = false;
	clearingAnimationTimer = 0.0f;
	clearingRows.clear();
	isTspin = false;
	return true;
}

std::vector<std::vector<int>> CreditBlockPosition(int index, const std::vector<std::string>& creditWords, const std::vector<int>& colorId, int& length) {
	if (index < 6) {
		int wLen = static_cast<int>(creditWords[index].size());
		length = wLen;
		std::vector<std::vector<int>> grid(length, std::vector<int>(length, 0));
		for (int k = 0; k < wLen; k++) {
			grid[0][k] = colorId[index];
		}
		return grid;
	}else{
		length = 4;
		return {
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 }
		};
	}
}

void resetCredits(std::vector<std::vector<int>>& creditGrid,std::vector<std::vector<char>>& creditLetters,std::vector<std::vector<int>>& currentCreditGrid,int& cLength,std::vector<std::string>& creditWords,std::vector<int>& startingColumn,std::vector<int>& colorId,int& index,int& creditY,bool& creditReachedBottom,float& creditFallTimer,sf::Clock& creditClock)
{
	creditGrid.assign(12, std::vector<int>(11, 0));
	creditLetters.assign(12, std::vector<char>(11, ' '));
	creditWords = { "FAIZANEZAD", "ALINAADNAN", "HASEEB", "SFML", "MADEWITH", "PF", "I" };
	startingColumn = { 0,            0,            0,        6,      0,          8,    10 };
	colorId = { 1,            2,            3,        6,      5,          7,    1 };

	index = 0;
	creditY = 0;
	creditReachedBottom = false;
	creditFallTimer = 0.0f;
	currentCreditGrid = CreditBlockPosition(index, creditWords, colorId, cLength);
	creditClock.restart();
}

int main() {
	//--------------------------------------------Random Blocks Generation  Code-------------------------
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 6);
	//----------------------------------Screen Dimensions-----------------------------------------------
	sf::RenderWindow window(sf::VideoMode({ 600,800 }), "Tetris Game");
	window.setIcon(sf::Image(std::filesystem::path("Tetris-logo.PNG")));
	window.setFramerateLimit(60);
	const int viewRow = 20;
	const int bufferRow = 2;
	const int row = viewRow + bufferRow;
	const int column = 10;
	//-----------------------------------Definition-----------------------------------------------------
	std::vector<std::vector<int>>GameGrid(row, std::vector<int>(column, 0));
	sf::RectangleShape background({ 39, 39 });
	int blockX = 3;
	int blockY = 1;
	int rotate = 0;
	int length;
	int Nextlength;
	block Current = static_cast<block>(dist(rng));
	block Next = static_cast<block>(dist(rng));
	Gameevents gevent = Gameevents::Starting;
	//----------------------------------------Hold Code----------------------------------------------------
	block Hold = block::I;
	bool hasHold = false;
	bool canHold = true;
	int Holdlength = 0;
	sf::RectangleShape Holdshape({ 29, 29 });
	sf::RectangleShape Holdrect({ 120, 120 });
	//---------------------------Playing field and blocks----------------------------------------------------
	sf::RectangleShape shape({ 39, 39 });
	sf::RectangleShape Gridshape({ 39, 39 });
	sf::RectangleShape Previewshape({ 29, 29 });
	sf::RectangleShape Previewrect({ 120, 120 });
	sf::RectangleShape Scorerect({ 120, 210 });
	sf::RectangleShape start({ 200, 50 });
	sf::RectangleShape load({ 200, 50 });
	sf::RectangleShape overCreditRect({ 200, 50 });
	sf::RectangleShape menuRect({ 200, 50 });
	sf::RectangleShape pauseMenuRect({ 200, 50 });
	sf::RectangleShape resumeRect({ 200, 50 });
	sf::RectangleShape restartRect({ 200, 50 });
	sf::RectangleShape credit({ 200, 46 });
	sf::RectangleShape ghost({ 39,39 });
	sf::RectangleShape Mode({ 270,46 });
	sf::RectangleShape pause(sf::Vector2f(window.getSize()));
	sf::RectangleShape GameOver(sf::Vector2f(window.getSize()));
	sf::Texture img("Tetris-logo.PNG");
	sf::Sprite Image(img);
	sf::Clock Timer;
	//-------------------------------------------High Score Handeling---------------------------------------------
	int highScores[3] = { 0 };
	bool isNewHighScore = false;
	{
		std::ifstream inFile("highscore.txt");
		if (inFile.is_open()) {
			inFile >> highScores[0] >> highScores[1] >> highScores[2];
		}
	}
	bool hasSavedGame[3] = {
	std::filesystem::exists("save_marathon.txt"),
	std::filesystem::exists("save_timer.txt"),
	std::filesystem::exists("save_endless.txt")
	};
	//---------------------------------Game modes-------------------------------------------------------------------
	GameModes CurrentMode = GameModes::Endless;
	float modeTimer = 0.0f;
	bool isVictory = false;
	int MarathonLines = 150;
	if (CurrentMode == GameModes::Timer) {
		modeTimer = 300.0f;
	}
	//-------------------------------Scoring and GameControl---------------------------------------------------------
	bool lastButtonSpin = false;
	int lineCleared = 0;
	int level = 1;
	int score = 0;
	float fallTimer = 0.0f;
	std::vector<int> clearingRows;
	bool isClearingAnimation = false;
	float clearingAnimationTimer = 0.0f;
	bool reachedBottom = false;
	bool hasLocked = false;
	bool isTspin = false;
	bool hardDrop = true;
	bool softDrop = false;
	bool clockWise = false;
	std::vector<std::vector<int>> Currentgrid = BlockPosition(shape, Current, length);
	//--------------------------------------------Credits-----------------------------------------------------------------
	std::vector<std::vector<char>> creditLetters(12, std::vector<char>(11, ' '));
	std::vector<std::vector<int>> creditGrid(12, std::vector<int>(11, 0));
	std::vector<std::vector<int>> currentCreditGrid;
	std::vector<std::string> creditWords;
	std::vector<int> startingColoumn;
	std::vector<int> creditColor;
	int cLength = 0;
	int index= 0;
	int creditX = 0;
	int creditY = 0;
	int creditDummyLines = 0;
	int creditDummyLevel = 1;
	int creditDummyScore = 0;
	bool creditDummyTspin = false;
	bool creditReachedBottom = false;
	float creditFallTimer = 0.0f;
	sf::Clock creditClock;
	sf::RectangleShape creditBackRect({ 200, 50 });
	creditBackRect.setFillColor(sf::Color::Cyan);
	creditBackRect.setPosition({ 200, 675 });
	sf::RectangleShape controls({ 200, 46 });
	controls.setFillColor(sf::Color(247, 150, 46));
	sf::RectangleShape controlsBox({ 480, 460 });
	controlsBox.setFillColor(sf::Color(22, 25, 37));
	controlsBox.setOutlineThickness(1);
	controlsBox.setOutlineColor(sf::Color::White);
	controlsBox.setPosition({ 60, 160 });
	sf::RectangleShape controlsBackRect({ 200, 50 });
	controlsBackRect.setFillColor(sf::Color::Cyan);
	controlsBackRect.setPosition({ 200, 690 });
	sf::RectangleShape pauseControlsRect({ 200, 50 });
	pauseControlsRect.setFillColor(sf::Color(247, 150, 46));
	pauseControlsRect.setPosition({ 200, 560 });
	Gameevents previousEvent = Gameevents::Starting;
	std::string controlKeys[6] = { "Left / Right", "Down Arrow", "Spacebar", "Z / X", "C", "P" };
	std::string controlActions[6] = { "Move Left / Right", "Soft Drop", "Hard Drop", "Rotate Block", "Hold Feature", "Pause Game" };

	//-----------------------------------Customization------------------------------------------------------
	background.setFillColor(sf::Color(22, 25, 37));
	background.setOutlineThickness(1);
	background.setOutlineColor(sf::Color::White);
	Previewshape.setOutlineThickness(1);
	Previewshape.setOutlineColor(sf::Color::White);
	Previewrect.setOutlineThickness(1);
	Previewrect.setOutlineColor(sf::Color::White);
	Previewrect.setFillColor(sf::Color::Black);
	Previewrect.setPosition({ 440, 80 });
	Scorerect.setOutlineThickness(1);
	Scorerect.setOutlineColor(sf::Color::White);
	Scorerect.setFillColor(sf::Color::Black);
	Scorerect.setPosition({ 440, 230 });
	Holdshape.setOutlineThickness(1);
	Holdshape.setOutlineColor(sf::Color::White);
	Holdrect.setOutlineThickness(1);
	Holdrect.setOutlineColor(sf::Color::White);
	Holdrect.setFillColor(sf::Color::Black);
	Holdrect.setPosition({ 440, 455 });
	Image.setPosition({ 120, 25 });
	Image.setScale({ 1.25, 1.25 });
	Mode.setFillColor(sf::Color::Yellow);
	Mode.setPosition({ 165, 400 });
	start.setFillColor(sf::Color::Cyan);
	load.setFillColor(sf::Color::Magenta);
	load.setPosition({ 200, 550 });
	menuRect.setFillColor(sf::Color::Blue);
	menuRect.setPosition({ 200, 320 });
	pauseMenuRect.setFillColor(sf::Color::Blue);
	pauseMenuRect.setPosition({ 200, 480 });
	overCreditRect.setPosition({ 200, 480 });
	overCreditRect.setFillColor(sf::Color::Green);
	resumeRect.setFillColor(sf::Color::Green);
	resumeRect.setPosition({ 200, 320 });
	restartRect.setFillColor(sf::Color::Magenta);
	restartRect.setPosition({ 200, 400 });
	credit.setFillColor(sf::Color::Green);
	pause.setFillColor(sf::Color(0, 0, 0, 160));
	GameOver.setFillColor(sf::Color(0, 0, 0, 200));
	//--------------------------Fonts and Text-------------------------------------------------------------------------------
	sf::Font font;
	font.openFromFile("C:/Windows/Fonts/arial.ttf");
	sf::Font Gamefont;
	Gamefont.openFromFile("PressStart2P-Regular.ttf");
	sf::Text scoreText(font);
	sf::Text lineClearedText(font);
	sf::Text levelText(font);
	sf::Text remTime(font);
	sf::Text pauseText(Gamefont, "PAUSED", 48);
	sf::Text gameOver(Gamefont, "Game Over", 48);
	sf::Text returnToMenue(font, "Main Menu", 30);
	sf::Text pauseReturnToMenue(font, "Main Menu", 30);
	sf::Text restartText(font, "Restart", 30);
	sf::Text resumeText(font, "Resume", 30);
	sf::Text modeText(font, "Mode: ", 30);
	sf::Text startText(font, "Start", 30);
	sf::Text loadText(font, "Load", 30);
	sf::Text CreditText(font, "Credit ", 30);
	sf::Text overCreditText(font, "Credit ", 30);
	sf::Text holdText(font, "Hold H", 16);
	sf::Text highScoreText(font);
	sf::Text mainHighScoreText(font);
	sf::Text overHighScoreText(font);
	sf::Text overScoreText(font);
	sf::Text creditBackText(font, "Main Menu", 30);
	sf::Text controlsText(font, "Controls", 30);
	controlsText.setFillColor(sf::Color::Black);
	sf::Text controlsTitle(Gamefont, "CONTROLS", 40);
	controlsTitle.setFillColor(sf::Color::Cyan);
	controlsTitle.setOutlineThickness(1);
	controlsTitle.setOutlineColor(sf::Color::White);
	controlsTitle.setPosition({ 140, 70 });
	sf::Text controlsBackText(font, "Main Menu", 30);
	controlsBackText.setFillColor(sf::Color::Black);
	controlsBackText.setPosition({ 225, 695 });
	sf::Text pauseControlsText(font, "Controls", 30);
	pauseControlsText.setFillColor(sf::Color::Black);
	pauseControlsText.setPosition({ 245, 565 });
	sf::Text rowKey(font, "", 22);
	rowKey.setFillColor(sf::Color::Yellow);
	sf::Text rowAction(font, "", 22);
	rowAction.setFillColor(sf::Color::White);
	creditBackText.setFillColor(sf::Color::Black);
	creditBackText.setPosition({ 225, 590 });
	highScoreText.setFillColor(sf::Color::Yellow);
	highScoreText.setCharacterSize(16);
	highScoreText.setPosition({ 448, 405 });
	mainHighScoreText.setFillColor(sf::Color::Yellow);
	mainHighScoreText.setCharacterSize(32);
	mainHighScoreText.setPosition({ 185, 325 });
	overHighScoreText.setFillColor(sf::Color::Yellow);
	overHighScoreText.setString("NEW HIGH SCORE!");
	creditBackText.setFillColor(sf::Color::Black);
	creditBackText.setPosition({ 225, 565 });
	overCreditText.setFillColor(sf::Color::Black);
	overCreditText.setPosition({ 255, 485 });
	overHighScoreText.setCharacterSize(32);
	overHighScoreText.setPosition({ 160, 185 });
	overScoreText.setFillColor(sf::Color::Magenta);
	overScoreText.setCharacterSize(38);
	overScoreText.setPosition({ 205, 185 });
	holdText.setFillColor(sf::Color::White);
	holdText.setPosition({ 440, 425 });
	pauseText.setPosition({ 150,50 });
	pauseText.setFillColor(sf::Color::Cyan);
	pauseText.setOutlineThickness(1);
	pauseText.setOutlineColor(sf::Color::White);
	gameOver.setPosition({ 95,100 });
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setOutlineThickness(1);
	gameOver.setOutlineColor(sf::Color::Black);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(16);
	scoreText.setPosition({ 448, 235 });
	lineClearedText.setFillColor(sf::Color::White);
	lineClearedText.setCharacterSize(16);
	lineClearedText.setPosition({ 448, 278 });
	levelText.setFillColor(sf::Color::White);
	levelText.setCharacterSize(16);
	levelText.setPosition({ 448, 321 });
	remTime.setFillColor(sf::Color::White);
	remTime.setCharacterSize(16);
	remTime.setPosition({ 448, 364 });
	returnToMenue.setFillColor(sf::Color::Black);
	returnToMenue.setPosition({ 225, 325 });
	resumeText.setFillColor(sf::Color::Black);
	resumeText.setPosition({ 250, 325 });
	pauseReturnToMenue.setFillColor(sf::Color::Black);
	pauseReturnToMenue.setPosition({ 225, 485 });
	restartText.setFillColor(sf::Color::Black);
	restartText.setPosition({ 250, 405 });
	overCreditText.setFillColor(sf::Color::Black);
	overCreditText.setPosition({ 260, 485 });
	modeText.setFillColor(sf::Color::Black);
	CreditText.setFillColor(sf::Color::Black);
	startText.setFillColor(sf::Color::Black);
	startText.setPosition({ 265, 480 });
	loadText.setFillColor(sf::Color::Black);
	loadText.setPosition({ 265, 560 });
	//-------------------------------------------Music---------------------------------------------------------
	std::string songs[5] = { "song1.wav", "song2.wav", "song3.wav", "song4.wav", "song5.wav" };
	int currentTrack = 0;
	sf::Music bgMusic;
	bgMusic.openFromFile(songs[currentTrack]);
	bgMusic.setLooping(false);
	bgMusic.setVolume(50.f);
	bgMusic.play();
	//--------------------------------Game Loop-------------------------------------------------------------
	while (window.isOpen()) {
		//------------------------------------Music lOop----------------------------------------------------
		if (bgMusic.getStatus() == sf::SoundSource::Status::Stopped) {
			currentTrack++;
			currentTrack %= 5;
			bgMusic.openFromFile(songs[currentTrack]);
			bgMusic.play();
		}
		//------------------------------------Event Check & Movement----------------------------------------
		while (const auto event = window.pollEvent()) {
			if (event->is < sf::Event::Closed >()) {
				window.close();
			}
			//---------------------------------------------Satrting screen Controls----------------------------------------------------------
			if (gevent == Gameevents::Starting) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = start.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + start.getSize().x && mousePos.y >= pos.y && mousePos.y <= pos.y + start.getSize().y) {
						reset(blockX, blockY, GameGrid, row, column, level, score, canHold, fallTimer, hasHold, lineCleared, modeTimer, isVictory, lastButtonSpin, CurrentMode, isNewHighScore, Gridshape);
						Timer.restart();
						gevent = Gameevents::Playing;
					}
				}
				if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
					if (key->code == sf::Keyboard::Key::Right) {
						CurrentMode = static_cast<GameModes>((static_cast<int>(CurrentMode) + 1) % 3);
					}
					else if (key->code == sf::Keyboard::Key::Left) {
						CurrentMode = static_cast<GameModes>((static_cast<int>(CurrentMode) + 2) % 3);
					}
					else if (key->code == sf::Keyboard::Key::Enter) {
						reset(blockX, blockY, GameGrid, row, column, level, score, canHold, fallTimer, hasHold, lineCleared, modeTimer, isVictory, lastButtonSpin, CurrentMode, isNewHighScore, Gridshape);
						Timer.restart();
						gevent = Gameevents::Playing;
					}
				}
			}
			//----------------------------------------------Playing Screen Controls-----------------------------------
			if (gevent == Gameevents::Playing&&!isClearingAnimation) {
				if (event->is < sf::Event::KeyPressed >()) {
					auto key = event->getIf < sf::Event::KeyPressed >();
					if (key->code == sf::Keyboard::Key::Left) {
						int offset = -1;
						lastButtonSpin = false;
						if (!(HorizontalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column, offset))) {
							blockX--;
						}
					}
					else if (key->code == sf::Keyboard::Key::Right) {
						int offset = 1;
						lastButtonSpin = false;
						if (!(HorizontalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column, offset))) {
							blockX++;
						}
					}
					else if (key->code == sf::Keyboard::Key::Down) {
						softDrop = true;
					}
					else if (key->code == sf::Keyboard::Key::Space && hardDrop) {
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
					else if (key->code == sf::Keyboard::Key::X || key->code == sf::Keyboard::Key::Z) {
						if (key->code == sf::Keyboard::Key::X) {
							clockWise = true;
						}
						else {
							clockWise = false;
						}
						ShadowRotate(Currentgrid, GameGrid, length, blockX, blockY, row, column, Current, lastButtonSpin, clockWise);
					}
					else if (key->code == sf::Keyboard::Key::C && canHold && !reachedBottom) {
						lastButtonSpin = false;
						if (!hasHold) {
							Hold = Current;
							hasHold = true;
							Current = Next;
							Next = static_cast<block>(dist(rng));
						}
						else {
							block temp = Current;
							Current = Hold;
							Hold = temp;
						}
						blockX = 3;
						blockY = 1;
						fallTimer = 0.0f;
						lastButtonSpin = false;
						Currentgrid = BlockPosition(shape, Current, length);
						canHold = false;
						if (VerticalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column, 0)) {
							gevent = Gameevents::GameOver;
						}
					}

				}
			}
				if (event->is < sf::Event::KeyReleased >()) {
					auto key = event->getIf < sf::Event::KeyReleased >();
					if (key->code == sf::Keyboard::Key::Down) {
						softDrop = false;
					}
					if (key->code == sf::Keyboard::Key::Space) {
						hardDrop = true;
					}
				}
			
			if (event->is < sf::Event::KeyPressed >()) {
				auto key = event->getIf < sf::Event::KeyPressed >();
				if (key->code == sf::Keyboard::Key::P) {
					if (gevent == Gameevents::Playing) {
						gevent = Gameevents::Pause;
						bgMusic.pause();
						Timer.stop();
					}
					else if (gevent == Gameevents::Pause) {
						Timer.start();
						bgMusic.play();
						gevent = Gameevents::Playing;
					}
				}
			}
		}
		std::vector<std::vector<int>> Nextgrid = BlockPosition(Previewshape, Next, Nextlength);
		//--------------------------Constant Drop and Locking Positon------------------------------------------------------------
		if (gevent == Gameevents::Playing) {
			float deltaTime = Timer.restart().asSeconds();
			if (isClearingAnimation) {
				clearingAnimationTimer += deltaTime;
				if (clearingAnimationTimer > 0.5f) {
					clearance(GameGrid, row, column, lineCleared, level, score, isTspin);
					clearingRows.clear();
					isClearingAnimation = false;
					clearingAnimationTimer = 0.0f;
				}
			}
			//----------------------------------------Game Modes---------------------------------------------------------------------
			switch (CurrentMode) {
			case::GameModes::Timer:
				modeTimer -= deltaTime;
				if (modeTimer <= 0.0f) {
					modeTimer = 0.0f;
					gevent = Gameevents::GameOver;
				}
				break;
			case::GameModes::Marathon:
				modeTimer += deltaTime;
				if (MarathonLines <= lineCleared) {
					gevent = Gameevents::GameOver;
					isVictory = true;
				}
				break;
			case::GameModes::Endless:
				modeTimer += deltaTime;
				break;
			}
			//---------------------------------------------------T Spin-----------------------------------------------------------------
			
			if (Current == block::T && lastButtonSpin && reachedBottom) {
				isTspin = Tspin(Currentgrid, GameGrid, blockX, blockY, length, row, column);
			}
			//------------------------------------------Collision and Line clearing------------------------------------------------------------
			if (reachedBottom && !hasLocked) {
				CollisionResult(Currentgrid, GameGrid, length, blockX, blockY, row, column, reachedBottom);
				hasLocked = true;
			}
			if (!isClearingAnimation) {
				clearingRows = FindCompletedRow(GameGrid, row, column);
				if (!clearingRows.empty()) {
					isClearingAnimation = true;
					clearingAnimationTimer = 0.0f;
				}
				else {
					clearance(GameGrid, row, column, lineCleared, level, score, isTspin);
					isTspin = false;
				}
			}
			//---------------------------------------------High Score-------------------------------------------------------------------
			int modeIdx = static_cast<int>(CurrentMode);
			if (score > highScores[modeIdx]) {
				highScores[modeIdx] = score;
				isNewHighScore = true;
				std::ofstream outFile("highscore.txt");
				if (outFile.is_open()) {
					outFile << highScores[0] << " " << highScores[1] << " " << highScores[2];
				}
			}
			//--------------------------------------------Vertical Drop and Locking--------------------------------------------------
			if (reachedBottom && !isClearingAnimation) {
				Current = Next;
				Next = static_cast<block>(dist(rng));
				blockX = 3;
				blockY = 0;
				canHold = true;
				lastButtonSpin = false;
				Currentgrid = BlockPosition(shape, Current, length);
				reachedBottom = false;
				hasLocked = false;
				if (VerticalCollisionCheck(Currentgrid, GameGrid, length, blockX, blockY, row, column, 0)) {
					gevent = Gameevents::GameOver;
				}
			}
			else {
				Drop(blockY, level, deltaTime, fallTimer, Currentgrid, length, reachedBottom, blockX, column, row, GameGrid, Current, lastButtonSpin,softDrop);
			}
			//------------------------------------------Game Over Conditions-------------------------------------------------------------------
			for (int i = 0; i < bufferRow; i++) {
				for (int j = 0; j < column; j++) {
					if (GameGrid[i][j] != 0) {
						gevent = Gameevents::GameOver;
					}
				}
			}
		}
		//----------------------------------------Drawing------------------------------------------------------
		if (gevent == Gameevents::Playing || gevent == Gameevents::Pause || gevent == Gameevents::GameOver) {
			window.clear(sf::Color(15, 17, 26));
			for (int i = bufferRow;i < row;i++) {
				for (int j = 0;j < column;j++) {
					background.setPosition(sf::Vector2f(20.0f + (j * 40.0f), (i - bufferRow) * 40.0f));
					window.draw(background);

					if (GameGrid[i][j] != 0) {
						int colour = GameGrid[i][j];
						Gridshape.setPosition(sf::Vector2f(20.0f + (j * 40.0f), (i - bufferRow) * 40.0f));
						GridShapeColour(Gridshape, colour);
						bool whiteRow = false;
						if (isClearingAnimation) {
							for (int k = 0;k < clearingRows.size();k++) {
								if (i == clearingRows[k]) {
									whiteRow = true;
									break;
								}
							}
						}
						if (whiteRow) {
							Gridshape.setFillColor(sf::Color::White);
						}
						window.draw(Gridshape);
					}
				}
			}
			window.draw(Previewrect);
			window.draw(Holdrect);
			window.draw(holdText);
			if (gevent == Gameevents::Playing) {
				window.draw(Scorerect);
				scoreText.setString(
					"Score: " + std::to_string(score));
				window.draw(scoreText);
				if (CurrentMode == GameModes::Marathon)
					lineClearedText.setString("Lines: " + std::to_string(lineCleared) + "/" + std::to_string(MarathonLines));
				else
					lineClearedText.setString("Lines: " + std::to_string(lineCleared));
				window.draw(lineClearedText);

				levelText.setString("Level: " + std::to_string(level));
				window.draw(levelText);

				int totalSecs = static_cast<int>(modeTimer);
				int mins = totalSecs / 60;
				int secs = totalSecs % 60;
				std::string timeStr = std::to_string(mins) + ":" + (secs < 10 ? "0" : "") + std::to_string(secs);
				remTime.setString("Time: " + timeStr);
				window.draw(remTime);
				highScoreText.setString("Best: " + std::to_string(highScores[static_cast<int>(CurrentMode)]));
				window.draw(highScoreText);
				if (!reachedBottom) {
					for (int i = 0;i < length;i++) {
						for (int j = 0;j < length;j++) {
							if (Currentgrid[i][j] != 0) {
								float posX = 20.0f + ((j + blockX) * 40.0f);
								float posY = (i + blockY - bufferRow) * 40.0f;
								shape.setPosition(sf::Vector2f(posX, posY));
								float ghostY = (float)blockY;
								while (!VerticalCollisionCheck(Currentgrid, GameGrid, length, blockX, (int)ghostY, row, column, 1)) {
									ghostY++;
								}
								ghostY = (i + ghostY - bufferRow) * 40.0f;
								ghost.setPosition({ posX,ghostY });
								sf::Color ghostColor = shape.getFillColor();
								ghostColor.a = 160;
								ghost.setFillColor(sf::Color::Transparent);
								ghost.setOutlineThickness(2.0f);
								ghost.setOutlineColor(ghostColor);
								window.draw(ghost);
								window.draw(shape);
							}
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
				if (hasHold) {
					int holdId = 1;
					std::vector<std::vector<int>> Holdgrid = BlockPosition(Holdshape, Hold, Holdlength);
					if (!canHold) {
						Holdshape.setFillColor(sf::Color(90, 90, 90));
					}
					float holdStartX = 500.0f - ((Holdlength * previewCellSize) / 2.0f);
					float holdStartY = 515.0f - ((Holdlength * previewCellSize) / 2.0f);
					for (int i = 0; i < Holdlength; i++) {
						for (int j = 0; j < Holdlength; j++) {
							if (Holdgrid[i][j] != 0) {
								Holdshape.setPosition(sf::Vector2f(holdStartX + (j * previewCellSize),
									holdStartY + (i * previewCellSize)));
								window.draw(Holdshape);
							}
						}
					}
				}
			}
			if (gevent == Gameevents::Pause) {
				window.draw(pause);
				window.draw(pauseText);
				window.draw(pauseMenuRect);
				window.draw(pauseReturnToMenue);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = pauseMenuRect.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + start.getSize().x && mousePos.y >= pos.y && mousePos.y <= pos.y + start.getSize().y) {
						gevent = Gameevents::Starting;
						saveGameState(GameGrid, row, column, CurrentMode, score, lineCleared, level, modeTimer, Current, Next, Hold, hasHold, canHold, blockX, blockY);
						hasSavedGame[static_cast<int>(CurrentMode)] = true;
						bgMusic.play();
					}
				}
				window.draw(resumeRect);
				window.draw(resumeText);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = resumeRect.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + start.getSize().x && mousePos.y >= pos.y && mousePos.y <= pos.y + start.getSize().y) {
						gevent = Gameevents::Playing;
						Timer.start();
						bgMusic.play();
					}
				}
				window.draw(restartRect);
				window.draw(restartText);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = restartRect.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + start.getSize().x && mousePos.y >= pos.y && mousePos.y <= pos.y + start.getSize().y) {
						reset(blockX, blockY, GameGrid, row, column, level, score, canHold, fallTimer, hasHold, lineCleared, modeTimer, isVictory, lastButtonSpin, CurrentMode, isNewHighScore, Gridshape);
						gevent = Gameevents::Playing;
						bgMusic.play();
					}
				}
				window.draw(pauseControlsRect);
				window.draw(pauseControlsText);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = pauseControlsRect.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + pauseControlsRect.getSize().x &&
						mousePos.y >= pos.y && mousePos.y <= pos.y + pauseControlsRect.getSize().y) {
						previousEvent = Gameevents::Pause;
						gevent = Gameevents::Controls;
					}
				}
			}
			if (gevent == Gameevents::GameOver) {
				window.draw(GameOver);
				for (int i = bufferRow;i < row;i++) {
					for (int j = 0;j < column;j++) {
						if (GameGrid[i][j] != 0) {
							Gridshape.setPosition(sf::Vector2f(20.0f + (j * 40.0f), (i - bufferRow) * 40.0f));
							Gridshape.setOutlineThickness(1);
							Gridshape.setOutlineColor(sf::Color(0, 0, 0, 160));
							Gridshape.setFillColor(sf::Color(53, 56, 57));
							window.draw(Gridshape);
						}
					}
				}
				if (isVictory) {
					gameOver.setString("VICTORY!");
					gameOver.setFillColor(sf::Color::Green);
					gameOver.setPosition({ 115, 100 });
				}
				else {
					gameOver.setString("Game Over");
					gameOver.setFillColor(sf::Color::Red);
					gameOver.setPosition({ 95, 100 });
				}
				window.draw(gameOver);
				if (isNewHighScore) {
					window.draw(overHighScoreText);
					overScoreText.setString("Score: " + std::to_string(score));
					overScoreText.setPosition({ 205, 245 });
					window.draw(overScoreText);
				}
				else {
					overScoreText.setString("Score: " + std::to_string(score));
					window.draw(overScoreText);
				}
				window.draw(menuRect);
				window.draw(returnToMenue);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = menuRect.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + start.getSize().x && mousePos.y >= pos.y && mousePos.y <= pos.y + start.getSize().y) {
						gevent = Gameevents::Starting;
						reset(blockX, blockY, GameGrid, row, column, level, score, canHold, fallTimer, hasHold, lineCleared, modeTimer, isVictory, lastButtonSpin, CurrentMode, isNewHighScore, Gridshape);
					}
				}
				window.draw(restartRect);
				window.draw(restartText);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = restartRect.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + start.getSize().x && mousePos.y >= pos.y && mousePos.y <= pos.y + start.getSize().y) {
						reset(blockX, blockY, GameGrid, row, column, level, score, canHold, fallTimer, hasHold, lineCleared, modeTimer, isVictory, lastButtonSpin, CurrentMode, isNewHighScore, Gridshape);
						gevent = Gameevents::Playing;
					}
				}
				hasSavedGame[static_cast<int>(CurrentMode)] = false;
				std::filesystem::remove(getSaveFileName(CurrentMode));
				window.draw(overCreditRect);
				window.draw(overCreditText);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = overCreditRect.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + credit.getSize().x &&
						mousePos.y >= pos.y && mousePos.y <= pos.y + credit.getSize().y) {
						resetCredits(creditGrid, creditLetters, currentCreditGrid, cLength, creditWords, startingColoumn, creditColor, index, creditY, creditReachedBottom, creditFallTimer, creditClock);
						gevent = Gameevents::Credits;
					}
				}
			}
			window.display();
		}
		else if (gevent == Gameevents::Starting) {
			window.clear(sf::Color(10, 12, 18, 200));
			window.draw(Image);
			window.draw(Mode);
			if (CurrentMode == GameModes::Endless) {
				modeText.setString("< Mode: Endless >");
				modeText.setPosition({ 175, 405 });
				window.draw(modeText);
			}
			else if (CurrentMode == GameModes::Marathon) {
				modeText.setString("< Mode: Marathon >");
				modeText.setPosition({ 165, 405 });
				window.draw(modeText);
			}
			else {
				modeText.setString("< Mode: Timer >");
				modeText.setPosition({ 190, 405 });
				window.draw(modeText);
			}
			mainHighScoreText.setString("High Score: " + std::to_string(highScores[(int)CurrentMode]));
			window.draw(mainHighScoreText);
			if (hasSavedGame[static_cast<int>(CurrentMode)]) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = load.getPosition();
					if (mousePos.x >= pos.x && mousePos.x <= pos.x + load.getSize().x &&
						mousePos.y >= pos.y && mousePos.y <= pos.y + load.getSize().y) {
						if (loadGameState(GameGrid, row, column, CurrentMode, score, lineCleared, level, modeTimer,Current, Next, Hold, hasHold, canHold, blockX, blockY,hasLocked,isClearingAnimation,clearingAnimationTimer,isTspin,clearingRows)) {
							Currentgrid = BlockPosition(shape, Current, length);
							reachedBottom = false;
							lastButtonSpin = false;
							isVictory = false;
							isNewHighScore = false;
							Gridshape.setOutlineThickness(0.0f);
							Timer.restart();
							gevent = Gameevents::Playing;
						}
					}
				}
				start.setPosition({ 200, 460 });
				startText.setPosition({ 265, 468 });
				load.setPosition({ 200, 520 });
				loadText.setPosition({ 265, 528 });
				controls.setPosition({ 200, 580 });
				controlsText.setPosition({ 245, 585 });
				credit.setPosition({ 200, 636 });
				CreditText.setPosition({ 260, 641 });
				window.draw(start);
				window.draw(startText);
				window.draw(load);
				window.draw(loadText);
				window.draw(controls);
				window.draw(controlsText);
				window.draw(credit);
				window.draw(CreditText);
			}
			else {
				start.setPosition({ 200, 470 });
				startText.setPosition({ 265, 478 });
				controls.setPosition({ 200, 540 });
				controlsText.setPosition({ 245, 545 });
				credit.setPosition({ 200, 610 });
				CreditText.setPosition({ 260, 615 });
				window.draw(start);
				window.draw(startText);
				window.draw(controls);
				window.draw(controlsText);
				window.draw(credit);
				window.draw(CreditText);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f pos = controls.getPosition();
				if (mousePos.x >= pos.x && mousePos.x <= pos.x + controls.getSize().x &&
					mousePos.y >= pos.y && mousePos.y <= pos.y + controls.getSize().y) {
					previousEvent = Gameevents::Starting;
					gevent = Gameevents::Controls;
				}
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f pos = credit.getPosition();
				if (mousePos.x >= pos.x && mousePos.x <= pos.x + credit.getSize().x &&
					mousePos.y >= pos.y && mousePos.y <= pos.y + credit.getSize().y) {
					resetCredits(creditGrid, creditLetters, currentCreditGrid, cLength, creditWords, startingColoumn, creditColor, index, creditY, creditReachedBottom, creditFallTimer, creditClock);
					gevent = Gameevents::Credits;
				}
			}
			window.display();
		}
		else if (gevent == Gameevents::Credits) {
			window.clear();
			float dt = creditClock.restart().asSeconds();
			if (index < static_cast<int>(creditWords.size())) {
			creditX = startingColoumn[index];
				if (creditReachedBottom) {
					CollisionResult(currentCreditGrid, creditGrid, cLength, creditX, creditY, 12, 11, creditReachedBottom);
					std::string w = creditWords[index];
					for (int k = 0; k < static_cast<int>(w.size()); k++) {
						creditLetters[creditY][creditX + k] = w[k];
					}
					if (index == 6) {
						clearance(creditGrid, 12, 11, creditDummyLines, creditDummyLevel, creditDummyScore, creditDummyTspin);
						creditLetters.assign(12, std::vector<char>(11, ' '));
						resetCredits(creditGrid, creditLetters, currentCreditGrid, cLength, creditWords, startingColoumn, creditColor, index, creditY, creditReachedBottom, creditFallTimer, creditClock);
						continue;
					}

					creditReachedBottom = false;
					index++;
					if (index < static_cast<int>(creditWords.size())) {
						creditX = startingColoumn[index];
						creditY = 0;
						currentCreditGrid = CreditBlockPosition(index, creditWords, creditColor, cLength);
					}
				}
				else {
					Drop(creditY, 5, dt, creditFallTimer, currentCreditGrid, cLength, creditReachedBottom, creditX, 11, 12, creditGrid, block::I, creditDummyTspin);
				}
			}

			float startX = (static_cast<float>(window.getSize().x) - (11 * 40.0f)) / 2.0f;
			float startY = (static_cast<float>(window.getSize().y) - (12 * 40.0f)) / 2.0f;

			sf::Text credTitle(Gamefont, "CREDITS", 42);
			credTitle.setFillColor(sf::Color::Magenta);
			credTitle.setPosition({ (static_cast<float>(window.getSize().x) - credTitle.getGlobalBounds().size.x) / 2.0f, startY - 85.0f });
			window.draw(credTitle);

			sf::Text letterText(Gamefont, "", 20);
			letterText.setFillColor(sf::Color::Black);
			for (int r = 0; r < 12; r++) {
				for (int c = 0; c < 11; c++) {
					if (creditGrid[r][c] == 0) {
						Gridshape.setFillColor(sf::Color(22, 25, 37));
					}
					else {
						GridShapeColour(Gridshape, creditGrid[r][c]);
					}

					Gridshape.setOutlineThickness(1.0f);
					Gridshape.setOutlineColor(sf::Color::White);
					float px = startX + c * 40.0f;
					float py = startY + r * 40.0f;

					Gridshape.setPosition({ px, py });
					window.draw(Gridshape);

					if (creditGrid[r][c] != 0 && creditLetters[r][c] != ' ') {
						letterText.setString(std::string(1, creditLetters[r][c]));
						sf::FloatRect bounds = letterText.getLocalBounds();
						letterText.setPosition({
							px + (40.0f - bounds.size.x) / 2.0f - bounds.position.x,
							py + (40.0f - bounds.size.y) / 2.0f - bounds.position.y
							});
						window.draw(letterText);
					}
				}
			}

			if(index < static_cast<int>(creditWords.size())) {
				for (int i = 0; i < cLength; i++) {
					for (int j = 0; j < cLength; j++) {
						if (currentCreditGrid[i][j] != 0) {
							float px = startX + (creditX + j) * 40.0f;
							float py = startY + (creditY + i) * 40.0f;
							GridShapeColour(Gridshape, currentCreditGrid[i][j]);
							Gridshape.setPosition({ px, py });
							window.draw(Gridshape);

							if (index < 6) {
								letterText.setString(std::string(1, creditWords[index][j]));
								sf::FloatRect bounds = letterText.getLocalBounds();
								letterText.setPosition({
									px + (40.0f - bounds.size.x) / 2.0f - bounds.position.x,
									py + (40.0f - bounds.size.y) / 2.0f - bounds.position.y
									});
								window.draw(letterText);
							}
						}
					}
				}
			}

			creditBackRect.setPosition({ (static_cast<float>(window.getSize().x) - creditBackRect.getSize().x) / 2.0f, startY + 480.0f + 35.0f });
			creditBackText.setPosition({ creditBackRect.getPosition().x + 25.0f, creditBackRect.getPosition().y + 5.0f });
			window.draw(creditBackRect);
			window.draw(creditBackText);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f pos = creditBackRect.getPosition();
				if (mousePos.x >= pos.x && mousePos.x <= pos.x + credit.getSize().x &&
					mousePos.y >= pos.y && mousePos.y <= pos.y + credit.getSize().y) {
					gevent = Gameevents::Starting;
				}
			}
			window.display();
		}
		else if (gevent == Gameevents::Controls) {
			window.clear(sf::Color(15, 17, 26));
			window.draw(controlsTitle);
			window.draw(controlsBox);

			for (int i = 0; i < 6; i++) {
				rowKey.setString(controlKeys[i]);
				rowKey.setPosition({ 90.0f, 195.0f + (i * 70.0f) });
				window.draw(rowKey);

				rowAction.setString("=  " + controlActions[i]);
				rowAction.setPosition({ 275.0f, 195.0f + (i * 70.0f) });
				window.draw(rowAction);
			}

			if (previousEvent == Gameevents::Pause) {
				controlsBackText.setString("Back");
				controlsBackText.setPosition({ 265, 695 });
			}
			else {
				controlsBackText.setString("Main Menu");
				controlsBackText.setPosition({ 225, 695 });
			}
			window.draw(controlsBackRect);
			window.draw(controlsBackText);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f pos = controlsBackRect.getPosition();
				if (mousePos.x >= pos.x && mousePos.x <= pos.x + controlsBackRect.getSize().x &&
					mousePos.y >= pos.y && mousePos.y <= pos.y + controlsBackRect.getSize().y) {
					gevent = previousEvent;
				}
			}
			window.display();
		}
	}
	return 0;
}
