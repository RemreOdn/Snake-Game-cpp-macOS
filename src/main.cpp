#include <iostream> // cout, endl
#include <vector> // vector<> cend()
#include <cstdlib> // rand()
#include <ncurses.h> // ncurses ile terminal kontrolü
#include <unistd.h>  // usleep() fonksiyonu için
#include <ctime>


constexpr int width = 50; // size of game board
constexpr int height = 25; // size of game board
unsigned short int fruitX, fruitY; // to store coordinates of fruit
unsigned short int score = 0; // to keep score
float speed = 60; // argument for Sleep() function (waiting time between iterations)
bool GameOver; // current state of game (flag)

std::vector<unsigned short int> TailX; // to store x coordinates of tail parts
std::vector<unsigned short int> TailY; // to store y coordinates of tail parts

enum sDirection { START, LEFT, RIGHT, UP, DOWN };

// to store current move of the snake. enum bcuz we'll use in logic operations
sDirection state; // TODO: make it NULL safe

inline void fruitSpawner() // generates new location for the fruit (why inline?)
{
    for (unsigned short int i = 0; i < TailX.size(); i++) {
        fruitX = rand() % 45 + 2; // returns a random x coordinates in range of map (0 - 47)
        fruitY = rand() % 20 + 2; // returns a random y coordinates in range of map (0 - 22)

        if (fruitX != TailX[i] || fruitY != TailY[i]) // the fruit is on the snake? if not you made it.
            break;
    }
}

void Init() {
    // game initials
    initscr(); // ncurses başlat
    noecho(); // Kullanıcının tuşları ekranda görünmez
    cbreak(); // raw modu gibi, ancak Ctrl+C gibi özel tuşları engellemiyor
    curs_set(0); // İmleci gizle
    keypad(stdscr, TRUE); // Ok tuşlarını kullanabiliriz
    timeout(100); // Non-blocking getch için

    GameOver = false;
    state = START;
    srand((unsigned int) time(NULL));
    TailX.push_back(width / 2);
    TailY.push_back(height / 2);
    fruitSpawner();
}

void clearScreen() {
    clear();
    refresh();
}

void Draw() // draws the visuals like game board, the snake, directives and scoreboard
{
    //TODO: rewrite this fuction
    clearScreen();
    // to draw horizontal visuals
    // to draw snake's visuals
    for (int i = 0; i < height; i++) // iterate rows
    {
        for (unsigned int j = 0; j < width; j++) // iterate columns
        {
            bool print = false;
            if (i == 0 || i == height - 1) // fill horizontal walls with '='
                printw("="); // ceiling and floor
            else if (j == 0 || j == width - 1) // fill vertical lines with 'H'
                printw("H");
            else if (i == fruitY && j == fruitX) // fruit = '@'. coordinates of fruit created in FruitSpawner()
                printw("@");
            else if (TailX[0] == j && TailY[0] == i) // tailX[0], tailY[0] means snake's head = 'O'
                printw("O");
            else {
                for (unsigned int k = 1; k < TailX.size(); k++) // iterate snake parts
                {
                    if (TailX[k] == j && TailY[k] == i) // draw snake parts
                    {
                        print = true;
                        printw("o"); // for tail parts = 'o'
                    }
                }
                if (!print)
                    printw(" ");
            }
        }
        printw("\n"); // Satır sonu (endl yerine)
    }
    printw("Score: %d\n", score);
    printw("To guide the snake use WASD keys\n");
    printw("Use X to pause, use SPACE to end the game\n");
    printw("Guide the snake wisely\n");

    refresh();
}

void Input() // Klavye girişlerini yakala ve işle
{
    //TODO: learn naming conventions
    int ch = getch(); // Kullanıcının bastığı tuşu oku

    if (ch != ERR) // Eğer bir tuşa basılmışsa
    {
        switch (ch) {
            case 'a':
                if (state != RIGHT) // Yılan ters yöne gidemez
                    state = LEFT;
                break;
            case 'w':
                if (state != DOWN)
                    state = UP;
                break;
            case 'd':
                if (state != LEFT) state = RIGHT;
                break;
            case 's':
                if (state != UP)
                    state = DOWN;
                break;
            case 'x': // Oyunu duraklat
                printw("Game Paused! Press any key to continue...");
                refresh();
                nodelay(stdscr, FALSE); // Blocking mode
                getch(); // Kullanıcı bir tuşa basana kadar bekle
                nodelay(stdscr, TRUE); // Non-blocking mode tekrar
                break;
            case ' ': // Space tuşu (oyunu bitir)
                GameOver = true;
                break;
        }
    }
}

void Basics() {
    int prevX = TailX.back(); // previous x position
    int prevY = TailY.back(); // previous y position
    int i;

    for (i = TailX.size() - 1; i != 0; i--) // for moving each tail part to new position for x coordinate
        TailX[i] = TailX[i - 1];

    for (i = TailY.size() - 1; i != 0; i--) // for moving each tail part to new position for y coordinate
        TailY[i] = TailY[i - 1];

    switch (state) // tailX[0], tailY[0] means snake's head
    {
        case LEFT: // move snake's head according to Input()
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

    if (TailX[0] >= width - 1 || TailX[0] <= 0 || TailY[0] <= 0 || TailY[0] >= height - 1) // check wall collision
    {
        // if the coordinates of the snake's head are greater than the game board's size, TryAgain
        clearScreen();
        printw("Game Over! You Hit The Wall\n");
        printw("Your score: %d\n", score);
        printw("Try Again\n");
        GameOver = true; // change flag condition
    }

    for (i = 1; i < TailX.size(); i++) // check tail collision
        if (TailX[0] == TailX[i] && TailY[0] == TailY[i])
        //  if the coordinates of the snake's head are same the any tail part of snake, TryAgain
        {
            clearScreen();
            printw("Game Over You Ate Yourself\n");
            printw("Your score: %d\n", score);
            printw("Try Again\n");
            GameOver = true; // change flag condition
        }
}

void Score() // Did you just eat a fruit?
{
    int prevX = TailX.back();
    int prevY = TailY.back();

    if (TailX[0] == fruitX && TailY[0] == fruitY) {
        score += 10; // there is your 10 points
        fruitSpawner(); // new fruit has been spawned
        TailX.push_back(prevX); // add a new tail part
        TailY.push_back(prevY);
        if (speed > 10)
            speed -= 5; // decreasing speed variable increases the speed of snake
        else if (speed >= 10)
            speed -= 1;
    }
}

int main() {
    Init();

    while (!GameOver) {
        Draw();
        Score();
        Input();
        Basics();
    }

    printw("Game Over! Press any key to exit...");
    refresh();
    nodelay(stdscr, FALSE); // Blocking mode
    getch(); // Çıkış için tuşa basmayı bekle
    endwin(); // ncurses modundan çık

    return 0;
}
