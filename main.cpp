#include <iostream> // cout, endl
#include <conio.h> // _kbhit() 
#include <Windows.h> // system(), Sleep()
#include <vector> // vector<> cend()
#include <cstdlib> // rand()

//SNEAK GAME ON CONSOLE

using namespace std;

//#define KEY_UP 72    //using arrow keys to control snake is under construction //construction site is abandoned
//#define KEY_RIGHT 77
//#define KEY_DOWN 80
//#define KEY_LEFT 75

const int width = 50; // size of game board
const int height = 25; // size of game board
unsigned short int  fruitX, fruitY; // to store coordinates of fruit
unsigned short int  score = 0; // to keep score
float speed = 60; // argument for Sleep() function (waiting time between iterations)
bool GameOver; // current state of game (flag)

vector<unsigned short  int> TailX; // to store x coordinates of tail parts 
vector<unsigned short  int> TailY; // to store y coordinates of tail parts

enum sDirection { START, LEFT, RIGHT, UP, DOWN }; // to store current move of the snake. enum bcuz we'll use in logic operations
sDirection state;

inline void fruitSpawner() // generates new location for the fruit (why inline?)
{
	for (unsigned short int i = 0; i < size(TailX); i++)
	{

		fruitX = rand() % 45 + 2; // returns a random x coordinates in range of map (0 - 47)
		fruitY = rand() % 20 + 2; // returns a random y coordinates in range of map (0 - 22)

		if (fruitX != TailX[i] || fruitY != TailY[i]) // the fruit is on the snake? if not you made it.
			break;
	}
}

void Init() // game initials
{
	GameOver = false;
	state = START;
	srand((unsigned int)time(NULL)); // inital random location for the fruit
	TailX.push_back(width / 2); // the snake always start from the middle of the board
	TailY.push_back(height / 2); // the snake always start from the middle of the board

	fruitSpawner();
}

void Draw() // draws the visuals like game board, the snake, directives and scoreboard
{
	system("cls"); // cleans console for every iteration. cause of the blinking/flickering problem
	register unsigned int i; // to draw vertical visuals
	register unsigned int j; // to draw horizontal visuals
	register unsigned int k; // to draw snake's visuals
	for (i = 0; i < height; i++) // iterate rows 
	{
		for (j = 0; j < width; j++) // iterate columns
		{
			bool print = false;
			if (i == 0 || i == height - 1)		// fill horizontal walls with '='
				cout << "=";					// ceiling and floor
			else if (j == 0 || j == width - 1) // fill vertical lines with 'H'
				cout << "H";
			else if (i == fruitY && j == fruitX) // fruit = '@'. coordinates of fruit created in FruitSpawner()
				cout << "@";
			else if (TailX[0] == j && TailY[0] == i) // tailX[0], tailY[0] means snake's head = 'O'
				cout << "O";
			else
			{
				for (k = 1; k < TailX.size(); k++) // iterate snake parts
				{
					if (TailX[k] == j && TailY[k] == i) // draw snake parts
					{
						print = true;
						cout << "o"; // for tail parts = 'o'
					}
				}
				if (!print)
					cout << " ";
			}
		}
		cout << endl;
	}
	cout << "Score: " << score << endl;
	cout << "To guide the snake use WASD keys" << endl;
	cout << "Use X to pause, use SPACE to end the game" << endl;
	cout << "Guide the snake wisely " << endl;
}

void Input() // to catch and process keyboard hits
{
	if (_kbhit()) // is there any keyboard hit
	{
		switch (_getch())
		{
		case 'a':
			state = LEFT; // match input with the movement
			break;
		case 'w':
			state = UP;
			break;
		case 'd':
			state = RIGHT;
			break;
		case 's':
			state = DOWN;
			break;
		case 'x':
			system("PAUSE"); // to pause the game
			break;
		case (char)32: //ascii code of space
			GameOver = true; // to end the game 
			break;
		}
	}
}


void Basics()
{
	int prevX = *(TailX.cend() - 1); // previous x position 
	int prevY = *(TailY.cend() - 1); // previous y position
	int i;

	for (i = TailX.size() - 1; i != 0; i--) // for moving each tail part to new position for x coordinate
		TailX[i] = TailX[i - 1];

	for (i = TailY.size() - 1; i != 0; i--) // for moving each tail part to new position for y coordinate
		TailY[i] = TailY[i - 1];

	switch (state)	// tailX[0], tailY[0] means snake's head
	{
	case LEFT:		// move snake's head according to Input()
		--TailX[0];
		break;

	case RIGHT:
		++TailX[0];
		break;

	case UP:
		--TailY[0];
		break;

	case DOWN:
		++TailY[0];
		break;

	default:
		break;
	}

	if (TailX[0] > width || TailX[0] < 0 || TailY[0]<0 || TailY[0]>height) // check wall collision
	{																	  // if the coordinates of the snake's head are greater than the game board's size, TryAgain
		system("cls");
		cout << "Game Over You Hit The Wall" << endl;
		cout << "Your score: " << score << endl;
		cout << "Try Again" << endl;
		GameOver = true; // change flag condition 
	}

	for (i = 1; i < TailX.size(); i++)									// check tail collision
		if (TailX[0] == TailX[i] && TailY[0] == TailY[i])				//  if the coordinates of the snake's head are same the any tail part of snake, TryAgain
		{
			system("cls");
			cout << "Game Over You Ate Yourself" << endl;
			cout << "Your score: " << score << endl;
			cout << "Try Again" << endl;
			GameOver = true; // change flag condition
		}
}
void Score() // Did you just eat a fruit?
{
	int prevX = *(TailX.cend() - 1);
	int prevY = *(TailY.cend() - 1);

	if (TailX[0] == fruitX && TailY[0] == fruitY)
	{
		score += 10;			// there is your 10 points
		fruitSpawner();			// new fruit has been spawned
		TailX.push_back(prevX); // add a new tail part
		TailY.push_back(prevY);
		if (speed > 10)
			speed -= 5;			// decreasing speed variable increases the speed of snake
		else if (speed >= 10)
			speed -= 1;
	}

}
int main()
{
	SetConsoleTitle(L"snaek");
	Init();
	while (!GameOver) // to prevent crashes
	{
		Draw(); // draw visuals
		if (state == UP || DOWN)
			Sleep(speed * 3); // reduces the speed effect caused by rectangular cells in the console
		else
			Sleep(speed); // refresh rate if we increase or decrease the speed of the snake will change
		Score(); //when snake eats a fruits score increases 10 points
		Input(); // catch keyboard inputs
		Basics(); // movement basics of the snake

	}
	system("pause");
	return 0;
}