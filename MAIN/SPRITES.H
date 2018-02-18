#ifndef SPRITES_H
#define SPRITES_H

#include "common.h"
#include "video.h"

extern CVideo cVideo;

void PutStaticSprite(int x,int y,unsigned char *wdp,int number);
void PutBar(unsigned char *wdp,int number,int x,int y);
void PutSprite(int x,int y,unsigned char *wdp,int number,int type);
#endif