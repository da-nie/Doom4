#ifndef SPECIAL_H
#define SPECIAL_H

#include "common.h"
#include "timer.h"
#include "video.h"

extern int TimerUser;
extern CVideo cVideo;

//===========================спецэфекты=======================================
void RunningScreen(void);
void LightingPlus(void);
void LightingMinus(void);
int rnd(int max);
#endif