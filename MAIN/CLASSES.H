#ifndef CLASSES_H
#define CLASSES_H

#include "common.h"
#include "loading.h"
#include "video.h"

extern CVideo cVideo;
extern SData sData;
extern SPlayer sPlayer;
extern SZLines sZLines[321];
extern float CosTable[361],SinTable[361];//⠡���� ᨭ�ᮢ � ��ᨭ�ᮢ
extern SSector *sSector;

//============================================================================
struct SParameters
{
  float X;//���न����
  float Y;
  float Z;//������ ���न���
  int Height;//����
  int Radius;//ࠤ���
};
struct SFootage
{
 int Faze;//����� 䠧�
};
//============================================================================
//����� ��饩
class CThing
{
 public:
  SParameters Coord;
  SFootage* Footage;
  int Obstruction;//1-��ꥪ� �९���⢨�
  int Type;//⨯ ��ꥪ�
  int Faze;//����� 䠧� �����樨
  int ThingNo;//����� �।���(�� ⨯�)
  char *Particular;//����� ��䨪�
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void SetCoord(SParameters p);
  void SetFootage(SFootage* f,int max);
  void SetThingToFloor(void);
  void Put(void);
};
//����� �ࠣ��
class CAlien
{
 public:
  SParameters Coord;
  SFootage* Footage;//����� �����樨
  int Destroy;//����� ����樨 ���஢ 㭨�⮦����
  int Fire;//����� ����樨 ���஢ ��५��
  int Life;//����� �ࠣ�
  int Faze;//����� 䠧� �����樨
  float Dx,Dy,Dz;//���饭�� ���न��� �ࠣ�
  int Active;//1-�ࠣ ��⨢��
  int Visible;//�ࠣ ����� ��ப��
  int Angle;//㣮� ����鸞 �ࠣ�(�㦥� ��� �ᮢ���� ��� � ࠧ��� ��஭)
  int Time;//�६� �������� � ������ ���ࠢ�����
  int Type;//⨯ ������(1,2,...)
  char *Particular;//����� ��䨪�
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void SetCoord(SParameters p);
  void SetFootage(SFootage* f,int max);
  void SetAlienToFloor(void);
  void Put(void);
};
//����� ᭠�冷�
struct CMissile
{
 int Footage[15];//����� ���஢
 float X;//⥪�騥 ���न����
 float Y;
 float Z;
 float Dx;//���饭�� ���न���(�� ������� ᪮���);
 float Dy;
 float Dz;
 int Radius;//ࠤ��� ᭠�鸞
 int Speed;//᪮����
 int Power;//㡮���� ᨫ�
 int Faze;//䠧� �����樨
 int Destroy;//����� 䠧� 㭨�⮦����
 int sWeaponNo;//����� ��㦨�(⨯), �� ���ண� ���饭 ᭠��
 int Object;//�� ��५�(-1=��ப);
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
void CreateNewRedTorch(int x,int y);//ᮧ���� ���� 䠪��
void CreateNewBlueTorch(int x,int y);//ᮧ���� ᨭ�� 䠪��
void CreateNewButt(int x,int y);//ᮧ���� ����
void CreateNewBattery(int x,int y);//ᮧ���� ����� � ����������
void CreateNewMedicine(int x,int y);//ᮧ���� �����
void CreateNewSuperMedicine(int x,int y);//ᮧ���� �㯥� �����
void CreateNewGun(int x,int y);//ᮧ���� ����
void CreateNewMachineGun(int x,int y);//ᮧ���� �㫥���
void CreateNewPlasmaGun(int x,int y);//ᮧ���� ���������
void CreateNewGunAmmo(int x,int y);//ᮧ���� ���஭� � ����
void CreateNewMGunAmmo(int x,int y);//ᮧ���� ���஭� � �㫥����
void CreateNewGreyAltar(int x,int y);//ᮧ���� ��� �����
void CreateNewRedAltar(int x,int y);//ᮧ���� ���� �����
void CreateNewSuppository(int x,int y);//ᮧ���� ᢥ�
void CreateNewPistolAmmo(int x,int y);//ᮧ���� ���஭� � ���⮫���
void CreateNewLocMap(int x,int y);//ᮧ���� �����
void CreateNewSpacesuit(int x,int y);//ᮧ���� ᪠䠭��
void CreateNewMiniSuppository(int x,int y);//ᮧ���� �������� ᢥ��
void CreateNewBlood(int x,int y);//ᮧ���� �஢���� ��⭮
void CreateNewMiniBlood(int x,int y);//ᮧ���� �����쪮� �஢���� ��⭮
void CreateNewMegaSphere(int x,int y);//ᮧ���� ��������
void CreateNewSuperSphere(int x,int y);//ᮧ���� �㯥�����
void CreateNewHealingWater(int x,int y);//ᮧ���� "����� ����"
//============================================================================
void CreateNewDemon1(int x,int y);
void CreateNewDemon2(int x,int y);
//============================================================================
void CreateNewRocket(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewPlasmaGunFire(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewGunFire(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewBlueBall(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);

#endif