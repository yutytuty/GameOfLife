#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>

const int WIDTH = 40;
const int HEIGHT = 40;
const int CELL_SIZE = 20;

const int TICK_SPEED = 100;


void drawGridLines(int grid[WIDTH][HEIGHT], sf::RenderWindow& window) {
	for (int i = 0; i < WIDTH; i++) {
		
		sf::Vertex vertical[] = {
			sf::Vertex(sf::Vector2f(i * CELL_SIZE, 0), sf::Color(0, 0, 0)),
			sf::Vertex(sf::Vector2f(i * CELL_SIZE, HEIGHT * CELL_SIZE), sf::Color(0, 0, 0))
		};

		window.draw(vertical, 2, sf::Lines);
	}

	for (int i = 0; i < HEIGHT; i++) {
		sf::Vertex horizontal[] = {
			sf::Vertex(sf::Vector2f(0, i * CELL_SIZE), sf::Color(0, 0, 0)),
			sf::Vertex(sf::Vector2f(WIDTH * CELL_SIZE, i * CELL_SIZE), sf::Color(0, 0, 0))
		};

		window.draw(horizontal, 2, sf::Lines);
	}
}


void drawGrid(int grid[WIDTH][HEIGHT], sf::RenderWindow& window) {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (grid[i][j] == 1) {
				sf::RectangleShape cell;
				cell.setPosition(sf::Vector2f(i * CELL_SIZE, j * CELL_SIZE));
				cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
				cell.setFillColor(sf::Color(0, 0, 0));

				window.draw(cell);
			}
		}
	}
}


int getNeighbourNum(int grid[WIDTH][HEIGHT], int location[2]) {
	int neighbourNum = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!(i == j && i == 0)) {
				if (0 <= location[0] + i && location[0] + i < WIDTH && 0 <= location[1] + j && location[1] + j < HEIGHT) {
					if (grid[location[0] + i][location[1] + j] == 1) {
						neighbourNum++;
					}
				}
			}
		}
	}

	return neighbourNum;
}


int** nextTurn(int grid[WIDTH][HEIGHT]) {
	int** newGrid = new int* [HEIGHT];

	for (int i = 0; i < HEIGHT; i++) {
		newGrid[i] = new int[WIDTH];
	}

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			newGrid[i][j] = grid[i][j];
		}
	}

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			int neighbourNum = getNeighbourNum(grid, new int[] {i, j});

			if (grid[i][j] == 1 && neighbourNum < 2) {
				newGrid[i][j] = 0;
			}

			else if (grid[i][j] == 1 && neighbourNum > 3) {
				newGrid[i][j] = 0;
			}

			else if (grid[i][j] == 0 && neighbourNum == 3) {
				newGrid[i][j] = 1;
			}
		}
	}
	return newGrid;
}


int main() {
	//Initilize Variables
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	int grid[WIDTH][HEIGHT];
	sf::RenderWindow window(sf::VideoMode(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE), "Conway's Game of Life");
	bool doneDrawing = false;

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			grid[i][j] = 0;
		}
	}

	//Main Game Logic
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while (window.isOpen()) {
		sf::Event event;

		//Draw A shape
		//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		while (!doneDrawing) {
			
			while (window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::Closed:
					window.close();
					return 0;
					break;

				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Space) {
						doneDrawing = true;
					}
					break;
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				grid[(int)sf::Mouse::getPosition(window).x / CELL_SIZE][(int)sf::Mouse::getPosition(window).y / CELL_SIZE] = 1;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				grid[(int)sf::Mouse::getPosition(window).x / CELL_SIZE][(int)sf::Mouse::getPosition(window).y / CELL_SIZE] = 0;
			}

			window.clear(sf::Color(255, 255, 255));

			drawGridLines(grid, window);
			drawGrid(grid, window);

			window.display();
		}

		//Game Speed Logic
		//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto startTime = std::chrono::system_clock::now();
		auto currentTime = std::chrono::system_clock::now();
		auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		while (elapsedMilliseconds.count() < TICK_SPEED) {
			currentTime = std::chrono::system_clock::now();

			elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
					return 0;
				}
			}
		}

		startTime = std::chrono::system_clock::now();

		window.clear(sf::Color(255, 255, 255));

		drawGridLines(grid, window);
		drawGrid(grid, window);
		
		window.display();

		int** newGrid = nextTurn(grid);

		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < HEIGHT; j++) {
				grid[i][j] = newGrid[i][j];
			}
		}
	}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	return 0;
}
