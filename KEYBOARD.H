char KeyTable[255];//таблица состояния клавиш
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Класс обработки клавиатуры.                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class Keyboard
{
 public:
  void (__interrupt __far *inter)();
  static void interrupt NewKeyboard();
  Keyboard();
  ~Keyboard();
  char GetPressKey(unsigned char key);
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Новый обработчик клавиатуры.                                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void interrupt Keyboard::NewKeyboard()
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
Keyboard::Keyboard()
{
 short n;
 for(n=0;n<255;n++) KeyTable[n]=0;
 inter=_dos_getvect(9);
 _dos_setvect(9,NewKeyboard);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Деструктор класса обработки клавиатуры.                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
Keyboard::~Keyboard()
{
 _dos_setvect(9,inter);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает состояние клавиши с номером key.0-не нажата.           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
char Keyboard::GetPressKey(unsigned char key)
{
 return(KeyTable[key]);
}



