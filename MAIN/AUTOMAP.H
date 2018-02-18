#ifndef AUTOMAP_H
#define AUTOMAP_H

#include "common.h"
#include "loading.h"
#include "video.h"

extern SPlayer sPlayer;
extern CVideo cVideo;
extern SInfo sInfo;

extern float CosTable[361],SinTable[361];//таблицы синусов и косинусов
extern SSegment *sSegment;
extern SSector *sSector;

void Line(int x1,int y1,int x2,int y2,char color);
void putpixel(int x,int y,char color);
void PutAutoMap(void);

#endif