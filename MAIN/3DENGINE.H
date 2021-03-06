#ifndef _3DENGINE_H
#define _3DENGINE_H

#include "common.h"
#include "loading.h"
#include "video.h"
#include "classes.h"

extern SPlayer sPlayer;
extern SData sData;
extern SMaze sMaze;
extern CVideo cVideo;

extern SZLines sZLines[321];
extern short FloorBuffer[64000];//���� ��ࠬ��஢ �����
extern float CosTable[361],SinTable[361];//⠡���� ᨭ�ᮢ � ��ᨭ�ᮢ
extern SSegment *sSegment;
extern SSector *sSector;
extern SBSPNode *Node;

extern CThing* cThing[500];//⠡��� 㪠��⥫�� �� ��ꥪ��
extern int NowThing;//�ᥣ� ������� ��ꥪ⮢ �� ����� ������
extern CAlien* cAlien[500];//⠡��� 㪠��⥫�� �� �ࠣ��
extern int NowAlien;//�ᥣ� ������� �ࠣ�� �� ����� ������
extern CMissile* cMissile[500];//⠡��� 㪠��⥫�� �� ᭠���
extern int NowMissile;//�ᥣ� ᭠�冷� �� ����� ������

//===========================================================================
int SeenBox(SBSPNode* tree,int type);
inline void DrawBSPTree(SBSPNode* tree);
void DrawSubSector(SBSPNode* root);
void DrawFloorLine(int x,int y1,int y2,short f_texture);
unsigned char *FindTexture(int ky,int offsett,int texture,int *mask,int *dy_,int *yt_);
void DrawSimpleLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short sector);
inline void DrawLowerLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short sector);
inline void DrawUpperLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short sector);
void DrawFloor_Flow(void);
void View(void);
#endif
