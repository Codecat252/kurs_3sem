#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <clocale>
#include <fstream>
#include <string>

using namespace std;

// Функция для очистки консоли
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Функция для отображения лабиринта
void displayMaze(const vector<vector<char>>& maze, int playerX, int playerY, int robotX, int robotY) {
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (i == playerX && j == playerY) {
                cout << 'P'; // Отображение игрока
            }
            else if (i == robotX && j == robotY) {
                cout << 'R'; // Отображение робота
            }
            else {
                cout << maze[i][j];
            }
        }
        cout << endl;
    }
}

// Проверка возможности перемещения на новую позицию
bool canMove(const vector<vector<char>>& maze, int newX, int newY) {
    return maze[newX][newY] == ' ' || maze[newX][newY] == 'E';
}

// Генерация пути от входа до выхода в лабиринте с помощью DFS
void generatePathDFS(vector<vector<char>>& maze, int x, int y) {
    // Фиксируем направление: вверх, вправо, вниз, влево
    int directions[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

    // Перемешиваем направления для случайности
    for (int i = 0; i < 4; ++i) {
        int r = rand() % 4;
        swap(directions[i], directions[r]);
    }

    maze[x][y] = ' ';

    for (const auto& dir : directions) {
        int nx = x + dir[0] * 2;
        int ny = y + dir[1] * 2;
        if (nx > 0 && ny > 0 && nx < maze.size() - 1 && ny < maze[0].size() - 1 && maze[nx][ny] == '#') {
            maze[x + dir[0]][y + dir[1]] = ' ';
            generatePathDFS(maze, nx, ny);
        }
    }
}

// Функция для генерации случайного лабиринта с гарантированным путём
vector<vector<char>> generateRandomMaze(int size) {
    vector<vector<char>> maze(size, vector<char>(size, '#'));

    // Генерация пути с помощью DFS от входа до выхода
    generatePathDFS(maze, 1, 1);

    // Устанавливаем вход и выход
    maze[1][1] = ' ';
    maze[size - 2][size - 3] = 'E';

    return maze;
}

// Функция для загрузки лабиринта из файла
vector<vector<char>> loadMazeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        exit(1);
    }

    vector<vector<char>> maze;
    string line;
    while (getline(file, line)) {
        vector<char> row(line.begin(), line.end());
        maze.push_back(row);
    }

    file.close();
    return maze;
}

// Функция для сохранения прогресса игры в файл
void saveGame(const vector<vector<char>>& maze, int playerX, int playerY, int robotX, int robotY, int robotDirection) {
    ofstream file("Save.txt");
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для записи: Save.txt" << endl;
        exit(1);
    }

    // Сохраняем лабиринт
    for (const auto& row : maze) {
        for (char cell : row) {
            file << cell;
        }
        file << endl;
    }

    // Сохраняем позиции игрока и робота, а также направление робота
    file << "POSITIONS " << playerX << " " << playerY << " " << robotX << " " << robotY << " " << robotDirection << endl;

    file.close();
}

// Функция для загрузки прогресса игры из файла
void loadGame(const string& filename, vector<vector<char>>& maze, int& playerX, int& playerY, int& robotX, int& robotY, int& robotDirection) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для чтения: " << filename << endl;
        exit(1);
    }

    // Загружаем лабиринт
    maze.clear();
    string line;
    while (getline(file, line)) {
        if (line.find("POSITIONS") != string::npos) {
            // Если строка содержит "POSITIONS", это позиции игрока и робота
            sscanf(line.c_str(), "POSITIONS %d %d %d %d %d", &playerX, &playerY, &robotX, &robotY, &robotDirection);
            break;
        }
        vector<char> row(line.begin(), line.end());
        maze.push_back(row);
    }

    file.close();
}

// Основная функция игры
int main() {
    setlocale(LC_ALL, "");

    srand(time(0)); // Инициализация генератора случайных чисел

    bool exitToMainMenu = false;

    do {
        int choice;
        cout << "Выберите опцию:\n1. Случайный лабиринт\n2. Загрузить лабиринт из файла\n3. Загрузить сохраненную игру\n";
        cin >> choice;

        vector<vector<char>> maze;
        int playerX, playerY, robotX, robotY;
        int robotDirection;

        if (choice == 1) {
            int size;
            cout << "Введите размер поля (напр., 7 для 7x7): ";
            cin >> size;

            if (size < 5) {
                cout << "Размер поля должен быть не менее 5x5." << endl;
                return 1;
            }

            maze = generateRandomMaze(size);
            playerX = playerY = robotX = robotY = 1;
            robotDirection = 1; // Начинаем движение вправо
        }
        else if (choice == 2) {
            string filename;
            cout << "Введите имя файла с лабиринтом: ";
            cin >> filename;

            maze = loadMazeFromFile(filename);
            playerX = playerY = robotX = robotY = 1;
            robotDirection = 1; // Начинаем движение вправо
        }
        else if (choice == 3) {
            string filename = "Save.txt";
            loadGame(filename, maze, playerX, playerY, robotX, robotY, robotDirection);
        }
        else {
            cout << "Неверный выбор!" << endl;
            continue;
        }

        char move;
        bool gameRunning = true;

        while (gameRunning) {
            clearConsole();
            displayMaze(maze, playerX, playerY, robotX, robotY);
            cout << "Введите W (вверх), A (влево), S (вниз), D (вправо) для перемещения, P для сохранения игры или Q для выхода в главное меню: ";
            cin >> move;

            if (move == 'P' || move == 'p') {
                saveGame(maze, playerX, playerY, robotX, robotY, robotDirection);
                cout << "Игра сохранена в файл Save.txt" << endl;
                continue;
            }
            else if (move == 'Q' || move == 'q') {
                exitToMainMenu = true;
                break;
            }

            int newX = playerX, newY = playerY;

            switch (move) {
            case 'W':
            case 'w':
                newX--;
                break;
            case 'A':
            case 'a':
                newY--;
                break;
            case 'S':
            case 's':
                newX++;
                break;
            case 'D':
            case 'd':
                newY++;
                break;
            default:
                cout << "Неверное движение!" << endl;
                continue;
            }

            // Проверка на возможность перемещения игрока
            if (canMove(maze, newX, newY)) {
                playerX = newX;
                playerY = newY;

                // Проверка на выход из лабиринта
                if (maze[playerX][playerY] == 'E') {
                    cout << "Вы нашли выход! Победа!" << endl;
                    gameRunning = false;
                }
            }
            else {
                cout << "Вы не можете туда пойти!" << endl;
            }

            // Движение робота по алгоритму "вечно идти возле левой стенки"
            int rightDirection = (robotDirection + 1) % 4;
            int forwardDirection = robotDirection;
            int leftDirection = (robotDirection + 3) % 4;

            int directions[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

            int rightX = robotX + directions[rightDirection][0];
            int rightY = robotY + directions[rightDirection][1];

            int forwardX = robotX + directions[forwardDirection][0];
            int forwardY = robotY + directions[forwardDirection][1];

            int leftX = robotX + directions[leftDirection][0];
            int leftY = robotY + directions[leftDirection][1];

            if (canMove(maze, leftX, leftY)) {
                robotX = leftX;
                robotY = leftY;
                robotDirection = leftDirection;

            
            }
            else if (canMove(maze, forwardX, forwardY)) {
                robotX = forwardX;
                robotY = forwardY;
            }
            else if (canMove(maze, rightX, rightY)) {
                robotX = rightX;
                robotY = rightY;
                robotDirection = rightDirection;
            }
            else {
                // Если не можем никуда пойти, поворачиваем назад
                robotDirection = (robotDirection + 2) % 4;
            }

            // Проверка на выход из лабиринта роботом
            if (maze[robotX][robotY] == 'E') {
                cout << "Робот нашел выход!" << endl;
                gameRunning = false;
            }
        }
    } while (exitToMainMenu);

    return 0;
}