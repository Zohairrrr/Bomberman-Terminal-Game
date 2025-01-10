#include <iostream>
#include <conio.h>
#include <ctime>
#include <windows.h> 
#include <iomanip>
#include <fstream>
#include <string>
#include <mmsystem.h>
using namespace std;
const int rows = 22;
const int cols = 38;
bool MultiplayerStatus;
int PlayerHealth = 4, Player2Health = 4;
char grid[rows][cols];

//=====================================================FUNCTIONS DECLARATIONS===================================================================================================================
void IsGameOver(int& PlayerHealth, int& BossHealth, bool& GameOver, int& NoEnemy, int& score, int& Player2Health);
void MakeGrid(char grid[rows][cols]);
void PrintGrid(char grid[rows][cols]);
void map(char grid[rows][cols], int& level);
void EnemyMotion(char grid[rows][cols], int EnemyX[], int EnemyY[], int& NoEnemy, int& PlayerHealth, int& BossHealth, bool& GameOver, int& score, int& Player2Health);
void BombExplosion(char grid[rows][cols], int& x, int& y, int& level, int EnemyX[], int EnemyY[], int& NoEnemy, int& PlayerHealth, int& BossHealth, bool& GameOver, int& Blocks, int& Bombs, int BombX, int BombY, int& score, bool& PowerUp, int& Player2Health);
void PlayerMotion(char grid[rows][cols], int& x, int& y, int& key, int& PlayerHealth);//manipulates the actual value of x and y at their address
void EnemyPlacement(char grid[rows][cols], int& level, int EnemyX[], int EnemyY[], int& NoEnemy, int& NoBoss);
void GameSpeed(int& level);
void GameStats(int& score, int& level, int& PlayerHealth, int& BossHealth, bool& PowerUp, int& Player2Health);
void Multiplayer();
void Player2Motion(char grid[rows][cols], int& x2, int& y2, int& key, int& Player2Health);
void Menu();
void Leaderboard();
void BossMovement(int& BossX, int& BossY, int& BossHealth, bool& GameOver, int& PlayerHealth, int& Player2Health);
void NewLeaderboard(int Score);
void FunChoice();
//================================================================================================================================================================================================

int EnemyX[7], EnemyY[7], NoEnemy = 0, NoBoss = 1;
int Blocks = 0;
int x = rows - 2, x2 = 1;
int y = cols / 2, y2 = 2;
int level = 1;
int score = 0;
int key;
int timer = 0;
int BossHealth = 6, Bombs = 0;
bool GameOver = false, IsBombActive = false, PowerUp = false;
bool BombActive1 = false, BombActive2 = false;
int BombX, BombY, BombX2, BombY2;
int BossX = 5, BossY = 1;
char choice;
bool flag = false;

//===============================================================================MAIN===============================================================================================
int main() {


    FunChoice();
    while (flag);
    // Initialize the grid and player position
    cout << "\033[?25l"; // Hide the cursor
    while (choice != '4') {
        switch (choice) {
        case '1':
            GameOver = false;
            MultiplayerStatus = false;
            system("cls");
            MakeGrid(grid);
            grid[x][y] = 'P';
            map(grid, level);
            EnemyPlacement(grid, level, EnemyX, EnemyY, NoEnemy, NoBoss);
            while (!GameOver) {

                if (GameOver == true) {
                    system("cls");
                    break;
                }
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0 });
                PrintGrid(grid);
                GameStats(score, level, PlayerHealth, BossHealth, PowerUp, Player2Health);
                EnemyMotion(grid, EnemyX, EnemyY, NoEnemy, PlayerHealth, BossHealth, GameOver, score, Player2Health);
                if (level == 4) {
                    BossMovement(BossX, BossY, BossHealth, GameOver, PlayerHealth, Player2Health);
                }
                // stats(score, level, IsGameOver);
                if (_kbhit()) {
                    key = _getch();
                    if (key == 0 || key == 224) {
                        key = _getch();
                        PlayerMotion(grid, x, y, key, PlayerHealth);
                    }
                    else if (key == 32) { // Space key for bomb
                        BombX = x;
                        BombY = y;
                        IsBombActive = true;
                        grid[BombX][BombY] = 'B';
                    }
                    if (IsBombActive) {
                        timer++;
                        if (timer >= 10) { // I counted the seconds, so 10 game iterations=3 seconds
                            BombExplosion(grid, x, y, level, EnemyX, EnemyY, NoEnemy, PlayerHealth, BossHealth, GameOver, Blocks, Bombs, BombX, BombY, score, PowerUp, Player2Health);
                            timer = 0;
                            grid[BombX][BombY] = ' ';
                            IsBombActive = false;
                        }
                    }
                }
                if (NoEnemy <= 0) {
                    //PlaySound(TEXT("clicking.wav"), NULL, SND_FILENAME | SND_ASYNC);
                    level++;
                    if (level > 4) {
                        cout << "You have defeated all the enemies adventurer!" << endl;
                        break;
                    }
                    if (level <= 4) {
                        if (level == 4) {
                            grid[BossX][BossY] = 'C';
                        }
                        system("cls");
                        cout << setw(35) << "\033[41m LEVEL  \033[0m" << " : " << level << endl;

                        Sleep(3000);
                        MakeGrid(grid);
                        map(grid, level);
                        EnemyPlacement(grid, level, EnemyX, EnemyY, NoEnemy, NoBoss);
                        x = rows - 2;
                        y = cols / 2;
                        grid[x][y] = 'P';
                    }
                }
                //controls game speed depending upon the level and the difficulty
                GameSpeed(level);
            }
            break;


        case '2':

            MultiplayerStatus = true;
            grid[x2][y2] = 'Q';
            Multiplayer();

            break;
        case '3':

            system("cls");


            Leaderboard();
            break;
        case '4':
            GameOver = false;
            //PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
            cout << "===== THANKS FOR PLAYING BOMBERMAN =====" << endl;
            cout << "        SEE YOU SOON AGAIN!             " << endl;

        }
        break;
    }

    system("pause");
    return 0;
}

//[MAIN ENDS]
// 
void FunChoice() {
    do {
        Menu();
        cin >> choice;
        if (choice != '1' && choice != '2' && choice != '3' && choice != '4') {
            flag = true;
            system("cls");
        }
        else {
            //PlaySound(TEXT("clicking.wav"), NULL, SND_FILENAME | SND_ASYNC);
            flag = false;
        }
    } while (flag);
}
//=====================================================================================================================================================================================
//====================================================================MOTION FUNCTIONS=================================================================================================
// controlls Enemy Motion
void GameSpeed(int& level) {
    if (level == 4) {
        Sleep(65);
    }
    else if (level == 3) {
        Sleep(75);
    }
    else if (level == 2) {
        Sleep(95);
    }
    else {
        Sleep(105);
    }
}
void EnemyPlacement(char grid[rows][cols], int& level, int EnemyX[], int EnemyY[], int& NoEnemy, int& NoBoss) {
    srand(time(0));
    int Ex, Ey;
    if (level == 1) {
        NoEnemy = 5;
        for (int i = 0;i < 5;i++) {
            Ex = rand() % (rows - 2) + 1;
            Ey = rand() % (cols - 2) + 1;
            if (grid[Ex][Ey] == ' ') {
                grid[Ex][Ey] = 'E';
                EnemyX[i] = Ex;
                EnemyY[i] = Ey;
            }
            else {
                i--;
                continue;
            }
        }
    }
    else if (level == 2) {
        NoEnemy = 6;
        for (int i = 0;i < 6;i++) {
            Ex = rand() % (rows - 2) + 1;
            Ey = rand() % (cols - 2) + 1;
            if (grid[Ex][Ey] == ' ') {
                grid[Ex][Ey] = 'E';
                EnemyX[i] = Ex;
                EnemyY[i] = Ey;
            }
            else {
                i--;
                continue;
            }
        }
    }
    else if (level == 3) {
        NoEnemy = 5;
        for (int i = 0;i < 5;i++) {
            Ex = rand() % (rows - 2) + 1;
            Ey = rand() % (cols - 2) + 1;
            if (grid[Ex][Ey] == ' ') {
                grid[Ex][Ey] = 'E';
                EnemyX[i] = Ex;
                EnemyY[i] = Ey;
            }
            else {
                i--;
                continue;
            }
        }
    }
    if (level == 4) {
        NoEnemy = 7;
        for (int i = 0;i < 7;i++) {
            Ex = rand() % (rows - 2) + 1;
            Ey = rand() % (cols - 2) + 1;
            if (grid[Ex][Ey] == ' ') {
                grid[Ex][Ey] = 'E';
                EnemyX[i] = Ex;
                EnemyY[i] = Ey;
            }
            else {
                i--;
                continue;
            }
        }
    }
}
void EnemyMotion(char grid[rows][cols], int EnemyX[], int EnemyY[], int& NoEnemy, int& PlayerHealth, int& BossHealth, bool& GameOver, int& score, int& Playerr2Health) {
    srand(time(0));
    for (int i = 0; i < NoEnemy; i++) {
        int direction = rand() % 4;
        int x = EnemyX[i], y = EnemyY[i];

        if (direction == 0 && grid[x - 1][y] != '#' && grid[x - 1][y] != '0' && grid[x - 1][y] != 'E' && grid[x - 1][y] != 'B' && grid[x - 1][y] != 'C') {//upwards
            if (grid[x - 1][y] == 'P') {
                //PlaySound(TEXT("touching.wav"), NULL, SND_FILENAME | SND_ASYNC);
                PlayerHealth--;
            }
            if (grid[x - 1][y] == 'Q') {
                //PlaySound(TEXT("touching.wav"), NULL, SND_FILENAME | SND_ASYNC);
                Player2Health--;
            }
            grid[x][y] = ' ';
            x--;


            grid[x][y] = 'E';

            EnemyX[i] = x;
            EnemyY[i] = y;
        }
        else if (direction == 1 && grid[x + 1][y] != '#' && grid[x + 1][y] != '0' && grid[x + 1][y] != 'E' && grid[x + 1][y] != 'B' && grid[x + 1][y] != 'C') { //downwards
            if (grid[x + 1][y] == 'P') {
                //PlaySound(TEXT("touching.wav"), NULL, SND_FILENAME | SND_ASYNC);
                PlayerHealth--;
            }
            if (grid[x + 1][y] == 'Q') {
                //PlaySound(TEXT("touching.wav"), NULL, SND_FILENAME | SND_ASYNC);
                Player2Health--;
            }
            grid[x][y] = ' ';
            x++;
            grid[x][y] = 'E';

            EnemyX[i] = x;
            EnemyY[i] = y;
        }
        else if (direction == 2 && grid[x][y - 1] != '#' && grid[x][y - 1] != '0' && grid[x][y - 1] != 'E' && grid[x][y - 1] != 'B' && grid[x][y - 1] != 'C') { //left
            if (grid[x][y - 1] == 'P') {
                //PlaySound(TEXT("touching.wav"), NULL, SND_FILENAME | SND_ASYNC);
                PlayerHealth--;
            }
            if (grid[x][y - 1] == 'Q') {
                //PlaySound(TEXT("touching.wav"), NULL, SND_FILENAME | SND_ASYNC);
                Player2Health--;
            }
            grid[x][y] = ' ';
            y--;
            grid[x][y] = 'E';
            EnemyX[i] = x;
            EnemyY[i] = y;
        }
        else if (direction == 3 && grid[x][y + 1] != '#' && grid[x][y + 1] != '0' && grid[x][y + 1] != 'E' && grid[x][y + 1] != 'B' && grid[x][y + 1] != 'C') { //down
            if (grid[x][y + 1] == 'P') {
                //PlaySound(TEXT("touching.wav"), NULL, SND_FILENAME | SND_ASYNC);
                PlayerHealth--;
            }
            if (grid[x][y + 1] == 'Q') {
                //PlaySound(TEXT("touching.wav"), NULL, SND_FILENAME | SND_ASYNC);
                Player2Health--;
            }
            grid[x][y] = ' ';
            y++;
            grid[x][y] = 'E';
            EnemyX[i] = x;
            EnemyY[i] = y;
        }
        IsGameOver(PlayerHealth, BossHealth, GameOver, NoEnemy, score, Player2Health);
    }
}
void BossMovement(int& BossX, int& BossY, int& BossHealth, bool& GameOver, int& PlayerHealth, int& Player2Health) {
    int direction = rand() % 4;
    int x = BossX, y = BossY;
    if (direction == 0 && grid[x - 1][y] != '#' && grid[x - 1][y] != '0' && grid[x - 1][y] != 'E' && grid[x - 1][y] != 'B' && grid[x - 1][y] != 'C') {//upwards
        if (grid[x - 1][y] == 'P') {
            PlayerHealth -= 2;
        }
        if (grid[x - 1][y] == 'Q') {
            Player2Health -= 2;
        }
        grid[x][y] = ' ';
        x--;


        grid[x][y] = 'C';

        BossX = x;
        BossY = y;
    }
    else if (direction == 1 && grid[x + 1][y] != '#' && grid[x + 1][y] != '0' && grid[x + 1][y] != 'E' && grid[x + 1][y] != 'B' && grid[x + 1][y] != 'C') { //downwards
        if (grid[x + 1][y] == 'P') {

            PlayerHealth -= 2;
        }
        if (grid[x + 1][y] == 'Q') {

            Player2Health -= 2;
        }
        grid[x][y] = ' ';
        x++;
        grid[x][y] = 'C';

        BossX = x;
        BossY = y;
    }
    else if (direction == 2 && grid[x][y - 1] != '#' && grid[x][y - 1] != '0' && grid[x][y - 1] != 'E' && grid[x][y - 1] != 'B' && grid[x][y - 1] != 'C') { //left
        if (grid[x][y - 1] == 'P') {

            PlayerHealth -= 2;
        }
        if (grid[x][y - 1] == 'Q') {

            Player2Health -= 2;
        }
        grid[x][y] = ' ';
        y--;
        grid[x][y] = 'C';
        BossX = x;
        BossY = y;
    }
    else if (direction == 3 && grid[x][y + 1] != '#' && grid[x][y + 1] != '0' && grid[x][y + 1] != 'E' && grid[x][y + 1] != 'B' && grid[x][y + 1] != 'C') { //down
        if (grid[x][y + 1] == 'P') {

            PlayerHealth -= 2;
        }
        if (grid[x][y + 1] == 'Q') {

            Player2Health -= 2;
        }
        grid[x][y] = ' ';
        y++;
        grid[x][y] = 'C';
        BossX = x;
        BossY = y;
    }
    IsGameOver(PlayerHealth, BossHealth, GameOver, NoEnemy, score, Player2Health);
}
















//Controls player's motion [START]
void PlayerMotion(char grid[rows][cols], int& x, int& y, int& key, int& PlayerHealth) {
    switch (key) {
    case 72:   // moves upwards
        if (grid[x - 1][y] == 'E') {
            PlayerHealth--;
        }
        if (grid[x - 1][y] != '#' && grid[x - 1][y] != '0' && grid[x - 1][y] != 'E' && grid[x - 1][y] != 'Q') { //ensures player doesn,t go throught the boundary
            if (grid[x][y] == 'B') {
                grid[x][y] = 'B';
            }
            else {
                grid[x][y] = ' ';
            }
            x--;
            grid[x][y] = 'P';
        }
        break;
    case 80:  // moves downwards
        if (grid[x + 1][y] == 'E') {
            PlayerHealth--;
        }
        if (grid[x + 1][y] != '#' && grid[x + 1][y] != '0' && grid[x + 1][y] != 'E' && grid[x + 1][y] != 'Q') {
            if (grid[x][y] == 'B') {
                grid[x][y] = 'B';
            }
            else {
                grid[x][y] = ' ';
            }
            x++;
            grid[x][y] = 'P';
        }
        break;
    case 75:  //moves left
        if (grid[x][y - 1] == 'E') {
            PlayerHealth--;
        }
        if (grid[x][y - 1] != '#' && grid[x][y - 1] != '0' && grid[x][y - 1] != 'E' && grid[x][y - 1] != 'Q') {
            if (grid[x][y] == 'B') {
                grid[x][y] = 'B';
            }
            else {
                grid[x][y] = ' ';
            }
            y--;
            grid[x][y] = 'P';
        }
        break;
    case 77:  // moves right
        if (grid[x][y + 1] == 'E') {
            PlayerHealth--;
        }
        if (grid[x][y + 1] != '#' && grid[x][y + 1] != '0' && grid[x][y + 1] != 'E' && grid[x][y + 1] != 'Q') {
            if (grid[x][y] == 'B') {
                grid[x][y] = 'B';
            }
            else {
                grid[x][y] = ' ';
            }
            y++;
            grid[x][y] = 'P';
        }
        break;
    default:

        break;
    }
    //PlaySound(TEXT("beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
    return;
}
//[END]
//========================================================================================================================================================================================================================
//=============================================================================GAME BOARD FUNCTIONS=======================================================================================================================
void MakeGrid(char grid[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                grid[i][j] = '#';
            }
            else {
                grid[i][j] = ' ';
            }
        }
    }

}
void PrintGrid(char grid[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            //cout << grid[i][j]; //changes the appearance of the game. From colours to characters
            //if (grid[i][j] == 'X') {
            //    Sleep(1);
            //    grid[i][j] = ' ';
            //}
            if (grid[i][j] == '#') {
                cout << "\033[107m  \033[0m";
            }
            else if (grid[i][j] == '0') {
                cout << "\033[100m  \033[0m";
            }
            else if (grid[i][j] == 'B') {
                cout << "\033[41m  \033[0m";
            }
            else if (grid[i][j] == 'E') {
                cout << "\033[45m  \033[0m";
            }
            else if (grid[i][j] == ' ') {
                cout << "\033[40m  \033[0m";
            }

            else if (grid[i][j] == 'Q' || grid[i][j] == 'P' || grid[i][j] == 'C') { // same colours to confuse the player hence more difficulty
                cout << "\033[44m  \033[0m";
            }
            else if (grid[i][j] == 'X') {
                cout << "\033[48;5;214m  \033[0m";

                Sleep(1);
                grid[i][j] = ' ';

            }

        }
        cout << endl;
    }
}
void map(char grid[rows][cols], int& level) {

    if (level == 1) {

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == ' ') { // Only place obstacles in empty cells
                    if (j % 2 == 0 && i % 2 == 0 && i != 0 && j != 0 && i != rows - 1 && j != cols - 1) {
                        grid[i][j] = '0';
                    }

                }
            }
        }
    }
    else if (level == 2) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if ((j == 3 && i > 3 && i < 19) || (j == 5 && i > 3 && i < 19) || (j == 35 && i > 3 && i < 19) || (j == 33 && i > 3 && i < 19) && grid[i][j] != 'P' && grid[i][j] != 'Q' && i != 0 && i != rows - 1) {
                    grid[i][j] = '#';
                }
                else if ((j > 5 && j < 33) && j % 2 == 0 && (i > 3 && i < 19) && i % 2 == 0 && i != 0 && j != 0 && i != rows - 1 && j != cols - 1) {
                    grid[i][j] = '0';
                }
            }
        }
    }
    else if (level == 3) {

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // Create an empty center
                if (i >= rows / 4 && i < rows * 3 / 4 && j >= cols / 4 && j < cols * 3 / 4 && grid[i][j] == ' ') {
                    grid[i][j] = ' '; // Empty space
                }

                else if (i % 2 == 0) {

                    int num1 = rand() % 2;

                    int num2 = rand() % 2;
                    if (num1 == 0 && num2 == 0 && grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                        grid[i][j] = '#';
                    }
                    else if (num1 == 1 && num2 == 0 && grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                        grid[i][j] = '#';
                    }
                    else if (num1 == 0 && num2 == 1 && grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                        grid[i][j] = '0';
                    }
                    else if (num1 == 1 && num2 == 1 && grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                        grid[i][j] = '0';
                    }
                }
                else if (grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                    grid[i][j] = ' ';
                }
            }
        }
    }
    else if (level == 4) {

        for (int i = 1;i < rows - 1;i++) {
            for (int j = 1;j < cols - 1;j++) {
                if (i >= 8 && i <= 18 && j >= 13 && j <= 22) {
                    grid[i][j] = ' ';
                }
                else {
                    if (i % 2 != 0) {
                        int num1 = rand() % 2;
                        int num2 = rand() % 2;
                        if (num1 == 0 && num2 == 0 && grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                            grid[i][j] = '#';
                        }
                        else if (num1 == 1 && num2 == 0 && grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                            grid[i][j] = '#';
                        }
                        else if (num1 == 0 && num2 == 1 && grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                            grid[i][j] = '0';
                        }
                        else if (num1 == 1 && num2 == 1 && grid[i][j] != '#' && grid[i][j] != '0' && grid[i][j] != 'E' && grid[i][j] != 'P' && grid[i][j] != 'Q') {
                            grid[i][j] = '0';
                        }
                    }

                }
            }
        }
    }
    PrintGrid(grid);
}
void IsGameOver(int& PlayerHealth, int& BossHealth, bool& GameOver, int& NoEnemy, int& score, int& Player2Health) {
    if (PlayerHealth <= 0 || Player2Health <= 0) {
        system("cls");
        cout << "YOU LOSE!" << endl;
        Sleep(500);
        NewLeaderboard(score);
        GameOver = true;
    }
    if (level == 4) {
        if (BossHealth <= 0) {
            system("cls");
            cout << "YOU WON! HURRAY!" << endl;
            Sleep(500);
            NewLeaderboard(score);
            GameOver = true;
            FunChoice();
        }
    }
    else {
        if (NoEnemy <= 0) {
            system("cls");
            cout << "YOU WON! HURRAY!" << endl;
            Sleep(500);
            NewLeaderboard(score);
            GameOver = true;
            FunChoice();
        }
    }
}
//========================================================================================================================================================================================
void BombExplosion(char grid[rows][cols], int& x, int& y, int& level, int EnemyX[], int EnemyY[], int& NoEnemy, int& PlayerHealth, int& BossHealth, bool& GameOver, int& Blocks, int& Bombs, int BombX, int BombY, int& score, bool& PowerUp, int& Player2Health) {
    // Explosion radius in square shape

    int radius, Power;
    switch (level) {
    case 1:
        Bombs = 4;
        Power = 24;
        radius = 2;
        break;
    case 2:
        Bombs = 5;
        radius = 2;
        Power = 34;
        break;
    case 3:
        Bombs = 5;
        radius = 3;
        Power = 30;
        break;
    case 4:

        Bombs = 6;
        radius = 4;
        Power = 36;
        break;
    }
    if (Bombs > 0) {

        for (int i = BombX - radius; i <= BombX + radius; i++) {
            for (int j = BombY - radius; j <= BombY + radius; j++) {
                if (i >= 0 && i < rows && j >= 0 && j < cols) {
                    if (grid[i][j] != '#') {

                        if (grid[i][j] == 'P') {

                            PlayerHealth--;

                        }
                        if (grid[i][j] == ' ') {
                            grid[i][j] = 'X';
                        }
                        if (grid[i][j] == 'Q') {

                            Player2Health--;
                        }

                        if (grid[i][j] == 'E') {
                            score += 50;
                        }

                        if (grid[i][j] == '0') {
                            grid[i][j] = 'X';
                            score += 10;
                            Blocks++;
                        }
                        if (grid[i][j] == 'C') {
                            BossHealth--;
                            score += 90;
                            if (BossHealth <= 0) {
                                grid[i][j] = ' ';
                                IsGameOver(PlayerHealth, BossHealth, GameOver, NoEnemy, score, Player2Health);
                            }
                        }

                        if (grid[i][j] == 'E' || grid[i][j] == 'C') {
                            for (int k = 0; k < NoEnemy; k++) {
                                if (EnemyX[k] == i && EnemyY[k] == j && grid[i][j] == 'E') {
                                    EnemyX[k] = EnemyX[NoEnemy - 1];
                                    EnemyY[k] = EnemyY[NoEnemy - 1];
                                    grid[i][j] = 'X';
                                    NoEnemy--;
                                }
                            }
                        }
                    }



                }



            }


        }
        //PlaySound(TEXT("bombexplosion.wav"), NULL, SND_FILENAME | SND_ASYNC);
        if (Blocks >= Power) {
            Player2Health++;
            PlayerHealth++;
            Bombs++;
            score += 20;
            Blocks = 0;
            PowerUp = true;
        }
    }
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0 });
    grid[x][y] = 'P';
}
//[END]
void GameStats(int& score, int& level, int& PlayerHealth, int& BossHealth, bool& PowerUp, int& Player2Health) {

    if (MultiplayerStatus) {
        cout << "\033[44m LEVEL:  \033[0m" << level << endl;
        cout << "\033[45m SCORE: \033[0m" << score << endl;
        cout << "\033[42mPlayer 1 Health: \033[0m" << PlayerHealth << endl;
        cout << "\033[42mPlayer 1 Health: \033[0m" << Player2Health << endl;
    }
    else {
        cout << "\033[44m LEVEL:  \033[0m" << level << endl;
        cout << "\033[45m SCORE: \033[0m" << score << endl;
        cout << "\033[42m Health: \033[0m" << PlayerHealth << endl;
    }
    if (level == 4) {
        cout << "BOSS HEALTH " << BossHealth << endl;
    }
    if (PowerUp) {
        cout << setw(105) << "\033[42m" << "POWER UP" << "\033[0m" << endl;
        Sleep(10);
        PowerUp = false;
    }
}
//========================================================================Multiplayer==============================================================================================================
void Multiplayer() {
    system("cls");
    MakeGrid(grid);
    grid[x2][y2] = 'Q';
    grid[x][y] = 'P';
    map(grid, level);
    EnemyPlacement(grid, level, EnemyX, EnemyY, NoEnemy, NoBoss);
    while (!GameOver) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0 });
        PrintGrid(grid);
        GameStats(score, level, PlayerHealth, BossHealth, PowerUp, Player2Health);
        EnemyMotion(grid, EnemyX, EnemyY, NoEnemy, PlayerHealth, BossHealth, GameOver, score, Player2Health);
        if (level == 4) {
            BossMovement(BossX, BossY, BossHealth, GameOver, PlayerHealth, Player2Health);
        }
        // stats(score, level, IsGameOver);
        if (_kbhit()) {
            key = _getch();

            if (key == 0 || key == 224) {
                key = _getch();
                PlayerMotion(grid, x, y, key, PlayerHealth);
            }
            else if (key == 119 || key == 87 || key == 97 || key == 65 ||
                key == 115 || key == 83 || key == 100 || key == 68 ||
                key == 102 || key == 70) {
                Player2Motion(grid, x2, y2, key, Player2Health);
            }
            else if (key == 98 || key == 66) { //B or b for player 2 for bomb
                BombX2 = x2;
                BombY2 = y2;
                BombActive2 = true;
                grid[BombX2][BombY2] = 'B';
            }
            if (BombActive2) {
                timer++;
                if (timer >= 10) { // I counted the seconds, so 10 game iterations=3 seconds
                    BombExplosion(grid, x, y, level, EnemyX, EnemyY, NoEnemy, PlayerHealth, BossHealth, GameOver, Blocks, Bombs, BombX2, BombY2, score, PowerUp, Player2Health);
                    timer = 0;
                    grid[BombX2][BombY2] = ' ';
                    BombActive2 = false;

                }
            }
            else if (key == 32) { // Space key for bomb
                BombX = x;
                BombY = y;
                BombActive1 = true;

                grid[BombX][BombY] = 'B';

            }
            if (BombActive1) {
                timer++;
                if (timer >= 10) { // I counted the seconds, so 10 game iterations=3 seconds
                    BombExplosion(grid, x, y, level, EnemyX, EnemyY, NoEnemy, PlayerHealth, BossHealth, GameOver, Blocks, Bombs, BombX, BombY, score, PowerUp, Player2Health);
                    timer = 0;
                    grid[BombX][BombY] = ' ';
                    BombActive1 = false;
                }
            }
        }
        GameSpeed(level);
    }
    if (NoEnemy <= 0) {
        //PlaySound(TEXT("clicking.wav"), NULL, SND_FILENAME | SND_ASYNC);
        level++;
        if (level > 4) {
            cout << "You have defeated all the enemies adventurer!" << endl;

        }
        if (level <= 4) {
            if (level == 4) {
                grid[BossX][BossY] = 'C';
            }
            system("cls");
            cout << setw(35) << "\033[41m LEVEL  \033[0m" << " : " << level << endl;

            Sleep(3000);
            MakeGrid(grid);
            map(grid, level);
            EnemyPlacement(grid, level, EnemyX, EnemyY, NoEnemy, NoBoss);
            x = rows - 2;
            y = cols / 2;
            grid[x][y] = 'P';
            grid[x2][y2] = 'Q';
        }
    }
    //controls game speed depending upon the level and the difficulty
}
void Player2Motion(char grid[rows][cols], int& x2, int& y2, int& key, int& Player2Health) {
    if (key == 119 || key == 87) {
        if (grid[x2 - 1][y2] == 'E') {
            Player2Health--;
        }
        if (grid[x2 - 1][y2] == ' ') {
            if (grid[x2][y2] == 'B') {
                grid[x2][y2] = 'B';
            }
            else {
                grid[x2][y2] = ' ';
            }
            x2--;
            grid[x2][y2] = 'Q';
        }
    }
    if (key == 115 || key == 83) {

        if (grid[x2 + 1][y2] == 'E') {
            Player2Health--;
        }
        if (grid[x2 + 1][y2] == ' ') {
            if (grid[x2][y2] == 'B') {
                grid[x2][y2] = 'B';
            }
            else {
                grid[x2][y2] = ' ';
            }

            x2++;
            grid[x2][y2] = 'Q';
        }
    }
    if (key == 97 || key == 65) {

        if (grid[x2][y2 - 1] == 'E') {
            Player2Health--;
        }
        if (grid[x2][y2 - 1] == ' ') {
            if (grid[x2][y2] == 'B') {
                grid[x2][y2] = 'B';
            }
            else {
                grid[x2][y2] = ' ';
            }
            y2--;
            grid[x2][y2] = 'Q';
        }
    }
    if (key == 100 || key == 68) {

        if (grid[x2][y2 + 1] == 'E') {
            Player2Health--;
        }
        if (grid[x2][y2 + 1] == ' ') {
            if (grid[x2][y2] == 'B') {
                grid[x2][y2] = 'B';
            }
            else {
                grid[x2][y2] = ' ';
            }
            y2++;
            grid[x2][y2] = 'Q';
        }
    }
    else {
        return;
    }
    //PlaySound(TEXT("beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
//=========================================================================================MENU==============================================================================================
void Menu() {
    cout << "\033[?25l";
    //PlaySound(TEXT("intro.wav"), NULL, SND_FILENAME | SND_ASYNC);
    system("cls");
    level = 1;
    GameOver = false;
    cout << setw(35) << "\033[42m\033[30m====================Bomberman Game =====================\033[0m\n";
    cout << endl;

    cout << setw(35) << "\033[44m\033[33m 1. Single Player\033[0m\n";
    cout << endl;
    cout << setw(35) << "\033[44m\033[33m 2. Multiplayer\033[0m\n";
    cout << endl;
    cout << setw(35) << "\033[44m\033[33m 3. Leaderboard\033[0m\n";
    cout << endl;
    cout << setw(35) << "\033[44m\033[33m 4. Exit\033[0m\n";
    cout << endl;
    cout << setw(35) << "Enter your choice: ";
    Sleep(1000);
}
void Leaderboard() {
    //PlaySound(TEXT("clicking.wav"), NULL, SND_FILENAME | SND_ASYNC);
    ifstream file("leaderboardscore3.txt");
    int scores[3] = { 0 };
    int score, count = 0;


    while (file >> score && count < 3) {
        scores[count] = score;
        count++;
    }


    cout << "===== Leaderboard =====" << endl;
    cout << "Top 3 Scores:" << endl;
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << scores[i] << endl;

    }
    cout << endl;
}

void NewLeaderboard(int Score) {
    int scores[3] = { 0 };
    int score, count = 0;
    ifstream file("leaderboardscore3.txt");


    if (file.is_open()) {
        while (file >> score && count < 3) {
            scores[count] = score;
            count++;
        }
        file.close();
    }


    if (count < 3) {
        scores[count] = Score;
        count++;
    }
    else {

        if (Score > scores[2]) {
            scores[2] = Score;
        }
    }


    for (int i = 1; i < count; i++) {
        int key = scores[i];
        int j = i - 1;
        while (j >= 0 && scores[j] < key) {
            scores[j + 1] = scores[j];
            j = j - 1;
        }
        scores[j + 1] = key;
    }


    ofstream outFile("leaderboardscore3.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < count; i++) {
            outFile << scores[i] << "\n";
        }
        outFile.close();
    }
    else {
        cout << "Failed to open the leaderboard file for writing.\n";
    }
}
//===============================================================================================================================================================
//===============================================================================================================================================================
//===============================================================================================================================================================