#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <math.h>
#include <string>

const int COUNT_OF_MOBS = 20;

int x, y;
int arrowX, arrowY;

int width, height;
int score;

int ammo;
int randAmmo, randAmmoRange, randAmmoSpawnIn, stepsToSpawnAmmo;
std::vector<int> ammoX, ammoY;

int randWidth, randHeight;

const int MAX_STEPS = 2;
const int MAX_STEPS_SPAWN = 20;
int steps, steps_to_spawnMobs;

int rangeX, rangeY;
int randMove;

int bulletX, bulletY;
std::vector<int> enemyX, enemyY;
enum { NONE, UP, DOWN, RIGHT, LEFT } key;
enum { lookUP, lookDOWN, lookRIGHT, lookLEFT} yaw;
enum { NOFIRE, FIRE} control;
bool gameActive;
bool bulletFLy;
bool showNeedAmmo;
bool continueGame;

std::string YesNo;

HANDLE wndHandle = GetStdHandle(STD_OUTPUT_HANDLE);

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

				if (map[Y][X] != '#')
				{
					if (X == arrowX && Y == arrowY && arrowY + 1 == y)
					{
						map[arrowY][arrowX] = '^';
					}
					else if (X == arrowX && Y == arrowY && arrowX + 1 == x)
					{
						map[arrowY][arrowX] = '<';
					}
					else if (X == arrowX && Y == arrowY && arrowX - 1 == x)
					{
						map[arrowY][arrowX] = '>';
					}
					else if (X == arrowX && Y == arrowY && arrowY - 1 == y)
					{
						map[arrowY][arrowX] = 'V';
					}

					for (int i = 0; i < ammoX.size(); i++)
					{
						if (X == ammoX[i] && Y == ammoY[i])
						{
							map[ammoY[i]][ammoX[i]] = ':';
						}
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
						if (X == enemyX[i] && Y == enemyY[i])
						{
							map[enemyY[i]][enemyX[i]] = '&';
						}
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

void map_changes()
{
	for (int Y = 1; Y < height - 1; Y++)
	{	
		for (int X = 1; X < width - 1; X++)
		{
			map[Y][X] = ' ';

			if (X == arrowX && Y == arrowY && arrowY + 1 == y)
			{
				map[arrowY][arrowX] = '^';
			}
			else if (X == arrowX && Y == arrowY && arrowX + 1 == x)
			{
				map[arrowY][arrowX] = '<';
			}
			else if (X == arrowX && Y == arrowY && arrowX - 1 == x)
			{
				map[arrowY][arrowX] = '>';
			}
			else if (X == arrowX && Y == arrowY && arrowY - 1 == y)
			{
				map[arrowY][arrowX] = 'V';
			}

			for (int i = 0; i < ammoX.size(); i++)
			{
				if (X == ammoX[i] && Y == ammoY[i])
				{
					map[ammoY[i]][ammoX[i]] = ':';
				}
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
				if (X == enemyX[i] && Y == enemyY[i])
				{
					map[enemyY[i]][enemyX[i]] = '&';
				}
			}			
		}	
	}
}

void initialization()
{
	width = 42;
	height = 24;

	x = width / 2;
	y = height / 2;

	score = 0;

	ammo = COUNT_OF_MOBS;
	stepsToSpawnAmmo = 55;
	randAmmoSpawnIn = rand() % 5 + stepsToSpawnAmmo;

	steps_to_spawnMobs = MAX_STEPS_SPAWN;
	steps = MAX_STEPS;

	arrowX = x;
	arrowY = y - 1;

	gameActive = true;
	bulletFLy = false;

	map = new char*[width];

	map_initialization();

	for (int i = 0; i < COUNT_OF_MOBS; i++)
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
	SetConsoleCursorPosition(wndHandle, { 0, 0 });

	for (int i = 0; i < height; i++)
	{
		std::cout << map[i];
		if (i == 0)
		{
			std::cout << " Kill all zombies to win!";
		}
		else if (i == 1)
		{
			std::cout << " Control: Arrows - walk";
		}
		else if (i == 2)
		{
			std::cout << "          Space - shoot";
		}
		else if (i == 3)
		{
			std::cout << "          WASD - direction to shoot";
		}
		std::cout << "\n";
	}

	std::cout << "Ammo: " << ammo << "  Kills: " << score << "  Enemies: " << enemyX.size() << "  Yaw: ";
	if (yaw == lookUP)
	{
		std::cout << "Up    \n";
	}
	else if (yaw == lookDOWN)
	{
		std::cout << "Down  \n";
	}
	else if (yaw == lookLEFT)
	{
		std::cout << "Left  \n";
	}
	else if (yaw == lookRIGHT)
	{
		std::cout << "Right \n";
	}

	std::cout << "Next step enemy will ";
	if (steps == 2)
	{
		std::cout << "stay\n";
	}
	else if (steps == 1)
	{
		std::cout << "go  \n";
	}

	if (ammo <= 0)
	{
		std::cout << "You need ammo, collect ammo ':'\n";
		showNeedAmmo = true;
	}
	if (ammo > 0 && showNeedAmmo)
	{
		std::cout << "                                ";
		showNeedAmmo = false;
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
	case 'a':
		yaw = lookLEFT;
		arrowX = x - 1;
		arrowY = y;
		break;
	case 'd':
		yaw = lookRIGHT;
		arrowX = x + 1;
		arrowY = y;
		break;
	case 'w':
		yaw = lookUP;
		arrowX = x;
		arrowY = y - 1;
		break;
	case 's':
		yaw = lookDOWN;
		arrowX = x;
		arrowY = y + 1;
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
			if (map[y - 1][x] != '#' && map[y - 1][x] != '&')
			{
				y--;
				steps_to_spawnMobs--;
				steps--;
				randAmmoSpawnIn--;
				yaw = lookUP;
				arrowX = x;
				arrowY = y - 1;
			}
			break;
		case DOWN:
			if (map[y + 1][x] != '#' && map[y + 1][x] != '&')
			{
				y++;
				steps_to_spawnMobs--;
				steps--;
				randAmmoSpawnIn--;
				yaw = lookDOWN;
				arrowX = x;
				arrowY = y + 1;
			}
			break;
		case LEFT:
			if (map[y][x - 1] != '#' && map[y][x - 1] != '&')
			{
				x--;
				steps_to_spawnMobs--;
				steps--;
				randAmmoSpawnIn--;
				yaw = lookLEFT;
				arrowX = x - 1;
				arrowY = y;
			}
			break;
		case RIGHT:
			if (map[y][x + 1] != '#' && map[y][x + 1] != '&')
			{
				x++;
				steps_to_spawnMobs--;
				steps--;
				randAmmoSpawnIn--;
				yaw = lookRIGHT;
				arrowX = x + 1;
				arrowY = y;
			}
			break;
		}
		key = NONE;
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
				switch (yaw)
				{
				case lookUP:
					bulletY--;
					break;
				case lookDOWN:
					bulletY++;
					break;
				case lookLEFT:
					bulletX--;
					break;
				case lookRIGHT:
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

					for (int i = 0; i < enemyX.size(); i++)
					{
						if (bulletX == enemyX[i] && bulletY == enemyY[i])
						{
							enemyX.erase(enemyX.begin() + i);
							enemyY.erase(enemyY.begin() + i);
							map_changes();
							Render();
							break;
						}
					}
				}

				map_changes();
				Render();
				Sleep(10);
			}
			bulletX = -1;
			bulletY = -1;
		}
		control = NOFIRE;
	}

	if (steps <= 0)
	{
		for (int i = 0; i < enemyX.size(); i++)
		{
			rangeX = enemyX[i] - x;
			rangeY = enemyY[i] - y;

			if (abs(rangeX) > abs(rangeY))
			{
				if (rangeX > rangeY && map[enemyY[i]][enemyX[i] - 1] == ' ' || map[enemyY[i]][enemyX[i] - 1] == '@' || enemyX[i] - 1 == arrowX)
				{
					enemyX[i]--;
				}
				else if (rangeX < rangeY && map[enemyY[i]][enemyX[i] + 1] == ' ' || map[enemyY[i]][enemyX[i] + 1] == '@' || enemyX[i] + 1 == arrowX)
				{
					enemyX[i]++;
				}
				else
				{
					if (rangeY > 0 && map[enemyY[i] + 1][enemyX[i]] == ' ' || map[enemyY[i] + 1][enemyX[i]] == '@' || enemyY[i] + 1 == arrowY)
					{
						enemyY[i]++;
					}
					else if (rangeY < 0 && map[enemyY[i] - 1][enemyX[i]] == ' ' || map[enemyY[i] - 1][enemyX[i]] == '@' || enemyY[i] - 1 == arrowY)
					{
						enemyY[i]--;
					}
				}
			}
			else if (abs(rangeX) < abs(rangeY))
			{
				if (rangeX < rangeY && map[enemyY[i] - 1][enemyX[i]] == ' ' || map[enemyY[i] - 1][enemyX[i]] == '@' || enemyY[i] - 1 == arrowY)
				{
					enemyY[i]--;
				}
				else if (rangeX > rangeY && map[enemyY[i] + 1][enemyX[i]] == ' ' || map[enemyY[i] + 1][enemyX[i]] == '@' || enemyY[i] + 1 == arrowY)
				{
					enemyY[i]++;
				}
				else
				{
					if (rangeX > 0 && map[enemyY[i]][enemyX[i] - 1] == ' ' || map[enemyY[i]][enemyX[i] - 1] == '@' || enemyX[i] - 1 == arrowX)
					{
						enemyX[i]--;
					}
					else if (rangeX < 0 && map[enemyY[i]][enemyX[i] + 1] == ' ' || map[enemyY[i]][enemyX[i] + 1] == '@' || enemyX[i] + 1 == arrowX)
					{
						enemyX[i]++;
					}
				}
			}
			else if (abs(rangeX) == abs(rangeY))
			{
				if (rangeX > 0 && rangeY < 0)
				{
					randMove = rand() % 1;
					if (randMove && map[enemyY[i]][enemyX[i] - 1] == ' ' || map[enemyY[i]][enemyX[i] - 1] == '@' || enemyX[i] - 1 == arrowX)
					{
						enemyX[i]--;
					}
					else if (map[enemyY[i] + 1][enemyX[i]] == ' ' || map[enemyY[i] + 1][enemyX[i]] == '@' || enemyY[i] + 1 == arrowY)
					{
						enemyY[i]++;
					}
				}
				else if (rangeX < 0 && rangeY > 0)
				{
					randMove = rand() % 1;
					if (randMove && map[enemyY[i]][enemyX[i] + 1] == ' ' || map[enemyY[i]][enemyX[i] + 1] == '@' || enemyX[i] + 1 == arrowX)
					{
						enemyX[i]++;
					}
					else if (map[enemyY[i] - 1][enemyX[i]] == ' ' || map[enemyY[i] - 1][enemyX[i]] == '@' || enemyY[i] - 1 == arrowY)
					{
						enemyY[i]--;
					}
				}
				else if (rangeX < 0 && rangeY < 0)
				{
					randMove = rand() % 1;
					if (randMove && map[enemyY[i]][enemyX[i] + 1] == ' ' || map[enemyY[i]][enemyX[i] + 1] == '@' || enemyX[i] + 1 == arrowX)
					{
						enemyX[i]++;
					}
					else if (map[enemyY[i] + 1][enemyX[i]] == ' ' || map[enemyY[i] + 1][enemyX[i]] == '@' || enemyY[i] + 1 == arrowY)
					{
						enemyY[i]++;
					}
				}
				else if (rangeX > 0 && rangeY > 0)
				{
					randMove = rand() % 1;
					if (randMove && map[enemyY[i]][enemyX[i] - 1] == ' ' || map[enemyY[i]][enemyX[i] - 1] == '@' || enemyX[i] - 1 == arrowX)
					{
						enemyX[i]--;
					}
					else if (map[enemyY[i] - 1][enemyX[i]] == ' ' || map[enemyY[i] - 1][enemyX[i]] == '@' || enemyY[i] - 1 == arrowY)
					{
						enemyY[i]--;
					}
				}
			}
			map[enemyY[i]][enemyX[i]] = '&';
		}

		steps = MAX_STEPS;
	}

	if (steps_to_spawnMobs <= 0)
	{
		do
		{
			do
			{
				randWidth = rand() % (width - 2) + 1;
				randHeight = rand() % (height - 2) + 1;

				rangeX = abs(randWidth - x);
				rangeY = abs(randHeight - y);
			} while (map[randHeight][randWidth] != ' ');
		} while (rangeX < 3 || rangeY < 3);

		enemyX.push_back(randWidth);
		enemyY.push_back(randHeight);

		steps_to_spawnMobs = MAX_STEPS_SPAWN;
	}

	if (randAmmoSpawnIn <= 0)
	{
		do
		{
			randWidth = rand() % (width - 2) + 1;
			randHeight = rand() % (height - 2) + 1;
		} while (map[randHeight][randWidth] != ' ');

		ammoX.push_back(randWidth);
		ammoY.push_back(randHeight);

		randAmmoSpawnIn = rand() % 5 + stepsToSpawnAmmo;
	}

	if (map[y][x] == ':')
	{
		randAmmoRange = enemyX.size() / (ammo + 3);
		randAmmo = rand() % 2 + (randAmmoRange + 3);
		ammo += randAmmo;

		for (int i = 0; i < ammoX.size(); i++)
		{
			if (x == ammoX[i] && y == ammoY[i])
			{
				ammoX.erase(ammoX.begin() + i);
				ammoY.erase(ammoY.begin() + i);
				break;
			}
		}
	}

	if (map[y][x] == '&')
	{
		map_changes();
		Render();
		std::cout << "< You died >\n";
		std::cout << "Would you like to play again? (y / n)\n";
		continueGame = true;
		while (continueGame)
		{
			std::cin >> YesNo;
			if (YesNo == "y" || YesNo == "yes" || YesNo == "Yes")
			{
				ammoX.clear();
				ammoY.clear();

				enemyX.clear();
				enemyY.clear();

				delete[] map;

				initialization();
				continueGame = false;

				system("cls");
			}
			else if (YesNo == "n" || YesNo == "no" || YesNo == "No")
			{
				gameActive = false;
				continueGame = false;
			}
			else
			{
				std::cout << "What?\n";
			}
		}		
	}

	if (enemyX.size() <= 0)
	{
		gameActive = false;
		std::cout << "< You won >\n";
	}
	
	map_changes();
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
	delete[] map;
	return 0;
}
