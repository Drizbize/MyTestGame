#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <math.h>

int x, y;
int width, height;
int score;
int ammo;

int randWidth, randHeight;

const int MAX_STEPS = 2;
const int MAX_STEPS_SPAWN = 16;

int steps, steps_to_spawnMobs;

int rangeX;
int rangeY;

int bulletX, bulletY;
std::vector<int> enemyX, enemyY;
enum { NONE, UP, DOWN, RIGHT, LEFT } key;
enum { NOFIRE, FIRE} control;
bool gameActive;
bool bulletFLy;

char **map;

void map_initialization()
{
	for (int Y = 0; Y < height; Y++)
	{
		map[Y] = new char[width + 1];
		if (Y == 0 || Y == height - 1)
		{
			for (int X = 0; X <= width; X++)
			{
				map[Y][X] = '#';

				if (X == width)
				{
					map[Y][X] = '\0';
				}
			}
		}
		else
		{
			for (int X = 0; X <= width; X++)
			{
				if (X == 0 || X == width - 1)
				{
					map[Y][X] = '#';
				}
				else
				{
					map[Y][X] = ' ';
				}

				if (Y == y && X == x)
				{
					map[y][x] = '@';
				}

				if (Y == bulletY && X == bulletX)
				{
					map[bulletY][bulletX] = '+';
				}

				for (int i = 0; i < enemyX.size(); i++)
				{
					if (enemyX[i] == X && enemyY[i] == Y)
					{
						map[enemyY[i]][enemyX[i]] = '&';
					}
				}

				if (X == width)
				{
					map[Y][X] = '\0';
				}
			}
		}
	}
}

void initialization()
{
	width = 42;
	height = 26;

	x = width / 2;
	y = height / 2;

	score = 0;
	ammo = 20;

	steps_to_spawnMobs = MAX_STEPS_SPAWN;
	steps = MAX_STEPS;

	gameActive = true;
	bulletFLy = false;

	map = new char*[width];

	map_initialization();

	for (int i = 0; i < 20; i++)
	{
		do
		{
			randWidth = rand() % (width - 2) + 1;
			randHeight = rand() % (height - 2) + 1;
		} while (map[randHeight][randWidth] != ' ');

		enemyX.push_back(randWidth);
		enemyY.push_back(randHeight);
	}

	map_initialization();
}

void Render()
{
	system("cls");

	for (int i = 0; i < height; i++)
	{
		std::cout << map[i] << "\n";
	}
	std::cout << "ammo: " << ammo << "   score: " << score << "\n";
	if (ammo <= 0)
	{
		std::cout << "You need ammo\n";
	}
}

void Input()
{
	switch (_getch())
	{
	case 75:
		key = LEFT;
		break;
	case 77:
		key = RIGHT;
		break;
	case 72:
		key = UP;
		break;
	case 80:
		key = DOWN;
		break;
	case 32:
		control = FIRE;
		break;
	case 'x':
		gameActive = false;
		break;
	default:
		Input();
	}
}

void LogicGame()
{
	switch (control)
	{
	case NOFIRE:
		switch (key)
		{
		case UP:
			if (map[y - 1][x] == ' ')
			{
				y--;
				steps_to_spawnMobs--;
				steps--;
			}
			break;
		case DOWN:
			if (map[y + 1][x] == ' ')
			{
				y++;
				steps_to_spawnMobs--;
				steps--;
			}
			break;
		case LEFT:
			if (map[y][x - 1] == ' ')
			{
				x--;
				steps_to_spawnMobs--;
				steps--;
			}
			break;
		case RIGHT:
			if (map[y][x + 1] == ' ')
			{
				x++;
				steps_to_spawnMobs--;
				steps--;
			}
			break;
		}
		break;
	case FIRE:
		if (ammo > 0)
		{
			bulletFLy = true;
			bulletX = x;
			bulletY = y;
			ammo--;
			steps--;
			while (bulletFLy)
			{
				switch (key)
				{
				case UP:
					bulletY--;
					break;
				case DOWN:
					bulletY++;
					break;
				case LEFT:
					bulletX--;
					break;
				case RIGHT:
					bulletX++;
					break;
				}

				if (map[bulletY][bulletX] == '#')
				{
					bulletFLy = false;
				}
				else if (map[bulletY][bulletX] == '&')
				{
					score++;
					bulletFLy = false;

					for (int i = 0; enemyX.size(); i++)
					{
						if (bulletX == enemyX[i] && bulletY == enemyY[i])
						{
							enemyX.erase(enemyX.begin() + i);
							enemyY.erase(enemyY.begin() + i);
							map_initialization();
							Render();
							break;
						}
					}
				}

				map_initialization();
				Render();
			}
			control = NOFIRE;
			bulletX = -1;
			bulletY = -1;
		}
	}

	if (steps <= 0)
	{
		for (int i = 0; i < enemyX.size(); i++)
		{
			rangeX = enemyX[i] - x;
			rangeY = enemyY[i] - y;

			if (abs(rangeX) > abs(rangeY))
			{
				if (rangeX > rangeY && map[enemyY[i]][enemyX[i] - 1] == ' ' || map[enemyY[i]][enemyX[i] - 1] == '@')
				{
					enemyX[i]--;
				}
				else if (rangeX < rangeY && map[enemyY[i]][enemyX[i] + 1] == ' ' || map[enemyY[i]][enemyX[i] + 1] == '@')
				{
					enemyX[i]++;
				}
				else
				{
					if (rangeY > 0 && map[enemyY[i] + 1][enemyX[i]] == ' ' || map[enemyY[i] + 1][enemyX[i]] == '@')
					{
						enemyY[i]++;
					}
					else if (rangeY < 0 && map[enemyY[i] - 1][enemyX[i]] == ' ' || map[enemyY[i] - 1][enemyX[i]] == '@')
					{
						enemyY[i]--;
					}
				}
			}
			else if (abs(rangeX) < abs(rangeY))
			{
				if (rangeX < rangeY && map[enemyY[i] - 1][enemyX[i]] == ' ' || map[enemyY[i] - 1][enemyX[i]] == '@')
				{
					enemyY[i]--;
				}
				else if (rangeX > rangeY && map[enemyY[i] + 1][enemyX[i]] == ' ' || map[enemyY[i] + 1][enemyX[i]] == '@')
				{
					enemyY[i]++;
				}
				else
				{
					if (rangeX > 0 && map[enemyY[i]][enemyX[i] - 1] == ' ' || map[enemyY[i]][enemyX[i] - 1] == '@')
					{
						enemyX[i]--;
					}
					else if (rangeX < 0 && map[enemyY[i]][enemyX[i] + 1] == ' ' || map[enemyY[i]][enemyX[i] + 1] == ' ')
					{
						enemyX[i]++;
					}
				}
			}
			else if (abs(rangeX) == abs(rangeY))
			{

			}
			map[enemyY[i]][enemyX[i]] = '&';
		}

		steps = MAX_STEPS;
	}

	if (steps_to_spawnMobs <= 0)
	{
		do
		{
			randWidth = rand() % (width - 2) + 1;
			randHeight = rand() % (height - 2) + 1;

			rangeX = abs(randWidth - x);
			rangeY = abs(randHeight - y);
		} while (map[randHeight][randWidth] != ' ');

		enemyX.push_back(randWidth);
		enemyY.push_back(randHeight);

		steps_to_spawnMobs = MAX_STEPS_SPAWN;
	}

	map_initialization();

	if (map[y][x] == '&')
	{
		gameActive = false;
		Render();
	}
}

int main()
{
	srand(time(0));
	initialization();

	while (gameActive)
	{
		Render();
		Sleep(100);
		Input();
		LogicGame();
	}
}
