#ifndef CLASSES_H
#define CLASSES_H

#include "common.h"
#include "loading.h"
#include "video.h"

extern CVideo cVideo;
extern SData sData;
extern SPlayer sPlayer;
extern SZLines sZLines[321];
extern float CosTable[361],SinTable[361];//таблицы синусов и косинусов
extern SSector *sSector;

//============================================================================
struct SParameters
{
  float X;//координаты
  float Y;
  float Z;//верхняя координата
  int Height;//высота
  int Radius;//радиус
};
struct SFootage
{
 int Faze;//номер фазы
};
//============================================================================
//класс вещей
class CThing
{
 public:
  SParameters Coord;
  SFootage* Footage;
  int Obstruction;//1-объект препятствие
  int Type;//тип объекта
  int Faze;//номер фазы анимации
  int ThingNo;//номер предмета(по типу)
  char *Particular;//данные графики
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void SetCoord(SParameters p);
  void SetFootage(SFootage* f,int max);
  void SetThingToFloor(void);
  void Put(void);
};
//класс врагов
class CAlien
{
 public:
  SParameters Coord;
  SFootage* Footage;//кадры анимации
  int Destroy;//номер позиции кадров уничтожения
  int Fire;//номер позиции кадров стрельбы
  int Life;//жизнь врага
  int Faze;//номер фазы анимации
  float Dx,Dy,Dz;//приращения координат врага
  int Active;//1-враг активен
  int Visible;//враг видим игроком
  int Angle;//угол взгляда врага(нужен для рисования его с разных сторон)
  int Time;//время движения в данном направлении
  int Type;//тип демона(1,2,...)
  char *Particular;//данные графики
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void SetCoord(SParameters p);
  void SetFootage(SFootage* f,int max);
  void SetAlienToFloor(void);
  void Put(void);
};
//класс снарядов
struct CMissile
{
 int Footage[15];//номера кадров
 float X;//текущие координаты
 float Y;
 float Z;
 float Dx;//приращения координат(на единицу скорости);
 float Dy;
 float Dz;
 int Radius;//радиус снаряда
 int Speed;//скорость
 int Power;//убойная сила
 int Faze;//фаза анимации
 int Destroy;//номер фазы уничтожения
 int sWeaponNo;//номер оружия(тип), из которого выпущен снаряд
 int Object;//кто стрелял(-1=игрок);
 char *Particular;
 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 void Put(void);
};
//============================================================================
CThing* CreateThing(void);
void DeleteThing(int no);
//============================================================================
CAlien* CreateAlien(void);
void DeleteAlien(int no);
//============================================================================
CMissile* CreateMissile(void);
void DeleteMissile(int no);
//============================================================================
void CreateNewRedTorch(int x,int y);//создать красный факел
void CreateNewBlueTorch(int x,int y);//создать синий факел
void CreateNewButt(int x,int y);//создать бочку
void CreateNewBattery(int x,int y);//создать заряды к плазмомёту
void CreateNewMedicine(int x,int y);//создать аптечку
void CreateNewSuperMedicine(int x,int y);//создать супер аптечку
void CreateNewGun(int x,int y);//создать ружъё
void CreateNewMachineGun(int x,int y);//создать пулемёт
void CreateNewPlasmaGun(int x,int y);//создать плазмомёт
void CreateNewGunAmmo(int x,int y);//создать патроны к ружъю
void CreateNewMGunAmmo(int x,int y);//создать патроны к пулемёту
void CreateNewGreyAltar(int x,int y);//создать серый алтарь
void CreateNewRedAltar(int x,int y);//создать красный алтарь
void CreateNewSuppository(int x,int y);//создать свечи
void CreateNewPistolAmmo(int x,int y);//создать патроны к пистолету
void CreateNewLocMap(int x,int y);//создать карту
void CreateNewSpacesuit(int x,int y);//создать скафандр
void CreateNewMiniSuppository(int x,int y);//создать маленькую свечу
void CreateNewBlood(int x,int y);//создать кровавое пятно
void CreateNewMiniBlood(int x,int y);//создать маленькое кровавое пятно
void CreateNewMegaSphere(int x,int y);//создать мегасферу
void CreateNewSuperSphere(int x,int y);//создать суперсферу
void CreateNewHealingWater(int x,int y);//создать "живую воду"
//============================================================================
void CreateNewDemon1(int x,int y);
void CreateNewDemon2(int x,int y);
//============================================================================
void CreateNewRocket(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewPlasmaGunFire(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewGunFire(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewBlueBall(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);

#endif