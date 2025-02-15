#include <iostream>
#include <windows.h>
#include <conio.h>
#include <list>
#include <ctime>
using namespace std;
bool operator==(const COORD& a, const COORD& b) {
    return (a.X == b.X && a.Y == b.Y);
}
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
class Snake {
public:
    list<COORD> body;
    Direction direction;

    Snake() {
        COORD startPos = { 10, 10 };
        body.push_back(startPos);
        direction = RIGHT;
    }

    void move(bool grow = false) {
        COORD head = body.front();
        COORD newHead = head;
        switch (direction) {
        case LEFT:  newHead.X--; break;
        case RIGHT: newHead.X++; break;
        case UP:    newHead.Y--; break;
        case DOWN:  newHead.Y++; break;
        default: break;
        }

        body.push_front(newHead);
        if (!grow) body.pop_back();
    }

    bool isSelfCollision() {
        COORD head = body.front();
        auto it = body.begin();
        ++it;
        for (; it != body.end(); ++it) {
            if (*it == head) return true;
        }
        return false;
    }
};

class Food {
public:
    COORD position;

    Food() { generateFood(40, 20, {}); }

    void generateFood(int width, int height, const std::list<COORD>& snakeBody) {
        bool valid = false;
        while (!valid) {
            position.X = rand() % width;
            position.Y = rand() % height;
            valid = true;
            for (const auto& segment : snakeBody) {
                if (segment == position) {
                    valid = false;
                    break;
                }
            }
        }
    }
};

class Game {
private:
    static Game* instance;
    int width, height, score;
    bool gameOver;
    Snake snake;
    Food food;

    Game() : width(40), height(20), score(0), gameOver(false) {
        food.generateFood(width, height, snake.body);
    }

public:
    static Game* getInstance() {
        if (!instance) instance = new Game();
        return instance;
    }

    void run() {
        while (!gameOver) {
            processInput();
            update();
            render();
            Sleep(100); 
        }

        system("cls");
        cout << "Game Over! Final Score: " << score << std::endl;
        system("pause");
    }

    void processInput() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) { 
                key = _getch();
                switch (key) {
                case 72: if (snake.direction != DOWN) snake.direction = UP; break;
                case 80: if (snake.direction != UP) snake.direction = DOWN; break;
                case 75: if (snake.direction != RIGHT) snake.direction = LEFT; break;
                case 77: if (snake.direction != LEFT) snake.direction = RIGHT; break;
                }
            }
        }
    }

    void update() {
        COORD nextHead = snake.body.front();
        switch (snake.direction) {
        case LEFT:  nextHead.X--; break;
        case RIGHT: nextHead.X++; break;
        case UP:    nextHead.Y--; break;
        case DOWN:  nextHead.Y++; break;
        default: break;
        }

        bool grow = (nextHead == food.position);
        if (grow) {
            score += 10;
            food.generateFood(width, height, snake.body);
        }

        snake.move(grow);

        COORD head = snake.body.front();
        if (head.X < 0 || head.X >= width || head.Y < 0 || head.Y >= height || snake.isSelfCollision()) {
            gameOver = true;
        }
    }

    void render() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cursorPosition = { 0, 0 };
        SetConsoleCursorPosition(hConsole, cursorPosition);

        for (int i = 0; i < width + 2; i++) std::cout << "#";
        cout << "\n";
        for (int y = 0; y < height; y++) {
            cout << "#";
            for (int x = 0; x < width; x++) {
                bool printed = false;
                for (const auto& segment : snake.body) {
                    if (segment.X == x && segment.Y == y) {
                        cout << (segment == snake.body.front() ? "O" : "o");
                        printed = true;
                        break;
                    }
                }
                if (!printed) cout << ((food.position.X == x && food.position.Y == y) ? "*" : " ");
            }
            cout << "#\n";
        }

        // Draw bottom border
        for (int i = 0; i < width + 2; i++) std::cout << "#";
        cout << "\nScore: " << score << std::endl;
    }
};

Game* Game::instance = nullptr;

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Game::getInstance()->run();
    return 0;
}
