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
int steps;

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

	steps = MAX_STEPS;

	gameActive = true;
	bulletFLy = false;

	map = new char*[width];

	map_initialization();

	for (int i = 0; i < 150; i++)
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
}

void Input()
{
	switch (_getch())
	{
	case 'a':	
		key = LEFT;	
		break;
	case 'd':	
		key = RIGHT;
		break;
	case 'w':		
		key = UP;	
		break;
	case 's':
		key = DOWN;
		break;
	case 'x':
		gameActive = false;
		break;
	//case 32:
		//control = FIRE;
		//break;
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
				steps--;
			}
			break;
		case DOWN:
			if (map[y + 1][x] == ' ')
			{
				y++;
				steps--;
			}
			break;
		case LEFT:
			if (map[y][x - 1] == ' ')
			{
				x--;
				steps--;
			}
			break;
		case RIGHT:
			if (map[y][x + 1] == ' ')
			{
				x++;
				steps--;
			}
			break;
		}
		break;
	/*case FIRE:
		bulletFLy = true;
		bulletX = x;
		bulletY = y;
		ammo--;
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

			}
			else if (map[bulletY][bulletX] == '&')
			{
				score++;
			}

			Render();
		}*/
	}

	if (steps <= 0)
	{
		int rangeX;
		int rangeY;

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

	map_initialization();

	if (map[y][x] == '&')
	{
		gameActive = false;
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