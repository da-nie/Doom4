//============================================================================
struct PARAMETER
{
  float X;//координаты
  float Y;
  float Z;//верхняя координата
  int Height;//высота
  int Radius;//радиус
};
struct FOOTAGE
{
 int Faze;//номер фазы
};
//============================================================================
//класс вещей
class THING
{
 public:
  PARAMETER Coord;
  FOOTAGE* Footage;
  int Obstruction;//1-объект препятствие
  int Type;//тип объекта
  int Faze;//номер фазы анимации
  int ThingNo;//номер предмета(по типу)
  char *Particular;//данные графики
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void SetCoord(PARAMETER p);
  void SetFootage(FOOTAGE* f,int max);
  void SetThingToFloor(void);
  void Put(void);
};
void THING::SetCoord(PARAMETER p)
{
 Coord=p;
}
void THING::SetFootage(FOOTAGE* f,int max)
{
 Footage=(FOOTAGE*)malloc(max*sizeof(FOOTAGE));
 for(int n=0;n<max;n++) Footage[n]=f[n];
}
void THING::SetThingToFloor(void)
{
 int ofs1=2+Footage[0].Faze*4;
 int offset=Particular[ofs1]+Particular[ofs1+1]*256+Particular[ofs1+2]*65536+Particular[ofs1+3]*16777216;
 int Len=Particular[offset]+256*Particular[offset+1];
 int Hgt=Particular[offset+2]+256*Particular[offset+3];
 Coord.Z=Sector[FindSector(Coord.X,Coord.Y)].Down+Hgt;
 Coord.Height=Hgt;
 Coord.Radius=Len/3;
}
void THING::Put(void)
{
 int ofs1=2+Footage[Faze].Faze*4;
 int offset=Particular[ofs1]+Particular[ofs1+1]*256+Particular[ofs1+2]*65536+Particular[ofs1+3]*16777216;
 register char *dpt=Particular;
 dpt+=offset;
 int len=dpt[0]+256*dpt[1];
 int hgt=dpt[2]+256*dpt[3];
 offset=3;
 dpt+=offset;
 float cs=CosTable[Player.Angle];
 float ss=SinTable[Player.Angle];
 float x1=Coord.X-Player.X;
 float y1=Coord.Y-Player.Y;
 register int v=x1*ss+y1*cs;
 if (v<=1) return;
 if (v>6400) return;
 register int u=x1*cs-y1*ss;
 int co1=160+160*(u-len/3)/v;
 int co2=160+160*(u+len/3)/v;
 if (co1>319) return;
 if (co2<0) return;
 register int dx=0;
 if (co1!=co2) dx=256*len/(co2-co1);
 register int x=0;
 if (co1<0)
  {
   x=-(dx*co1);
   co1=0;
  }
 if (co2>319) co2=319;
 int ys=84-(-Player.Z+Player.Pz+Coord.Z+Player.SitZ)*100/v;
 int ys1=84-(-Player.Z+Player.Pz+Coord.Z-hgt+Player.SitZ)*100/v;
 if (ys1==ys) return;
 register int dy=0;
 register int y0=0;
 dy=256*hgt/(ys1-ys+1);
 if (ys<1)
  {
   y0=dy*(1-ys);
   ys=1;
  }
 if (ys1>167) ys1=167;
 for(register int n=co1;n<co2;n++,x+=dx)
  {
   int pt=Z_Lines[n].Pointer;
   register unsigned char *vpt=Video.Buffer+n+320*(int)ys;
   register unsigned char *dpt1=dpt+((int)(x>>8))*hgt+1;
   register int y=y0;
   for(register int m=ys;m<=ys1;m++,vpt+=320,y+=dy)
    {
     unsigned char a=*(dpt1+(int)(y>>8));
     if (a==117) continue;
     int st=0;
     for(register int t=0;t<pt;t++)
      {
       if (m>=Z_Lines[n].Begin_Y[t] && m<=Z_Lines[n].End_Y[t])
        {
         if (Z_Lines[n].Z_Value[t]>v) *(vpt)=a;
         st++;
        }
       if (st>=2) break;
      }
     if (st==0) *(vpt)=a;
    }
  }
}
//============================================================================
THING* Thing[500];//таблица указателей на объекты
int NowThing;//всего существует объектов на данный момент
//============================================================================
THING* CreateThing(void);
void DeleteThing(int no);
//============================================================================
THING* CreateThing(void)
{
 Thing[NowThing]=new(THING);
 Thing[NowThing]->Faze=0;
 NowThing++;
 return(Thing[NowThing-1]);
}
void DeleteThing(int no)
{
 delete(Thing[no]->Footage);
 delete(Thing[no]);
 for(int n=no;n<NowThing-1;n++) Thing[n]=Thing[n+1];
 NowThing--;
}
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
void CreateNewRedTorch(int x,int y)
{
 FOOTAGE ft[10];
 ft[0].Faze=20;
 ft[1].Faze=20;
 ft[2].Faze=20;
 ft[3].Faze=21;
 ft[4].Faze=21;
 ft[5].Faze=21;
 ft[6].Faze=22;
 ft[7].Faze=22;
 ft[8].Faze=22;
 ft[9].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,10);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=1;
 t->Type=1;
 t->ThingNo=1;
}
void CreateNewBlueTorch(int x,int y)
{
 FOOTAGE ft[10];
 ft[0].Faze=39;
 ft[1].Faze=39;
 ft[2].Faze=39;
 ft[3].Faze=40;
 ft[4].Faze=40;
 ft[5].Faze=40;
 ft[6].Faze=41;
 ft[7].Faze=41;
 ft[8].Faze=41;
 ft[9].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,10);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=1;
 t->Type=1;
 t->ThingNo=2;
}
void CreateNewButt(int x,int y)
{
 FOOTAGE ft[10];
 ft[0].Faze=17;
 ft[1].Faze=17;
 ft[2].Faze=17;
 ft[3].Faze=18;
 ft[4].Faze=18;
 ft[5].Faze=18;
 ft[6].Faze=19;
 ft[7].Faze=19;
 ft[8].Faze=19;
 ft[9].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,10);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=1;
 t->Type=1;
 t->ThingNo=3;
}
void CreateNewBattery(int x,int y)
{
 FOOTAGE ft[20];
 ft[0].Faze=4;
 ft[1].Faze=4;
 ft[2].Faze=4;
 ft[3].Faze=5;
 ft[4].Faze=5;
 ft[5].Faze=5;
 ft[6].Faze=6;
 ft[7].Faze=6;
 ft[8].Faze=6;
 ft[9].Faze=7;
 ft[10].Faze=7;
 ft[11].Faze=7;
 ft[12].Faze=6;
 ft[13].Faze=6;
 ft[14].Faze=6;
 ft[15].Faze=5;
 ft[16].Faze=5;
 ft[17].Faze=5;
 ft[18].Faze=4;
 ft[19].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,20);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=2;
 t->ThingNo=4;
}
void CreateNewMedicine(int x,int y)
{
 FOOTAGE ft[15];
 ft[0].Faze=0;
 ft[1].Faze=0;
 ft[2].Faze=0;
 ft[3].Faze=0;
 ft[4].Faze=0;
 ft[5].Faze=0;
 ft[6].Faze=0;
 ft[7].Faze=1;
 ft[8].Faze=1;
 ft[9].Faze=1;
 ft[10].Faze=1;
 ft[11].Faze=1;
 ft[12].Faze=1;
 ft[13].Faze=1;
 ft[14].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,15);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=3;
 t->ThingNo=5;
}
void CreateNewSuperMedicine(int x,int y)
{
 FOOTAGE ft[15];
 ft[0].Faze=2;
 ft[1].Faze=2;
 ft[2].Faze=2;
 ft[3].Faze=2;
 ft[4].Faze=2;
 ft[5].Faze=2;
 ft[6].Faze=2;
 ft[7].Faze=3;
 ft[8].Faze=3;
 ft[9].Faze=3;
 ft[10].Faze=3;
 ft[11].Faze=3;
 ft[12].Faze=3;
 ft[13].Faze=3;
 ft[14].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,15);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=4;
 t->ThingNo=6;
}
void CreateNewGun(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=8;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=5;
 t->ThingNo=7;
}
void CreateNewMachineGun(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=9;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=6;
 t->ThingNo=8;
}
void CreateNewPlasmaGun(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=10;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=7;
 t->ThingNo=9;
}
void CreateNewGunAmmo(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=11;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=8;
 t->ThingNo=10;
}
void CreateNewMGunAmmo(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=12;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=11;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=9;
 t->ThingNo=11;
}
void CreateNewGreyAltar(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=13;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=1;
 t->Type=1;
 t->ThingNo=12;
}
void CreateNewRedAltar(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=14;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=1;
 t->Type=1;
 t->ThingNo=13;
}
void CreateNewSuppository(int x,int y)
{
 FOOTAGE ft[13];
 ft[0].Faze=15;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=1;
 t->Type=1;
 t->ThingNo=14;
}
void CreateNewPistolAmmo(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=16;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=10;
 t->ThingNo=15;
}
void CreateNewLocMap(int x,int y)
{
 FOOTAGE ft[9];
 ft[0].Faze=23;
 ft[1].Faze=23;
 ft[2].Faze=23;
 ft[3].Faze=23;
 ft[4].Faze=24;
 ft[5].Faze=24;
 ft[6].Faze=24;
 ft[7].Faze=24;
 ft[8].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,9);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=11;
 t->ThingNo=16;
}
void CreateNewSpacesuit(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=25;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=12;
 t->ThingNo=17;
}
void CreateNewMiniSuppository(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=26;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=1;
 t->ThingNo=18;
}
void CreateNewBlood(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=28;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=1;
 t->ThingNo=19;
}
void CreateNewMiniBlood(int x,int y)
{
 FOOTAGE ft[2];
 ft[0].Faze=27;
 ft[1].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,2);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=1;
 t->ThingNo=20;
}
void CreateNewMegaSphere(int x,int y)
{
 FOOTAGE ft[13];
 ft[0].Faze=29;
 ft[1].Faze=29;
 ft[2].Faze=29;
 ft[3].Faze=30;
 ft[4].Faze=30;
 ft[5].Faze=30;
 ft[6].Faze=31;
 ft[7].Faze=31;
 ft[8].Faze=31;
 ft[9].Faze=30;
 ft[10].Faze=30;
 ft[11].Faze=30;
 ft[12].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,13);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=13;
 t->ThingNo=21;
}
void CreateNewSuperSphere(int x,int y)
{
 FOOTAGE ft[13];
 ft[0].Faze=32;
 ft[1].Faze=32;
 ft[2].Faze=32;
 ft[3].Faze=33;
 ft[4].Faze=33;
 ft[5].Faze=33;
 ft[6].Faze=34;
 ft[7].Faze=34;
 ft[8].Faze=34;
 ft[9].Faze=33;
 ft[10].Faze=33;
 ft[11].Faze=33;
 ft[12].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,13);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=14;
 t->ThingNo=22;
}
void CreateNewHealingWater(int x,int y)
{
 FOOTAGE ft[19];
 ft[0].Faze=35;
 ft[1].Faze=35;
 ft[2].Faze=35;
 ft[3].Faze=36;
 ft[4].Faze=36;
 ft[5].Faze=36;
 ft[6].Faze=37;
 ft[7].Faze=37;
 ft[8].Faze=37;
 ft[9].Faze=38;
 ft[10].Faze=38;
 ft[11].Faze=38;
 ft[12].Faze=37;
 ft[13].Faze=37;
 ft[14].Faze=37;
 ft[15].Faze=36;
 ft[16].Faze=36;
 ft[17].Faze=36;
 ft[18].Faze=-1;
 PARAMETER p;
 p.Radius=10;
 THING *t;
 p.X=x;
 p.Y=y;
 t=CreateThing();
 t->SetCoord(p);
 t->SetFootage(ft,19);
 t->Particular=Data.Thing;
 t->SetThingToFloor();
 t->Obstruction=0;
 t->Type=15;
 t->ThingNo=23;
}
//============================================================================
//класс врагов
class ALIEN
{
 public:
  PARAMETER Coord;
  FOOTAGE* Footage;//кадры анимации
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
  void SetCoord(PARAMETER p);
  void SetFootage(FOOTAGE* f,int max);
  void SetAlienToFloor(void);
  void Put(void);
};
void ALIEN::SetCoord(PARAMETER p)
{
 Coord=p;
}
void ALIEN::SetFootage(FOOTAGE* f,int max)
{
 Footage=(FOOTAGE*)malloc(max*sizeof(FOOTAGE));
 for(int n=0;n<max;n++) Footage[n]=f[n];
}
void ALIEN::SetAlienToFloor(void)
{
 int ofs1=2+Footage[0].Faze*4;
 int offset=Particular[ofs1]+Particular[ofs1+1]*256+Particular[ofs1+2]*65536+Particular[ofs1+3]*16777216;
 int Len=Particular[offset]+256*Particular[offset+1];
 int Hgt=Particular[offset+2]+256*Particular[offset+3];
 Coord.Z=Sector[FindSector(Coord.X,Coord.Y)].Down+Hgt*1.5;
 Coord.Height=Hgt*1.5;
 Coord.Radius=Len/2;
}
void ALIEN::Put(void)
{
 int ofs1=2+Footage[Faze].Faze*4;
 int offset=Particular[ofs1]+Particular[ofs1+1]*256+Particular[ofs1+2]*65536+Particular[ofs1+3]*16777216;
 register char *dpt=Particular;
 dpt+=offset;
 int len=dpt[0]+256*dpt[1];
 int hgt=dpt[2]+256*dpt[3];
 offset=3;
 dpt+=offset;
 float cs=CosTable[Player.Angle];
 float ss=SinTable[Player.Angle];
 float x1=Coord.X-Player.X;
 float y1=Coord.Y-Player.Y;
 register int v=x1*ss+y1*cs;
 if (v<=1) return;
 if (v>6400) return;
 register int u=x1*cs-y1*ss;
 int co1=160+160*(u-len/2)/v;
 int co2=160+160*(u+len/2)/v;
 if (co1>319) return;
 if (co2<0) return;
 register int dx=0;
 if (co1!=co2) dx=256*len/(co2-co1);
 register int x=0;
 if (co1<0)
  {
   x=-(dx*co1);
   co1=0;
  }
 if (co2>319) co2=319;
 int ys=84-(-Player.Z+Player.Pz+Coord.Z+Player.SitZ)*100/v;
 int ys1=84-(-Player.Z+Player.Pz+Coord.Z-Coord.Height+Player.SitZ)*100/v;
 if (ys1==ys) return;
 register int dy=0;
 register int y0=0;
 dy=256*hgt/(ys1-ys+1);
 if (ys<1)
  {
   y0=dy*(1-ys);
   ys=1;
  }
 if (ys1>167) ys1=167;
 int active=0;
 for(register int n=co1;n<co2;n++,x+=dx)
  {
   int pt=Z_Lines[n].Pointer;
   register unsigned char *vpt=Video.Buffer+n+320*(int)ys;
   register unsigned char *dpt1=dpt+((int)(x>>8))*hgt+1;
   register int y=y0;
   for(register int m=ys;m<=ys1;m++,vpt+=320,y+=dy)
    {
     unsigned char a=*(dpt1+(int)(y>>8));
     if (a==117) continue;
     int st=0;
     for(register int t=0;t<pt;t++)
      {
       if (m>=Z_Lines[n].Begin_Y[t] && m<=Z_Lines[n].End_Y[t])
        {
         if (Z_Lines[n].Z_Value[t]>v)
          {
           *(vpt)=a;
           active=1;
          }
         st++;
        }
       if (st>=2) break;
      }
     if (st==0)
      {
       active=1;
       *(vpt)=a;
      }
    }
  }
 if (active==1)
  {
   Visible=1;
   Active=1;
  }
 else Visible=0;
}
//============================================================================
ALIEN* Alien[500];//таблица указателей на врагов
int NowAlien;//всего существует врагов на данный момент
//============================================================================
ALIEN* CreateAlien(void);
void DeleteAlien(int no);
//============================================================================
ALIEN* CreateAlien(void)
{
 Alien[NowAlien]=new(ALIEN);
 Alien[NowAlien]->Faze=0;
 NowAlien++;
 return(Alien[NowAlien-1]);
}
void DeleteAlien(int no)
{
 delete(Alien[no]->Footage);
 delete(Alien[no]);
 for(int n=no;n<NowAlien-1;n++) Alien[n]=Alien[n+1];
 NowAlien--;
}
//============================================================================
void CreateNewDemon1(int x,int y);
void CreateNewDemon2(int x,int y);
//============================================================================
void CreateNewDemon1(int x,int y)
{
 FOOTAGE ft[33];
 ft[0].Faze=0;
 ft[1].Faze=-1;
 ft[2].Faze=0;
 ft[3].Faze=0;
 ft[4].Faze=0;
 ft[5].Faze=1;
 ft[6].Faze=1;
 ft[7].Faze=1;
 ft[8].Faze=1;
 ft[9].Faze=2;
 ft[10].Faze=2;
 ft[11].Faze=2;
 ft[12].Faze=2;
 ft[13].Faze=3;
 ft[14].Faze=3;
 ft[15].Faze=3;
 ft[16].Faze=3;
 ft[17].Faze=2;
 ft[18].Faze=2;
 ft[19].Faze=2;
 ft[20].Faze=2;
 ft[21].Faze=-1;
 ft[22].Faze=4;
 ft[23].Faze=4;
 ft[24].Faze=4;
 ft[25].Faze=4;
 ft[26].Faze=4;
 ft[27].Faze=5;
 ft[28].Faze=5;
 ft[29].Faze=5;
 ft[30].Faze=5;
 ft[31].Faze=6;
 ft[32].Faze=-1;
 PARAMETER p;
 ALIEN *t;
 p.X=x;
 p.Radius=30;
 p.Y=y;
 t=CreateAlien();
 t->SetCoord(p);
 t->SetFootage(ft,33);
 t->Destroy=22;
 t->Fire=2;
 t->Particular=Data.Alien;
 t->SetAlienToFloor();
 t->Life=50;
 t->Dx=0;
 t->Dy=0;
 t->Dz=0;
 t->Angle=0;
 t->Time=0;
 t->Active=0;
 t->Visible=0;
 t->Type=1;
}
void CreateNewDemon2(int x,int y)
{
 FOOTAGE ft[44];
 ft[0].Faze=7;
 ft[1].Faze=7;
 ft[2].Faze=7;
 ft[3].Faze=8;
 ft[4].Faze=8;
 ft[5].Faze=8;
 ft[6].Faze=9;
 ft[7].Faze=9;
 ft[8].Faze=9;
 ft[9].Faze=8;
 ft[10].Faze=8;
 ft[11].Faze=8;
 ft[12].Faze=-1;
 ft[13].Faze=10;
 ft[14].Faze=10;
 ft[15].Faze=10;
 ft[16].Faze=10;
 ft[17].Faze=11;
 ft[18].Faze=11;
 ft[19].Faze=11;
 ft[20].Faze=11;
 ft[21].Faze=12;
 ft[22].Faze=12;
 ft[23].Faze=12;
 ft[24].Faze=12;
 ft[25].Faze=11;
 ft[26].Faze=11;
 ft[27].Faze=-1;
 ft[28].Faze=13;
 ft[29].Faze=13;
 ft[30].Faze=13;
 ft[31].Faze=14;
 ft[32].Faze=14;
 ft[33].Faze=14;
 ft[34].Faze=15;
 ft[35].Faze=15;
 ft[36].Faze=15;
 ft[37].Faze=16;
 ft[38].Faze=16;
 ft[39].Faze=16;
 ft[40].Faze=17;
 ft[41].Faze=17;
 ft[42].Faze=17;
 ft[43].Faze=-1;
 PARAMETER p;
 ALIEN *t;
 p.X=x;
 p.Radius=30;
 p.Y=y;
 t=CreateAlien();
 t->SetCoord(p);
 t->SetFootage(ft,44);
 t->Destroy=28;
 t->Fire=13;
 t->Particular=Data.Alien;
 t->SetAlienToFloor();
 t->Life=100;
 t->Dx=0;
 t->Dy=0;
 t->Dz=0;
 t->Angle=0;
 t->Time=0;
 t->Active=0;
 t->Visible=0;
 t->Type=2;
}
//============================================================================
//класс снарядов
struct MISSILE
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
 int WeaponNo;//номер оружия(тип), из которого выпущен снаряд
 int Object;//кто стрелял(-1=игрок);
 char *Particular;
 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 void Put(void);
};
//============================================================================
MISSILE* Missile[500];//таблица указателей на снаряды
int NowMissile;//всего снарядов на данный момент
//============================================================================
MISSILE* CreateMissile(void);
void DeleteMissile(int no);
//============================================================================
MISSILE* CreateMissile(void)
{
 Missile[NowMissile]=new(MISSILE);
 NowMissile++;
 return(Missile[NowMissile-1]);
}
void DeleteMissile(int no)
{
 delete(Missile[no]);
 for(int n=no;n<NowMissile-1;n++) Missile[n]=Missile[n+1];
 NowMissile--;
}
void MISSILE::Put(void)
{
 int ofs1=2+Footage[Faze]*4;
 int offset=Particular[ofs1]+Particular[ofs1+1]*256+Particular[ofs1+2]*65536+Particular[ofs1+3]*16777216;
 register char *dpt=Particular;
 dpt+=offset;
 int len=dpt[0]+256*dpt[1];
 int hgt=dpt[2]+256*dpt[3];
 offset=3;
 dpt+=offset;
 float cs=CosTable[Player.Angle];
 float ss=SinTable[Player.Angle];
 float x1=X-Player.X;
 float y1=Y-Player.Y;
 register int v=x1*ss+y1*cs;
 if (v<=1) return;
 if (v>6400) return;
 register int u=x1*cs-y1*ss;
 int co1=160+160*(u-len/3)/v;
 int co2=160+160*(u+len/3)/v;
 if (co1>319) return;
 if (co2<0) return;
 register int dx=0;
 if (co1!=co2) dx=256*len/(co2-co1);
 register int x=0;
 if (co1<0)
  {
   x=-(dx*co1);
   co1=0;
  }
 if (co2>319) co2=319;
 int ys=84-(-Player.Z+Player.Pz+Z+Player.SitZ)*100/v;
 int ys1=84-(-Player.Z+Player.Pz+Z-hgt+Player.SitZ)*100/v;
 if (ys1==ys) return;
 register int dy=0;
 register int y0=0;
 dy=256*hgt/(ys1-ys+1);
 if (ys<1)
  {
   y0=dy*(1-ys);
   ys=1;
  }
 if (ys1>167) ys1=167;
 for(register int n=co1;n<co2;n++,x+=dx)
  {
   int pt=Z_Lines[n].Pointer;
   register unsigned char *vpt=Video.Buffer+n+320*(int)ys;
   register unsigned char *dpt1=dpt+((int)(x>>8))*hgt+1;
   register int y=y0;
   for(register int m=ys;m<=ys1;m++,vpt+=320,y+=dy)
    {
     unsigned char a=*(dpt1+(int)(y>>8));
     if (a==117) continue;
     int st=0;
     for(register int t=0;t<pt;t++)
      {
       if (m>=Z_Lines[n].Begin_Y[t] && m<=Z_Lines[n].End_Y[t])
        {
         if (Z_Lines[n].Z_Value[t]>v) *(vpt)=a;
         st++;
        }
       if (st>=2) break;
      }
     if (st==0) *(vpt)=a;
    }
  }
}
//============================================================================
void CreateNewRocket(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewPlasmaGunFire(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewGunFire(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
void CreateNewBlueBall(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object);
//============================================================================
void CreateNewRocket(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object)
{
 MISSILE *m=CreateMissile();
 m->Footage[0]=0;
 m->Footage[1]=1;
 m->Footage[2]=2;
 m->Footage[3]=3;
 m->Footage[4]=-1;
 m->Footage[5]=12;
 m->Footage[6]=12;
 m->Footage[7]=12;
 m->Footage[8]=13;
 m->Footage[9]=13;
 m->Footage[10]=13;
 m->Footage[11]=14;
 m->Footage[12]=14;
 m->Footage[13]=14;
 m->Footage[14]=-1;
 m->X=x;
 m->Y=y;
 m->Z=z;
 m->Dx=dx;
 m->Dy=dy;
 m->Dz=dz;
 m->Speed=speed;
 m->Power=power;
 m->Faze=0;
 m->WeaponNo=weapon;
 m->Object=object;
 m->Radius=10;
 m->Particular=Data.Fire;
 m->Destroy=5;
}
void CreateNewPlasmaGunFire(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object)
{
 MISSILE *m=CreateMissile();
 m->Footage[0]=4;
 m->Footage[1]=5;
 m->Footage[2]=6;
 m->Footage[3]=7;
 m->Footage[4]=-1;
 m->Footage[5]=15;
 m->Footage[6]=15;
 m->Footage[7]=15;
 m->Footage[8]=16;
 m->Footage[9]=16;
 m->Footage[10]=16;
 m->Footage[11]=17;
 m->Footage[12]=17;
 m->Footage[13]=17;
 m->Footage[14]=-1;
 m->X=x;
 m->Y=y;
 m->Z=z;
 m->Dx=dx;
 m->Dy=dy;
 m->Dz=dz;
 m->Speed=speed;
 m->Power=power;
 m->Faze=0;
 m->WeaponNo=weapon;
 m->Object=object;
 m->Radius=5;
 m->Particular=Data.Fire;
 m->Destroy=5;
}
void CreateNewGunFire(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object)
{
 MISSILE *m=CreateMissile();
 m->Footage[0]=8;
 m->Footage[1]=9;
 m->Footage[2]=10;
 m->Footage[3]=11;
 m->Footage[4]=-1;
 m->X=x;
 m->Y=y;
 m->Z=z;
 m->Dx=dx;
 m->Dy=dy;
 m->Dz=dz;
 m->Speed=speed;
 m->Power=power;
 m->Faze=0;
 m->WeaponNo=weapon;
 m->Object=object;
 m->Radius=5;
 m->Particular=Data.Fire;
 m->Destroy=0;
}
void CreateNewBlueBall(float x,float y,float z,float dx,float dy,float dz,int speed,int power,int weapon,int object)
{
 MISSILE *m=CreateMissile();
 m->Footage[0]=18;
 m->Footage[1]=19;
 m->Footage[2]=20;
 m->Footage[3]=21;
 m->Footage[4]=-1;
 m->Footage[5]=22;
 m->Footage[6]=22;
 m->Footage[7]=22;
 m->Footage[8]=23;
 m->Footage[9]=23;
 m->Footage[10]=23;
 m->Footage[11]=24;
 m->Footage[12]=24;
 m->Footage[13]=24;
 m->Footage[14]=-1;
 m->X=x;
 m->Y=y;
 m->Z=z;
 m->Dx=dx;
 m->Dy=dy;
 m->Dz=dz;
 m->Speed=speed;
 m->Power=power;
 m->Faze=0;
 m->WeaponNo=weapon;
 m->Object=object;
 m->Radius=10;
 m->Particular=Data.Fire;
 m->Destroy=5;
}






