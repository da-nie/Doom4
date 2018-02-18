#ifndef LOADING_H
#define LOADING_H

#include "common.h"
#include "video.h"

extern SData sData;
extern SMaze sMaze;
extern CVideo cVideo;
extern float CosTable[361],SinTable[361];//таблицы синусов и косинусов

//============================================================================
struct SBSPNode
{
 SBSPNode* Left;//левое поддерево
 SBSPNode* Right;//правое поддерево
 int X1;
 int Y1;
 int X2;
 int Y2;
 int Offset;//смещение текстуры
 unsigned char Split;//что за линия (1-линия раздела,2-не рисуемая линия раздела,3-не рисуемая линия раздела с сектором,0-субсектор)
 unsigned char Frontier;//1-линия раздела секторов 0-нет
 int Sector1;//какой сектор (если Split=2);
 int Sector2;//если это линия раздела то тут два сектора от неё
 int Type;//текстура для линии раздела и субсектора
 int LX1,LX2,LY1,LY2;//левый описывающий прямоугольник
 int RX1,RX2,RY1,RY2;//правый описывающий прямоугольник
 int Number;
};

struct SInfo
{
 int SectorNumber;
 int SegmentNumber;
 int ThingNumber;
 int AlienNumber;
 int X;
 int Y;
 int *TX;
 int *TY;
 int *TType;
 int *AX;
 int *AY;
 int *AType;
};
struct SSegment
{
 int X1;
 int Y1;
 int X2;
 int Y2;
 int Offset;//смещение текстуры
 int Type;//текстура
 int Frontier;//линия раздела=1
 int Sector1;//сектора для линии раздела
 int Sector2;
 int Len;
 char Visible;//видел ли его игрок(нужно для карты)
};
struct SSector
{
 int XB1;
 int YB1;
 int XB2;
 int YB2;
 int Segment;
 int X[20];
 int Y[20];
 int Type;//тип сектора
 int Sector;//номер сектора для переключателя
 int Down1;//конечная высота пола для платформы и для двери
 int Down2;//старое значение высоты для платформы
 int UpTexture;
 int DownTexture;
 int Up;
 int Down;
 int DZ;
 int Time;
};
void FreeTree(SBSPNode *root);
int LoadInt(FILE *f);
void SaveInt(FILE *f,int t);
SBSPNode* LoadTree(FILE *in);
SBSPNode* LoadBSPTree(FILE *file,SSegment *Sgm);
//============================================================================
int LoadGame(int level);
unsigned char* LoadSpriteFile(char *FileName);
void LoadResurs(void);
int Diagnostik(float x,float y,float x1,float y1,float x2,float y2);
int FindSector(float x,float y);
int CollizeLine (float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy);
int FindPaletteIndex(int r,int g,int b);
void MipMapping(FILE *f,int t);
FILE *GoToBlock(int number);
#endif