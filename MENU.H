void Menu(void);
void GetCursorData(int x,int y,char *d);
void SetCursorData(int x,int y,char *d);
int AddString(char *string,char *s,int n);
void PrintSlot(void);
void SaveGame(void);
int LoadingGame(int game);
void ClearResurs(void);
void StartGame(void);
int Select(int x,int y,char *text);
//============================================================================
void Menu(void)
{
 char dpt[400];
 PutSprite(98,3,Data.Menu,1,1);
 PutSprite(97,90,Data.Menu,2,1);
 int y=0;
 GetCursorData(67,16*y+88,dpt);
 PutSprite(67,88+y*16,Data.Menu,3,1);
 int Timer=TimerUser;
 int Faze=0;
 int game=0;//нет игры
 while(1)
  {
   if (Timer/10==TimerUser/10) continue;
   Timer=TimerUser;
   PutSprite(67,88+y*16,Data.Menu,3+Faze/3,1);
   Faze++;
   if (Faze>=6) Faze=0;
   if (Consol.GetPressKey(UP))
    {
     SetCursorData(67,16*y+88,dpt);
     y--;
     if (y<0) y=4;
     GetCursorData(67,16*y+88,dpt);
     PutSprite(67,88+y*16,Data.Menu,3+Faze/3,1);
     KeyTable[UP]=0;
    }
   if (Consol.GetPressKey(DOWN))
    {
     SetCursorData(67,16*y+88,dpt);
     y++;
     if (y>4) y=0;
     GetCursorData(67,16*y+88,dpt);
     PutSprite(67,88+y*16,Data.Menu,3+Faze/3,1);
     KeyTable[DOWN]=0;
    }
   if (Consol.GetPressKey(ESC) && game==1)//вернёмся к игре
    {
     KeyTable[ESC]=0;
     Video.View();
     int i=Control();
     KeyTable[ESC]=0;
     if (i==1)//вы выиграли
      {
       ClearResurs();
       Player.Level++;//переходим к следующему уровню
       int result=LoadGame(Player.Level);
       Player.X=Info.X;
       Player.Y=Info.Y;
       Player.TimeSpacesuit=0;
       Player.TimeMegaSphere=0;
       Video.SetRGB_DAC(1,1,1);
       Player.Angle=0;
       if (result!=0) game=0;//вы прошли все уровни
        else
         {
          SetThing();
          StartGame();
         }
      }
     GetCursorData(67,16*y+88,dpt);
     PutSprite(98,3,Data.Menu,1,1);
     PutSprite(97,90,Data.Menu,2,1);
     PutSprite(67,88+y*16,Data.Menu,3,1);
    }
   if (Consol.GetPressKey(USE))
    {
     if (y==4)
      {
       if (!Select(20,120,"ARE YOU SURE YOU WANT TO QUIT (Y-N)")) continue;
       if (game!=0) ClearResurs();
       free(Data.Thing);
       free(Data.Alien);
       free(Data.WallTexture1);
       free(Data.WallTexture2);
       free(Data.WallTexture3);
       free(Data.WallTexture4);
       free(Data.WallTexture5);
       free(Data.WallTexture6);
       free(Data.WallTexture7);
       free(Data.WallTexture8);
       free(Data.Bar);
       free(Data.Weapon0);
       free(Data.Weapon1);
       free(Data.Weapon2);
       free(Data.Weapon3);
       free(Data.Weapon4);
       free(Data.Fire);
       free(Data.Font);
       free(Data.Menu);
       return;
      }
     if (y==3)
      {
       char *b=(char *)malloc(65000);
       char *vpt=(char *)0xa0000;
       for(int n=0;n<64000;n++) b[n]=*(vpt+n);
       KeyTable[USE]=0;
       PutSprite(0,0,Data.Menu,5,2);
       RunningScreen();
       while(1)
        {
         if (Consol.GetPressKey(USE)) break;
        }
       for(n=0;n<64000;n++) *(Video.Buffer+n)=b[n];
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
       Player.X=Info.X;
       Player.Y=Info.Y;
       Player.Angle=0;
       StartGame();
       int i=Control();
       KeyTable[ESC]=0;
       if (i==1)//вы выиграли
        {
         ClearResurs();
         Player.Level++;//переходим к следующему уровню
         int result=LoadGame(Player.Level);
         Player.X=Info.X;
         Player.Y=Info.Y;
         Player.TimeSpacesuit=0;
         Player.TimeMegaSphere=0;
         Video.SetRGB_DAC(1,1,1);
         Player.Angle=0;
         if (result!=0) game=0;//вы прошли все уровни
          else
           {
            SetThing();
            StartGame();
           }
        }
       GetCursorData(67,16*y+88,dpt);
       PutSprite(98,3,Data.Menu,1,1);
       PutSprite(97,90,Data.Menu,2,1);
       PutSprite(67,88+y*16,Data.Menu,3,1);
      }
     if (y==2 && game!=0)
      {
       char *b=(char *)malloc(65000);
       char *vpt=(char *)0xa0000;
       for(int n=0;n<64000;n++) b[n]=*(vpt+n);
       SaveGame();
       for(n=0;n<64000;n++) *(vpt+n)=b[n];
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
       for(n=0;n<64000;n++) *(vpt+n)=b[n];
       free(b);
       if (result!=0)
        {
         game=1;
         StartGame();
         GetCursorData(67,16*y+88,dpt);
         PutSprite(98,3,Data.Menu,1,1);
         PutSprite(97,90,Data.Menu,2,1);
         PutSprite(67,88+y*16,Data.Menu,3,1);
        }
       KeyTable[USE]=0;
      }
    }
  }
}
void GetCursorData(int x,int y,char *d)
{
 char *vpt=(char *)0xa0000;
 int n,m,offset=0;
 for(n=0;n<20;n++)
  for(m=0;m<20;m++,offset++)
   {
    *(d+offset)=*(vpt+n+x+(y+m)*320);
   }
}
void SetCursorData(int x,int y,char *d)
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
   Video.PrintToScreen(60,34+16*n,string);
  }
}
void SaveGame(void)
{
 KeyTable[USE]=0;
 char dpt[400];
 for(int n=0;n<10;n++) PutSprite(56,30+n*16,Data.Menu,6,1);
 int y=Player.SavePos;
 PrintSlot();
 GetCursorData(30,26+16*y,dpt);
 PutSprite(30,26+y*16,Data.Menu,3,1);
 int Timer=TimerUser;
 int Faze=0;
 while(1)
  {
   if (Timer/10==TimerUser/10) continue;
   Timer=TimerUser;
   PutSprite(30,26+y*16,Data.Menu,3+Faze/3,1);
   Faze++;
   if (Faze>=6) Faze=0;
   if (Consol.GetPressKey(UP))
    {
     SetCursorData(30,26+y*16,dpt);
     y--;
     if (y<0) y=9;
     GetCursorData(30,26+y*16,dpt);
     PutSprite(30,26+y*16,Data.Menu,3+Faze/3,1);
     KeyTable[UP]=0;
    }
   if (Consol.GetPressKey(DOWN))
    {
     SetCursorData(30,26+y*16,dpt);
     y++;
     if (y>9) y=0;
     GetCursorData(30,26+y*16,dpt);
     PutSprite(30,26+y*16,Data.Menu,3+Faze/3,1);
     KeyTable[DOWN]=0;
    }
   if (Consol.GetPressKey(ESC))
    {
     KeyTable[ESC]=0;
     GetCursorData(30,26+y*16,dpt);
     return;
    }
   if (Consol.GetPressKey(USE))//записываем
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
     SaveInt(file,Player.Level);
     //сохраняем данные игрока
     SaveInt(file,Player.X*100);
     SaveInt(file,Player.Y*100);
     SaveInt(file,Player.Z*100);
     SaveInt(file,Player.P_X);
     SaveInt(file,Player.P_Y);
     SaveInt(file,Player.Life);
     SaveInt(file,Player.Angle);
     SaveInt(file,Player.Dz*100);
     SaveInt(file,Player.PV);
     SaveInt(file,Player.Pz);
     SaveInt(file,Player.AutoMap);
     SaveInt(file,Player.D_Angle);
     SaveInt(file,Player.SitZ);
     SaveInt(file,Player.RedScreen);
     SaveInt(file,Player.RedScreenOn);
     SaveInt(file,Player.TimeSpacesuit);
     SaveInt(file,Player.TimeMegaSphere);
     SaveInt(file,Player.IDDQD);
     SaveInt(file,Player.MessageNo);
     SaveInt(file,Player.MessageTime);
     SaveInt(file,Player.Level);
     //сохраняем параметры оружия
     SaveInt(file,Weapon.Fire_Faze);
     SaveInt(file,Weapon.WeaponNo);
     SaveInt(file,Weapon.Weapon);
     SaveInt(file,Weapon.Dy);
     SaveInt(file,Weapon.Vy);
     for(n=0;n<10;n++)
      {
       SaveInt(file,Weapon.Ammo[n]);
       SaveInt(file,Weapon.MaxAmmo[n]);
       SaveInt(file,Weapon.Attendance[n]);
       for(int m=0;m<50;m++) SaveInt(file,Weapon.Faze[n][m]);
      }
     for(n=0;n<6;n++)
      for(int m=0;m<50;m++)
       {
        SaveInt(file,Weapon.X[n][m]);
        SaveInt(file,Weapon.Y[n][m]);
       }
     //сохраняем параметры сегментов
     for(n=0;n<Info.SegmentNumber;n++)
      {
       SaveInt(file,Segment[n].Visible);
       SaveInt(file,Segment[n].Type);
      }
     //сохраняем параметры секторов
     for(n=0;n<Info.SectorNumber;n++)
      {
       SaveInt(file,Sector[n].UpTexture);
       SaveInt(file,Sector[n].DownTexture);
       SaveInt(file,Sector[n].Up);
       SaveInt(file,Sector[n].Down);
       SaveInt(file,Sector[n].Down1);
       SaveInt(file,Sector[n].Down2);
       SaveInt(file,Sector[n].DZ);
       SaveInt(file,Sector[n].Time);
       SaveInt(file,Sector[n].Type);
       SaveInt(file,Sector[n].Sector);
      }
     //сохраняем существующие снаряды
     SaveInt(file,NowMissile);
     for(n=0;n<NowMissile;n++)
      {
       SaveInt(file,Missile[n]->WeaponNo);
       SaveInt(file,Missile[n]->X*100);
       SaveInt(file,Missile[n]->Y*100);
       SaveInt(file,Missile[n]->Z*100);
       SaveInt(file,Missile[n]->Dx*100);
       SaveInt(file,Missile[n]->Dy*100);
       SaveInt(file,Missile[n]->Dz*100);
       SaveInt(file,Missile[n]->Speed);
       SaveInt(file,Missile[n]->Power);
       SaveInt(file,Missile[n]->Faze);
       SaveInt(file,Missile[n]->Object);
      }
     //сохраняем существующих врагов
     SaveInt(file,NowAlien);
     for(n=0;n<NowAlien;n++)
      {
       SaveInt(file,Alien[n]->Type);
       SaveInt(file,Alien[n]->Coord.X*100);
       SaveInt(file,Alien[n]->Coord.Y*100);
       SaveInt(file,Alien[n]->Coord.Z*100);
       SaveInt(file,Alien[n]->Coord.Height);
       SaveInt(file,Alien[n]->Coord.Radius);
       SaveInt(file,Alien[n]->Life);
       SaveInt(file,Alien[n]->Faze);
       SaveInt(file,Alien[n]->Dx*100);
       SaveInt(file,Alien[n]->Dy*100);
       SaveInt(file,Alien[n]->Dz*100);
       SaveInt(file,Alien[n]->Active);
       SaveInt(file,Alien[n]->Visible);
       SaveInt(file,Alien[n]->Angle);
       SaveInt(file,Alien[n]->Time);
       SaveInt(file,Alien[n]->Destroy);
      }
     //сохраняем существующие предметы
     SaveInt(file,NowThing);
     for(n=0;n<NowThing;n++)
      {
       SaveInt(file,Thing[n]->ThingNo);
       SaveInt(file,Thing[n]->Coord.X);
       SaveInt(file,Thing[n]->Coord.Y);
       SaveInt(file,Thing[n]->Coord.Z);
       SaveInt(file,Thing[n]->Faze);
      }
     fclose(file);
     GetCursorData(30,26+y*16,dpt);
     Player.MessageNo=15;
     Player.MessageTime=50;
     Player.SavePos=y;
     return;
    }
  }
}
int LoadingGame(int game)
{
 KeyTable[USE]=0;
 char dpt[400];
 for(int n=0;n<10;n++) PutSprite(56,30+n*16,Data.Menu,6,1);
 PrintSlot();
 int y=Player.LoadPos;
 GetCursorData(30,26+16*y,dpt);
 PutSprite(30,26+y*16,Data.Menu,3,1);
 int Timer=TimerUser;
 int Faze=0;
 while(1)
  {
   if (Timer/10==TimerUser/10) continue;
   Timer=TimerUser;
   PutSprite(30,26+y*16,Data.Menu,3+Faze/3,1);
   Faze++;
   if (Faze>=6) Faze=0;
   if (Consol.GetPressKey(UP))
    {
     SetCursorData(30,26+y*16,dpt);
     y--;
     if (y<0) y=9;
     GetCursorData(30,26+y*16,dpt);
     PutSprite(30,26+y*16,Data.Menu,3+Faze/3,1);
     KeyTable[UP]=0;
    }
   if (Consol.GetPressKey(DOWN))
    {
     SetCursorData(30,26+y*16,dpt);
     y++;
     if (y>9) y=0;
     GetCursorData(30,26+y*16,dpt);
     PutSprite(30,26+y*16,Data.Menu,3+Faze/3,1);
     KeyTable[DOWN]=0;
    }
   if (Consol.GetPressKey(ESC))
    {
     KeyTable[ESC]=0;
     GetCursorData(30,26+y*16,dpt);
     return(0);
    }
   if (Consol.GetPressKey(USE))//загружаем
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
     Player.X=LoadInt(file)/100;
     Player.Y=LoadInt(file)/100;
     Player.Z=LoadInt(file)/100;
     Player.P_X=LoadInt(file);
     Player.P_Y=LoadInt(file);
     Player.Life=LoadInt(file);
     Player.Angle=LoadInt(file);
     Player.Dz=LoadInt(file)/100;
     Player.PV=LoadInt(file);
     Player.Pz=LoadInt(file);
     Player.AutoMap=LoadInt(file);
     Player.D_Angle=LoadInt(file);
     Player.SitZ=LoadInt(file);
     Player.RedScreen=LoadInt(file);
     Player.RedScreenOn=LoadInt(file);
     Player.TimeSpacesuit=LoadInt(file);
     Player.TimeMegaSphere=LoadInt(file);
     Player.IDDQD=LoadInt(file);
     Player.MessageNo=LoadInt(file);
     Player.MessageTime=LoadInt(file);
     Player.Level=LoadInt(file);
     //восстанавливаем параметры оружия
     Weapon.Fire_Faze=LoadInt(file);
     Weapon.WeaponNo=LoadInt(file);
     Weapon.Weapon=LoadInt(file);
     Weapon.Dy=LoadInt(file);
     Weapon.Vy=LoadInt(file);
     for(n=0;n<10;n++)
      {
       Weapon.Ammo[n]=LoadInt(file);
       Weapon.MaxAmmo[n]=LoadInt(file);
       Weapon.Attendance[n]=LoadInt(file);
       for(int m=0;m<50;m++)  Weapon.Faze[n][m]=LoadInt(file);
      }
     for(n=0;n<6;n++)
      for(int m=0;m<50;m++)
       {
        Weapon.X[n][m]=LoadInt(file);
        Weapon.Y[n][m]=LoadInt(file);
       }
     //загружаем карту(с учётом Player.Level)
     LoadGame(Player.Level);
     //восстанавливаем параметры сегментов
     for(n=0;n<Info.SegmentNumber;n++)
      {
       Segment[n].Visible=LoadInt(file);
       Segment[n].Type=LoadInt(file);
      }
     //загружаем параметры секторов
     for(n=0;n<Info.SectorNumber;n++)
      {
       Sector[n].UpTexture=LoadInt(file);
       Sector[n].DownTexture=LoadInt(file);
       Sector[n].Up=LoadInt(file);
       Sector[n].Down=LoadInt(file);
       Sector[n].Down1=LoadInt(file);
       Sector[n].Down2=LoadInt(file);
       Sector[n].DZ=LoadInt(file);
       Sector[n].Time=LoadInt(file);
       Sector[n].Type=LoadInt(file);
       Sector[n].Sector=LoadInt(file);
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
       Missile[n]->Faze=faze;
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
       Alien[n]->Coord.Z=z;
       Alien[n]->Coord.Height=height;
       Alien[n]->Coord.Radius=radius;
       Alien[n]->Life=life;
       Alien[n]->Faze=faze;
       Alien[n]->Dx=dx;
       Alien[n]->Dy=dy;
       Alien[n]->Dz=dz;
       Alien[n]->Active=active;
       Alien[n]->Visible=visible;
       Alien[n]->Angle=angle;
       Alien[n]->Time=time;
       Alien[n]->Destroy=destroy;
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
       Thing[n]->Coord.X=x;
       Thing[n]->Coord.Y=y;
       Thing[n]->Coord.Z=z;
       Thing[n]->Faze=faze;
      }
     Player.LoadPos=y;
     Info.TX=new int[1];
     Info.TY=new int[1];
     Info.TType=new int[1];
     Info.AX=new int[1];
     Info.AY=new int[1];
     Info.AType=new int[1];
     Info.ThingNumber=0;
     Info.AlienNumber=0;
     fclose(file);
     GetCursorData(30,26+y*16,dpt);
     return(1);
    }
  }
}
void ClearResurs(void)
{
 int x,y,n;
 for(x=0;x<100;x++)
  for(y=0;y<100;y++) free(Maze.BlockMap[y][x]);
 free(Sector);
 free(Segment);
 FreeTree(Node);
 delete(Info.TX);
 delete(Info.TY);
 delete(Info.TType);
 delete(Info.AX);
 delete(Info.AY);
 delete(Info.AType);
 for(n=0;n<NowThing;n++)
  {
   delete(Thing[n]->Footage);
   delete(Thing[n]);
  }
 for(n=0;n<NowAlien;n++)
  {
   delete(Alien[n]->Footage);
   delete(Alien[n]);
  }
 for(n=0;n<NowMissile;n++) delete(Missile[n]);
 NowThing=0;
 NowAlien=0;
 NowMissile=0;
}
void StartGame(void)
{
 Video.SetRGB_DAC(1,1,1);
 if (Player.TimeSpacesuit!=0) Video.SetRGB_DAC(0.7,1,0.7);
 if (Player.TimeMegaSphere!=0) Video.SetRGB_DAC(0.7,0.7,1);
 Player.Z=Sector[FindSector(Player.X,Player.Y)].Down+64;
 int w=Weapon.WeaponNo;
 char s[255];
 PutBar(Data.Bar,0,0,168);
 itoa(Weapon.Ammo[w],s,10);
 Video.Print(13,180,s);
 itoa(Player.Life,s,10);
 Video.Print(60,180,s);
 Player.P_X=Player.X*65536;
 Player.P_Y=Player.Y*65536;
 int sector=FindSector(Player.X,Player.Y);
 Video.ClearWindow();
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
 Video.PrintToScreen(x,y,text);
 int result;
 KeyTable[21]=0;
 KeyTable[49]=0;
 while(1)
  {
   if (Consol.GetPressKey(21))
    {
     result=1;
     break;
    }
   if (Consol.GetPressKey(49))
    {
     result=0;
     break;
    }
  }
 for(n=0;n<64000;n++) *(vpt+n)=b[n];
 free(b);
 return(result);
}









