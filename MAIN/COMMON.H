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
 int LoadPos;//����樨 � ���� ��࠭���� � ����㧪�
 int SavePos;
 float X;//���न���� ��ப�
 float Y;
 float Z;
 int P_X;
 int P_Y;
 int Life;
 int Angle;//㣮� ������ ��ப�
 float Dz;//���饭�� ����� �� 室졥
 float PV;//᪮���� �������
 float Pz;//�������� ��室��
 int AutoMap;//०�� �����
 int D_Angle;//���饭�� 㣫� ������
 int SitZ;//��������� ����� �� ��ᥤ����
 int RedScreen;//�᫮ 横��� ��᭮�� ��࠭�
 char RedScreenOn;//���� �� ���� ��࠭
 int TimeSpacesuit;//�६� ����⢮����� ᪠䠭��
 int TimeMegaSphere;//�६� ࠡ��� ��������
 int IDDQD;//���ᬥ�⨥
 int MessageNo;//����� ��᫥����� ᮮ�饭��
 int MessageTime;//�६� ����⢮����� ᮮ�饭��
 int Level;//����� �஢��
 int Inerci;//������ �������� ��ப�
 int FPS;//����� � ᥪ㭤�
};
struct SWeapon
{
 int Fire_Faze;//䠧� ��㦨�
 int WeaponNo;//����� ��㦨�
 int Weapon;//����� ��㦨� �� ���஥ ���塞 ⥪�饥
 int Ammo[10];//���஭� � ��㦨�
 int MaxAmmo[10];//���ᨬ��쭮 ���஭�� � ��㦨�
 int Attendance[10];//����稥 ��㦨�
 int Faze[10][50];//䠧� �����樨 ��㦨�([����� ��㦨�][����� 䠧�])(-1=�����)
 int X[6][50];//���न���� �뢮�� 䠧� ��㦨�
 int Y[6][50];//���न���� �뢮�� 䠧� ��㦨�
 int Dy;//���饭�� �� 㡨࠭��-���������� ��㦨�
 int Vy;//���न��� �� 㡨࠭�� ��㦨�
};
struct SMaze//����� � ����ਭ� � ⠡���� ��ਧ��⠫쭮�� ⥪����஢����
{
 short *BlockMap[100][100];
 int dcTable[361];
 int dsTable[361];
 int diTable[361];
 int djTable[361];
};
struct SZLines//����� ����� ��㡨��
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