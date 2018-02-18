#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Класс обработки клавиатуры.                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class CKeyboard
{
 public:
  void (__interrupt __far *inter)();
  static void interrupt NewCKeyboard();
  CKeyboard();
  ~CKeyboard();
  char GetPressKey(unsigned char key);
};
#endif