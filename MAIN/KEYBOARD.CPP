#include "keyboard.h"

char KeyTable[255];//таблица состояния клавиш

CKeyboard cKeyboard;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Новый обработчик клавиатуры.                                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void interrupt CKeyboard::NewCKeyboard()
{
 unsigned char ScanCode=inp(0x60)&127;
 unsigned char type=inp(0x60)&128;
 unsigned char OtherCode=inp(0x61);
 if (type) KeyTable[ScanCode]=0;
      else KeyTable[ScanCode]=1;
 outp(0x61,OtherCode|0x80);
 outp(0x61,OtherCode);
 outp(0x20,0x20);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Конструктор класса обработки клавиатуры.                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
CKeyboard::CKeyboard()
{
 short n;
 for(n=0;n<255;n++) KeyTable[n]=0;
 inter=_dos_getvect(9);
 _dos_setvect(9,NewCKeyboard);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Деструктор класса обработки клавиатуры.                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
CKeyboard::~CKeyboard()
{
 _dos_setvect(9,inter);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает состояние клавиши с номером key.0-не нажата.           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
char CKeyboard::GetPressKey(unsigned char key)
{
 return(KeyTable[key]);
}



