//Endless_mode.cpp
#include <iostream>
#include<Windows.h>
#include<conio.h>
#include<cstdlib>
#include"Endless_mode.h"
#include"Enemy.h"
#include"Basic.h"
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define WHITE "\033[37m"

using namespace std;
bool EndlessGameRun = true;



void drawNewScreen() {
	for (int y = 0; y < ScreenY; y++) {//先扫每一行
		if (y == 0)//第一行
		{
			for (int x = 0; x < ScreenX; x++) {//扫第一行每一格
				GameScreen[y][x] = '#';//在每一格画个#作为边界
			}
		}
		else//不是第一行
		{
			for (int x = 0; x < ScreenX; x++)//扫每一列
			{
				if (x == 0 || x == ScreenX - 1) {//在每一列第一格和最后一格画#
					GameScreen[y][x] = '#';
				}
				else
				{
					GameScreen[y][x] = ' ';//其他地方放空格
				}
			}
		}
	}
	GameScreen[BoatY][BoatX] = flyBoat::Aboat;
	for (int i = 0; i < Max_Bullet; i++)
	{
		if (Bulletpool[i].isActive == true)
		{
			if (Bulletpool[i].BulletX > 0 && Bulletpool[i].BulletX < ScreenX && Bulletpool[i].BulletY>0)//判断子弹在不在游戏范围内
			{
				GameScreen[Bulletpool[i].BulletY][Bulletpool[i].BulletX] = flyBullet::Bullet;//生成子弹
			}
		}
	}//这一个for循环是用来生成子弹的
	for (int i = 0; i < Max_Enemy; i++)
	{
		if (Enemypool[i].isActive == true)
		{
			if (Enemypool[i].EnemyX > 0 && Enemypool[i].EnemyX < ScreenX - 1&&Enemypool[i].EnemyY<ScreenY) {
				GameScreen[Enemypool[i].EnemyY][Enemypool[i].EnemyX] = Enemypool[i].EnemyDisplay;
			}
		}
	}//这一个for循环是生成敌人的
	for (int y = 0; y < ScreenY; y++) {
		for (int x = 0; x < ScreenX; x++) {
			bool hasEnemy = false;
			for (int i = 0; i < Max_Enemy; i++)
			{
				if (Enemypool[i].isActive && x == Enemypool[i].EnemyX && y == Enemypool[i].EnemyY) {
					hasEnemy = true;
					switch (Enemypool[i].type) {
					case EasyEnemy:
						cout << WHITE << Enemypool[i].EnemyDisplay << RESET;
						break;
					case NormalEnemy:
						cout << GREEN << Enemypool[i].EnemyDisplay << RESET;
						break;
					case HardEnemy:
						cout << YELLOW << Enemypool[i].EnemyDisplay << RESET;
						break;
					case DifficultEnemy:
						cout << RED << Enemypool[i].EnemyDisplay << RESET;
						break;
					case Wall1:
						cout << WHITE << Enemypool[i].EnemyDisplay << RESET;
						break;
					case Wall2:
						cout << GREEN << Enemypool[i].EnemyDisplay << RESET;
						break;
					case Wall3:
						cout << YELLOW << Enemypool[i].EnemyDisplay << RESET;
						break;
					default:
						cout << WHITE << Enemypool[i].EnemyDisplay << RESET;
						break;
					}
					break;//找到一个敌人就跳出循环
				}
			}
			if (!hasEnemy)
			{
				cout << GameScreen[y][x];
			}
		}
		cout << endl;  // 每行结束后换行

	}//这一个for循环是画出每一行的内容
	cout << "\nYour Life:" << life<<endl;
	cout << "Your Score:" << Score << endl;
	if (currentGameMode==Mode_Endless)
	{
		cout << "Time: " << gameTimeMinute << "min " << gameTimeSecond << "s" << endl;
	}
	cout << "A/D控制方向 Q退出游戏" << endl;

}//绘制新画面

void updateGame() {
	//设置一个帧计时器（计算哪一帧发射子弹）（已在basic.cpp中定义）
	BulletFPS++;//每一帧+1
	if (BulletFPS % 3 == 0)//每3帧就执行
	{
		for (int i = 0; i < Max_Bullet; i++) {//遍历子弹数组中每一个元素
			if (Bulletpool[i].isActive == false)
			{
				Bulletpool[i].isActive = true;
				Bulletpool[i].BulletX = BoatX;
				Bulletpool[i].BulletY = BoatY;
				break;
			}
		}
		BulletFPS = 0;//重置一下帧计时器
	}
	for (int i = 0; i < Max_Bullet; i++)
	{
		if (Bulletpool[i].isActive == true)
		{
			Bulletpool[i].BulletY--;
			if (Bulletpool[i].BulletY < 0)
			{
				Bulletpool[i].isActive = false;
			}
		}
	}
	moveEnemy();
	for (int i = 0; i < Max_Bullet; i++)
	{
		if (Bulletpool[i].isActive == true)
		{
			for (int j = 0; j < Max_Enemy; j++)
			{
				if (Enemypool[j].isActive == true && Enemypool[j].EnemyX == Bulletpool[i].BulletX && Enemypool[j].EnemyY == Bulletpool[i].BulletY) {
					switch (Enemypool[j].type) {
					case Wall1:
					case EasyEnemy:
						Enemypool[j].isActive = false;
						Score += 10;
						Bulletpool[i].isActive = false;
						break;
					case Wall2:
						Enemypool[j].type = Wall1;
						Score += 10;
						Bulletpool[i].isActive = false;
					case NormalEnemy:
						Enemypool[j].type = EasyEnemy;
						Score += 10;
						Bulletpool[i].isActive = false;
						break;
					case Wall3:
						Enemypool[j].type = Wall2;
						Score += 10;
						Bulletpool[i].isActive = false;
					case HardEnemy:
						Enemypool[j].type = NormalEnemy;
						Score += 10;
						Bulletpool[i].isActive = false;
						break;
					case DifficultEnemy:
						Enemypool[j].health--;
						if (Enemypool[j].health==0)
						{
							Score += 40;
							Bulletpool[i].isActive = false;
							Enemypool[j].isActive = false;
						}
						Bulletpool[i].isActive = false;
						break;
					}
					break;
				}
			}
		}
	}
	if (life == 0)
	{
		EndlessGameRun = false;
	}
	double currentTime = difftime(time(nullptr), gameTimeStart);
	gameTimeMinute = (int)currentTime / 60;
	gameTimeSecond = (int)currentTime % 60;
}//游戏状态更新

void keyboardMove() {
	if (_kbhit())
	{
		char key = _getch();
		switch (key)
		{
		case 'a':
		case 'A':
			BoatX -= 1;
			break;
		case 'd':
		case 'D':
			BoatX += 1;
			break;
		case 'q':
		case 'Q':
			EndlessGameRun = false;
			LevelGameRun = false;
			life = 0;
			break;
		}
	}
	if (BoatX >= ScreenX - 1)
	{
		BoatX = ScreenX - 2;
	}
	else if (BoatX < 1)
	{
		BoatX = 1;
	}
}

void EndlessMode() {
	currentGameMode = Mode_Endless;
	gameTimeStart = time(nullptr);//初始时间
	EndlessGameRun = true;
	while (EndlessGameRun) {

		RestartGame();
		EndlessGameRun = true;
		bool gameActive = true;
		while (life > 0 && gameActive)
		{
			cleanScreen();
			keyboardMove();
			drawNewScreen();
			Sleep(50);
			updateGame();
			if (!EndlessGameRun) {
				gameActive = false;
			}
		}
		if (life <= 0)
		{
			cleanScreen();
			cout << "YOU DIED" << endl;
			cout << "YOUR SCORE: " << Score << endl;
			cout << "按Q退出游戏" << endl;
			cout << "按R重新开始" << endl;
		}
		bool waitforInput = true;
		while (waitforInput)
		{
			if (_kbhit()) {
				char key = _getch();

				if (key == 'q' || key == 'Q')
				{
					EndlessGameRun = false;
					waitforInput = false;
				}
				else if (key == 'r' || key == 'R') {
					EndlessGameRun = true;
					break;
				}
			}
		}
	}
}
