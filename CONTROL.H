//=========================Управление игрой===================================
int Timer;
//============================================================================
int Impact(float x,float y,float z,int radius,int height,int objno);
int ImpactAll(float x,float y,float z,int radius,int height,int objno,int **collizionarray,int *collizionamount);
int Intersection(float xa1,float ya1,float xa2,float ya2,float xb1,float yb1,float xb2,float yb2,float* xo,float* yo);
void ImpactPlayerToThing(float x,float y,float z,int radius,int height);
int CheckTouchLine(int xb,int yb,int x1,int y1,int z1,int x2,int y2,int z2,float dz,int height,int man,float* xo,float* yo,int *object);
int CheckWay(float x1,float y1,float z1,float x2,float y2,float z2,int radius,int height,int man,float* xo,float *yo,int *object);
int ImpactPlayerToWall(float x,float y,float z);
void SlideTraverse(float xp1,float yp1,float xp2,float yp2);
void InitPlayer(void);
int DistanceOfVector(float xa,float ya,float xb,float yb);
void AnimateDoor(int k);
void AnimateGantry(int k);
void AnimateFloor_Flow(void);
void SetThing(void);
void AnimateThing(void);
void SpecialSector(int sector);
void MoveAlien(void);
void ColorEffect(void);
void AnimateFace(int t);
void PutMessage(void);
void AnimateAll(int sector);
int ViewAll(void);
int KeyboardControl(int sector);
int Control(void);
int Use(int w);
void RotateLeft(void);
void RotateRight(void);
void MoveUp(int type);
void MoveDown(int type);
void MoveLeft(void);
void MoveRight(void);
void Fallen(int sector);
void PutWeapon(void);
void AnimateWeapon(void);
void Fire(void);
void ChangeWeapon(int number);
void AnimateMissile(void);
void PutStatistik();
float FindAlien(void);
//===========================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает номер сегмента либо 0, с которым произошло столкновение//
//объекта с координатами x,y,z.                                     //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int Impact(float x,float y,float z,int radius,int height,int objno)//z-верх объекта
{
 int mindist=1000000000;//нужна ближайшая линия пересечения
 int dist;
 int line;
 int top=z;
 int bottom=z-height;
 int xb1=x/128-1;//соседние блоки
 int yb1=y/128-1;
 int xb2=x/128+1;
 int yb2=y/128+1;
 if (xb1<0) xb1=0;//если вышли за границу поля
 if (yb1<0) yb1=0;
 if (xb2>99) xb2=99;
 if (yb2>99) yb2=99;
 int n,m,u,l,k;
 for(n=xb1;n<=xb2;n++)
  for(m=yb1;m<=yb2;m++)
   {
    u=Maze.BlockMap[m][n][0];
    for(l=1;l<=u;l++)
     {
      k=Maze.BlockMap[m][n][l];
      float xw1=Segment[k].X1-x;//координаты стен
      float xw2=Segment[k].X2-x;
      float yw1=Segment[k].Y1-y;
      float yw2=Segment[k].Y2-y;
      float a=xw2-xw1;
      float b=yw2-yw1;
      float kd=a*a+b*b;
      float ld=2*a*xw1+2*b*yw1;
      float md=xw1*xw1+yw1*yw1-radius*radius;
      float d=ld*ld-4*kd*md;//дескриминант
      if (d<0) continue;//нет пересечения
      float sd=sqrt(d);
      float l1=(-ld+sd)/(2*kd);
      float l2=(-ld-sd)/(2*kd);
      float xp1=xw1+l1*a;//2 точки пересечения
      float yp1=yw1+l1*b;
      float xp2=xw1+l2*a;
      float yp2=yw1+l2*b;
      if (((xp1>=xw1 && xp1<=xw2) || (xp1>=xw2 && xp1<=xw1)) &&
          ((yp1>=yw1 && yp1<=yw2) || (yp1>=yw2 && yp1<=yw1)))
           {
            if (Segment[k].Frontier!=0)//линия раздела
             {
              //проверим по высоте
              int down1=Sector[Segment[k].Sector1].Down-32;
              int up1=Sector[Segment[k].Sector1].Up;
              int down2=Sector[Segment[k].Sector2].Down-32;
              int up2=Sector[Segment[k].Sector2].Up;
              if (!(up1>=top && bottom>=down1) || up2-down2<=height ||
                  !(up2>=top && bottom>=down2) || up1-down1<=height)
                   {
                    dist=sqrt(xp1*xp1+yp1*yp1);
                    if (dist<=mindist)
                     {
                      mindist=dist;
                      line=k;
                     }
                   }
             }
            else
             {
              dist=sqrt(xp1*xp1+yp1*yp1);
              if (dist<=mindist)
               {
                mindist=dist;
                line=k;
               }
             }
           }
      if (((xp2>=xw1 && xp2<=xw2) || (xp2>=xw2 && xp2<=xw1)) &&
          ((yp2>=yw1 && yp2<=yw2) || (yp2>=yw2 && yp2<=yw1)))
           {
            if (Segment[k].Frontier!=0)//линия раздела
             {
              //проверим по высоте
              int down1=Sector[Segment[k].Sector1].Down-32;
              int up1=Sector[Segment[k].Sector1].Up;
              int down2=Sector[Segment[k].Sector2].Down-32;
              int up2=Sector[Segment[k].Sector2].Up;
              if (!(up1>top && bottom>=down1) || up2-down2<=height ||
                  !(up2>top && bottom>=down2) || up1-down1<=height)
                   {
                    dist=sqrt(xp2*xp2+yp2*yp2);
                    if (dist<=mindist)
                     {
                      mindist=dist;
                      line=k;
                     }
                   }
             }
            else
             {
              dist=sqrt(xp2*xp2+yp2*yp2);
              if (dist<=mindist)
               {
                mindist=dist;
                line=k;
               }
             }
           }
     }
   }
 if (mindist<1000000000) return(line+1);
 for(n=0;n<NowThing;n++)
  {
   if (Thing[n]->Coord.Z<bottom || Thing[n]->Coord.Z-Thing[n]->Coord.Height>top) continue;
   int x1=Thing[n]->Coord.X-x;
   int y1=Thing[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<Thing[n]->Coord.Radius+radius)
    {
     if (Thing[n]->Obstruction!=0) return(-1);//объект является препятствием
    }
  }
 //проверим столкновение с врагами
 for(n=0;n<NowAlien;n++)
  {
   if (n==objno) continue;
   if (Alien[n]->Life==0) continue;
   if (Alien[n]->Coord.Z<bottom || Alien[n]->Coord.Z-Alien[n]->Coord.Height>top) continue;
   int x1=Alien[n]->Coord.X-x;
   int y1=Alien[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<Alien[n]->Coord.Radius+radius) return(-1);
  }
 if (objno!=-1)//для врагов проверим столкновение с игроком
  {
   if (Alien[objno]->Life!=0 &&
      !(Alien[objno]->Coord.Z<Player.Z-64 || Alien[objno]->Coord.Z-Alien[objno]->Coord.Height>Player.Z-Player.SitZ))
     {
      int x1=Alien[objno]->Coord.X-Player.X;
      int y1=Alien[objno]->Coord.Y-Player.Y;
      int len=sqrt(x1*x1+y1*y1);
      if (len<Alien[objno]->Coord.Radius+20) return(-1);
     }
  }
 return(0);
}
int ImpactAll(float x,float y,float z,int radius,int height,int objno,int **collizionarray,int *collizionamount)
{
 int collizion_amount=0;
 int *collizion_array=(int*)malloc(sizeof(int));
 int top=z;
 int bottom=z-height;
 int xb1=x/128-1;//соседние блоки
 int yb1=y/128-1;
 int xb2=x/128+1;
 int yb2=y/128+1;
 if (xb1<0) xb1=0;//если вышли за границу поля
 if (yb1<0) yb1=0;
 if (xb2>99) xb2=99;
 if (yb2>99) yb2=99;
 int n,m,u,l,k;
 for(n=xb1;n<=xb2;n++)
  for(m=yb1;m<=yb2;m++)
   {
    u=Maze.BlockMap[m][n][0];
    for(l=1;l<=u;l++)
     {
      k=Maze.BlockMap[m][n][l];
      float xw1=Segment[k].X1-x;//координаты стен
      float xw2=Segment[k].X2-x;
      float yw1=Segment[k].Y1-y;
      float yw2=Segment[k].Y2-y;
      float a=xw2-xw1;
      float b=yw2-yw1;
      float kd=a*a+b*b;
      float ld=2*a*xw1+2*b*yw1;
      float md=xw1*xw1+yw1*yw1-radius*radius;
      float d=ld*ld-4*kd*md;//дескриминант
      if (d<0) continue;//нет пересечения
      float sd=sqrt(d);
      float l1=(-ld+sd)/(2*kd);
      float l2=(-ld-sd)/(2*kd);
      float xp1=xw1+l1*a;//2 точки пересечения
      float yp1=yw1+l1*b;
      float xp2=xw1+l2*a;
      float yp2=yw1+l2*b;
      if (((xp1>=xw1 && xp1<=xw2) || (xp1>=xw2 && xp1<=xw1)) &&
          ((yp1>=yw1 && yp1<=yw2) || (yp1>=yw2 && yp1<=yw1)))
           {
            if (Segment[k].Frontier!=0)//линия раздела
             {
              //проверим по высоте
              int down1=Sector[Segment[k].Sector1].Down-32;
              int up1=Sector[Segment[k].Sector1].Up;
              int down2=Sector[Segment[k].Sector2].Down-32;
              int up2=Sector[Segment[k].Sector2].Up;
              if (!(up1>=top && bottom>=down1) || up2-down2<=height ||
                  !(up2>=top && bottom>=down2) || up1-down1<=height)
                   {
                    collizion_array=(int *)realloc(collizion_array,sizeof(int)*(collizion_amount+1));
                    collizion_array[collizion_amount]=k;
                    collizion_amount++;
                   }
             }
            else
             {
              collizion_array=(int *)realloc(collizion_array,sizeof(int)*(collizion_amount+1));
              collizion_array[collizion_amount]=k;
              collizion_amount++;
             }
           }
      if (((xp2>=xw1 && xp2<=xw2) || (xp2>=xw2 && xp2<=xw1)) &&
          ((yp2>=yw1 && yp2<=yw2) || (yp2>=yw2 && yp2<=yw1)))
           {
            if (Segment[k].Frontier!=0)//линия раздела
             {
              //проверим по высоте
              int down1=Sector[Segment[k].Sector1].Down-32;
              int up1=Sector[Segment[k].Sector1].Up;
              int down2=Sector[Segment[k].Sector2].Down-32;
              int up2=Sector[Segment[k].Sector2].Up;
              if (!(up1>top && bottom>=down1) || up2-down2<=height ||
                  !(up2>top && bottom>=down2) || up1-down1<=height)
                   {
                    collizion_array=(int *)realloc(collizion_array,sizeof(int)*(collizion_amount+1));
                    collizion_array[collizion_amount]=k;
                    collizion_amount++;
                   }
             }
            else
             {
              collizion_array=(int *)realloc(collizion_array,sizeof(int)*(collizion_amount+1));
              collizion_array[collizion_amount]=k;
              collizion_amount++;
             }
           }
     }
   }
 if (collizion_amount>0)
  {
   *collizionamount=collizion_amount;
   *collizionarray=collizion_array;
   return(1);
  }
 for(n=0;n<NowThing;n++)
  {
   if (Thing[n]->Coord.Z<bottom || Thing[n]->Coord.Z-Thing[n]->Coord.Height>top) continue;
   int x1=Thing[n]->Coord.X-x;
   int y1=Thing[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<Thing[n]->Coord.Radius+radius)
    {
     if (Thing[n]->Obstruction!=0)
      {
       *collizionamount=collizion_amount;
       *collizionarray=collizion_array;
       return(-1);//объект является препятствием
      }
    }
  }
 //проверим столкновение с врагами
 for(n=0;n<NowAlien;n++)
  {
   if (n==objno) continue;
   if (Alien[n]->Life==0) continue;
   if (Alien[n]->Coord.Z<bottom || Alien[n]->Coord.Z-Alien[n]->Coord.Height>top) continue;
   int x1=Alien[n]->Coord.X-x;
   int y1=Alien[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<Alien[n]->Coord.Radius+radius)
    {
     *collizionamount=collizion_amount;
     *collizionarray=collizion_array;
     return(-1);
    }
  }
 if (objno!=-1)//для врагов проверим столкновение с игроком
  {
   if (Alien[objno]->Life!=0 &&
      !(Alien[objno]->Coord.Z<Player.Z-64 || Alien[objno]->Coord.Z-Alien[objno]->Coord.Height>Player.Z-Player.SitZ))
     {
      int x1=Alien[objno]->Coord.X-Player.X;
      int y1=Alien[objno]->Coord.Y-Player.Y;
      int len=sqrt(x1*x1+y1*y1);
      if (len<Alien[objno]->Coord.Radius+20)
      {
       *collizionamount=collizion_amount;
       *collizionarray=collizion_array;
       return(-1);
      }
     }
  }
 *collizionamount=collizion_amount;
 *collizionarray=collizion_array;
 return(0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает в xo,yo точку пересечения двух прямых. Если пересечения//
//не было, то возвращает 0.                                         //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int Intersection(float xa1,float ya1,float xa2,float ya2,float xb1,float yb1,float xb2,float yb2,float* xo,float* yo)
{
 float lx=xa2-xa1;
 float ly=ya2-ya1;
 float kx=xb2-xb1;
 float ky=yb2-yb1;
 float xl=xa1;
 float yl=ya1;
 float xk=xb1;
 float yk=yb1;
 float d=-ly*kx+lx*ky;//определитель
 if (d==0) return(0);//не пересекаются
 float d1=(ly*xl-lx*yl)*(-kx)+lx*(ky*xk-kx*yk);
 float d2=ly*(ky*xk-kx*yk)-ky*(ly*xl-lx*yl);
 float xot=d1/d;//решаем методом Крамера
 float yot=d2/d;
 *xo=xot;
 *yo=yot;
 return(1);
}
void ImpactPlayerToThing(float x,float y,float z,int radius,int height)
{
 int top=z;
 int bottom=z-height;
 for(int n=0;n<NowThing;n++)
  {
   if (Thing[n]->Coord.Z<bottom || Thing[n]->Coord.Z-Thing[n]->Coord.Height>top) continue;
   int x1=Thing[n]->Coord.X-x;
   int y1=Thing[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<Thing[n]->Coord.Radius+radius)
    {
     if (Thing[n]->Type==3)//аптечка
      {
       if (Player.Life<100)
        {
         Player.Life+=20;
         if (Player.Life>100) Player.Life=100;
         DeleteThing(n);
         Player.MessageNo=4;
         Player.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (Thing[n]->Type==4)//супер аптечка
      {
       if (Player.Life<100)
        {
         Player.Life=100;
         DeleteThing(n);
         Player.MessageNo=5;
         Player.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (Thing[n]->Type==5)//ружъё
      {
       int i=0;
       if (Weapon.Attendance[2]!=0 && Weapon.Ammo[2]!=0) i=1;
       if (Weapon.Attendance[3]!=0 && Weapon.Ammo[3]!=0) i=1;
       if (Weapon.Attendance[4]!=0 && Weapon.Ammo[4]!=0) i=1;
       if (Weapon.Ammo[2]<Weapon.MaxAmmo[2] || Weapon.Attendance[2]==0)
        {
         Weapon.Attendance[2]=1;
         Weapon.Ammo[2]+=30;
         if (Weapon.Ammo[2]>Weapon.MaxAmmo[2]) Weapon.Ammo[2]=Weapon.MaxAmmo[2];
         DeleteThing(n);
         Player.MessageNo=9;
         Player.MessageTime=50;
         PutStatistik();
         if (i==0) ChangeWeapon(2);
         continue;
        }
      }
     if (Thing[n]->Type==6)//пулемёт
      {
       int i=0;
       if (Weapon.Attendance[3]!=0 && Weapon.Ammo[3]!=0) i=1;
       if (Weapon.Attendance[4]!=0 && Weapon.Ammo[4]!=0) i=1;
       if (Weapon.Ammo[3]<Weapon.MaxAmmo[3] || Weapon.Attendance[3]==0)
        {
         Weapon.Attendance[3]=1;
         Weapon.Ammo[3]+=100;
         if (Weapon.Ammo[3]>Weapon.MaxAmmo[3]) Weapon.Ammo[3]=Weapon.MaxAmmo[3];
         DeleteThing(n);
         Player.MessageNo=10;
         Player.MessageTime=50;
         PutStatistik();
         if (i==0) ChangeWeapon(3);
         continue;
        }
      }
     if (Thing[n]->Type==7)//плазмомёт
      {
       int i=0;
       if (Weapon.Attendance[4]!=0 && Weapon.Ammo[4]!=0) i=1;
       if (Weapon.Ammo[4]<Weapon.MaxAmmo[4] || Weapon.Attendance[4]==0)
        {
         Weapon.Attendance[4]=1;
         Weapon.Ammo[4]+=10;
         if (Weapon.Ammo[4]>Weapon.MaxAmmo[4]) Weapon.Ammo[4]=Weapon.MaxAmmo[4];
         DeleteThing(n);
         Player.MessageNo=11;
         Player.MessageTime=50;
         PutStatistik();
         if (i==0) ChangeWeapon(4);
         continue;
        }
      }
     if (Thing[n]->Type==8)//патроны к ружъю
      {
       if (Weapon.Ammo[2]<Weapon.MaxAmmo[2])
        {
         Weapon.Ammo[2]+=15;
         if (Weapon.Ammo[2]>Weapon.MaxAmmo[2]) Weapon.Ammo[2]=Weapon.MaxAmmo[2];
         DeleteThing(n);
         Player.MessageNo=3;
         Player.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (Thing[n]->Type==9)//патроны к пулемёту
      {
       if (Weapon.Ammo[3]<Weapon.MaxAmmo[3])
        {
         Weapon.Ammo[3]+=100;
         if (Weapon.Ammo[3]>Weapon.MaxAmmo[3]) Weapon.Ammo[3]=Weapon.MaxAmmo[3];
         DeleteThing(n);
         Player.MessageNo=8;
         Player.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (Thing[n]->Type==2)//патроны к плазмомёту
      {
       if (Weapon.Ammo[4]<Weapon.MaxAmmo[4])
        {
         Weapon.Ammo[4]+=10;
         if (Weapon.Ammo[4]>Weapon.MaxAmmo[4]) Weapon.Ammo[4]=Weapon.MaxAmmo[4];
         DeleteThing(n);
         Player.MessageNo=12;
         Player.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (Thing[n]->Type==10)//патроны к пистолету
      {
       if (Weapon.Ammo[1]<Weapon.MaxAmmo[1])
        {
         Weapon.Ammo[1]+=10;
         if (Weapon.Ammo[1]>Weapon.MaxAmmo[1]) Weapon.Ammo[1]=Weapon.MaxAmmo[1];
         DeleteThing(n);
         Player.MessageNo=2;
         Player.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (Thing[n]->Type==11)//карта
      {
       for(int m=0;m<Info.SegmentNumber;m++) Segment[m].Visible=1;
       DeleteThing(n);
       Player.MessageNo=14;
       Player.MessageTime=50;
       continue;
      }
     if (Thing[n]->Type==12)//скафандр
      {
       if (Player.TimeSpacesuit!=0) continue;
       Player.TimeSpacesuit=1000;
       DeleteThing(n);
       Player.MessageNo=13;
       Player.MessageTime=50;
       continue;
      }
     if (Thing[n]->Type==13)//мегасфера(бессмертие)
      {
       if (Player.TimeMegaSphere!=0) continue;
       Player.TimeMegaSphere=1000;
       Player.MessageNo=6;
       Player.MessageTime=50;
       DeleteThing(n);
       continue;
      }
     if (Thing[n]->Type==14)//суперсфера(+100 жизней)
      {
       if (Player.Life>=200) continue;
       Player.Life+=100;
       if (Player.Life>200) Player.Life=200;
       DeleteThing(n);
       Player.MessageNo=7;
       Player.MessageTime=50;
       PutStatistik();
       continue;
      }
     if (Thing[n]->Type==15)//живая вода
      {
       if (Player.Life>=200) continue;
       Player.Life++;
       if (Player.Life>200) Player.Life=200;
       DeleteThing(n);
       Player.MessageNo=1;
       Player.MessageTime=50;
       PutStatistik();
       continue;
      }
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает в xo,yo точку пересечения вектора со стенами из        //
//структуры BlockMap.Возвращает минимальную дистанцию.              //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int CheckTouchLine(int xb,int yb,int x1,int y1,int z1,int x2,int y2,int z2,float dz,int height,int man,float* xo,float* yo,int *object)
{
 int zu,zd;
 if (xb<0 || yb<0 || xb>99 || yb>99) return(100000000);
 int mindist=100000000,dist;
 int u=Maze.BlockMap[yb][xb][0];
 float xot,yot;
 float minx,miny;
 int obj=0;
 for(int n=1;n<=u;n++)
  {
   int k=Maze.BlockMap[yb][xb][n];
   int xw1=Segment[k].X1;
   int xw2=Segment[k].X2;
   int yw1=Segment[k].Y1;
   int yw2=Segment[k].Y2;
   if (CollizeLine(xw1,yw1,xw2,yw2,x1,y1,x2,y2)!=0)
    {
     Intersection(xw1,yw1,xw2,yw2,x1,y1,x2,y2,&xot,&yot);
     dist=sqrt((x1-xot)*(x1-xot)+(y1-yot)*(y1-yot));
     zu=z1+dz*dist;
     zd=zu-height;
     if (Segment[k].Frontier!=0)
      {
       int down1=Sector[Segment[k].Sector1].Down;
       int up1=Sector[Segment[k].Sector1].Up;
       int down2=Sector[Segment[k].Sector2].Down;
       int up2=Sector[Segment[k].Sector2].Up;
       if ((!(up1>=zu && zd>=down1)) ||
           (!(up2>=zu && zd>=down2)))
            {
             if (dist<mindist)
              {
               minx=xot;
               miny=yot;
               mindist=dist;
               obj=0;
              }
            }
      }
     else
     {
      if (dist<mindist)
       {
        minx=xot;
        miny=yot;
        mindist=dist;
        obj=0;
       }
     }
   }
  }
 //проверим пересечение с полом(потолком) сектора
 int sector=FindSector(xb+8,yb+8);
 //проверим пересечение с полом(потолком) сектора
 if (sector!=-1)
  {
   int sectorUp=Sector[sector].Up;
   int sectorDown=Sector[sector].Down;
   if ((z1-sectorUp)*(z2-sectorUp)<=0)//снаряд попал в потолок
    {
     float dx=x2-x1;
     float dy=y2-y1;
     float len=sqrt(dx*dx+dy*dy);
     float lenz=(sectorUp-z1)/dz;
     mindist=lenz;
     minx=x1+(dx/len)*lenz;
     miny=y1+(dy/len)*lenz;
     obj=0;
    }
   if ((z1-sectorDown)*(z2-sectorDown)<=0)//снаряд попал в пол
    {
     float dx=x2-x1;
     float dy=y2-y1;
     float len=sqrt(dx*dx+dy*dy);
     float lenz=(sectorDown-z1)/dz;
     mindist=lenz;
     minx=x1+(dx/len)*lenz;
     miny=y1+(dy/len)*lenz;
     obj=0;
    }
  }
 //исследуем пересечение с объектами
 for(n=0;n<NowThing;n++)
  {
   if (Thing[n]->Obstruction==0) continue;
   int up=Thing[n]->Coord.Z;
   int down=Thing[n]->Coord.Z-Thing[n]->Coord.Height;
   int x=Thing[n]->Coord.X;
   int y=Thing[n]->Coord.Y;
   int xw1=x1-x;
   int xw2=x2-x;
   int yw1=y1-y;
   int yw2=y2-y;
   float a=xw2-xw1;
   float b=yw2-yw1;
   float kd=a*a+b*b;
   float ld=2*a*xw1+2*b*yw1;
   float md=xw1*xw1+yw1*yw1-Thing[n]->Coord.Radius*Thing[n]->Coord.Radius;
   float d=ld*ld-4*kd*md;//дескриминант
   if (d<0) continue;//нет пересечения
   float sd=sqrt(d);
   float l1=(-ld+sd)/(2*kd);
   float l2=(-ld-sd)/(2*kd);
   float xp1=x1+l1*a;//2 точки пересечения
   float yp1=y1+l1*b;
   float xp2=x1+l2*a;
   float yp2=y1+l2*b;
   if (((xp1>=x1 && xp1<=x2) || (xp1>=x2 && xp1<=x1)) &&//пересечение есть
       ((yp1>=y1 && yp1<=y2) || (yp1>=y2 && yp1<=y1)))
        {
         dist=sqrt((x1-xp1)*(x1-xp1)+(y1-yp1)*(y1-yp1));
         zu=z1+dz*dist;
         zd=zu-height;
         if (!(up>=zu && zd>=down)) continue;
         if (dist<mindist)
          {
           minx=xp1;
           miny=yp1;
           mindist=dist;
           obj=0;
          }
        }
   if (((xp2>=x1 && xp2<=x2) || (xp2>=x2 && xp2<=x1)) &&//пересечение есть
       ((yp2>=y1 && yp2<=y2) || (yp2>=y2 && yp2<=y1)))
        {
         dist=sqrt((x1-xp2)*(x1-xp2)+(y1-yp2)*(y1-yp2));
         zu=z1+dz*dist;
         zd=zu-height;
         if (!(up>=zu && zd>=down)) continue;
         if (dist<mindist)
          {
           minx=xp2;
           miny=yp2;
           mindist=dist;
           obj=0;
          }
        }
  }
 //проверим столкновение с врагами
 for(n=0;n<NowAlien;n++)
  {
   if (n==man) continue;
   if (Alien[n]->Life==0) continue;
   int up=Alien[n]->Coord.Z;
   int down=Alien[n]->Coord.Z-Alien[n]->Coord.Height;
   int x=Alien[n]->Coord.X;
   int y=Alien[n]->Coord.Y;
   int xw1=x1-x;
   int xw2=x2-x;
   int yw1=y1-y;
   int yw2=y2-y;
   float a=xw2-xw1;
   float b=yw2-yw1;
   float kd=a*a+b*b;
   float ld=2*a*xw1+2*b*yw1;
   float md=xw1*xw1+yw1*yw1-Alien[n]->Coord.Radius*Alien[n]->Coord.Radius*1.5;
   float d=ld*ld-4*kd*md;//дескриминант
   if (d<0) continue;//нет пересечения
   float sd=sqrt(d);
   float l1=(-ld+sd)/(2*kd);
   float l2=(-ld-sd)/(2*kd);
   float xp1=x1+l1*a;//2 точки пересечения
   float yp1=y1+l1*b;
   float xp2=x1+l2*a;
   float yp2=y1+l2*b;
   if (((xp1>=x1 && xp1<=x2) || (xp1>=x2 && xp1<=x1)) &&//пересечение есть
       ((yp1>=y1 && yp1<=y2) || (yp1>=y2 && yp1<=y1)))
        {
         dist=sqrt((x1-xp1)*(x1-xp1)+(y1-yp1)*(y1-yp1));
         zu=z1+dz*dist;
         zd=zu-height;
         if (!(up>=zu && zd>=down)) continue;
         if (dist<mindist)
          {
           minx=xp1;
           miny=yp1;
           mindist=dist;
           obj=n+1;
          }
        }
   if (((xp2>=x1 && xp2<=x2) || (xp2>=x2 && xp2<=x1)) &&//пересечение есть
       ((yp2>=y1 && yp2<=y2) || (yp2>=y2 && yp2<=y1)))
        {
         dist=sqrt((x1-xp2)*(x1-xp2)+(y1-yp2)*(y1-yp2));
         zu=z1+dz*dist;
         zd=zu-height;
         if (!(up>=zu && zd>=down)) continue;
         if (dist<mindist)
          {
           minx=xp2;
           miny=yp2;
           mindist=dist;
           obj=n+1;
          }
        }
   }
 //исследуем пересечение с игроком(снаряд попал в игрока)
 int up=Player.Z-Player.SitZ;
 int down=Player.Z-64;
 if (man!=-1)
  {
   int x=Player.X;
   int y=Player.Y;
   int xw1=x1-x;
   int xw2=x2-x;
   int yw1=y1-y;
   int yw2=y2-y;
   float a=xw2-xw1;
   float b=yw2-yw1;
   float kd=a*a+b*b;
   float ld=2*a*xw1+2*b*yw1;
   float md=xw1*xw1+yw1*yw1-400;
   float d=ld*ld-4*kd*md;//дескриминант
   if (d>=0)
    {
     float sd=sqrt(d);
     float l1=(-ld+sd)/(2*kd);
     float l2=(-ld-sd)/(2*kd);
     float xp1=x1+l1*a;//2 точки пересечения
     float yp1=y1+l1*b;
     float xp2=x1+l2*a;
     float yp2=y1+l2*b;
     if (((xp1>=x1 && xp1<=x2) || (xp1>=x2 && xp1<=x1)) &&//пересечение есть
         ((yp1>=y1 && yp1<=y2) || (yp1>=y2 && yp1<=y1)))
          {
           dist=sqrt((x1-xp1)*(x1-xp1)+(y1-yp1)*(y1-yp1));
           zu=z1+dz*dist;
           zd=zu-height;
           if ((up>=zu && zd>=down) && dist<mindist)
            {
             minx=xp1;
             miny=yp1;
             mindist=dist;
             obj=-1;
            }
          }
     if (((xp2>=x1 && xp2<=x2) || (xp2>=x2 && xp2<=x1)) &&//пересечение есть
         ((yp2>=y1 && yp2<=y2) || (yp2>=y2 && yp2<=y1)))
          {
           dist=sqrt((x1-xp2)*(x1-xp2)+(y1-yp2)*(y1-yp2));
           zu=z1+dz*dist;
           zd=zu-height;
           if ((up>=zu && zd>=down) && dist<mindist)
            {
             minx=xp2;
             miny=yp2;
             mindist=dist;
             obj=-1;
            }
          }
     }
   }
 *object=obj;
 *xo=minx;
 *yo=miny;
 return(mindist);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает в xo,yo точку пересечения вектора на всём своём пути.  //
//Возвращает 1 при столкновении.                                    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int CheckWay(float x1,float y1,float z1,float x2,float y2,float z2,int radius,int height,int man,float* xo,float *yo,int *object)
{
 //используем алгоритм Брезенхейма
 int xb1=x1/128;
 int yb1=y1/128;
 int xb2=x2/128;
 int yb2=y2/128;
 int obj=0,objold=0;
 int mindist=100000000,dist;
 int minx,miny;
 float xot,yot;
 if (xb1<0 && xb2<0) return(0);
 if (yb1<0 && yb2<0) return(0);
 if (xb1>99 && xb2>99) return(0);
 if (yb1>99 && yb2>99) return(0);
 int dx=abs(xb2-xb1);
 int dy=abs(yb2-yb1);
 int sx=xb2>=xb1?1:-1;
 int sy=yb2>=yb1?1:-1;
 float dz=(z2-z1)/(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
 if (dy<=dx)
  {
   int d=(dy<<1)-dx;
   int d1=dy<<1;
   int d2=(dy-dx)<<1;
   //у нас теперь xb1,yb1-номер блока BlockMap
   dist=CheckTouchLine(xb1,yb1,x1,y1,z1,x2,y2,z2,dz,height,man,&xot,&yot,&obj);
   if (mindist>dist)
    {
     mindist=dist;
     minx=xot;
     miny=yot;
     objold=obj;
    }
   for(int x=xb1+sx,z=z1,y=yb1,i=1;i<=dx;i++,x+=sx)
    {
     if (d>0)
      {
       d+=d2;
       y+=sy;
      }
     else d+=d1;
     dist=CheckTouchLine(x,y,x1,y1,z1,x2,y2,z2,dz,height,man,&xot,&yot,&obj);
     if (mindist>dist)
      {
       mindist=dist;
       minx=xot;
       miny=yot;
       objold=obj;
       break;
      }
    }
  }
 else
  {
   int d=(dx<<1)-dy;
   int d1=dx<<1;
   int d2=(dx-dy)<<1;
   dist=CheckTouchLine(xb1,yb1,x1,y1,z1,x2,y2,z2,dz,height,man,&xot,&yot,&obj);
   if (mindist>dist)
    {
     mindist=dist;
     minx=xot;
     miny=yot;
     objold=obj;
    }
   for(int x=xb1,y=yb1+sy,i=1;i<=dy;i++,y+=sy)
    {
     if (d>0)
      {
       d+=d2;
       x+=sx;
      }
     else d+=d1;
     dist=CheckTouchLine(x,y,x1,y1,z1,x2,y2,z2,dz,height,man,&xot,&yot,&obj);
     if (mindist>dist)
      {
       mindist=dist;
       minx=xot;
       miny=yot;
       objold=obj;
       break;
      }
    }
  }
 if (mindist!=100000000)
  {
   *xo=minx;
   *yo=miny;
   *object=objold;
   return(1);
  }
 return(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Обеспечивает скольжение игрока вдоль стены.                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void SlideTraverse(float xp1,float yp1,float xp2,float yp2)
{
 //формируем вектор скольжения
 int *Collizion_Array;
 int Collizion_Amount;
 if (ImpactAll(Player.X,Player.Y,Player.Z-Player.SitZ,20,64-Player.SitZ,-1,&Collizion_Array,&Collizion_Amount)<0)
 {
  Player.X=xp1;
  Player.Y=yp1;
  free(Collizion_Array);
  return;
 }
 Player.X=xp1;
 Player.Y=yp1;
 //если же пересечение со стенами есть
 float x=xp1;
 float y=yp1;
 float dx=xp2-xp1;
 float dy=yp2-yp1;
 float Dx=0;
 float Dy=0;
 for(int n=0;n<10;n++)
 for(int m=0;m<Collizion_Amount;m++)
  {
   int line=Collizion_Array[m];
   float xw1=Segment[line].X1;
   float yw1=Segment[line].Y1;
   float xw2=Segment[line].X2;
   float yw2=Segment[line].Y2;
   float vx=xw2-xw1;
   float vy=yw2-yw1;
   float ux=dx;
   float uy=dy;
   float normav=(float)(sqrt(vx*vx+vy*vy));
   float normau=(float)(sqrt(ux*ux+uy*uy));
   vx=vx/normav;
   vy=vy/normav;
   if (normau!=0)
    {
     ux=ux/normau;
     uy=uy/normau;
    }
   float cosa=ux*vx+uy*vy;
   float modul=(float)(fabs(cosa));
   if (cosa>0)
    {
     Dx=vx*modul;
     Dy=vy*modul;
    }
   else
    {
     Dx=-vx*modul;
     Dy=-vy*modul;
    }
    Dx=Dx/Collizion_Amount;
    Dy=Dy/Collizion_Amount;
    //проверим, не пересекаем ли мы что-нибудь снова
    if (Impact(x+Dx,y+Dy,Player.Z-Player.SitZ,20,64-Player.SitZ,-1)!=0)
     {
      Dx=0;
      Dy=0;
     }
    x=x+Dx;
    y=y+Dy;
  }
 Player.X=x;
 Player.Y=y;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Инициализирует данные игрока                                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void InitPlayer(void)
{
 Player.SitZ=0;
 Player.Dz=0;
 Player.Pz=0;
 Player.AutoMap=0;
 Player.D_Angle=0;
 Player.PV=9.8;
 Player.Life=100;
 Player.RedScreenOn=0;
 Player.RedScreen=0;
 Player.TimeSpacesuit=0;
 Player.TimeMegaSphere=0;
 Player.IDDQD=0;//нет бессмертия
 Player.MessageNo=0;
 Player.MessageTime=0;
 Player.Level=1;
 Player.Inerci=0;
 //установка оружия
 Weapon.Fire_Faze=0;
 Weapon.WeaponNo=1;
 Weapon.Vy=0;
 Weapon.Dy=0;
 int i;
 //======================0======================
 i=0;
 Weapon.Ammo[i]=1;
 Weapon.MaxAmmo[i]=1;
 Weapon.Attendance[i]=1;
 Weapon.X[i][0]=190;
 Weapon.Y[i][0]=146;
 Weapon.Faze[i][0]=0;
 Weapon.X[i][1]=190;
 Weapon.Y[i][1]=146;
 Weapon.Faze[i][1]=0;
 Weapon.X[i][2]=50;
 Weapon.Y[i][2]=135;
 Weapon.Faze[i][2]=1;
 Weapon.X[i][3]=50;
 Weapon.Y[i][3]=135;
 Weapon.Faze[i][3]=1;
 Weapon.X[i][4]=50;
 Weapon.Y[i][4]=135;
 Weapon.Faze[i][4]=1;
 Weapon.X[i][5]=50;
 Weapon.Y[i][5]=135;
 Weapon.Faze[i][5]=1;
 Weapon.X[i][6]=50;
 Weapon.Y[i][6]=135;
 Weapon.Faze[i][6]=1;
 Weapon.X[i][7]=50;
 Weapon.Y[i][7]=135;
 Weapon.Faze[i][7]=1;
 Weapon.X[i][8]=50;
 Weapon.Y[i][8]=121;
 Weapon.Faze[i][8]=2;
 Weapon.X[i][9]=50;
 Weapon.Y[i][9]=121;
 Weapon.Faze[i][9]=2;
 Weapon.X[i][10]=50;
 Weapon.Y[i][10]=121;
 Weapon.Faze[i][10]=2;
 Weapon.X[i][11]=50;
 Weapon.Y[i][11]=121;
 Weapon.Faze[i][11]=2;
 Weapon.X[i][12]=50;
 Weapon.Y[i][12]=121;
 Weapon.Faze[i][12]=2;
 Weapon.X[i][13]=50;
 Weapon.Y[i][13]=121;
 Weapon.Faze[i][13]=2;
 Weapon.X[i][14]=50;
 Weapon.Y[i][14]=121;
 Weapon.Faze[i][14]=2;
 Weapon.X[i][15]=50;
 Weapon.Y[i][15]=121;
 Weapon.Faze[i][15]=2;
 Weapon.X[i][16]=50;
 Weapon.Y[i][16]=135;
 Weapon.Faze[i][16]=1;
 Weapon.X[i][17]=50;
 Weapon.Y[i][17]=135;
 Weapon.Faze[i][17]=1;
 Weapon.X[i][18]=50;
 Weapon.Y[i][18]=135;
 Weapon.Faze[i][18]=1;
 Weapon.X[i][19]=50;
 Weapon.Y[i][19]=135;
 Weapon.Faze[i][19]=1;
 Weapon.X[i][20]=50;
 Weapon.Y[i][20]=135;
 Weapon.Faze[i][20]=1;
 Weapon.X[i][21]=190;
 Weapon.Y[i][21]=146;
 Weapon.Faze[i][21]=0;
 Weapon.X[i][22]=190;
 Weapon.Y[i][22]=146;
 Weapon.Faze[i][22]=0;
 Weapon.X[i][23]=190;
 Weapon.Y[i][23]=146;
 Weapon.Faze[i][23]=0;
 Weapon.Faze[i][24]=-1;
 //======================1======================
 i=1;
 Weapon.Ammo[i]=50;
 Weapon.MaxAmmo[i]=100;
 Weapon.Attendance[i]=1;
 Weapon.X[i][0]=133;
 Weapon.Y[i][0]=126;
 Weapon.Faze[i][0]=0;
 Weapon.X[i][1]=133;
 Weapon.Y[i][1]=126;
 Weapon.Faze[i][1]=0;
 Weapon.X[i][2]=131;
 Weapon.Y[i][2]=107;
 Weapon.Faze[i][2]=1;
 Weapon.X[i][3]=131;
 Weapon.Y[i][3]=107;
 Weapon.Faze[i][3]=1;
 Weapon.X[i][4]=131;
 Weapon.Y[i][4]=107;
 Weapon.Faze[i][4]=1;
 Weapon.X[i][5]=131;
 Weapon.Y[i][5]=107;
 Weapon.Faze[i][5]=1;
 Weapon.X[i][6]=130;
 Weapon.Y[i][6]=86;
 Weapon.Faze[i][6]=2;
 Weapon.X[i][7]=130;
 Weapon.Y[i][7]=86;
 Weapon.Faze[i][7]=2;
 Weapon.X[i][8]=130;
 Weapon.Y[i][8]=86;
 Weapon.Faze[i][8]=2;
 Weapon.X[i][9]=131;
 Weapon.Y[i][9]=107;
 Weapon.Faze[i][9]=1;
 Weapon.X[i][10]=131;
 Weapon.Y[i][10]=107;
 Weapon.Faze[i][10]=1;
 Weapon.X[i][11]=131;
 Weapon.Y[i][11]=107;
 Weapon.Faze[i][11]=1;
 Weapon.X[i][12]=133;
 Weapon.Y[i][12]=126;
 Weapon.Faze[i][12]=0;
 Weapon.X[i][13]=133;
 Weapon.Y[i][13]=126;
 Weapon.Faze[i][13]=0;
 Weapon.Faze[i][14]=-1;//конец анимации
 //======================2======================
 i=2;
 Weapon.Ammo[i]=0;
 Weapon.MaxAmmo[i]=100;
 Weapon.Attendance[i]=0;
 Weapon.X[i][0]=131;
 Weapon.Y[i][0]=132;
 Weapon.Faze[i][0]=0;
 Weapon.X[i][1]=131;
 Weapon.Y[i][1]=119;
 Weapon.Faze[i][1]=1;
 Weapon.X[i][2]=131;
 Weapon.Y[i][2]=119;
 Weapon.Faze[i][2]=1;
 Weapon.X[i][3]=131;
 Weapon.Y[i][3]=119;
 Weapon.Faze[i][3]=1;

 Weapon.X[i][4]=127;
 Weapon.Y[i][4]=110;
 Weapon.Faze[i][4]=2;
 Weapon.X[i][5]=127;
 Weapon.Y[i][5]=110;
 Weapon.Faze[i][5]=2;
 Weapon.X[i][6]=127;
 Weapon.Y[i][6]=110;
 Weapon.Faze[i][6]=2;

 Weapon.X[i][7]=126;
 Weapon.Y[i][7]=107;
 Weapon.Faze[i][7]=4;
 Weapon.X[i][8]=126;
 Weapon.Y[i][8]=107;
 Weapon.Faze[i][8]=4;
 Weapon.X[i][9]=126;
 Weapon.Y[i][9]=107;
 Weapon.Faze[i][9]=4;
 Weapon.X[i][10]=126;
 Weapon.Y[i][10]=107;
 Weapon.Faze[i][10]=4;
 Weapon.X[i][11]=126;
 Weapon.Y[i][11]=107;
 Weapon.Faze[i][11]=4;
 Weapon.X[i][12]=126;
 Weapon.Y[i][12]=107;
 Weapon.Faze[i][12]=4;
 Weapon.X[i][13]=126;
 Weapon.Y[i][13]=107;
 Weapon.Faze[i][13]=4;

 Weapon.X[i][14]=99;
 Weapon.Y[i][14]=57;
 Weapon.Faze[i][14]=3;
 Weapon.X[i][15]=99;
 Weapon.Y[i][15]=57;
 Weapon.Faze[i][15]=3;
 Weapon.X[i][16]=99;
 Weapon.Y[i][16]=57;
 Weapon.Faze[i][16]=3;
 Weapon.X[i][17]=99;
 Weapon.Y[i][17]=57;
 Weapon.Faze[i][17]=3;
 Weapon.X[i][18]=99;
 Weapon.Y[i][18]=57;
 Weapon.Faze[i][18]=3;
 Weapon.X[i][19]=126;
 Weapon.Y[i][19]=107;
 Weapon.Faze[i][19]=4;
 Weapon.X[i][20]=126;
 Weapon.Y[i][20]=107;
 Weapon.Faze[i][20]=4;
 Weapon.X[i][21]=126;
 Weapon.Y[i][21]=107;
 Weapon.Faze[i][21]=4;
 Weapon.X[i][22]=126;
 Weapon.Y[i][22]=107;
 Weapon.Faze[i][22]=4;
 Weapon.X[i][23]=70;
 Weapon.Y[i][23]=124;
 Weapon.Faze[i][23]=5;
 Weapon.X[i][24]=70;
 Weapon.Y[i][24]=124;
 Weapon.Faze[i][24]=5;
 Weapon.X[i][25]=70;
 Weapon.Y[i][25]=124;
 Weapon.Faze[i][25]=5;
 Weapon.X[i][26]=70;
 Weapon.Y[i][26]=124;
 Weapon.Faze[i][26]=5;

 Weapon.X[i][27]=123;
 Weapon.Y[i][27]=136;
 Weapon.Faze[i][27]=6;
 Weapon.X[i][28]=123;
 Weapon.Y[i][28]=136;
 Weapon.Faze[i][28]=6;
 Weapon.X[i][29]=123;
 Weapon.Y[i][29]=136;
 Weapon.Faze[i][29]=6;

 Weapon.X[i][30]=131;
 Weapon.Y[i][30]=132;
 Weapon.Faze[i][30]=0;
 Weapon.X[i][31]=131;
 Weapon.Y[i][31]=132;
 Weapon.Faze[i][31]=0;
 Weapon.Faze[i][32]=-1;//конец анимации
 //======================3======================
 i=3;
 Weapon.Ammo[i]=0;
 Weapon.MaxAmmo[i]=500;
 Weapon.Attendance[i]=0;
 Weapon.X[i][0]=114;
 Weapon.Y[i][0]=137;
 Weapon.Faze[i][0]=0;
 Weapon.X[i][1]=114;
 Weapon.Y[i][1]=117;
 Weapon.Faze[i][1]=1;
 Weapon.X[i][2]=114;
 Weapon.Y[i][2]=117;
 Weapon.Faze[i][2]=1;
 Weapon.X[i][3]=114;
 Weapon.Y[i][3]=116;
 Weapon.Faze[i][3]=1;
 Weapon.X[i][4]=114;
 Weapon.Y[i][4]=116;
 Weapon.Faze[i][4]=2;
 Weapon.X[i][5]=114;
 Weapon.Y[i][5]=136;
 Weapon.Faze[i][5]=0;
 Weapon.Faze[i][6]=-1;
 //======================4======================
 i=4;
 Weapon.Ammo[i]=0;
 Weapon.MaxAmmo[i]=100;
 Weapon.Attendance[i]=0;
 Weapon.X[i][0]=127;
 Weapon.Y[i][0]=126;
 Weapon.Faze[i][0]=0;
 Weapon.X[i][1]=123;
 Weapon.Y[i][1]=114;
 Weapon.Faze[i][1]=1;
 Weapon.X[i][2]=123;
 Weapon.Y[i][2]=114;
 Weapon.Faze[i][2]=1;
 Weapon.X[i][3]=123;
 Weapon.Y[i][3]=114;
 Weapon.Faze[i][3]=1;
 Weapon.X[i][4]=127;
 Weapon.Y[i][4]=126;
 Weapon.Faze[i][4]=0;
 Weapon.X[i][5]=127;
 Weapon.Y[i][5]=126;
 Weapon.Faze[i][5]=0;
 Weapon.X[i][6]=112;
 Weapon.Y[i][6]=76;
 Weapon.Faze[i][6]=2;
 Weapon.X[i][7]=112;
 Weapon.Y[i][7]=76;
 Weapon.Faze[i][7]=2;
 Weapon.X[i][8]=112;
 Weapon.Y[i][8]=76;
 Weapon.Faze[i][8]=2;
 Weapon.X[i][9]=112;
 Weapon.Y[i][9]=76;
 Weapon.Faze[i][9]=2;
 Weapon.X[i][10]=112;
 Weapon.Y[i][10]=76;
 Weapon.Faze[i][10]=2;
 Weapon.X[i][11]=112;
 Weapon.Y[i][11]=76;
 Weapon.Faze[i][11]=2;
 Weapon.X[i][12]=112;
 Weapon.Y[i][12]=76;
 Weapon.Faze[i][12]=2;
 Weapon.X[i][13]=127;
 Weapon.Y[i][13]=126;
 Weapon.Faze[i][13]=0;
 Weapon.X[i][14]=127;
 Weapon.Y[i][14]=126;
 Weapon.Faze[i][14]=0;
 Weapon.Faze[i][15]=-1;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает 0, если расстояние от игрока до сегмента с координатами//
//xa,ya-xb,yb больше 50, либо этот перпендикуляр находится вне      //
//данного сегмента.Иначе возвращает 1.                              //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int DistanceOfVector(float xa,float ya,float xb,float yb)
{
 float xc=Player.X;
 float yc=Player.Y;
 float xp,yp;
 float k;
 if (xa!=xb)
  {
   k=(yb-ya)/(xb-xa);
   xp=(xc+(yc+k*xa-ya)*k)/(k*k+1);
   yp=k*(xp-xa)+ya;
  }
 else
  {
   k=(xb-xa)/(yb-ya);
   yp=(yc+(xc+k*ya-xa)*k)/(k*k+1);
   xp=k*(yp-ya)+xa;
  }
 if (xa>xb)
  {
   float r=xa;
   xa=xb;
   xb=r;
  }
 if (ya>yb)
  {
   float r=ya;
   ya=yb;
   yb=r;
  }
 if (xp<xa || xp>xb || yp<ya || yp>yb) return(0);
 float l=sqrt((xc-xp)*(xc-xp)+(yc-yp)*(yc-yp));
 if (l<50) return(1);
 return(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Обеспечивает подъём/опускание дверей.K-номер сектора,где          //
//находится игрок.                                                 //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void AnimateDoor(int k)
{
 for(register int n=0;n<Info.SectorNumber;n++)
  {
   if (Sector[n].Type==1 || Sector[n].Type==3)
    {
     if (Sector[n].DZ>0 && Sector[n].Time==0 && n==k)//игрок зажат дверью
      {
       if (Sector[n].Up<=Player.Z-Player.SitZ)
        {
         Sector[n].DZ=-abs(Sector[n].DZ);//дверь опускается
         Sector[n].Time=0;
        }
      }
     if (Sector[n].Time==0)
      {
       Sector[n].Down+=Sector[n].DZ;
       if (Sector[n].Down>=Sector[n].Up) Sector[n].DZ=0;
       if (Sector[n].Down<=Sector[n].Down1)
        {
         Sector[n].Down=Sector[n].Down1;
         if (Sector[n].Type==1)//обычная дверь поднимается
          {
           Sector[n].DZ=-Sector[n].DZ;
           Sector[n].Time=200;
          }
         else//автоматическая нет
          {
           Sector[n].DZ=0;
           Sector[n].Time=0;
          }
        }
      }
    else Sector[n].Time--;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Обеспечивает подъём/опускание платформ.K-номер сектора,где        //
//находится игрок.                                                 //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void AnimateGantry(int k)
{
 register int n;
 for(n=0;n<Info.SectorNumber;n++)
  {
   if (Sector[n].Type==4 || Sector[n].Type==5 || Sector[n].Type==8)//платформа и мостик
    {
     if (Sector[n].DZ>0 && Sector[n].Time==0 && n==k)//игрок зажат платформой
      {
       if (Sector[n].Up<=Player.Z-Player.SitZ)
        {
         Sector[n].DZ=-abs(Sector[n].DZ);//платформа опускается
         Sector[n].Time=0;
        }
      }
     if (Sector[n].Time==0)
      {
       Sector[n].Down+=Sector[n].DZ;
       if ((Sector[n].Down>=Sector[n].Down1 && Sector[n].Down1-Sector[n].Down2>0) ||
           (Sector[n].Down<=Sector[n].Down1 && Sector[n].Down1-Sector[n].Down2<0))
        {
         Sector[n].Down=Sector[n].Down1;
         Sector[n].DZ=-Sector[n].DZ;
         Sector[n].Time=200;
         if (Sector[n].Type==8)
          {
           Sector[n].Time=0;
           Sector[n].DZ=0;
          }
        }
       if ((Sector[n].Down<=Sector[n].Down2 && Sector[n].Down1-Sector[n].Down2>0) ||
           (Sector[n].Down>=Sector[n].Down2 && Sector[n].Down1-Sector[n].Down2<0))
        {
         Sector[n].Down=Sector[n].Down2;
         Sector[n].Time=0;
         Sector[n].DZ=0;
        }
      }
     else Sector[n].Time--;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Производит смену текстур полов и потолков.                                   //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void AnimateFloor_Flow(void)
{
 register int n,t;
 if (TimerUser%11==0)
  {
   for(n=0;n<Info.SectorNumber;n++)
    {
     t=Sector[n].UpTexture;
     if (t>=0 && t<=2)
      {
       Sector[n].UpTexture=Sector[n].UpTexture+1;
       if (Sector[n].UpTexture==3) Sector[n].UpTexture=0;
      }
     if (t>=111 && t<=114)
      {
       Sector[n].UpTexture=Sector[n].UpTexture+1;
       if (Sector[n].UpTexture==115) Sector[n].UpTexture=111;
      }
     if (t>=119 && t<=122)
      {
       Sector[n].UpTexture=Sector[n].UpTexture+1;
       if (Sector[n].UpTexture==123) Sector[n].UpTexture=119;
      }
     if (t>=132 && t<=135)
      {
       Sector[n].UpTexture=Sector[n].UpTexture+1;
       if (Sector[n].UpTexture==136) Sector[n].UpTexture=132;
      }
     if (t>=145 && t<=148)
      {
       Sector[n].UpTexture=Sector[n].UpTexture+1;
       if (Sector[n].UpTexture==149) Sector[n].UpTexture=145;
      }
     if (t>=149 && t<=151)
      {
       Sector[n].UpTexture=Sector[n].UpTexture+1;
       if (Sector[n].UpTexture==152) Sector[n].UpTexture=149;
      }
     t=Sector[n].DownTexture;
     if (t>=0 && t<=2)
      {
       Sector[n].DownTexture=Sector[n].DownTexture+1;
       if (Sector[n].DownTexture==3) Sector[n].DownTexture=0;
      }
     if (t>=111 && t<=114)
      {
       Sector[n].DownTexture=Sector[n].DownTexture+1;
       if (Sector[n].DownTexture==115) Sector[n].DownTexture=111;
      }
     if (t>=119 && t<=122)
      {
       Sector[n].DownTexture=Sector[n].DownTexture+1;
       if (Sector[n].DownTexture==123) Sector[n].DownTexture=119;
      }
     if (t>=132 && t<=135)
      {
       Sector[n].DownTexture=Sector[n].DownTexture+1;
       if (Sector[n].DownTexture==136) Sector[n].DownTexture=132;
      }
     if (t>=145 && t<=148)
      {
       Sector[n].DownTexture=Sector[n].DownTexture+1;
       if (Sector[n].DownTexture==149) Sector[n].DownTexture=145;
      }
     if (t>=149 && t<=151)
      {
       Sector[n].DownTexture=Sector[n].DownTexture+1;
       if (Sector[n].DownTexture==152) Sector[n].DownTexture=149;
      }
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Инициализирует предметы                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void SetThing(void)
{
 int n;
 NowThing=0;
 NowAlien=0;
 NowMissile=0;
 for(n=0;n<Info.ThingNumber;n++)
  {
   int t=Info.TType[n];
   if (t>=0 && t<=1) CreateNewMedicine(Info.TX[n],Info.TY[n]);
   if (t>=2 && t<=3) CreateNewSuperMedicine(Info.TX[n],Info.TY[n]);
   if (t>=4 && t<=7) CreateNewBattery(Info.TX[n],Info.TY[n]);
   if (t==8) CreateNewGun(Info.TX[n],Info.TY[n]);
   if (t==9) CreateNewMachineGun(Info.TX[n],Info.TY[n]);
   if (t==10) CreateNewPlasmaGun(Info.TX[n],Info.TY[n]);
   if (t==11) CreateNewGunAmmo(Info.TX[n],Info.TY[n]);
   if (t==12) CreateNewMGunAmmo(Info.TX[n],Info.TY[n]);
   if (t==13) CreateNewGreyAltar(Info.TX[n],Info.TY[n]);
   if (t==14) CreateNewRedAltar(Info.TX[n],Info.TY[n]);
   if (t==15) CreateNewSuppository(Info.TX[n],Info.TY[n]);
   if (t==16) CreateNewPistolAmmo(Info.TX[n],Info.TY[n]);
   if (t>=17 && t<=19) CreateNewButt(Info.TX[n],Info.TY[n]);
   if (t>=20 && t<=22) CreateNewRedTorch(Info.TX[n],Info.TY[n]);
   if (t>=23 && t<=24) CreateNewLocMap(Info.TX[n],Info.TY[n]);
   if (t==25) CreateNewSpacesuit(Info.TX[n],Info.TY[n]);
   if (t==26) CreateNewMiniSuppository(Info.TX[n],Info.TY[n]);
   if (t==27) CreateNewMiniBlood(Info.TX[n],Info.TY[n]);
   if (t==28) CreateNewBlood(Info.TX[n],Info.TY[n]);
   if (t>=29 && t<=31) CreateNewMegaSphere(Info.TX[n],Info.TY[n]);
   if (t>=32 && t<=34) CreateNewSuperSphere(Info.TX[n],Info.TY[n]);
   if (t>=35 && t<=38) CreateNewHealingWater(Info.TX[n],Info.TY[n]);
   if (t>=39 && t<=41) CreateNewBlueTorch(Info.TX[n],Info.TY[n]);
  }
 for(n=0;n<Info.AlienNumber;n++)
  {
   int t=Info.AType[n];
   if (t==0) CreateNewDemon1(Info.AX[n],Info.AY[n]);
   if (t==1) CreateNewDemon2(Info.AX[n],Info.AY[n]);
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Анимация предметов и врагов                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void AnimateThing(void)
{
 for(int n=0;n<NowThing;n++)
  {
   Thing[n]->Faze++;
   if (Thing[n]->Footage[Thing[n]->Faze].Faze==-1) Thing[n]->Faze=0;
   int sector=FindSector(Thing[n]->Coord.X,Thing[n]->Coord.Y);
   int sdown=Sector[sector].Down;
   int sup=Sector[sector].Up;
   int zup=Thing[n]->Coord.Z;
   int zdown=Thing[n]->Coord.Z-Thing[n]->Coord.Height;
   if (zdown!=sdown) Thing[n]->Coord.Z=sdown+Thing[n]->Coord.Height;//предмет поднимает или опускает
  }
 for(n=0;n<NowAlien;n++)
  {
   if (Alien[n]->Destroy==-1)
    {
     if (Alien[n]->Type==2)
      {
       DeleteAlien(n);
       n--;
       continue;
      }
     if (Alien[n]->Dz!=0)
      {
       Alien[n]->Coord.Z+=Alien[n]->Dz;
       int z=Sector[FindSector(Alien[n]->Coord.X,Alien[n]->Coord.Y)].Down;
       if (Alien[n]->Coord.Z-Alien[n]->Coord.Height<z)
        {
         Alien[n]->Coord.Z=z+Alien[n]->Coord.Height;
         Alien[n]->Dz=0;
        }
      }
    }
   int sector=FindSector(Alien[n]->Coord.X,Alien[n]->Coord.Y);
   int sdown=Sector[sector].Down;
   int sup=Sector[sector].Up;
   int zup=Alien[n]->Coord.Z;
   int zdown=Alien[n]->Coord.Z-Alien[n]->Coord.Height;
   if (sup-sdown<zup-zdown && Alien[n]->Life!=0)//врага убило
    {
     Alien[n]->Life=0;
     Alien[n]->Faze=Alien[n]->Destroy;
     Alien[n]->Coord.Z=sdown+Alien[n]->Coord.Height;
    }
   if (zdown<sdown) Alien[n]->Coord.Z=sdown+Alien[n]->Coord.Height;//врага поднимает или опускает
   if (Alien[n]->Life==0 && zdown>sdown && Alien[n]->Destroy==0) Alien[n]->Coord.Z=sdown+Alien[n]->Coord.Height;
   Alien[n]->Faze++;
   if (Alien[n]->Faze>=Alien[n]->Destroy && Alien[n]->Footage[Alien[n]->Faze].Faze==-1)
    {
     Alien[n]->Faze--;
     Alien[n]->Destroy=-1;
     Alien[n]->Dz=-10;//враг падает
    }
   if (Alien[n]->Footage[Alien[n]->Faze].Faze==-1) Alien[n]->Faze=0;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Игрок в специальном секторе                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void SpecialSector(int sector)
{
 if (Player.TimeSpacesuit!=0 || Player.IDDQD!=0 || Player.TimeMegaSphere!=0) return;//у игрока есть скафандр или игрок бессмертен
 int pl=Player.Life;
 if (TimerUser%100==0)
  {
   int floor=Sector[sector].DownTexture;
   if (floor>=0 && floor<=2) Player.Life-=4;//кислота
   if (floor>=132 && floor<=135) Player.Life-=2;//токсичные отходы
   if (floor>=145 && floor<=148) Player.Life-=10;//лава
   if (floor>=149 && floor<=151) Player.Life-=1;//кровь
   if (Player.Life<0) Player.Life=0;
   if (Player.Life!=pl) PutStatistik();
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Управление врагами                                                //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveAlien(void)
{
 int n;
 for(n=0;n<NowAlien;n++)
  {
   if (Alien[n]->Life==0) continue;
   float xold=Alien[n]->Coord.X;
   float yold=Alien[n]->Coord.Y;
   float zoldup=Alien[n]->Coord.Z;
   float zolddown=Alien[n]->Coord.Z-Alien[n]->Coord.Height;
   int sectorold=FindSector(xold,yold);
   int upold=Sector[sectorold].Up;
   int downold=Sector[sectorold].Down;
   float px=Player.X;
   float py=Player.Y;
   float pz=Player.Z;
   int sectorplayer;
   if (Alien[n]->Active==1) sectorplayer=sectorold;
                       else sectorplayer=FindSector(px,py);
   float xo,yo;
   int object,len;
   len=sqrt((px-xold)*(px-xold)+(py-yold)*(py-yold));
   if (len<400 || sectorold==sectorplayer) Alien[n]->Active=1;//активизируем врага
   if (Alien[n]->Active==0) continue;
   if (rnd(100)>90 && Alien[n]->Time<100)
    {
     Alien[n]->Time=500;//время преследования
     Alien[n]->Dy=3*(py-yold)/len;
     Alien[n]->Dx=3*(px-xold)/len;
     Alien[n]->Dz=3*(Player.Z-zoldup)/len;
    }
   if (rnd(100)>90 && len<1000 && Alien[n]->Faze<Alien[n]->Fire)//враг стреляет
    {
     float dy=(py-yold)/len+sin(3.14*(rnd(4)-2)/180);
     float dx=(px-xold)/len+sin(3.14*(rnd(4)-2)/180);
     float dz=sin(3.14*(rnd(4)-2)/180);
     float zm=((zoldup-Alien[n]->Coord.Height/1.5)-(Player.Z-Player.SitZ))/len;
     if (Alien[n]->Type==1) CreateNewRocket(xold,yold,zoldup-Alien[n]->Coord.Height/1.5,dx,dy,dz-zm,15,5,5,n);
     if (Alien[n]->Type==2) CreateNewBlueBall(xold,yold,zoldup-Alien[n]->Coord.Height/1.5,dx,dy,dz-zm,9,3,6,n);
     Alien[n]->Faze=Alien[n]->Fire;
    }
   Alien[n]->Coord.X+=Alien[n]->Dx;
   Alien[n]->Coord.Y+=Alien[n]->Dy;
   Alien[n]->Coord.Z+=Alien[n]->Dz;
   float znewup=Alien[n]->Coord.Z;//проверим столкновения со стенами,полом и потолком
   float znewdown=Alien[n]->Coord.Z-Alien[n]->Coord.Height;
   int sectornew=FindSector(Alien[n]->Coord.X,Alien[n]->Coord.Y);
   int upnew=Sector[sectornew].Up;
   int downnew=Sector[sectornew].Down;
   if (upnew<znewup)
    {
     Alien[n]->Dz=-Alien[n]->Dz;
     Alien[n]->Coord.Z=upnew;
    }
   if (downnew>znewdown)
    {
     Alien[n]->Dz=-Alien[n]->Dz;
     Alien[n]->Coord.Z=downnew+Alien[n]->Coord.Height;
    }
   if (rnd(1000)>900 || Alien[n]->Time==0 || Impact(Alien[n]->Coord.X,Alien[n]->Coord.Y,Alien[n]->Coord.Z,Alien[n]->Coord.Radius,Alien[n]->Coord.Height,n))
    {
     Alien[n]->Coord.Y=yold;
     Alien[n]->Coord.X=xold;
     Alien[n]->Coord.Z=zoldup;
     int angle=rnd(360);
     Alien[n]->Dx=CosTable[angle]*3;
     Alien[n]->Dy=SinTable[angle]*3;
     Alien[n]->Dz=(rnd(200)-100)/30;
     Alien[n]->Time=200;
    }
   Alien[n]->Time--;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Цветовые эффекты                                                  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void ColorEffect(void)
{
 if (Player.TimeSpacesuit==1000 || (Player.TimeSpacesuit!=0 && Player.TimeSpacesuit<=100 && Player.TimeSpacesuit%5==0 && Player.TimeSpacesuit%2==0)) Video.SetRGB_DAC(0.7,1,0.7);
 if ((Player.TimeSpacesuit!=0 && Player.TimeSpacesuit<=100 && Player.TimeSpacesuit%5==0 && Player.TimeSpacesuit%2!=0)) Video.SetRGB_DAC(1,1,1);
 if (Player.TimeSpacesuit>0)
  {
   Player.TimeSpacesuit--;
   if (Player.TimeSpacesuit==0)
   if (Player.TimeMegaSphere==0) Video.SetRGB_DAC(1,1,1);
                            else Video.SetRGB_DAC(0.7,0.7,1);
  }
 if (Player.TimeMegaSphere==1000 || (Player.TimeMegaSphere!=0 && Player.TimeMegaSphere<=100 && Player.TimeMegaSphere%5==0 && Player.TimeMegaSphere%2==0)) Video.SetRGB_DAC(0.7,0.7,1);
 if ((Player.TimeMegaSphere!=0 && Player.TimeMegaSphere<=100 && Player.TimeMegaSphere%5==0 && Player.TimeMegaSphere%2!=0)) Video.SetRGB_DAC(1,1,1);
 if (Player.TimeMegaSphere>0)
  {
   Player.TimeMegaSphere--;
   if (Player.TimeMegaSphere==0)
    {
     if (Player.TimeSpacesuit==0) Video.SetRGB_DAC(1,1,1);
                             else Video.SetRGB_DAC(0.7,1,0.7);
    }
  }
 if (Player.RedScreenOn==1)
  {
   Player.RedScreen--;
   if (Player.RedScreen==0)
    {
     if (Player.TimeSpacesuit==0 && Player.TimeMegaSphere==0) Video.SetRGB_DAC(1,1,1);
      else
       {
        if (Player.TimeMegaSphere!=0) Video.SetRGB_DAC(0.7,0.7,1);
        if (Player.TimeSpacesuit!=0) Video.SetRGB_DAC(0.7,1,0.7);
       }
     Player.RedScreenOn=0;
    }
  }
 else
  {
   if (Player.RedScreen!=0)
    {
     Video.SetRGB_DAC(1,0.7,0.7);
     Player.RedScreenOn=1;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Анимация лица игрока                                              //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void AnimateFace(int t)
{
 if (Player.Life<=0)
  {
   PutBar(Data.Bar,1,142,168);
   Video.ViewBar();
   return;
  }
 if (Player.IDDQD!=0 || Player.TimeMegaSphere!=0)
  {
   PutBar(Data.Bar,2,142,168);
   Video.ViewBar();
   return;
  }
 int rd=rnd(1000);
 int variant;
 int face=0;
 if (rd>900)
  {
   rd=rnd(1000);
   if (rd>333 && rd<666) face=1;
   if (rd>666) face=2;
   if (rd<333) face=3;
  }
 if (face!=0 || t!=0)
  {
   if (face==0) face=1;
   if (t==2) face=4;
   if (Player.Life>80) variant=0;
   if (Player.Life>60 && Player.Life<=80) variant=1;
   if (Player.Life>40 && Player.Life<=60) variant=2;
   if (Player.Life>20 && Player.Life<=40) variant=3;
   if (Player.Life>0 && Player.Life<=20) variant=4;
   PutBar(Data.Bar,face+variant*4+2,142,168);
   Video.ViewBar();
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Вывод сообщений                                                   //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void PutMessage(void)
{
 if (Player.MessageNo==1) Video.Print(1,1,"PICKED UP A HEALTH BONUS.");
 if (Player.MessageNo==2) Video.Print(1,1,"PICKED UP A CLIP.");
 if (Player.MessageNo==3) Video.Print(1,1,"PICKED UP A 15 SHOTGUN SHELLS.");
 if (Player.MessageNo==4) Video.Print(1,1,"PICKED UP A MEDIKIT.");
 if (Player.MessageNo==5) Video.Print(1,1,"PICKED UP A SUPER MEDIKIT !");
 if (Player.MessageNo==6) Video.Print(1,1,"INVULNERABILITY !");
 if (Player.MessageNo==7) Video.Print(1,1,"SUPERCHARGE !");
 if (Player.MessageNo==8) Video.Print(1,1,"PICKED UP A BOX OF BULLETS.");
 if (Player.MessageNo==9) Video.Print(1,1,"YOU GOT THE SHOTGUN.");
 if (Player.MessageNo==10) Video.Print(1,1,"YOU GOT THE CHAINGUN.");
 if (Player.MessageNo==11) Video.Print(1,1,"YOU GOT THE PLASMAGUN.");
 if (Player.MessageNo==12) Video.Print(1,1,"PICKED UP AN ENERGY CELL PACK.");
 if (Player.MessageNo==13) Video.Print(1,1,"RADIATION SHEILDING SUIT.");
 if (Player.MessageNo==14) Video.Print(1,1,"PICKED UP A COMPUTER MAP.");
 if (Player.MessageNo==15) Video.Print(1,1,"GAME SAVED.");
 if (Player.MessageNo==16)
  {
   char s[255];
   itoa(Player.FPS,s,10);
   Video.Print(1,1,s);
  }
 if (Player.MessageTime!=0)
  {
   Player.MessageTime--;
   if (Player.MessageTime==0) Player.MessageNo=0;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Анимация всего                                                    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void AnimateAll(int sector)
{
 ColorEffect();
 AnimateDoor(sector);
 AnimateGantry(sector);
 AnimateFloor_Flow();
 AnimateThing();
 MoveAlien();
 AnimateWeapon();
 AnimateMissile();
 AnimateFace(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Вызов всех функций прорисовки и анимации                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int ViewAll(void)
{
 int sector;
 Player.P_X=Player.X*65536;
 Player.P_Y=Player.Y*65536;
 sector=FindSector(Player.X,Player.Y);
 if (Player.Life!=-1)
  {
   Fallen(sector);
   SpecialSector(sector);
  }
 Video.ClearWindow();
 AnimateAll(sector);
 if (Player.AutoMap==1) PutAutoMap();
  else
   {
    View();
    PutWeapon();
   }
 if (Player.Life==-1) Video.Print(115,79,"GAME OVER");
 PutMessage();
 Video.ViewWindow();
 return(sector);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Функция управления от клавиатуры  и мыши                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int KeyboardControl(int sector)
{
 int mButton,mX,mY;
 m_getpos(&mButton,&mX,&mY);
 int dx=mX-160;
 int dy=mY-100;
 if (dx!=0 || dy!=0) m_setpos(160,100);
 if (dx!=0)
  {
   Player.Angle+=dx;
   if (Player.Angle<0) Player.Angle+=360;
   if (Player.Angle>=360) Player.Angle-=360;
  }
 if (Consol.GetPressKey(USE) || mButton==2)
  {
   int done=Use(sector);//выход найден
   if (done==1)
    {
     Video.ClearWindow();
     View();
     Video.ViewWindow();
     Video.Clear();
     RunningScreen();
     return(done);
    }
  }
 if (Consol.GetPressKey(SIT))
  {
   Player.SitZ+=5;
   if (Player.SitZ>30) Player.SitZ=30;
  }
 else
  {
   Player.SitZ-=5;
   if (Player.SitZ<0) Player.SitZ=0;
   if (-Player.SitZ+Player.Z+20>Sector[sector].Up) Player.SitZ=-Sector[sector].Up+Player.Z+20;
  }
 if (Consol.GetPressKey(MAP))
  {
   if (Player.AutoMap==0) Player.AutoMap=1;
                     else Player.AutoMap=0;
   KeyTable[MAP]=0;
  }
 if (Consol.GetPressKey(WEAPON0)) ChangeWeapon(0);
 if (Consol.GetPressKey(WEAPON1)) ChangeWeapon(1);
 if (Consol.GetPressKey(WEAPON2)) ChangeWeapon(2);
 if (Consol.GetPressKey(WEAPON3)) ChangeWeapon(3);
 if (Consol.GetPressKey(WEAPON4)) ChangeWeapon(4);
 if ((Consol.GetPressKey(FIRE) || mButton==1) && Player.AutoMap==0) Fire();
 if (Consol.GetPressKey(UP)) MoveUp(0);
  else
   {
    if (Player.Inerci>0)
     {
      MoveUp(1);
      Player.Inerci-=2;
     }
   }
 if (Consol.GetPressKey(DOWN)) MoveDown(0);
  else
   {
    if (Player.Inerci<0)
     {
      MoveDown(1);
      Player.Inerci+=2;
     }
   }
 if (Consol.GetPressKey(REFRESH))
  {
   int t0=clock();
   Player.D_Angle=2;
   Player.Inerci=0;
   Player.Dz=0;
   for(int angle=0;angle<180;angle++)
    {
     RotateRight();
     Video.ClearWindow();
     View();
     Video.ViewWindow();
    }
   int t1=clock();
   Player.FPS=18000.0/(t1-t0);
   Player.MessageNo=16;
   Player.MessageTime=1000;
  }
 if (Consol.GetPressKey(SLEFT)) MoveLeft();
 if (Consol.GetPressKey(SRIGHT)) MoveRight();
 if (Consol.GetPressKey(LEFT))
  {
   if (Player.D_Angle>0) Player.D_Angle=0;
   Player.D_Angle--;
   if (Player.D_Angle<-5) Player.D_Angle=-5;
   RotateLeft();
  }
 if (Consol.GetPressKey(RIGHT))
  {
   if (Player.D_Angle<0) Player.D_Angle=0;
   Player.D_Angle++;
   if (Player.D_Angle>5) Player.D_Angle=5;
   RotateRight();
  }
 if (!(Consol.GetPressKey(RIGHT) || Consol.GetPressKey(LEFT))) Player.D_Angle=0;
 if (!(Consol.GetPressKey(UP) || Consol.GetPressKey(DOWN) || dy!=0 || Consol.GetPressKey(SRIGHT) || Consol.GetPressKey(SLEFT)))
  {
   Player.Pz=0;
   Player.Dz=0;
  }
 else
  {
   if (Player.Dz==0) Player.Dz=1;
  }
 if (Player.Pz>3) Player.Dz=-Player.Dz;
 if (Player.Pz<-3) Player.Dz=-Player.Dz;
 return(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Главная функция управления.                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int Control(void)
{
 int sector;
 KeyTable[ESC]=0;
 Player.Z=Sector[FindSector(Player.X,Player.Y)].Down+64;
 PutStatistik();
 m_setpos(160,100);
 while (1)
  {
   if (Timer/5==TimerUser/5) continue;
   Timer=TimerUser;
   sector=ViewAll();
   if (Consol.GetPressKey(ESC)) return(0);
   if (Player.Life==-1) continue;
   if (Player.Life==0)//вы проиграли
    {
     Player.SitZ=0;
     for(int n=0;n<48;n++)
      {
       while (Timer==TimerUser)
        {
         int r=0;//задержка
        }
       Timer=TimerUser;
       Player.SitZ++;
       ViewAll();
      }
     Player.Life=-1;//игрок в отключке
    }
   if (KeyboardControl(sector)) return(1);
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Отвечает за обработку нажатия кнопки "использовать".              //
//W-номер сектора,в котором находиться игрок.                       //
//Возвращает 0-если это не конец пути.Иначе-1                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int Use(int w)
{
 if (Sector[w].Type==2)//переключатель
  {
   if (Sector[Sector[w].Sector].DZ==0 && Sector[Sector[w].Sector].Sector==0)
    {
     for(register int l=0;l<Info.SegmentNumber;l++)
      {
       if (Segment[l].Type>=3 && Segment[l].Type<=21 && (Segment[l].Sector1==w || Segment[l].Sector2==w))
        {
         if (DistanceOfVector(Segment[l].X1,Segment[l].Y1,Segment[l].X2,Segment[l].Y2))
          {
           Segment[l].Type=Segment[l].Type+1;
           Sector[Sector[w].Sector].Sector=1;
           Sector[Sector[w].Sector].Time=0;
           if (Sector[Sector[w].Sector].Type==3) Sector[Sector[w].Sector].DZ=-2;
           if (Sector[Sector[w].Sector].Type==5 || Sector[Sector[w].Sector].Type==8)
            Sector[Sector[w].Sector].DZ=(Sector[Sector[w].Sector].Down1-Sector[Sector[w].Sector].Down2)/abs((Sector[Sector[w].Sector].Down1-Sector[Sector[w].Sector].Down2))*2;
          }
        }
      }
    }
  }
 if (Sector[w].Type==7)//телепортация
  {
   Player.X=Sector[w].Sector;
   Player.Y=Sector[w].Down1;
   Player.Z=Sector[FindSector(Player.X,Player.Y)].Down+64;
   Player.P_X=Player.X*65536;
   Player.P_Y=Player.Y*65536;
   LightingMinus();
   Video.ClearWindow();
   if (Player.AutoMap==1) PutAutoMap();
    else
     {
      View();
      PutWeapon();
     }
   Video.ViewWindow();
   LightingPlus();
   if (Player.TimeSpacesuit!=0) Video.SetRGB_DAC(0.7,1,0.7);
   if (Player.TimeMegaSphere!=0) Video.SetRGB_DAC(0.7,0.7,1);
  }
 if (Sector[w].Type==6)//конец пути
  {
   for(register int l=0;l<Info.SegmentNumber;l++)//меняем текстуру переключателя
    {
     if (Segment[l].Type>=3 && Segment[l].Type<=21 && (Segment[l].Sector1==w || Segment[l].Sector2==w))
      {
       if (DistanceOfVector(Segment[l].X1,Segment[l].Y1,Segment[l].X2,Segment[l].Y2))
        {
         Segment[l].Type=Segment[l].Type+1;
         return(1);
        }
      }
    }
  }
 for(register int n=0;n<Info.SectorNumber;n++)
 {
  if (Sector[n].Type==1 || Sector[n].Type==4 || Sector[n].Type==5)//двери и платформы
   {
    for(int t=0;t<Sector[n].Segment-1;t++)
     {
      float x1=Sector[n].X[t];
      float y1=Sector[n].Y[t];
      float x2=Sector[n].X[t+1];
      float y2=Sector[n].Y[t+1];
      int l1=DistanceOfVector(x1,y1,x1,y2);
      int l2=DistanceOfVector(x1,y2,x2,y2);
      int l3=DistanceOfVector(x2,y2,x2,y1);
      int l4=DistanceOfVector(x2,y1,x1,y1);
      if (l1!=0 || l2!=0 || l3!=0 || l4!=0 || n==w)
       {
        if (Sector[n].Type==1 && Sector[n].DZ==0)
         {
          Sector[n].DZ=-5;
         }
        if ((Sector[n].Type==4 || (Sector[n].Sector==1 && Sector[n].Type==5)) && Sector[n].DZ==0)
         {
          Sector[n].DZ=5*(Sector[n].Down1-Sector[n].Down2)/abs((Sector[n].Down1-Sector[n].Down2));
         }
       }
     }
   }
 }
 return(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Поворот игрока налево.                                            //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void RotateLeft(void)
{
 Player.Angle+=Player.D_Angle;
 if (Consol.GetPressKey(SPEED)) Player.Angle+=Player.D_Angle;
 if (Player.Angle<0) Player.Angle+=360;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Поворот игрока направо.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void RotateRight(void)
{
 Player.Angle+=Player.D_Angle;
 if (Consol.GetPressKey(SPEED)) Player.Angle+=Player.D_Angle;
 if (Player.Angle>360) Player.Angle-=360;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Движение игрока вперёд.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveUp(int type)
{
 float x=Player.X;
 float y=Player.Y;
 float ss=SinTable[Player.Angle];
 float cs=CosTable[Player.Angle];
 Player.X+=ss*Player.Inerci;
  Player.Y+=cs*Player.Inerci;
  if (Consol.GetPressKey(SPEED))
   {
    Player.X+=abs(Player.Inerci)*ss;
    Player.Y+=abs(Player.Inerci)*cs;
   }
  int k=Impact(Player.X,Player.Y,Player.Z-Player.SitZ,20,64-Player.SitZ,-1);
  if (k==0) ImpactPlayerToThing(Player.X,Player.Y,Player.Z,20,64);
  if (k<0)
   {
    Player.X=x;
    Player.Y=y;
   }
  if (k>0)//есть столкновение
   {
    SlideTraverse(x,y,Player.X,Player.Y);
    Player.Inerci=0;
   }
 Player.Pz+=Player.Dz;
 if (type==0)
  {
   Player.Inerci+=2;
   if (Player.Inerci>10) Player.Inerci=10;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Движение игрока назад.                                            //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveDown(int type)
{
 float x=Player.X;
 float y=Player.Y;
 float ss=SinTable[Player.Angle];
 float cs=CosTable[Player.Angle];
 Player.X+=Player.Inerci*ss;
 Player.Y+=Player.Inerci*cs;
 if (Consol.GetPressKey(SPEED))
  {
   Player.X+=Player.Inerci*ss;
   Player.Y+=Player.Inerci*cs;
  }
 int k=Impact(Player.X,Player.Y,Player.Z-Player.SitZ,20,64-Player.SitZ,-1);
 if (k==0) ImpactPlayerToThing(Player.X,Player.Y,Player.Z,20,64);
 if (k<0)
  {
   Player.X=x;
   Player.Y=y;
  }
 if (k>0)//есть столкновение
  {
   SlideTraverse(x,y,Player.X,Player.Y);
   Player.Inerci=0;
  }
 Player.Pz+=Player.Dz;
 if (type==0)
  {
   Player.Inerci-=2;
   if (Player.Inerci<-10) Player.Inerci=-10;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Движение игрока влево.                                            //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveLeft(void)
{
 float x=Player.X;
 float y=Player.Y;
 float ss=SinTable[Player.Angle];
 float cs=CosTable[Player.Angle];
 Player.X-=10*cs;
 Player.Y+=10*ss;
 if (Consol.GetPressKey(SPEED))
  {
   Player.X-=10*cs;
   Player.Y+=10*ss;
  }
 int k=Impact(Player.X,Player.Y,Player.Z-Player.SitZ,20,64-Player.SitZ,-1);
 if (k==0) ImpactPlayerToThing(Player.X,Player.Y,Player.Z,20,64);
 if (k!=0)
  {
   Player.X=x;
   Player.Y=y;
  }
 Player.Pz+=Player.Dz;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Движение игрока вправо.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveRight(void)
{
 float x=Player.X;
 float y=Player.Y;
 float ss=SinTable[Player.Angle];
 float cs=CosTable[Player.Angle];
 Player.X+=10*cs;
 Player.Y-=10*ss;
 if (Consol.GetPressKey(SPEED))
  {
   Player.X+=10*cs;
   Player.Y-=10*ss;
  }
 int k=Impact(Player.X,Player.Y,Player.Z-Player.SitZ,20,64-Player.SitZ,-1);
 if (k==0) ImpactPlayerToThing(Player.X,Player.Y,Player.Z,20,64);
 if (k!=0)
  {
   Player.X=x;
   Player.Y=y;
  }
 Player.Pz+=Player.Dz;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Падение игрока.                                                   //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Fallen(int sector)
{
 int down=Sector[sector].Down;
 int bottom=Player.Z-64;
 int dz=Player.PV;
 if (down<bottom)
  {
   if (bottom-down<dz) dz=bottom-down;
   if (Impact(Player.X,Player.Y,Player.Z-dz,20,64,-1)==0) Player.Z-=dz;
  }
 else Player.Z=down+64;
}
void PutWeapon(void)
{
 int i=Weapon.WeaponNo;
 int j=Weapon.Fire_Faze;
 int dw=Player.Pz/2;
 if (i==0) PutStaticSprite(Weapon.X[i][j],Weapon.Y[i][j]-dw+Weapon.Vy,Data.Weapon0,Weapon.Faze[i][j]);
 if (i==1) PutStaticSprite(Weapon.X[i][j],Weapon.Y[i][j]-dw+Weapon.Vy,Data.Weapon1,Weapon.Faze[i][j]);
 if (i==2) PutStaticSprite(Weapon.X[i][j],Weapon.Y[i][j]-dw+Weapon.Vy,Data.Weapon2,Weapon.Faze[i][j]);
 if (i==3) PutStaticSprite(Weapon.X[i][j],Weapon.Y[i][j]-dw+Weapon.Vy,Data.Weapon3,Weapon.Faze[i][j]);
 if (i==4) PutStaticSprite(Weapon.X[i][j],Weapon.Y[i][j]-dw+Weapon.Vy,Data.Weapon4,Weapon.Faze[i][j]);
}
void AnimateWeapon(void)
{
 int i=Weapon.WeaponNo;
 int j=Weapon.Fire_Faze;
 if (j==0)
  {
   Weapon.Vy+=Weapon.Dy;
   if (Weapon.Vy>70)
    {
     Weapon.Dy=-8;
     Weapon.WeaponNo=Weapon.Weapon;//меняем оружие
     PutStatistik();
    }
   if (Weapon.Vy==0 && Weapon.Dy!=0) Weapon.Dy=0;
  }
 if (j==0) return;//нечего анимировать
 Weapon.Fire_Faze=j+1;
 if (Weapon.Faze[i][j+1]==-1)
  {
   Weapon.Fire_Faze=0;//закончили анимацию
   if (i!=0) Weapon.Ammo[i]--;
   PutStatistik();
  }
}
void Fire(void)
{
 int i=Weapon.WeaponNo;
 int j=Weapon.Fire_Faze;
 if (j!=0) return;//ещё не закончился предыдущий выстрел
 if (Weapon.Dy!=0 || Weapon.Vy!=0) return;//оружие ещё не поднялось
 if (Weapon.Ammo[i]==0)//патронов нет
  {
   //ищем другое оружие
   for(int n=4;n>=0;n--)
    {
     if (Weapon.Ammo[n]>0 && Weapon.Attendance[n]!=0)
      {
       Weapon.Dy=8;
       Weapon.Weapon=n;
       return;
      }
    }
  }
 if (i==0)//если это кулаки
  {
   Weapon.Fire_Faze=1;//начали анимацию
   int minNo=-1;
   float mindist=10000000;
   float cs=CosTable[Player.Angle];
   float ss=SinTable[Player.Angle];
   for(int n=0;n<NowAlien;n++)
    {
     int z1=Alien[n]->Coord.Z;
     int z2=Alien[n]->Coord.Z-Alien[n]->Coord.Height;
     if (z2>Player.Z || z1<Player.Z) continue;//не совпадают высоты
     float x1=Alien[n]->Coord.X-Player.X;
     float y1=Alien[n]->Coord.Y-Player.Y;
     float v=x1*ss+y1*cs;
     if (v<=1) continue;
     if (v>70) continue;
     register int u=x1*cs-y1*ss;
     int col1=160+160*(u-Alien[n]->Coord.Radius/2)/v;
     int col2=160+160*(u+Alien[n]->Coord.Radius/2)/v;
     if (col1>319) continue;
     if (col2<0) continue;
     if (col2>=160 && col1<=160)
      {
       if (v<=mindist)
        {
         mindist=v;
         minNo=n;
        }
      }
    }
   if (minNo!=-1)
    {
     Alien[minNo]->Life-=25;
     if (Alien[minNo]->Life<=0)
      {
       Alien[minNo]->Life=0;
       Alien[minNo]->Faze=Alien[minNo]->Destroy;
      }
    }
   return;
  }
 //иначе посылаем снаряд
 if (i==1 || i==3 || i==4)//пистолет,пулемёт и плазмомёт создают один снаряд
  {
   int angle=Player.Angle+rnd(4)-2;
   if (angle>=360) angle-=360;
   if (angle<0) angle+=360;
   float ss=SinTable[angle];
   float cs=CosTable[angle];
   angle=rnd(4)-2;
   if (angle>=360) angle-=360;
   if (angle<0) angle+=360;
   float dz=SinTable[angle];
   float za=FindAlien();
   if (za!=0) dz=0;
   if (i==1) CreateNewGunFire(Player.X,Player.Y,Player.Z-Player.SitZ+10,ss,cs,dz+za,1000,10,i,-1);
   if (i==3) CreateNewGunFire(Player.X,Player.Y,Player.Z-Player.SitZ+10,ss,cs,dz+za,1000,15,i,-1);
   if (i==4) CreateNewPlasmaGunFire(Player.X,Player.Y,Player.Z-Player.SitZ+10,ss,cs,dz+za,40,50,i,-1);
  }
 if (i==2)//ружьё создаёт восемь снарядов
  {
   for(int n=0;n<8;n++)
    {
     int angle=Player.Angle+rnd(10)-5;
     if (angle>=360) angle-=360;
     if (angle<0) angle+=360;
     float ss=SinTable[angle];
     float cs=CosTable[angle];
     angle=rnd(10)-5;
     if (angle>=360) angle-=360;
     if (angle<0) angle+=360;
     float dz=SinTable[angle];
     float za=FindAlien();
     CreateNewGunFire(Player.X,Player.Y,Player.Z-Player.SitZ+5,ss,cs,dz+za,1000,10,i,-1);
    }
  }
 Weapon.Fire_Faze=1;//начали анимацию оружия
}
void ChangeWeapon(int number)
{
 int i=Weapon.WeaponNo;
 if (Weapon.Attendance[number]==0) return;//у нас нет такого оружия
 if (i==number) return;
 Weapon.Dy=8;
 Weapon.Weapon=number;
}
void AnimateMissile(void)
{
 int n;
 for(n=0;n<NowMissile;n++)
  {
   Missile[n]->Faze++;
   if (Missile[n]->Footage[Missile[n]->Faze]==-1)
    {
     Missile[n]->Faze=0;
     if (Missile[n]->Speed==0)
      {
       DeleteMissile(n);
       continue;
      }
    }
   if (Missile[n]->Speed==0) continue;
   float speed=Missile[n]->Speed;
   float dx=Missile[n]->Dx*speed;
   float dy=Missile[n]->Dy*speed;
   float dz=Missile[n]->Dz*sqrt(dx*dx+dy*dy);
   float x=Missile[n]->X+dx;
   float y=Missile[n]->Y+dy;
   float z=Missile[n]->Z+dz;
   if (dx==0 && dy==0 && dz==0)
    {
     Missile[n]->Speed=0;//делаем анимацию взрыва снаряда
     Missile[n]->Faze=Missile[n]->Destroy;
     continue;
    }
   float xo,yo;
   int object;
   int sw=CheckWay(Missile[n]->X,Missile[n]->Y,Missile[n]->Z-Missile[n]->Radius,x,y,z-Missile[n]->Radius,Missile[n]->Radius,2*Missile[n]->Radius,Missile[n]->Object,&xo,&yo,&object);
   if (sw!=0)
    {
     float dt=sqrt((Missile[n]->X-xo)*(Missile[n]->X-xo)+(Missile[n]->Y-yo)*(Missile[n]->Y-yo));
     Missile[n]->Z=Missile[n]->Z+Missile[n]->Dz*(dt-Missile[n]->Radius*2);
     Missile[n]->X=xo-Missile[n]->Dx*Missile[n]->Radius*2;
     Missile[n]->Y=yo-Missile[n]->Dy*Missile[n]->Radius*2;
     if (object>0)//снаряд попал во врага
      {
       int objectno=object-1;
       Alien[objectno]->Active=1;//активизируем врага
       Alien[objectno]->Life-=Missile[n]->Power;
       if (Alien[objectno]->Life<=0)
        {
         Alien[objectno]->Life=0;
         Alien[objectno]->Faze=Alien[objectno]->Destroy;
        }
      }
     if (object==-1 && Player.Life!=-1)//снаряд попал в игрока
      {
       if (Player.IDDQD==0 && Player.TimeMegaSphere==0) Player.Life-=Missile[n]->Power;
       if (Player.Life<0) Player.Life=0;
       PutStatistik();
       Player.RedScreen=3;//число циклов красного экрана
       Player.RedScreenOn=0;
       AnimateFace(2);
      }
     Missile[n]->Speed=0;//делаем анимацию взрыва снаряда
     Missile[n]->Faze=Missile[n]->Destroy;
    }
   else
    {
     Missile[n]->X=x;
     Missile[n]->Y=y;
     Missile[n]->Z=z;
    }
  }
}
void PutStatistik()
{
 int i=Weapon.WeaponNo;
 char s[255];
 PutBar(Data.Bar,0,0,168);
 itoa(Weapon.Ammo[i],s,10);
 Video.Print(13,180,s);
 itoa(Player.Life,s,10);
 Video.Print(60,180,s);
 AnimateFace(1);
}
float FindAlien(void)
{
 int minNo=-1;
 float mindist=10000000;
 float cs=CosTable[Player.Angle];
 float ss=SinTable[Player.Angle];
 for(int n=0;n<NowAlien;n++)
  {
   if (Alien[n]->Life==0) continue;
   if (Alien[n]->Visible==0) continue;
   float x1=Alien[n]->Coord.X-Player.X;
   float y1=Alien[n]->Coord.Y-Player.Y;
   float v=x1*ss+y1*cs;
   if (v<=1) continue;
   if (v>1000) continue;
   register int u=x1*cs-y1*ss;
   int col1=160+160*(u-Alien[n]->Coord.Radius/2)/v;
   int col2=160+160*(u+Alien[n]->Coord.Radius/2)/v;
   if (col1>319) continue;
   if (col2<0) continue;
   if (col1>=140 && col1<=180 || col2>=140 && col2<=180)
    {
     if (v<=mindist)
      {
       mindist=v;
       minNo=n;
      }
    }
  }
 if (minNo!=-1)
  {
   float dz=((Alien[minNo]->Coord.Z-Alien[minNo]->Coord.Height/2)-(Player.Z-Player.SitZ+5))/mindist;
   return(dz);
  }
 return(0);
}
