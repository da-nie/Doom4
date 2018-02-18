#ifndef CONTROL_H
#define CONTROL_H

#include "common.h"
#include "classes.h"
#include "loading.h"
#include "classes.h"
#include "timer.h"
#include "special.h"
#include "automap.h"
#include "3dengine.h"
#include "keyboard.h"
#include "mouse.h"
#include "sprites.h"

extern char KeyTable[255];//таблица состояния клавиш
extern CKeyboard cKeyboard;
extern SMaze sMaze;
extern SWeapon sWeapon;
extern SSegment *sSegment;
extern SSector *sSector;

extern int TimerUser;
extern SInfo sInfo;
extern CThing* cThing[500];//таблица указателей на объекты
extern int NowThing;//всего существует объектов на данный момент
extern CAlien* cAlien[500];//таблица указателей на врагов
extern int NowAlien;//всего существует врагов на данный момент
extern CMissile* cMissile[500];//таблица указателей на снаряды
extern int NowMissile;//всего снарядов на данный момент

//=========================Управление игрой===================================
int Impact(float x,float y,float z,int radius,int height,int objno);
int ImpactAll(float x,float y,float z,int radius,int height,int objno,int **collizionarray,int *collizionamount);
int Intersection(float xa1,float ya1,float xa2,float ya2,float xb1,float yb1,float xb2,float yb2,float* xo,float* yo);
void ImpactPlayerToThing(float x,float y,float z,int radius,int height);
int CheckTouchLine(int xb,int yb,int x1,int y1,int z1,int x2,int y2,int z2,float dz,int height,int man,float* xo,float* yo,int *object);
int CheckWay(float x1,float y1,float z1,float x2,float y2,float z2,int radius,int height,int man,float* xo,float *yo,int *object);
int ImpactPlayerToWall(float x,float y,float z);
void SlideTraverse(float xp1,float yp1,float xp2,float yp2);
void InitPlayer(void);
int DistanceOfVector(float xa,float ya,float xb,float yb);
void AnimateDoor(int k);
void AnimateGantry(int k);
void AnimateFloor_Flow(void);
void SetThing(void);
void AnimateThing(void);
void SpecialSector(int sector);
void MoveAlien(void);
void ColorEffect(void);
void AnimateFace(int t);
void PutMessage(void);
void AnimateAll(int sector);
int ViewAll(void);
int KeyboardControl(int sector);
int Control(void);
int Use(int w);
void RotateLeft(void);
void RotateRight(void);
void MoveUp(int type);
void MoveDown(int type);
void MoveLeft(void);
void MoveRight(void);
void Fallen(int sector);
void PutWeapon(void);
void AnimateWeapon(void);
void Fire(void);
void ChangeWeapon(int number);
void AnimateMissile(void);
void PutStatistik();
float FindAlien(void);
#endif