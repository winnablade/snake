#include <fstream>
#include<iostream>
#include<conio.h>
#include<chrono>
#include<thread>
#include<Windows.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

bool proebali;
const int w = 20;
const int h = 20;
int x, y, fX, fY, score;
int tailX[100], tailY[100];
int tailNum;
int menuChoice;
int fruitsCollected = 0;
bool canSave = false;

enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
direction dir;

enum GameState { MENU, PLAYING, GAME_OVER };
GameState gameState = MENU;

struct GameData {
	int x, y, score, tailNum;
	direction dir;
	int tailX[100], tailY[100];
	int fX, fY;
};

void setup() {
	proebali = false;
	dir = STOP;
	x = w / 2 - 1;
	y = h / 2 - 1;
	fX = rand() % w;
	fY = rand() % h;
	score = 0;
	menuChoice = 1;
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void saveGame() {
	GameData gameData;
	gameData.x = x;
	gameData.y = y;
	gameData.dir = dir;
	gameData.score = score;
	gameData.tailNum = tailNum;
	for (int i = 0; i < tailNum; ++i) {
		gameData.tailX[i] = tailX[i];
		gameData.tailY[i] = tailY[i];
	}
	gameData.fX = fX;
	gameData.fY = fY;

	json jsonData;
	jsonData["x"] = gameData.x;
	jsonData["y"] = gameData.y;
	jsonData["dir"] = static_cast<int>(gameData.dir);
	jsonData["score"] = gameData.score;
	jsonData["tailNum"] = gameData.tailNum;

	json tailArray = json::array();
	for (int i = 0; i < gameData.tailNum; ++i) {
		json tailItem;
		tailItem["x"] = gameData.tailX[i];
		tailItem["y"] = gameData.tailY[i];
		tailArray.push_back(tailItem);
	}
	jsonData["tail"] = tailArray;

	jsonData["fX"] = gameData.fX;
	jsonData["fY"] = gameData.fY;

	ofstream file("savegame.json");
	if (file.is_open()) {
		file << jsonData.dump();
		file.close();
	}
}

void loadGame() {
	ifstream file("savegame.json");
	if (file.is_open()) {
		json jsonData;
		file >> jsonData;

		GameData gameData;

		gameData.x = jsonData["x"];
		gameData.y = jsonData["y"];
		gameData.dir = static_cast<direction>(jsonData["dir"]);
		gameData.score = jsonData["score"];
		gameData.tailNum = jsonData["tailNum"];

		json tailArray = jsonData["tail"];
		int index = 0;
		for (const auto& tailItem : tailArray) {
			gameData.tailX[index] = tailItem["x"];
			gameData.tailY[index] = tailItem["y"];
			++index;
		}

		gameData.fX = jsonData["fX"];
		gameData.fY = jsonData["fY"];

		x = gameData.x;
		y = gameData.y;
		dir = gameData.dir;
		score = gameData.score;
		tailNum = gameData.tailNum;
		for (int i = 0; i < tailNum; ++i) {
			tailX[i] = gameData.tailX[i];
			tailY[i] = gameData.tailY[i];
		}
		fX = gameData.fX;
		fY = gameData.fY;

		gameState = PLAYING;

		file.close();
	}
}

void menuInput() {
	if (_kbhit()) {
		char key = _getch();
		if (key == 'w' || key == 'W') {
			menuChoice = (menuChoice - 1 < 1) ? 3 : menuChoice - 1;
		}
		else if (key == 's' || key == 'S') {
			menuChoice = (menuChoice + 1 > 3) ? 1 : menuChoice + 1;
		}
		else if (key == 13) {
			switch (menuChoice) {
			case 1:
				setup();
				gameState = PLAYING;
				break;
			case 2:
				if (gameState == MENU) {
					gameState = PLAYING;
					saveGame();
				}
				break;
			case 3:
				proebali = true;
				break;
			}
		}
	}
	system("cls");
	cout << "#(((SNAKE)))#" << endl;

	for (int i = 1; i <= 3; i++) {
		cout << "#";
		if (i == menuChoice) {
			cout << "> ";
		}
		else {
			cout << "  ";
		}
		switch (i) {
		case 1:
			cout << "1. New Game" << endl;
			break;
		case 2:
			if (i == menuChoice) {
				cout << "2. Continue <" << endl;
			}
			break;
		case 3:
			cout << "3. Exit" << endl;
			break;
		}
	}
	cout << "#############" << endl;
}

void mapa() {
	system("cls");

	for (int i = 0; i < w + 1; ++i) {
		cout << '#';
	}
	cout << endl;

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (j == 0 || j == w - 1)
				cout << '#';
			if (i == y && j == x) {
				if (dir == RIGHT)
					cout << '>';
				else if (dir == LEFT)
					cout << '<';
				else if (dir == DOWN)
					cout << 'v';
				else
					cout << '^';

			}
			else if (i == fY && j == fX)
				cout << '*';
			else {
				bool risovawka = false;
				for (int l = 0; l < tailNum; ++l) {
					if (tailX[l] == j && tailY[l] == i) {
						risovawka = true;
						cout << '0';
					}

				}
				if (!risovawka)
					cout << ' ';
			}

		}
		cout << endl;
	}
	for (int i = 0; i < w + 1; ++i) {
		cout << '#';
	}
	cout << endl;
	cout << "Score: " << score << endl;
}

void input() {
	if (_kbhit()) {
		char key = _getch();
		if (key == 'x') {
			proebali = true;
		}
		else if (key == 27) {
			saveGame();
			gameState = MENU;
		}
		else {
			switch (key) {
			case 'a':
				dir = LEFT;
				break;

			case 'd':
				dir = RIGHT;
				break;

			case 'w':
				dir = UP;
				break;

			case 's':
				dir = DOWN;
				break;
			}
		}
	}
}

void logic() {
	int prevX = tailX[0];
	int prevY = tailY[0];
	int p2X, p2Y;
	tailX[0] = x;
	tailY[0] = y;

	for (int i = 1; i < tailNum; ++i) {
		p2X = tailX[i];
		p2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = p2X;
		prevY = p2Y;
	}
	switch (dir)
	{

	case LEFT:
		--x;
		if (x < 0)
			x = w - 2;
		break;
	case RIGHT:
		++x;
		if (x >= w - 1)
			x = 0;
		break;
	case UP:
		--y;
		if (y < 0)
			y = h - 1;
		break;
	case DOWN:
		if (y >= h)
			y = 0;
		++y;
		break;
	}

	for (int i = 0; i < tailNum; ++i) {
		if (tailX[i] == x && tailY[i] == y) {
			proebali = true;
		}
	}

	if (x == fX && y == fY) {
		++score;
		fX = rand() % w;
		fY = rand() % h;
		++tailNum;
	}


}

void gameOverScreen() {
	system("cls");
	int centerX = w / 2;
	int centerY = h / 2;

	for (int i = 0; i < h; ++i) {
		if (i == centerY) {
			for (int j = 0; j < centerX - 5; ++j) {
				cout << " ";
			}
			cout << "Game Over" << endl;
		}
		else {
			cout << endl;
		}
	}

	for (int i = 0; i < centerY - 1; ++i) {
		cout << " ";
	}
	cout << "Your Score: " << score << endl;
}

int main() {
	setup();
	while (!proebali) {
		if (gameState == MENU) {
			system("cls");
			cout << "(((SNAKE)))" << endl;
			cout << "1. New Game" << endl;
			cout << "2. Continue" << endl;
			cout << "3. Exit" << endl;

			char key = _getch();
			if (key == '1') {
				setup();
				gameState = PLAYING;
			}
			if (key == '2') {
				loadGame();
				gameState = PLAYING;
			}
			else if (key == '3') {
				proebali = true;
			}
		}
		else if (gameState == PLAYING) {
			mapa();
			input();
			logic();
			if (proebali) {
				gameState = GAME_OVER;
			}
			if (_kbhit()) {
				char key = _getch();
				if (key == 27) {
					saveGame();
					gameState = MENU;
				}
			}

			auto start = std::chrono::steady_clock::now();
			auto end = std::chrono::steady_clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			if (diff.count() < 200) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200) - diff);
			}
		}
		else if (gameState == GAME_OVER) {
			gameOverScreen();
			char key = _getch();
			if (key == '1') {
				setup();
				gameState = PLAYING;
			}
			else if (key == '3') {
				proebali = true;
			}
		}
	}
	return 0;
}