#ifndef LOADING_H
#define LOADING_H

#include "common.h"
#include "video.h"

extern SData sData;
extern SMaze sMaze;
extern CVideo cVideo;
extern float CosTable[361],SinTable[361];//⠡���� ᨭ�ᮢ � ��ᨭ�ᮢ

//============================================================================
struct SBSPNode
{
 SBSPNode* Left;//����� �����ॢ�
 SBSPNode* Right;//�ࠢ�� �����ॢ�
 int X1;
 int Y1;
 int X2;
 int Y2;
 int Offset;//ᬥ饭�� ⥪�����
 unsigned char Split;//�� �� ����� (1-����� ࠧ����,2-�� ��㥬�� ����� ࠧ����,3-�� ��㥬�� ����� ࠧ���� � ᥪ�஬,0-��ᥪ��)
 unsigned char Frontier;//1-����� ࠧ���� ᥪ�஢ 0-���
 int Sector1;//����� ᥪ�� (�᫨ Split=2);
 int Sector2;//�᫨ �� ����� ࠧ���� � ��� ��� ᥪ�� �� ���
 int Type;//⥪���� ��� ����� ࠧ���� � ��ᥪ��
 int LX1,LX2,LY1,LY2;//���� ����뢠�騩 ��אַ㣮�쭨�
 int RX1,RX2,RY1,RY2;//�ࠢ� ����뢠�騩 ��אַ㣮�쭨�
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
 int Offset;//ᬥ饭�� ⥪�����
 int Type;//⥪����
 int Frontier;//����� ࠧ����=1
 int Sector1;//ᥪ�� ��� ����� ࠧ����
 int Sector2;
 int Len;
 char Visible;//����� �� ��� ��ப(�㦭� ��� �����)
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
 int Type;//⨯ ᥪ��
 int Sector;//����� ᥪ�� ��� ��४���⥫�
 int Down1;//����筠� ���� ���� ��� ������� � ��� ����
 int Down2;//��஥ ���祭�� ����� ��� �������
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