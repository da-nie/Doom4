#include "keyboard.h"

char KeyTable[255];//⠡��� ���ﭨ� ������

CKeyboard cKeyboard;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//���� ��ࠡ��稪 ����������.                                      //
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
//��������� ����� ��ࠡ�⪨ ����������.                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
CKeyboard::CKeyboard()
{
 short n;
 for(n=0;n<255;n++) KeyTable[n]=0;
 inter=_dos_getvect(9);
 _dos_setvect(9,NewCKeyboard);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//�������� ����� ��ࠡ�⪨ ����������.                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
CKeyboard::~CKeyboard()
{
 _dos_setvect(9,inter);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//�����頥� ���ﭨ� ������ � ����஬ key.0-�� �����.           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
char CKeyboard::GetPressKey(unsigned char key)
{
 return(KeyTable[key]);
}



