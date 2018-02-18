#include "menu.h"

//============================================================================
void Menu(void)
{
 char dpt[400];
 PutSprite(98,3,sData.Menu,1,1);
 PutSprite(97,90,sData.Menu,2,1);
 int y=0;
 GetCursorsData(67,16*y+88,dpt);
 PutSprite(67,88+y*16,sData.Menu,3,1);
 int Timer=TimerUser;
 int Faze=0;
 int game=0;//нет игры
 while(1)
  {
   if (Timer/10==TimerUser/10) continue;
   Timer=TimerUser;
   PutSprite(67,88+y*16,sData.Menu,3+Faze/3,1);
   Faze++;
   if (Faze>=6) Faze=0;
   if (cKeyboard.GetPressKey(UP))
    {
     SetCursorsData(67,16*y+88,dpt);
     y--;
     if (y<0) y=4;
     GetCursorsData(67,16*y+88,dpt);
     PutSprite(67,88+y*16,sData.Menu,3+Faze/3,1);
     KeyTable[UP]=0;
    }
   if (cKeyboard.GetPressKey(DOWN))
    {
     SetCursorsData(67,16*y+88,dpt);
     y++;
     if (y>4) y=0;
     GetCursorsData(67,16*y+88,dpt);
     PutSprite(67,88+y*16,sData.Menu,3+Faze/3,1);
     KeyTable[DOWN]=0;
    }
   if (cKeyboard.GetPressKey(ESC) && game==1)//вернёмся к игре
    {
     KeyTable[ESC]=0;
     cVideo.View();
     int i=Control();
     KeyTable[ESC]=0;
     if (i==1)//вы выиграли
      {
       ClearResurs();
       sPlayer.Level++;//переходим к следующему уровню
       int result=LoadGame(sPlayer.Level);
       sPlayer.X=sInfo.X;
       sPlayer.Y=sInfo.Y;
       sPlayer.TimeSpacesuit=0;
       sPlayer.TimeMegaSphere=0;
       cVideo.SetRGB_DAC(1,1,1);
       sPlayer.Angle=0;
       if (result!=0) game=0;//вы прошли все уровни
        else
         {
          SetThing();
          StartGame();
         }
      }
     GetCursorsData(67,16*y+88,dpt);
     PutSprite(98,3,sData.Menu,1,1);
     PutSprite(97,90,sData.Menu,2,1);
     PutSprite(67,88+y*16,sData.Menu,3,1);
    }
   if (cKeyboard.GetPressKey(USE))
    {
     if (y==4)
      {
       if (!Select(20,120,"ARE YOU SURE YOU WANT TO QUIT (Y-N)")) continue;
       if (game!=0) ClearResurs();
       free(sData.Thing);
       free(sData.Alien);
       free(sData.WallTexture1);
       free(sData.WallTexture2);
       free(sData.WallTexture3);
       free(sData.WallTexture4);
       free(sData.WallTexture5);
       free(sData.WallTexture6);
       free(sData.WallTexture7);
       free(sData.WallTexture8);
       free(sData.Bar);
       free(sData.Weapon0);
       free(sData.Weapon1);
       free(sData.Weapon2);
       free(sData.Weapon3);
       free(sData.Weapon4);
       free(sData.Fire);
       free(sData.Font);
       free(sData.Menu);
       return;
      }
     if (y==3)
      {
       char *b=(char *)malloc(65000);
       char *vpt=(char *)0xa0000;
       for(int n=0;n<64000;n++) b[n]=*(vpt+n);
       KeyTable[USE]=0;
       PutSprite(0,0,sData.Menu,5,2);
       RunningScreen();
       while(1)
        {
         if (cKeyboard.GetPressKey(USE)) break;
        }
       for(int n=0;n<64000;n++) *(cVideo.Buffer+n)=b[n];
       free(b);
       RunningScreen();
       KeyTable[USE]=0;
      }
     if (y==0)//новая игра
      {
       if (game!=0) ClearResurs();
       game=1;
       LoadGame(1);
       SetThing();
       InitPlayer();
       sPlayer.X=sInfo.X;
       sPlayer.Y=sInfo.Y;
       sPlayer.Angle=0;
       StartGame();
       int i=Control();
       KeyTable[ESC]=0;
       if (i==1)//вы выиграли
        {
         ClearResurs();
         sPlayer.Level++;//переходим к следующему уровню
         int result=LoadGame(sPlayer.Level);
         sPlayer.X=sInfo.X;
         sPlayer.Y=sInfo.Y;
         sPlayer.TimeSpacesuit=0;
         sPlayer.TimeMegaSphere=0;
         cVideo.SetRGB_DAC(1,1,1);
         sPlayer.Angle=0;
         if (result!=0) game=0;//вы прошли все уровни
          else
           {
            SetThing();
            StartGame();
           }
        }
       GetCursorsData(67,16*y+88,dpt);
       PutSprite(98,3,sData.Menu,1,1);
       PutSprite(97,90,sData.Menu,2,1);
       PutSprite(67,88+y*16,sData.Menu,3,1);
      }
     if (y==2 && game!=0)
      {
       char *b=(char *)malloc(65000);
       char *vpt=(char *)0xa0000;
       for(int n=0;n<64000;n++) b[n]=*(vpt+n);
       SaveGame();
       for(int n=0;n<64000;n++) *(vpt+n)=b[n];
       free(b);
       KeyTable[USE]=0;
       KeyTable[ESC]=1;
      }
     if (y==1)
      {
       char *b=(char *)malloc(65000);
       char *vpt=(char *)0xa0000;
       for(int n=0;n<64000;n++) b[n]=*(vpt+n);
       int result=LoadingGame(game);
       for(int n=0;n<64000;n++) *(vpt+n)=b[n];
       free(b);
       if (result!=0)
        {
         game=1;
         StartGame();
         GetCursorsData(67,16*y+88,dpt);
         PutSprite(98,3,sData.Menu,1,1);
         PutSprite(97,90,sData.Menu,2,1);
         PutSprite(67,88+y*16,sData.Menu,3,1);
        }
       KeyTable[USE]=0;
      }
    }
  }
}
void GetCursorsData(int x,int y,char *d)
{
 char *vpt=(char *)0xa0000;
 int n,m,offset=0;
 for(n=0;n<20;n++)
  for(m=0;m<20;m++,offset++)
   {
    *(d+offset)=*(vpt+n+x+(y+m)*320);
   }
}
void SetCursorsData(int x,int y,char *d)
{
 char *vpt=(char *)0xa0000;
 int n,m,offset=0;
 for(n=0;n<20;n++)
  for(m=0;m<20;m++,offset++)
   {
    *(vpt+n+x+(y+m)*320)=*(d+offset);
   }
}
int AddString(char *string,char *s,int n)
{
 for(int k=0;k<strlen(s);k++) string[n+k]=s[k];
 string[n+strlen(s)]=0;
 return (n+strlen(s));
}
void PrintSlot(void)
{
 for(int n=0;n<10;n++)
  {
   char s[255];
   FILE *file;
   if (n==0) file=fopen("resurs\\save\\slot1.sav","r+b");
   if (n==1) file=fopen("resurs\\save\\slot2.sav","r+b");
   if (n==2) file=fopen("resurs\\save\\slot3.sav","r+b");
   if (n==3) file=fopen("resurs\\save\\slot4.sav","r+b");
   if (n==4) file=fopen("resurs\\save\\slot5.sav","r+b");
   if (n==5) file=fopen("resurs\\save\\slot6.sav","r+b");
   if (n==6) file=fopen("resurs\\save\\slot7.sav","r+b");
   if (n==7) file=fopen("resurs\\save\\slot8.sav","r+b");
   if (n==8) file=fopen("resurs\\save\\slot9.sav","r+b");
   if (n==9) file=fopen("resurs\\save\\slot10.sav","r+b");
   LoadInt(file);
   int day=LoadInt(file);
   int month=LoadInt(file);
   int year=LoadInt(file);
   int hour=LoadInt(file);
   int minute=LoadInt(file);
   int second=LoadInt(file);
   int level=LoadInt(file);
   fclose(file);
   char string[255];
   itoa(level,s,10);
   int len=0;
   len=AddString(string,"LEVEL-",0);
   len=AddString(string,s,len);
   itoa(day,s,10);
   len=AddString(string," ",len);
   len=AddString(string,s,len);
   len=AddString(string,".",len);
   itoa(month,s,10);
   len=AddString(string,s,len);
   len=AddString(string,".",len);
   itoa(year,s,10);
   len=AddString(string,s,len);
   len=AddString(string," ",len);
   itoa(hour,s,10);
   len=AddString(string,s,len);
   len=AddString(string,"-",len);
   itoa(minute,s,10);
   len=AddString(string,s,len);
   len=AddString(string,"-",len);
   itoa(second,s,10);
   len=AddString(string,s,len);
   cVideo.PrintToCVideo(60,34+16*n,string);
  }
}
void SaveGame(void)
{
 KeyTable[USE]=0;
 char dpt[400];
 for(int n=0;n<10;n++) PutSprite(56,30+n*16,sData.Menu,6,1);
 int y=sPlayer.SavePos;
 PrintSlot();
 GetCursorsData(30,26+16*y,dpt);
 PutSprite(30,26+y*16,sData.Menu,3,1);
 int Timer=TimerUser;
 int Faze=0;
 while(1)
  {
   if (Timer/10==TimerUser/10) continue;
   Timer=TimerUser;
   PutSprite(30,26+y*16,sData.Menu,3+Faze/3,1);
   Faze++;
   if (Faze>=6) Faze=0;
   if (cKeyboard.GetPressKey(UP))
    {
     SetCursorsData(30,26+y*16,dpt);
     y--;
     if (y<0) y=9;
     GetCursorsData(30,26+y*16,dpt);
     PutSprite(30,26+y*16,sData.Menu,3+Faze/3,1);
     KeyTable[UP]=0;
    }
   if (cKeyboard.GetPressKey(DOWN))
    {
     SetCursorsData(30,26+y*16,dpt);
     y++;
     if (y>9) y=0;
     GetCursorsData(30,26+y*16,dpt);
     PutSprite(30,26+y*16,sData.Menu,3+Faze/3,1);
     KeyTable[DOWN]=0;
    }
   if (cKeyboard.GetPressKey(ESC))
    {
     KeyTable[ESC]=0;
     GetCursorsData(30,26+y*16,dpt);
     return;
    }
   if (cKeyboard.GetPressKey(USE))//записываем
    {
     FILE *file;
     int n;
     if (y==0) file=fopen("resurs\\save\\slot1.sav","w+b");
     if (y==1) file=fopen("resurs\\save\\slot2.sav","w+b");
     if (y==2) file=fopen("resurs\\save\\slot3.sav","w+b");
     if (y==3) file=fopen("resurs\\save\\slot4.sav","w+b");
     if (y==4) file=fopen("resurs\\save\\slot5.sav","w+b");
     if (y==5) file=fopen("resurs\\save\\slot6.sav","w+b");
     if (y==6) file=fopen("resurs\\save\\slot7.sav","w+b");
     if (y==7) file=fopen("resurs\\save\\slot8.sav","w+b");
     if (y==8) file=fopen("resurs\\save\\slot9.sav","w+b");
     if (y==9) file=fopen("resurs\\save\\slot10.sav","w+b");
     SaveInt(file,1);//слот заполнен
     dosdate_t date;
     dostime_t time;
     //сохраняем дату
     _dos_getdate(&date);
     _dos_gettime(&time);
     SaveInt(file,date.day);
     SaveInt(file,date.month);
     SaveInt(file,date.year);
     SaveInt(file,time.hour);
     SaveInt(file,time.minute);
     SaveInt(file,time.second);
     SaveInt(file,sPlayer.Level);
     //сохраняем данные игрока
     SaveInt(file,sPlayer.X*100);
     SaveInt(file,sPlayer.Y*100);
     SaveInt(file,sPlayer.Z*100);
     SaveInt(file,sPlayer.P_X);
     SaveInt(file,sPlayer.P_Y);
     SaveInt(file,sPlayer.Life);
     SaveInt(file,sPlayer.Angle);
     SaveInt(file,sPlayer.Dz*100);
     SaveInt(file,sPlayer.PV);
     SaveInt(file,sPlayer.Pz);
     SaveInt(file,sPlayer.AutoMap);
     SaveInt(file,sPlayer.D_Angle);
     SaveInt(file,sPlayer.SitZ);
     SaveInt(file,sPlayer.RedScreen);
     SaveInt(file,sPlayer.RedScreenOn);
     SaveInt(file,sPlayer.TimeSpacesuit);
     SaveInt(file,sPlayer.TimeMegaSphere);
     SaveInt(file,sPlayer.IDDQD);
     SaveInt(file,sPlayer.MessageNo);
     SaveInt(file,sPlayer.MessageTime);
     SaveInt(file,sPlayer.Level);
     //сохраняем параметры оружия
     SaveInt(file,sWeapon.Fire_Faze);
     SaveInt(file,sWeapon.WeaponNo);
     SaveInt(file,sWeapon.Weapon);
     SaveInt(file,sWeapon.Dy);
     SaveInt(file,sWeapon.Vy);
     for(n=0;n<10;n++)
      {
       SaveInt(file,sWeapon.Ammo[n]);
       SaveInt(file,sWeapon.MaxAmmo[n]);
       SaveInt(file,sWeapon.Attendance[n]);
       for(int m=0;m<50;m++) SaveInt(file,sWeapon.Faze[n][m]);
      }
     for(n=0;n<6;n++)
      for(int m=0;m<50;m++)
       {
        SaveInt(file,sWeapon.X[n][m]);
        SaveInt(file,sWeapon.Y[n][m]);
       }
     //сохраняем параметры сегментов
     for(n=0;n<sInfo.SegmentNumber;n++)
      {
       SaveInt(file,sSegment[n].Visible);
       SaveInt(file,sSegment[n].Type);
      }
     //сохраняем параметры секторов
     for(n=0;n<sInfo.SectorNumber;n++)
      {
       SaveInt(file,sSector[n].UpTexture);
       SaveInt(file,sSector[n].DownTexture);
       SaveInt(file,sSector[n].Up);
       SaveInt(file,sSector[n].Down);
       SaveInt(file,sSector[n].Down1);
       SaveInt(file,sSector[n].Down2);
       SaveInt(file,sSector[n].DZ);
       SaveInt(file,sSector[n].Time);
       SaveInt(file,sSector[n].Type);
       SaveInt(file,sSector[n].Sector);
      }
     //сохраняем существующие снаряды
     SaveInt(file,NowMissile);
     for(n=0;n<NowMissile;n++)
      {
       SaveInt(file,cMissile[n]->sWeaponNo);
       SaveInt(file,cMissile[n]->X*100);
       SaveInt(file,cMissile[n]->Y*100);
       SaveInt(file,cMissile[n]->Z*100);
       SaveInt(file,cMissile[n]->Dx*100);
       SaveInt(file,cMissile[n]->Dy*100);
       SaveInt(file,cMissile[n]->Dz*100);
       SaveInt(file,cMissile[n]->Speed);
       SaveInt(file,cMissile[n]->Power);
       SaveInt(file,cMissile[n]->Faze);
       SaveInt(file,cMissile[n]->Object);
      }
     //сохраняем существующих врагов
     SaveInt(file,NowAlien);
     for(n=0;n<NowAlien;n++)
      {
       SaveInt(file,cAlien[n]->Type);
       SaveInt(file,cAlien[n]->Coord.X*100);
       SaveInt(file,cAlien[n]->Coord.Y*100);
       SaveInt(file,cAlien[n]->Coord.Z*100);
       SaveInt(file,cAlien[n]->Coord.Height);
       SaveInt(file,cAlien[n]->Coord.Radius);
       SaveInt(file,cAlien[n]->Life);
       SaveInt(file,cAlien[n]->Faze);
       SaveInt(file,cAlien[n]->Dx*100);
       SaveInt(file,cAlien[n]->Dy*100);
       SaveInt(file,cAlien[n]->Dz*100);
       SaveInt(file,cAlien[n]->Active);
       SaveInt(file,cAlien[n]->Visible);
       SaveInt(file,cAlien[n]->Angle);
       SaveInt(file,cAlien[n]->Time);
       SaveInt(file,cAlien[n]->Destroy);
      }
     //сохраняем существующие предметы
     SaveInt(file,NowThing);
     for(n=0;n<NowThing;n++)
      {
       SaveInt(file,cThing[n]->ThingNo);
       SaveInt(file,cThing[n]->Coord.X);
       SaveInt(file,cThing[n]->Coord.Y);
       SaveInt(file,cThing[n]->Coord.Z);
       SaveInt(file,cThing[n]->Faze);
      }
     fclose(file);
     GetCursorsData(30,26+y*16,dpt);
     sPlayer.MessageNo=15;
     sPlayer.MessageTime=50;
     sPlayer.SavePos=y;
     return;
    }
  }
}
int LoadingGame(int game)
{
 KeyTable[USE]=0;
 char dpt[400];
 for(int n=0;n<10;n++) PutSprite(56,30+n*16,sData.Menu,6,1);
 PrintSlot();
 int y=sPlayer.LoadPos;
 GetCursorsData(30,26+16*y,dpt);
 PutSprite(30,26+y*16,sData.Menu,3,1);
 int Timer=TimerUser;
 int Faze=0;
 while(1)
  {
   if (Timer/10==TimerUser/10) continue;
   Timer=TimerUser;
   PutSprite(30,26+y*16,sData.Menu,3+Faze/3,1);
   Faze++;
   if (Faze>=6) Faze=0;
   if (cKeyboard.GetPressKey(UP))
    {
     SetCursorsData(30,26+y*16,dpt);
     y--;
     if (y<0) y=9;
     GetCursorsData(30,26+y*16,dpt);
     PutSprite(30,26+y*16,sData.Menu,3+Faze/3,1);
     KeyTable[UP]=0;
    }
   if (cKeyboard.GetPressKey(DOWN))
    {
     SetCursorsData(30,26+y*16,dpt);
     y++;
     if (y>9) y=0;
     GetCursorsData(30,26+y*16,dpt);
     PutSprite(30,26+y*16,sData.Menu,3+Faze/3,1);
     KeyTable[DOWN]=0;
    }
   if (cKeyboard.GetPressKey(ESC))
    {
     KeyTable[ESC]=0;
     GetCursorsData(30,26+y*16,dpt);
     return(0);
    }
   if (cKeyboard.GetPressKey(USE))//загружаем
    {
     FILE *file;
     int n,a;
     if (y==0) file=fopen("resurs\\save\\slot1.sav","r+b");
     if (y==1) file=fopen("resurs\\save\\slot2.sav","r+b");
     if (y==2) file=fopen("resurs\\save\\slot3.sav","r+b");
     if (y==3) file=fopen("resurs\\save\\slot4.sav","r+b");
     if (y==4) file=fopen("resurs\\save\\slot5.sav","r+b");
     if (y==5) file=fopen("resurs\\save\\slot6.sav","r+b");
     if (y==6) file=fopen("resurs\\save\\slot7.sav","r+b");
     if (y==7) file=fopen("resurs\\save\\slot8.sav","r+b");
     if (y==8) file=fopen("resurs\\save\\slot9.sav","r+b");
     if (y==9) file=fopen("resurs\\save\\slot10.sav","r+b");
     a=LoadInt(file);//состояние слота
     if (a!=1)//слот не заполнен
      {
       fclose(file);
       return(0);
      }
     //удаляем старый лабиринт
     if (game!=0) ClearResurs();
     //загружаем новый
     //загружаем дату
     LoadInt(file);
     LoadInt(file);
     LoadInt(file);
     LoadInt(file);
     LoadInt(file);
     LoadInt(file);
     LoadInt(file);
     //восстанавливаем данные игрока
     InitPlayer();
     sPlayer.X=LoadInt(file)/100;
     sPlayer.Y=LoadInt(file)/100;
     sPlayer.Z=LoadInt(file)/100;
     sPlayer.P_X=LoadInt(file);
     sPlayer.P_Y=LoadInt(file);
     sPlayer.Life=LoadInt(file);
     sPlayer.Angle=LoadInt(file);
     sPlayer.Dz=LoadInt(file)/100;
     sPlayer.PV=LoadInt(file);
     sPlayer.Pz=LoadInt(file);
     sPlayer.AutoMap=LoadInt(file);
     sPlayer.D_Angle=LoadInt(file);
     sPlayer.SitZ=LoadInt(file);
     sPlayer.RedScreen=LoadInt(file);
     sPlayer.RedScreenOn=LoadInt(file);
     sPlayer.TimeSpacesuit=LoadInt(file);
     sPlayer.TimeMegaSphere=LoadInt(file);
     sPlayer.IDDQD=LoadInt(file);
     sPlayer.MessageNo=LoadInt(file);
     sPlayer.MessageTime=LoadInt(file);
     sPlayer.Level=LoadInt(file);
     //восстанавливаем параметры оружия
     sWeapon.Fire_Faze=LoadInt(file);
     sWeapon.WeaponNo=LoadInt(file);
     sWeapon.Weapon=LoadInt(file);
     sWeapon.Dy=LoadInt(file);
     sWeapon.Vy=LoadInt(file);
     for(n=0;n<10;n++)
      {
       sWeapon.Ammo[n]=LoadInt(file);
       sWeapon.MaxAmmo[n]=LoadInt(file);
       sWeapon.Attendance[n]=LoadInt(file);
       for(int m=0;m<50;m++)  sWeapon.Faze[n][m]=LoadInt(file);
      }
     for(n=0;n<6;n++)
      for(int m=0;m<50;m++)
       {
        sWeapon.X[n][m]=LoadInt(file);
        sWeapon.Y[n][m]=LoadInt(file);
       }
     //загружаем карту(с учётом sPlayer.Level)
     LoadGame(sPlayer.Level);
     //восстанавливаем параметры сегментов
     for(n=0;n<sInfo.SegmentNumber;n++)
      {
       sSegment[n].Visible=LoadInt(file);
       sSegment[n].Type=LoadInt(file);
      }
     //загружаем параметры секторов
     for(n=0;n<sInfo.SectorNumber;n++)
      {
       sSector[n].UpTexture=LoadInt(file);
       sSector[n].DownTexture=LoadInt(file);
       sSector[n].Up=LoadInt(file);
       sSector[n].Down=LoadInt(file);
       sSector[n].Down1=LoadInt(file);
       sSector[n].Down2=LoadInt(file);
       sSector[n].DZ=LoadInt(file);
       sSector[n].Time=LoadInt(file);
       sSector[n].Type=LoadInt(file);
       sSector[n].Sector=LoadInt(file);
      }
     //восстанавливаем существующие снаряды
     a=LoadInt(file);
     for(n=0;n<a;n++)
      {
       int weapon=LoadInt(file);
       float x=LoadInt(file)/100;
       float y=LoadInt(file)/100;
       float z=LoadInt(file)/100;
       float dx=LoadInt(file)/100;
       float dy=LoadInt(file)/100;
       float dz=LoadInt(file)/100;
       int speed=LoadInt(file);
       int power=LoadInt(file);
       int faze=LoadInt(file);
       int object=LoadInt(file);
       if (weapon==1) CreateNewGunFire(x,y,z,dx,dy,dz,speed,power,weapon,object);
       if (weapon==3) CreateNewGunFire(x,y,z,dx,dy,dz,speed,power,weapon,object);
       if (weapon==4) CreateNewPlasmaGunFire(x,y,z,dx,dy,dz,speed,power,weapon,object);
       if (weapon==5) CreateNewRocket(x,y,z,dx,dy,dz,speed,power,weapon,object);
       if (weapon==6) CreateNewBlueBall(x,y,z,dx,dy,dz,speed,power,weapon,object);
       cMissile[n]->Faze=faze;
      }
     //восстанавливаем существующих врагов
     a=LoadInt(file);
     for(n=0;n<a;n++)
      {
       int type=LoadInt(file);
       float x=LoadInt(file)/100;
       float y=LoadInt(file)/100;
       float z=LoadInt(file)/100;
       int height=LoadInt(file);
       int radius=LoadInt(file);
       int life=LoadInt(file);
       int faze=LoadInt(file);
       float dx=LoadInt(file)/100;
       float dy=LoadInt(file)/100;
       float dz=LoadInt(file)/100;
       int active=LoadInt(file);
       int visible=LoadInt(file);
       int angle=LoadInt(file);
       int time=LoadInt(file);
       int destroy=LoadInt(file);
       if (type==1) CreateNewDemon1(x,y);
       if (type==2) CreateNewDemon2(x,y);
       cAlien[n]->Coord.Z=z;
       cAlien[n]->Coord.Height=height;
       cAlien[n]->Coord.Radius=radius;
       cAlien[n]->Life=life;
       cAlien[n]->Faze=faze;
       cAlien[n]->Dx=dx;
       cAlien[n]->Dy=dy;
       cAlien[n]->Dz=dz;
       cAlien[n]->Active=active;
       cAlien[n]->Visible=visible;
       cAlien[n]->Angle=angle;
       cAlien[n]->Time=time;
       cAlien[n]->Destroy=destroy;
      }
     //восстанавливаем существующие предметы
     a=LoadInt(file);
     for(n=0;n<a;n++)
      {
       int no=LoadInt(file);
       int x=LoadInt(file);
       int y=LoadInt(file);
       int z=LoadInt(file);
       int faze=LoadInt(file);
       if (no==1) CreateNewRedTorch(x,y);
       if (no==2) CreateNewBlueTorch(x,y);
       if (no==3) CreateNewButt(x,y);
       if (no==4) CreateNewBattery(x,y);
       if (no==5) CreateNewMedicine(x,y);
       if (no==6) CreateNewSuperMedicine(x,y);
       if (no==7) CreateNewGun(x,y);
       if (no==8) CreateNewMachineGun(x,y);
       if (no==9) CreateNewPlasmaGun(x,y);
       if (no==10) CreateNewGunAmmo(x,y);
       if (no==11) CreateNewMGunAmmo(x,y);
       if (no==12) CreateNewGreyAltar(x,y);
       if (no==13) CreateNewRedAltar(x,y);
       if (no==14) CreateNewSuppository(x,y);
       if (no==15) CreateNewPistolAmmo(x,y);
       if (no==16) CreateNewLocMap(x,y);
       if (no==17) CreateNewSpacesuit(x,y);
       if (no==18) CreateNewMiniSuppository(x,y);
       if (no==19) CreateNewBlood(x,y);
       if (no==20) CreateNewMiniBlood(x,y);
       if (no==21) CreateNewMegaSphere(x,y);
       if (no==22) CreateNewSuperSphere(x,y);
       if (no==23) CreateNewHealingWater(x,y);
       cThing[n]->Coord.X=x;
       cThing[n]->Coord.Y=y;
       cThing[n]->Coord.Z=z;
       cThing[n]->Faze=faze;
      }
     sPlayer.LoadPos=y;
     sInfo.TX=new int[1];
     sInfo.TY=new int[1];
     sInfo.TType=new int[1];
     sInfo.AX=new int[1];
     sInfo.AY=new int[1];
     sInfo.AType=new int[1];
     sInfo.ThingNumber=0;
     sInfo.AlienNumber=0;
     fclose(file);
     GetCursorsData(30,26+y*16,dpt);
     return(1);
    }
  }
}
void ClearResurs(void)
{
 int x,y,n;
 for(x=0;x<100;x++)
  for(y=0;y<100;y++) free(sMaze.BlockMap[y][x]);
 free(sSector);
 free(sSegment);
 FreeTree(Node);
 delete(sInfo.TX);
 delete(sInfo.TY);
 delete(sInfo.TType);
 delete(sInfo.AX);
 delete(sInfo.AY);
 delete(sInfo.AType);
 for(n=0;n<NowThing;n++)
  {
   delete(cThing[n]->Footage);
   delete(cThing[n]);
  }
 for(n=0;n<NowAlien;n++)
  {
   delete(cAlien[n]->Footage);
   delete(cAlien[n]);
  }
 for(n=0;n<NowMissile;n++) delete(cMissile[n]);
 NowThing=0;
 NowAlien=0;
 NowMissile=0;
}
void StartGame(void)
{
 cVideo.SetRGB_DAC(1,1,1);
 if (sPlayer.TimeSpacesuit!=0) cVideo.SetRGB_DAC(0.7,1,0.7);
 if (sPlayer.TimeMegaSphere!=0) cVideo.SetRGB_DAC(0.7,0.7,1);
 sPlayer.Z=sSector[FindSector(sPlayer.X,sPlayer.Y)].Down+64;
 int w=sWeapon.WeaponNo;
 char s[255];
 PutBar(sData.Bar,0,0,168);
 itoa(sWeapon.Ammo[w],s,10);
 cVideo.Print(13,180,s);
 itoa(sPlayer.Life,s,10);
 cVideo.Print(60,180,s);
 sPlayer.P_X=sPlayer.X*65536;
 sPlayer.P_Y=sPlayer.Y*65536;
 int sector=FindSector(sPlayer.X,sPlayer.Y);
 cVideo.ClearWindow();
 AnimateAll(sector);
 View();
 PutWeapon();
 PutMessage();
 RunningScreen();
}
int Select(int x,int y,char *text)
{
 char *b=(char *)malloc(65000);
 char *vpt=(char *)0xa0000;
 for(int n=0;n<64000;n++) b[n]=*(vpt+n);
 cVideo.PrintToCVideo(x,y,text);
 int result;
 KeyTable[21]=0;
 KeyTable[49]=0;
 while(1)
  {
   if (cKeyboard.GetPressKey(21))
    {
     result=1;
     break;
    }
   if (cKeyboard.GetPressKey(49))
    {
     result=0;
     break;
    }
  }
 for(int n=0;n<64000;n++) *(vpt+n)=b[n];
 free(b);
 return(result);
}









