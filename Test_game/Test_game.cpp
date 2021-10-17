#include <iostream>
#include <conio.h>

int x, y;
int width, height;
enum {NONE, UP, DOWN, RIGHT, LEFT} key;
bool gameActive;

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

	gameActive = true;

	map = new char*[width];

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
	}
}

void LogicGame()
{
	switch (key)
	{
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	}

	map_initialization();
}

int main()
{
	initialization();

	while (gameActive)
	{
		Render();
		Input();
		LogicGame();
	}
}