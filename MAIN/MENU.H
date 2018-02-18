#ifndef MENU_H
#define MENU_H

#include "common.h"
#include "control.h"
#include "video.h"
#include "keyboard.h"
#include "sprites.h"
#include "timer.h"
#include "special.h"
#include "loading.h"
#include "classes.h"

extern SInfo sInfo;
extern int TimerUser;
extern char KeyTable[255];//таблица состояния клавиш
extern CKeyboard cKeyboard;
extern SData sData;
extern SPlayer Playse;
extern CVideo cVideo;

void Menu(void);
void GetCursorsData(int x,int y,char *d);
void SetCursorsData(int x,int y,char *d);
int AddString(char *string,char *s,int n);
void PrintSlot(void);
void SaveGame(void);
int LoadingGame(int game);
void ClearResurs(void);
void StartGame(void);
int Select(int x,int y,char *text);
#endif