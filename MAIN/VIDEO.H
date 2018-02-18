#ifndef VIDEO_H
#define VIDEO_H

#include "common.h"
extern SData sData;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Класс обработки экрана.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class CVideo
{
 public:
  unsigned char *Buffer;
  unsigned char Pal[800];
  void InitVga(void);
  void CloseVga(void);
  void ClearWindow(void);
  void Clear(void);
  void ViewWindow(void);
  void ViewBar(void);
  void View(void);
  void SetRGB_DAC(float dr,float dg,float db);
  void LoadPalette(char *filename);
  int PutSymbol(int x,int y,char symbol,int type);
  void Print(int x,int y,char *Text);
  void PrintToCVideo(int x,int y,char *Text);
};
#endif;