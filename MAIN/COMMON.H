#ifndef COMMON_H
#define COMMON_H

#include <dos.h>
#include <string.h>
#include <malloc.h>
#include <graph.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <bios.h>

#define pi 3.14159265358

#define ESC     1
#define UP      72
#define DOWN    80
#define LEFT    75
#define RIGHT   77
#define FIRE    29
#define SPEED   54
#define USE     57
#define SIT     56
#define MAP     15
#define SLEFT   51
#define SRIGHT  52
#define REFRESH 82
#define WEAPON0 11
#define WEAPON1 2
#define WEAPON2 3
#define WEAPON3 4
#define WEAPON4 5

struct SData
{
 unsigned char *WallTexture1;
 unsigned char *WallTexture2;
 unsigned char *WallTexture3;
 unsigned char *WallTexture4;
 unsigned char *WallTexture5;
 unsigned char *WallTexture6;
 unsigned char *WallTexture7;
 unsigned char *WallTexture8;

 unsigned char *Bar;
 unsigned char *Weapon0;
 unsigned char *Weapon1;
 unsigned char *Weapon2;
 unsigned char *Weapon3;
 unsigned char *Weapon4;
 unsigned char *Font;
 unsigned char *Menu;

 unsigned char *Fire;
 unsigned char *Thing;
 unsigned char *Alien;
};
struct SPlayer
{
 int LoadPos;//позиции в меню сохранения и загрузки
 int SavePos;
 float X;//координаты игрока
 float Y;
 float Z;
 int P_X;
 int P_Y;
 int Life;
 int Angle;//угол поворота игрока
 float Dz;//приращение высоты при ходьбе
 float PV;//скорость падения
 float Pz;//качающаяся походка
 int AutoMap;//режим карты
 int D_Angle;//приращение угла поворота
 int SitZ;//изменение высоты при приседании
 int RedScreen;//число циклов красного экрана
 char RedScreenOn;//есть ли красный экран
 int TimeSpacesuit;//время существования скафандра
 int TimeMegaSphere;//время работы мегасферы
 int IDDQD;//бессмертие
 int MessageNo;//номер последнего сообщения
 int MessageTime;//время существования сообщения
 int Level;//номер уровня
 int Inerci;//инерция движения игрока
 int FPS;//кадры в секунду
};
struct SWeapon
{
 int Fire_Faze;//фаза оружия
 int WeaponNo;//номер оружия
 int Weapon;//номер оружия на которое меняем текущее
 int Ammo[10];//патроны к оружию
 int MaxAmmo[10];//максимально патронов к оружию
 int Attendance[10];//наличие оружия
 int Faze[10][50];//фазы анимации оружия([номер оружия][номер фазы])(-1=конец)
 int X[6][50];//координаты вывода фазы оружия
 int Y[6][50];//координаты вывода фазы оружия
 int Dy;//приращение при убирании-поднимании оружия
 int Vy;//координата при убирании оружия
};
struct SMaze//данные о лабиринте и таблицы горизонтального текстурирования
{
 short *BlockMap[100][100];
 int dcTable[361];
 int dsTable[361];
 int diTable[361];
 int djTable[361];
};
struct SZLines//линии буффера глубины
{
 short Pointer;
 short Begin_Y[170];
 short End_Y[170];
 int Z_Value[170];
};
struct SMouse
{
 int HSpeed;
 int VSpeed;
 int Double;
};

#endif