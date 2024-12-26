#pragma once
#include <stdbool.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <iostream>
using namespace std;

extern bool gameOver;
extern const int width;
extern const int height;
extern int x, y, pointx, pointy, score;
extern int tailX[100];
extern int tailY[100];
extern int nTail;
enum eDirection { STOP, LEFT, RIGHT, UP, DOWN };
eDirection dir;

using namespace std;

static void Setup() {
	gameOver = false;
	dir = STOP;
	x = width / 2 - 1;
	y = height / 2 - 1;
	pointx = rand() % width;
	pointy = rand() % height;
	score = 0;
}

static void Draw() {
	system("cls");
	for (int i = 0; i < width + 2; i++) {
		cout << "_";
	}
	cout << endl;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j <= width; j++) {
			if (j == 0 || j == width)
				cout << "|";
			if (i == y && j == x)
				cout << "0";
			else if (i == pointy && j == pointx)
				cout << "6";
			else {
				bool print = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						print = true;
						cout << "o";
					}
				}
				if (!print) {
					cout << " ";
				}
			}
		}
		cout << endl;
	}

	for (int i = 0; i < width + 2; i++) {
		cout << "=";
	}
	cout << endl;
	cout << "Score: " << score << endl;
}

static void Input() {
	if (_kbhit()) {
		switch (_getch())
		{
		case 'w':
			dir = UP;
			break;
		case 'a':
			dir = LEFT;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'x':
			gameOver = true;
			system("cls");
			cout << endl << "Your Score: " << score;
			cout << endl << "Number of Fruits: " << nTail;
			break;
		}
	}
}

static void Logic() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir)
	{
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	}


	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;
	}

	if (x == pointx && y == pointy) {
		score += 10;
		pointx = rand() % width;
		pointy = rand() % height;
		nTail++;
	}


	if (score >= 50) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
	}

	if (score >= 100) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
	}

	if (score >= 200) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	if (score >= 400) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
	}

	if (score >= 800) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	}

	if (score >= 1600) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	}

}