#include "control.h"

int Timer;

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
    u=sMaze.BlockMap[m][n][0];
    for(l=1;l<=u;l++)
     {
      k=sMaze.BlockMap[m][n][l];
      float xw1=sSegment[k].X1-x;//координаты стен
      float xw2=sSegment[k].X2-x;
      float yw1=sSegment[k].Y1-y;
      float yw2=sSegment[k].Y2-y;
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
            if (sSegment[k].Frontier!=0)//линия раздела
             {
              //проверим по высоте
              int down1=sSector[sSegment[k].Sector1].Down-32;
              int up1=sSector[sSegment[k].Sector1].Up;
              int down2=sSector[sSegment[k].Sector2].Down-32;
              int up2=sSector[sSegment[k].Sector2].Up;
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
            if (sSegment[k].Frontier!=0)//линия раздела
             {
              //проверим по высоте
              int down1=sSector[sSegment[k].Sector1].Down-32;
              int up1=sSector[sSegment[k].Sector1].Up;
              int down2=sSector[sSegment[k].Sector2].Down-32;
              int up2=sSector[sSegment[k].Sector2].Up;
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
   if (cThing[n]->Coord.Z<bottom || cThing[n]->Coord.Z-cThing[n]->Coord.Height>top) continue;
   int x1=cThing[n]->Coord.X-x;
   int y1=cThing[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<cThing[n]->Coord.Radius+radius)
    {
     if (cThing[n]->Obstruction!=0) return(-1);//объект является препятствием
    }
  }
 //проверим столкновение с врагами
 for(n=0;n<NowAlien;n++)
  {
   if (n==objno) continue;
   if (cAlien[n]->Life==0) continue;
   if (cAlien[n]->Coord.Z<bottom || cAlien[n]->Coord.Z-cAlien[n]->Coord.Height>top) continue;
   int x1=cAlien[n]->Coord.X-x;
   int y1=cAlien[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<cAlien[n]->Coord.Radius+radius) return(-1);
  }
 if (objno!=-1)//для врагов проверим столкновение с игроком
  {
   if (cAlien[objno]->Life!=0 &&
      !(cAlien[objno]->Coord.Z<sPlayer.Z-64 || cAlien[objno]->Coord.Z-cAlien[objno]->Coord.Height>sPlayer.Z-sPlayer.SitZ))
     {
      int x1=cAlien[objno]->Coord.X-sPlayer.X;
      int y1=cAlien[objno]->Coord.Y-sPlayer.Y;
      int len=sqrt(x1*x1+y1*y1);
      if (len<cAlien[objno]->Coord.Radius+20) return(-1);
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
    u=sMaze.BlockMap[m][n][0];
    for(l=1;l<=u;l++)
     {
      k=sMaze.BlockMap[m][n][l];
      float xw1=sSegment[k].X1-x;//координаты стен
      float xw2=sSegment[k].X2-x;
      float yw1=sSegment[k].Y1-y;
      float yw2=sSegment[k].Y2-y;
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
            if (sSegment[k].Frontier!=0)//линия раздела
             {
              //проверим по высоте
              int down1=sSector[sSegment[k].Sector1].Down-32;
              int up1=sSector[sSegment[k].Sector1].Up;
              int down2=sSector[sSegment[k].Sector2].Down-32;
              int up2=sSector[sSegment[k].Sector2].Up;
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
            if (sSegment[k].Frontier!=0)//линия раздела
             {
              //проверим по высоте
              int down1=sSector[sSegment[k].Sector1].Down-32;
              int up1=sSector[sSegment[k].Sector1].Up;
              int down2=sSector[sSegment[k].Sector2].Down-32;
              int up2=sSector[sSegment[k].Sector2].Up;
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
   if (cThing[n]->Coord.Z<bottom || cThing[n]->Coord.Z-cThing[n]->Coord.Height>top) continue;
   int x1=cThing[n]->Coord.X-x;
   int y1=cThing[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<cThing[n]->Coord.Radius+radius)
    {
     if (cThing[n]->Obstruction!=0)
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
   if (cAlien[n]->Life==0) continue;
   if (cAlien[n]->Coord.Z<bottom || cAlien[n]->Coord.Z-cAlien[n]->Coord.Height>top) continue;
   int x1=cAlien[n]->Coord.X-x;
   int y1=cAlien[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<cAlien[n]->Coord.Radius+radius)
    {
     *collizionamount=collizion_amount;
     *collizionarray=collizion_array;
     return(-1);
    }
  }
 if (objno!=-1)//для врагов проверим столкновение с игроком
  {
   if (cAlien[objno]->Life!=0 &&
      !(cAlien[objno]->Coord.Z<sPlayer.Z-64 || cAlien[objno]->Coord.Z-cAlien[objno]->Coord.Height>sPlayer.Z-sPlayer.SitZ))
     {
      int x1=cAlien[objno]->Coord.X-sPlayer.X;
      int y1=cAlien[objno]->Coord.Y-sPlayer.Y;
      int len=sqrt(x1*x1+y1*y1);
      if (len<cAlien[objno]->Coord.Radius+20)
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
void ImpactsPlayerToThing(float x,float y,float z,int radius,int height)
{
 int top=z;
 int bottom=z-height;
 for(int n=0;n<NowThing;n++)
  {
   if (cThing[n]->Coord.Z<bottom || cThing[n]->Coord.Z-cThing[n]->Coord.Height>top) continue;
   int x1=cThing[n]->Coord.X-x;
   int y1=cThing[n]->Coord.Y-y;
   int len=sqrt(x1*x1+y1*y1);
   if (len<cThing[n]->Coord.Radius+radius)
    {
     if (cThing[n]->Type==3)//аптечка
      {
       if (sPlayer.Life<100)
        {
         sPlayer.Life+=20;
         if (sPlayer.Life>100) sPlayer.Life=100;
         DeleteThing(n);
         sPlayer.MessageNo=4;
         sPlayer.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (cThing[n]->Type==4)//супер аптечка
      {
       if (sPlayer.Life<100)
        {
         sPlayer.Life=100;
         DeleteThing(n);
         sPlayer.MessageNo=5;
         sPlayer.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (cThing[n]->Type==5)//ружъё
      {
       int i=0;
       if (sWeapon.Attendance[2]!=0 && sWeapon.Ammo[2]!=0) i=1;
       if (sWeapon.Attendance[3]!=0 && sWeapon.Ammo[3]!=0) i=1;
       if (sWeapon.Attendance[4]!=0 && sWeapon.Ammo[4]!=0) i=1;
       if (sWeapon.Ammo[2]<sWeapon.MaxAmmo[2] || sWeapon.Attendance[2]==0)
        {
         sWeapon.Attendance[2]=1;
         sWeapon.Ammo[2]+=30;
         if (sWeapon.Ammo[2]>sWeapon.MaxAmmo[2]) sWeapon.Ammo[2]=sWeapon.MaxAmmo[2];
         DeleteThing(n);
         sPlayer.MessageNo=9;
         sPlayer.MessageTime=50;
         PutStatistik();
         if (i==0) ChangeWeapon(2);
         continue;
        }
      }
     if (cThing[n]->Type==6)//пулемёт
      {
       int i=0;
       if (sWeapon.Attendance[3]!=0 && sWeapon.Ammo[3]!=0) i=1;
       if (sWeapon.Attendance[4]!=0 && sWeapon.Ammo[4]!=0) i=1;
       if (sWeapon.Ammo[3]<sWeapon.MaxAmmo[3] || sWeapon.Attendance[3]==0)
        {
         sWeapon.Attendance[3]=1;
         sWeapon.Ammo[3]+=100;
         if (sWeapon.Ammo[3]>sWeapon.MaxAmmo[3]) sWeapon.Ammo[3]=sWeapon.MaxAmmo[3];
         DeleteThing(n);
         sPlayer.MessageNo=10;
         sPlayer.MessageTime=50;
         PutStatistik();
         if (i==0) ChangeWeapon(3);
         continue;
        }
      }
     if (cThing[n]->Type==7)//плазмомёт
      {
       int i=0;
       if (sWeapon.Attendance[4]!=0 && sWeapon.Ammo[4]!=0) i=1;
       if (sWeapon.Ammo[4]<sWeapon.MaxAmmo[4] || sWeapon.Attendance[4]==0)
        {
         sWeapon.Attendance[4]=1;
         sWeapon.Ammo[4]+=10;
         if (sWeapon.Ammo[4]>sWeapon.MaxAmmo[4]) sWeapon.Ammo[4]=sWeapon.MaxAmmo[4];
         DeleteThing(n);
         sPlayer.MessageNo=11;
         sPlayer.MessageTime=50;
         PutStatistik();
         if (i==0) ChangeWeapon(4);
         continue;
        }
      }
     if (cThing[n]->Type==8)//патроны к ружъю
      {
       if (sWeapon.Ammo[2]<sWeapon.MaxAmmo[2])
        {
         sWeapon.Ammo[2]+=15;
         if (sWeapon.Ammo[2]>sWeapon.MaxAmmo[2]) sWeapon.Ammo[2]=sWeapon.MaxAmmo[2];
         DeleteThing(n);
         sPlayer.MessageNo=3;
         sPlayer.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (cThing[n]->Type==9)//патроны к пулемёту
      {
       if (sWeapon.Ammo[3]<sWeapon.MaxAmmo[3])
        {
         sWeapon.Ammo[3]+=100;
         if (sWeapon.Ammo[3]>sWeapon.MaxAmmo[3]) sWeapon.Ammo[3]=sWeapon.MaxAmmo[3];
         DeleteThing(n);
         sPlayer.MessageNo=8;
         sPlayer.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (cThing[n]->Type==2)//патроны к плазмомёту
      {
       if (sWeapon.Ammo[4]<sWeapon.MaxAmmo[4])
        {
         sWeapon.Ammo[4]+=10;
         if (sWeapon.Ammo[4]>sWeapon.MaxAmmo[4]) sWeapon.Ammo[4]=sWeapon.MaxAmmo[4];
         DeleteThing(n);
         sPlayer.MessageNo=12;
         sPlayer.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (cThing[n]->Type==10)//патроны к пистолету
      {
       if (sWeapon.Ammo[1]<sWeapon.MaxAmmo[1])
        {
         sWeapon.Ammo[1]+=10;
         if (sWeapon.Ammo[1]>sWeapon.MaxAmmo[1]) sWeapon.Ammo[1]=sWeapon.MaxAmmo[1];
         DeleteThing(n);
         sPlayer.MessageNo=2;
         sPlayer.MessageTime=50;
         PutStatistik();
         continue;
        }
      }
     if (cThing[n]->Type==11)//карта
      {
       for(int m=0;m<sInfo.SegmentNumber;m++) sSegment[m].Visible=1;
       DeleteThing(n);
       sPlayer.MessageNo=14;
       sPlayer.MessageTime=50;
       continue;
      }
     if (cThing[n]->Type==12)//скафандр
      {
       if (sPlayer.TimeSpacesuit!=0) continue;
       sPlayer.TimeSpacesuit=1000;
       DeleteThing(n);
       sPlayer.MessageNo=13;
       sPlayer.MessageTime=50;
       continue;
      }
     if (cThing[n]->Type==13)//мегасфера(бессмертие)
      {
       if (sPlayer.TimeMegaSphere!=0) continue;
       sPlayer.TimeMegaSphere=1000;
       sPlayer.MessageNo=6;
       sPlayer.MessageTime=50;
       DeleteThing(n);
       continue;
      }
     if (cThing[n]->Type==14)//суперсфера(+100 жизней)
      {
       if (sPlayer.Life>=200) continue;
       sPlayer.Life+=100;
       if (sPlayer.Life>200) sPlayer.Life=200;
       DeleteThing(n);
       sPlayer.MessageNo=7;
       sPlayer.MessageTime=50;
       PutStatistik();
       continue;
      }
     if (cThing[n]->Type==15)//живая вода
      {
       if (sPlayer.Life>=200) continue;
       sPlayer.Life++;
       if (sPlayer.Life>200) sPlayer.Life=200;
       DeleteThing(n);
       sPlayer.MessageNo=1;
       sPlayer.MessageTime=50;
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
 int u=sMaze.BlockMap[yb][xb][0];
 float xot,yot;
 float minx,miny;
 int obj=0;
 for(int n=1;n<=u;n++)
  {
   int k=sMaze.BlockMap[yb][xb][n];
   int xw1=sSegment[k].X1;
   int xw2=sSegment[k].X2;
   int yw1=sSegment[k].Y1;
   int yw2=sSegment[k].Y2;
   if (CollizeLine(xw1,yw1,xw2,yw2,x1,y1,x2,y2)!=0)
    {
     Intersection(xw1,yw1,xw2,yw2,x1,y1,x2,y2,&xot,&yot);
     dist=sqrt((x1-xot)*(x1-xot)+(y1-yot)*(y1-yot));
     zu=z1+dz*dist;
     zd=zu-height;
     if (sSegment[k].Frontier!=0)
      {
       int down1=sSector[sSegment[k].Sector1].Down;
       int up1=sSector[sSegment[k].Sector1].Up;
       int down2=sSector[sSegment[k].Sector2].Down;
       int up2=sSector[sSegment[k].Sector2].Up;
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
   int sectorUp=sSector[sector].Up;
   int sectorDown=sSector[sector].Down;
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
 for(int n=0;n<NowThing;n++)
  {
   if (cThing[n]->Obstruction==0) continue;
   int up=cThing[n]->Coord.Z;
   int down=cThing[n]->Coord.Z-cThing[n]->Coord.Height;
   int x=cThing[n]->Coord.X;
   int y=cThing[n]->Coord.Y;
   int xw1=x1-x;
   int xw2=x2-x;
   int yw1=y1-y;
   int yw2=y2-y;
   float a=xw2-xw1;
   float b=yw2-yw1;
   float kd=a*a+b*b;
   float ld=2*a*xw1+2*b*yw1;
   float md=xw1*xw1+yw1*yw1-cThing[n]->Coord.Radius*cThing[n]->Coord.Radius;
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
 for(int n=0;n<NowAlien;n++)
  {
   if (n==man) continue;
   if (cAlien[n]->Life==0) continue;
   int up=cAlien[n]->Coord.Z;
   int down=cAlien[n]->Coord.Z-cAlien[n]->Coord.Height;
   int x=cAlien[n]->Coord.X;
   int y=cAlien[n]->Coord.Y;
   int xw1=x1-x;
   int xw2=x2-x;
   int yw1=y1-y;
   int yw2=y2-y;
   float a=xw2-xw1;
   float b=yw2-yw1;
   float kd=a*a+b*b;
   float ld=2*a*xw1+2*b*yw1;
   float md=xw1*xw1+yw1*yw1-cAlien[n]->Coord.Radius*cAlien[n]->Coord.Radius*1.5;
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
 int up=sPlayer.Z-sPlayer.SitZ;
 int down=sPlayer.Z-64;
 if (man!=-1)
  {
   int x=sPlayer.X;
   int y=sPlayer.Y;
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
 if (ImpactAll(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ,20,64-sPlayer.SitZ,-1,&Collizion_Array,&Collizion_Amount)<0)
 {
  sPlayer.X=xp1;
  sPlayer.Y=yp1;
  free(Collizion_Array);
  return;
 }
 sPlayer.X=xp1;
 sPlayer.Y=yp1;
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
   float xw1=sSegment[line].X1;
   float yw1=sSegment[line].Y1;
   float xw2=sSegment[line].X2;
   float yw2=sSegment[line].Y2;
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
    if (Impact(x+Dx,y+Dy,sPlayer.Z-sPlayer.SitZ,20,64-sPlayer.SitZ,-1)!=0)
     {
      Dx=0;
      Dy=0;
     }
    x=x+Dx;
    y=y+Dy;
  }
 sPlayer.X=x;
 sPlayer.Y=y;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Инициализирует данные игрока                                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void InitPlayer(void)
{
 sPlayer.SitZ=0;
 sPlayer.Dz=0;
 sPlayer.Pz=0;
 sPlayer.AutoMap=0;
 sPlayer.D_Angle=0;
 sPlayer.PV=9.8;
 sPlayer.Life=100;
 sPlayer.RedScreenOn=0;
 sPlayer.RedScreen=0;
 sPlayer.TimeSpacesuit=0;
 sPlayer.TimeMegaSphere=0;
 sPlayer.IDDQD=0;//нет бессмертия
 sPlayer.MessageNo=0;
 sPlayer.MessageTime=0;
 sPlayer.Level=1;
 sPlayer.Inerci=0;
 //установка оружия
 sWeapon.Fire_Faze=0;
 sWeapon.WeaponNo=1;
 sWeapon.Vy=0;
 sWeapon.Dy=0;
 int i;
 //======================0======================
 i=0;
 sWeapon.Ammo[i]=1;
 sWeapon.MaxAmmo[i]=1;
 sWeapon.Attendance[i]=1;
 sWeapon.X[i][0]=190;
 sWeapon.Y[i][0]=146;
 sWeapon.Faze[i][0]=0;
 sWeapon.X[i][1]=190;
 sWeapon.Y[i][1]=146;
 sWeapon.Faze[i][1]=0;
 sWeapon.X[i][2]=50;
 sWeapon.Y[i][2]=135;
 sWeapon.Faze[i][2]=1;
 sWeapon.X[i][3]=50;
 sWeapon.Y[i][3]=135;
 sWeapon.Faze[i][3]=1;
 sWeapon.X[i][4]=50;
 sWeapon.Y[i][4]=135;
 sWeapon.Faze[i][4]=1;
 sWeapon.X[i][5]=50;
 sWeapon.Y[i][5]=135;
 sWeapon.Faze[i][5]=1;
 sWeapon.X[i][6]=50;
 sWeapon.Y[i][6]=135;
 sWeapon.Faze[i][6]=1;
 sWeapon.X[i][7]=50;
 sWeapon.Y[i][7]=135;
 sWeapon.Faze[i][7]=1;
 sWeapon.X[i][8]=50;
 sWeapon.Y[i][8]=121;
 sWeapon.Faze[i][8]=2;
 sWeapon.X[i][9]=50;
 sWeapon.Y[i][9]=121;
 sWeapon.Faze[i][9]=2;
 sWeapon.X[i][10]=50;
 sWeapon.Y[i][10]=121;
 sWeapon.Faze[i][10]=2;
 sWeapon.X[i][11]=50;
 sWeapon.Y[i][11]=121;
 sWeapon.Faze[i][11]=2;
 sWeapon.X[i][12]=50;
 sWeapon.Y[i][12]=121;
 sWeapon.Faze[i][12]=2;
 sWeapon.X[i][13]=50;
 sWeapon.Y[i][13]=121;
 sWeapon.Faze[i][13]=2;
 sWeapon.X[i][14]=50;
 sWeapon.Y[i][14]=121;
 sWeapon.Faze[i][14]=2;
 sWeapon.X[i][15]=50;
 sWeapon.Y[i][15]=121;
 sWeapon.Faze[i][15]=2;
 sWeapon.X[i][16]=50;
 sWeapon.Y[i][16]=135;
 sWeapon.Faze[i][16]=1;
 sWeapon.X[i][17]=50;
 sWeapon.Y[i][17]=135;
 sWeapon.Faze[i][17]=1;
 sWeapon.X[i][18]=50;
 sWeapon.Y[i][18]=135;
 sWeapon.Faze[i][18]=1;
 sWeapon.X[i][19]=50;
 sWeapon.Y[i][19]=135;
 sWeapon.Faze[i][19]=1;
 sWeapon.X[i][20]=50;
 sWeapon.Y[i][20]=135;
 sWeapon.Faze[i][20]=1;
 sWeapon.X[i][21]=190;
 sWeapon.Y[i][21]=146;
 sWeapon.Faze[i][21]=0;
 sWeapon.X[i][22]=190;
 sWeapon.Y[i][22]=146;
 sWeapon.Faze[i][22]=0;
 sWeapon.X[i][23]=190;
 sWeapon.Y[i][23]=146;
 sWeapon.Faze[i][23]=0;
 sWeapon.Faze[i][24]=-1;
 //======================1======================
 i=1;
 sWeapon.Ammo[i]=50;
 sWeapon.MaxAmmo[i]=100;
 sWeapon.Attendance[i]=1;
 sWeapon.X[i][0]=133;
 sWeapon.Y[i][0]=126;
 sWeapon.Faze[i][0]=0;
 sWeapon.X[i][1]=133;
 sWeapon.Y[i][1]=126;
 sWeapon.Faze[i][1]=0;
 sWeapon.X[i][2]=131;
 sWeapon.Y[i][2]=107;
 sWeapon.Faze[i][2]=1;
 sWeapon.X[i][3]=131;
 sWeapon.Y[i][3]=107;
 sWeapon.Faze[i][3]=1;
 sWeapon.X[i][4]=131;
 sWeapon.Y[i][4]=107;
 sWeapon.Faze[i][4]=1;
 sWeapon.X[i][5]=131;
 sWeapon.Y[i][5]=107;
 sWeapon.Faze[i][5]=1;
 sWeapon.X[i][6]=130;
 sWeapon.Y[i][6]=86;
 sWeapon.Faze[i][6]=2;
 sWeapon.X[i][7]=130;
 sWeapon.Y[i][7]=86;
 sWeapon.Faze[i][7]=2;
 sWeapon.X[i][8]=130;
 sWeapon.Y[i][8]=86;
 sWeapon.Faze[i][8]=2;
 sWeapon.X[i][9]=131;
 sWeapon.Y[i][9]=107;
 sWeapon.Faze[i][9]=1;
 sWeapon.X[i][10]=131;
 sWeapon.Y[i][10]=107;
 sWeapon.Faze[i][10]=1;
 sWeapon.X[i][11]=131;
 sWeapon.Y[i][11]=107;
 sWeapon.Faze[i][11]=1;
 sWeapon.X[i][12]=133;
 sWeapon.Y[i][12]=126;
 sWeapon.Faze[i][12]=0;
 sWeapon.X[i][13]=133;
 sWeapon.Y[i][13]=126;
 sWeapon.Faze[i][13]=0;
 sWeapon.Faze[i][14]=-1;//конец анимации
 //======================2======================
 i=2;
 sWeapon.Ammo[i]=0;
 sWeapon.MaxAmmo[i]=100;
 sWeapon.Attendance[i]=0;
 sWeapon.X[i][0]=131;
 sWeapon.Y[i][0]=132;
 sWeapon.Faze[i][0]=0;
 sWeapon.X[i][1]=131;
 sWeapon.Y[i][1]=119;
 sWeapon.Faze[i][1]=1;
 sWeapon.X[i][2]=131;
 sWeapon.Y[i][2]=119;
 sWeapon.Faze[i][2]=1;
 sWeapon.X[i][3]=131;
 sWeapon.Y[i][3]=119;
 sWeapon.Faze[i][3]=1;

 sWeapon.X[i][4]=127;
 sWeapon.Y[i][4]=110;
 sWeapon.Faze[i][4]=2;
 sWeapon.X[i][5]=127;
 sWeapon.Y[i][5]=110;
 sWeapon.Faze[i][5]=2;
 sWeapon.X[i][6]=127;
 sWeapon.Y[i][6]=110;
 sWeapon.Faze[i][6]=2;

 sWeapon.X[i][7]=126;
 sWeapon.Y[i][7]=107;
 sWeapon.Faze[i][7]=4;
 sWeapon.X[i][8]=126;
 sWeapon.Y[i][8]=107;
 sWeapon.Faze[i][8]=4;
 sWeapon.X[i][9]=126;
 sWeapon.Y[i][9]=107;
 sWeapon.Faze[i][9]=4;
 sWeapon.X[i][10]=126;
 sWeapon.Y[i][10]=107;
 sWeapon.Faze[i][10]=4;
 sWeapon.X[i][11]=126;
 sWeapon.Y[i][11]=107;
 sWeapon.Faze[i][11]=4;
 sWeapon.X[i][12]=126;
 sWeapon.Y[i][12]=107;
 sWeapon.Faze[i][12]=4;
 sWeapon.X[i][13]=126;
 sWeapon.Y[i][13]=107;
 sWeapon.Faze[i][13]=4;

 sWeapon.X[i][14]=99;
 sWeapon.Y[i][14]=57;
 sWeapon.Faze[i][14]=3;
 sWeapon.X[i][15]=99;
 sWeapon.Y[i][15]=57;
 sWeapon.Faze[i][15]=3;
 sWeapon.X[i][16]=99;
 sWeapon.Y[i][16]=57;
 sWeapon.Faze[i][16]=3;
 sWeapon.X[i][17]=99;
 sWeapon.Y[i][17]=57;
 sWeapon.Faze[i][17]=3;
 sWeapon.X[i][18]=99;
 sWeapon.Y[i][18]=57;
 sWeapon.Faze[i][18]=3;
 sWeapon.X[i][19]=126;
 sWeapon.Y[i][19]=107;
 sWeapon.Faze[i][19]=4;
 sWeapon.X[i][20]=126;
 sWeapon.Y[i][20]=107;
 sWeapon.Faze[i][20]=4;
 sWeapon.X[i][21]=126;
 sWeapon.Y[i][21]=107;
 sWeapon.Faze[i][21]=4;
 sWeapon.X[i][22]=126;
 sWeapon.Y[i][22]=107;
 sWeapon.Faze[i][22]=4;
 sWeapon.X[i][23]=70;
 sWeapon.Y[i][23]=124;
 sWeapon.Faze[i][23]=5;
 sWeapon.X[i][24]=70;
 sWeapon.Y[i][24]=124;
 sWeapon.Faze[i][24]=5;
 sWeapon.X[i][25]=70;
 sWeapon.Y[i][25]=124;
 sWeapon.Faze[i][25]=5;
 sWeapon.X[i][26]=70;
 sWeapon.Y[i][26]=124;
 sWeapon.Faze[i][26]=5;

 sWeapon.X[i][27]=123;
 sWeapon.Y[i][27]=136;
 sWeapon.Faze[i][27]=6;
 sWeapon.X[i][28]=123;
 sWeapon.Y[i][28]=136;
 sWeapon.Faze[i][28]=6;
 sWeapon.X[i][29]=123;
 sWeapon.Y[i][29]=136;
 sWeapon.Faze[i][29]=6;

 sWeapon.X[i][30]=131;
 sWeapon.Y[i][30]=132;
 sWeapon.Faze[i][30]=0;
 sWeapon.X[i][31]=131;
 sWeapon.Y[i][31]=132;
 sWeapon.Faze[i][31]=0;
 sWeapon.Faze[i][32]=-1;//конец анимации
 //======================3======================
 i=3;
 sWeapon.Ammo[i]=0;
 sWeapon.MaxAmmo[i]=500;
 sWeapon.Attendance[i]=0;
 sWeapon.X[i][0]=114;
 sWeapon.Y[i][0]=137;
 sWeapon.Faze[i][0]=0;
 sWeapon.X[i][1]=114;
 sWeapon.Y[i][1]=117;
 sWeapon.Faze[i][1]=1;
 sWeapon.X[i][2]=114;
 sWeapon.Y[i][2]=117;
 sWeapon.Faze[i][2]=1;
 sWeapon.X[i][3]=114;
 sWeapon.Y[i][3]=116;
 sWeapon.Faze[i][3]=1;
 sWeapon.X[i][4]=114;
 sWeapon.Y[i][4]=116;
 sWeapon.Faze[i][4]=2;
 sWeapon.X[i][5]=114;
 sWeapon.Y[i][5]=136;
 sWeapon.Faze[i][5]=0;
 sWeapon.Faze[i][6]=-1;
 //======================4======================
 i=4;
 sWeapon.Ammo[i]=0;
 sWeapon.MaxAmmo[i]=100;
 sWeapon.Attendance[i]=0;
 sWeapon.X[i][0]=127;
 sWeapon.Y[i][0]=126;
 sWeapon.Faze[i][0]=0;
 sWeapon.X[i][1]=123;
 sWeapon.Y[i][1]=114;
 sWeapon.Faze[i][1]=1;
 sWeapon.X[i][2]=123;
 sWeapon.Y[i][2]=114;
 sWeapon.Faze[i][2]=1;
 sWeapon.X[i][3]=123;
 sWeapon.Y[i][3]=114;
 sWeapon.Faze[i][3]=1;
 sWeapon.X[i][4]=127;
 sWeapon.Y[i][4]=126;
 sWeapon.Faze[i][4]=0;
 sWeapon.X[i][5]=127;
 sWeapon.Y[i][5]=126;
 sWeapon.Faze[i][5]=0;
 sWeapon.X[i][6]=112;
 sWeapon.Y[i][6]=76;
 sWeapon.Faze[i][6]=2;
 sWeapon.X[i][7]=112;
 sWeapon.Y[i][7]=76;
 sWeapon.Faze[i][7]=2;
 sWeapon.X[i][8]=112;
 sWeapon.Y[i][8]=76;
 sWeapon.Faze[i][8]=2;
 sWeapon.X[i][9]=112;
 sWeapon.Y[i][9]=76;
 sWeapon.Faze[i][9]=2;
 sWeapon.X[i][10]=112;
 sWeapon.Y[i][10]=76;
 sWeapon.Faze[i][10]=2;
 sWeapon.X[i][11]=112;
 sWeapon.Y[i][11]=76;
 sWeapon.Faze[i][11]=2;
 sWeapon.X[i][12]=112;
 sWeapon.Y[i][12]=76;
 sWeapon.Faze[i][12]=2;
 sWeapon.X[i][13]=127;
 sWeapon.Y[i][13]=126;
 sWeapon.Faze[i][13]=0;
 sWeapon.X[i][14]=127;
 sWeapon.Y[i][14]=126;
 sWeapon.Faze[i][14]=0;
 sWeapon.Faze[i][15]=-1;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает 0, если расстояние от игрока до сегмента с координатами//
//xa,ya-xb,yb больше 50, либо этот перпендикуляр находится вне      //
//данного сегмента.Иначе возвращает 1.                              //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int DistanceOfVector(float xa,float ya,float xb,float yb)
{
 float xc=sPlayer.X;
 float yc=sPlayer.Y;
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
 for(register int n=0;n<sInfo.SectorNumber;n++)
  {
   if (sSector[n].Type==1 || sSector[n].Type==3)
    {
     if (sSector[n].DZ>0 && sSector[n].Time==0 && n==k)//игрок зажат дверью
      {
       if (sSector[n].Up<=sPlayer.Z-sPlayer.SitZ)
        {
         sSector[n].DZ=-abs(sSector[n].DZ);//дверь опускается
         sSector[n].Time=0;
        }
      }
     if (sSector[n].Time==0)
      {
       sSector[n].Down+=sSector[n].DZ;
       if (sSector[n].Down>=sSector[n].Up) sSector[n].DZ=0;
       if (sSector[n].Down<=sSector[n].Down1)
        {
         sSector[n].Down=sSector[n].Down1;
         if (sSector[n].Type==1)//обычная дверь поднимается
          {
           sSector[n].DZ=-sSector[n].DZ;
           sSector[n].Time=200;
          }
         else//автоматическая нет
          {
           sSector[n].DZ=0;
           sSector[n].Time=0;
          }
        }
      }
    else sSector[n].Time--;
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
 for(n=0;n<sInfo.SectorNumber;n++)
  {
   if (sSector[n].Type==4 || sSector[n].Type==5 || sSector[n].Type==8)//платформа и мостик
    {
     if (sSector[n].DZ>0 && sSector[n].Time==0 && n==k)//игрок зажат платформой
      {
       if (sSector[n].Up<=sPlayer.Z-sPlayer.SitZ)
        {
         sSector[n].DZ=-abs(sSector[n].DZ);//платформа опускается
         sSector[n].Time=0;
        }
      }
     if (sSector[n].Time==0)
      {
       sSector[n].Down+=sSector[n].DZ;
       if ((sSector[n].Down>=sSector[n].Down1 && sSector[n].Down1-sSector[n].Down2>0) ||
           (sSector[n].Down<=sSector[n].Down1 && sSector[n].Down1-sSector[n].Down2<0))
        {
         sSector[n].Down=sSector[n].Down1;
         sSector[n].DZ=-sSector[n].DZ;
         sSector[n].Time=200;
         if (sSector[n].Type==8)
          {
           sSector[n].Time=0;
           sSector[n].DZ=0;
          }
        }
       if ((sSector[n].Down<=sSector[n].Down2 && sSector[n].Down1-sSector[n].Down2>0) ||
           (sSector[n].Down>=sSector[n].Down2 && sSector[n].Down1-sSector[n].Down2<0))
        {
         sSector[n].Down=sSector[n].Down2;
         sSector[n].Time=0;
         sSector[n].DZ=0;
        }
      }
     else sSector[n].Time--;
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
   for(n=0;n<sInfo.SectorNumber;n++)
    {
     t=sSector[n].UpTexture;
     if (t>=0 && t<=2)
      {
       sSector[n].UpTexture=sSector[n].UpTexture+1;
       if (sSector[n].UpTexture==3) sSector[n].UpTexture=0;
      }
     if (t>=111 && t<=114)
      {
       sSector[n].UpTexture=sSector[n].UpTexture+1;
       if (sSector[n].UpTexture==115) sSector[n].UpTexture=111;
      }
     if (t>=119 && t<=122)
      {
       sSector[n].UpTexture=sSector[n].UpTexture+1;
       if (sSector[n].UpTexture==123) sSector[n].UpTexture=119;
      }
     if (t>=132 && t<=135)
      {
       sSector[n].UpTexture=sSector[n].UpTexture+1;
       if (sSector[n].UpTexture==136) sSector[n].UpTexture=132;
      }
     if (t>=145 && t<=148)
      {
       sSector[n].UpTexture=sSector[n].UpTexture+1;
       if (sSector[n].UpTexture==149) sSector[n].UpTexture=145;
      }
     if (t>=149 && t<=151)
      {
       sSector[n].UpTexture=sSector[n].UpTexture+1;
       if (sSector[n].UpTexture==152) sSector[n].UpTexture=149;
      }
     t=sSector[n].DownTexture;
     if (t>=0 && t<=2)
      {
       sSector[n].DownTexture=sSector[n].DownTexture+1;
       if (sSector[n].DownTexture==3) sSector[n].DownTexture=0;
      }
     if (t>=111 && t<=114)
      {
       sSector[n].DownTexture=sSector[n].DownTexture+1;
       if (sSector[n].DownTexture==115) sSector[n].DownTexture=111;
      }
     if (t>=119 && t<=122)
      {
       sSector[n].DownTexture=sSector[n].DownTexture+1;
       if (sSector[n].DownTexture==123) sSector[n].DownTexture=119;
      }
     if (t>=132 && t<=135)
      {
       sSector[n].DownTexture=sSector[n].DownTexture+1;
       if (sSector[n].DownTexture==136) sSector[n].DownTexture=132;
      }
     if (t>=145 && t<=148)
      {
       sSector[n].DownTexture=sSector[n].DownTexture+1;
       if (sSector[n].DownTexture==149) sSector[n].DownTexture=145;
      }
     if (t>=149 && t<=151)
      {
       sSector[n].DownTexture=sSector[n].DownTexture+1;
       if (sSector[n].DownTexture==152) sSector[n].DownTexture=149;
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
 for(n=0;n<sInfo.ThingNumber;n++)
  {
   int t=sInfo.TType[n];
   if (t>=0 && t<=1) CreateNewMedicine(sInfo.TX[n],sInfo.TY[n]);
   if (t>=2 && t<=3) CreateNewSuperMedicine(sInfo.TX[n],sInfo.TY[n]);
   if (t>=4 && t<=7) CreateNewBattery(sInfo.TX[n],sInfo.TY[n]);
   if (t==8) CreateNewGun(sInfo.TX[n],sInfo.TY[n]);
   if (t==9) CreateNewMachineGun(sInfo.TX[n],sInfo.TY[n]);
   if (t==10) CreateNewPlasmaGun(sInfo.TX[n],sInfo.TY[n]);
   if (t==11) CreateNewGunAmmo(sInfo.TX[n],sInfo.TY[n]);
   if (t==12) CreateNewMGunAmmo(sInfo.TX[n],sInfo.TY[n]);
   if (t==13) CreateNewGreyAltar(sInfo.TX[n],sInfo.TY[n]);
   if (t==14) CreateNewRedAltar(sInfo.TX[n],sInfo.TY[n]);
   if (t==15) CreateNewSuppository(sInfo.TX[n],sInfo.TY[n]);
   if (t==16) CreateNewPistolAmmo(sInfo.TX[n],sInfo.TY[n]);
   if (t>=17 && t<=19) CreateNewButt(sInfo.TX[n],sInfo.TY[n]);
   if (t>=20 && t<=22) CreateNewRedTorch(sInfo.TX[n],sInfo.TY[n]);
   if (t>=23 && t<=24) CreateNewLocMap(sInfo.TX[n],sInfo.TY[n]);
   if (t==25) CreateNewSpacesuit(sInfo.TX[n],sInfo.TY[n]);
   if (t==26) CreateNewMiniSuppository(sInfo.TX[n],sInfo.TY[n]);
   if (t==27) CreateNewMiniBlood(sInfo.TX[n],sInfo.TY[n]);
   if (t==28) CreateNewBlood(sInfo.TX[n],sInfo.TY[n]);
   if (t>=29 && t<=31) CreateNewMegaSphere(sInfo.TX[n],sInfo.TY[n]);
   if (t>=32 && t<=34) CreateNewSuperSphere(sInfo.TX[n],sInfo.TY[n]);
   if (t>=35 && t<=38) CreateNewHealingWater(sInfo.TX[n],sInfo.TY[n]);
   if (t>=39 && t<=41) CreateNewBlueTorch(sInfo.TX[n],sInfo.TY[n]);
  }
 for(n=0;n<sInfo.AlienNumber;n++)
  {
   int t=sInfo.AType[n];
   if (t==0) CreateNewDemon1(sInfo.AX[n],sInfo.AY[n]);
   if (t==1) CreateNewDemon2(sInfo.AX[n],sInfo.AY[n]);
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Анимация предметов и врагов                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void AnimateThing(void)
{
 for(int n=0;n<NowThing;n++)
  {
   cThing[n]->Faze++;
   if (cThing[n]->Footage[cThing[n]->Faze].Faze==-1) cThing[n]->Faze=0;
   int sector=FindSector(cThing[n]->Coord.X,cThing[n]->Coord.Y);
   int sdown=sSector[sector].Down;
   int sup=sSector[sector].Up;
   int zup=cThing[n]->Coord.Z;
   int zdown=cThing[n]->Coord.Z-cThing[n]->Coord.Height;
   if (zdown!=sdown) cThing[n]->Coord.Z=sdown+cThing[n]->Coord.Height;//предмет поднимает или опускает
  }
 for(int n=0;n<NowAlien;n++)
  {
   if (cAlien[n]->Destroy==-1)
    {
     if (cAlien[n]->Type==2)
      {
       DeleteAlien(n);
       n--;
       continue;
      }
     if (cAlien[n]->Dz!=0)
      {
       cAlien[n]->Coord.Z+=cAlien[n]->Dz;
       int z=sSector[FindSector(cAlien[n]->Coord.X,cAlien[n]->Coord.Y)].Down;
       if (cAlien[n]->Coord.Z-cAlien[n]->Coord.Height<z)
        {
         cAlien[n]->Coord.Z=z+cAlien[n]->Coord.Height;
         cAlien[n]->Dz=0;
        }
      }
    }
   int sector=FindSector(cAlien[n]->Coord.X,cAlien[n]->Coord.Y);
   int sdown=sSector[sector].Down;
   int sup=sSector[sector].Up;
   int zup=cAlien[n]->Coord.Z;
   int zdown=cAlien[n]->Coord.Z-cAlien[n]->Coord.Height;
   if (sup-sdown<zup-zdown && cAlien[n]->Life!=0)//врага убило
    {
     cAlien[n]->Life=0;
     cAlien[n]->Faze=cAlien[n]->Destroy;
     cAlien[n]->Coord.Z=sdown+cAlien[n]->Coord.Height;
    }
   if (zdown<sdown) cAlien[n]->Coord.Z=sdown+cAlien[n]->Coord.Height;//врага поднимает или опускает
   if (cAlien[n]->Life==0 && zdown>sdown && cAlien[n]->Destroy==0) cAlien[n]->Coord.Z=sdown+cAlien[n]->Coord.Height;
   cAlien[n]->Faze++;
   if (cAlien[n]->Faze>=cAlien[n]->Destroy && cAlien[n]->Footage[cAlien[n]->Faze].Faze==-1)
    {
     cAlien[n]->Faze--;
     cAlien[n]->Destroy=-1;
     cAlien[n]->Dz=-10;//враг падает
    }
   if (cAlien[n]->Footage[cAlien[n]->Faze].Faze==-1) cAlien[n]->Faze=0;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Игрок в специальном секторе                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void SpecialSector(int sector)
{
 if (sPlayer.TimeSpacesuit!=0 || sPlayer.IDDQD!=0 || sPlayer.TimeMegaSphere!=0) return;//у игрока есть скафандр или игрок бессмертен
 int pl=sPlayer.Life;
 if (TimerUser%100==0)
  {
   int floor=sSector[sector].DownTexture;
   if (floor>=0 && floor<=2) sPlayer.Life-=4;//кислота
   if (floor>=132 && floor<=135) sPlayer.Life-=2;//токсичные отходы
   if (floor>=145 && floor<=148) sPlayer.Life-=10;//лава
   if (floor>=149 && floor<=151) sPlayer.Life-=1;//кровь
   if (sPlayer.Life<0) sPlayer.Life=0;
   if (sPlayer.Life!=pl) PutStatistik();
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
   if (cAlien[n]->Life==0) continue;
   float xold=cAlien[n]->Coord.X;
   float yold=cAlien[n]->Coord.Y;
   float zoldup=cAlien[n]->Coord.Z;
   float zolddown=cAlien[n]->Coord.Z-cAlien[n]->Coord.Height;
   int sectorold=FindSector(xold,yold);
   int upold=sSector[sectorold].Up;
   int downold=sSector[sectorold].Down;
   float px=sPlayer.X;
   float py=sPlayer.Y;
   float pz=sPlayer.Z;
   int sectorplayer;
   if (cAlien[n]->Active==1) sectorplayer=sectorold;
                       else sectorplayer=FindSector(px,py);
   float xo,yo;
   int object,len;
   len=sqrt((px-xold)*(px-xold)+(py-yold)*(py-yold));
   if (len<400 || sectorold==sectorplayer) cAlien[n]->Active=1;//активизируем врага
   if (cAlien[n]->Active==0) continue;
   if (rnd(100)>90 && cAlien[n]->Time<100)
    {
     cAlien[n]->Time=500;//время преследования
     cAlien[n]->Dy=3*(py-yold)/len;
     cAlien[n]->Dx=3*(px-xold)/len;
     cAlien[n]->Dz=3*(sPlayer.Z-zoldup)/len;
    }
   if (rnd(100)>90 && len<1000 && cAlien[n]->Faze<cAlien[n]->Fire)//враг стреляет
    {
     float dy=(py-yold)/len+sin(3.14*(rnd(4)-2)/180);
     float dx=(px-xold)/len+sin(3.14*(rnd(4)-2)/180);
     float dz=sin(3.14*(rnd(4)-2)/180);
     float zm=((zoldup-cAlien[n]->Coord.Height/1.5)-(sPlayer.Z-sPlayer.SitZ))/len;
     if (cAlien[n]->Type==1) CreateNewRocket(xold,yold,zoldup-cAlien[n]->Coord.Height/1.5,dx,dy,dz-zm,15,5,5,n);
     if (cAlien[n]->Type==2) CreateNewBlueBall(xold,yold,zoldup-cAlien[n]->Coord.Height/1.5,dx,dy,dz-zm,9,3,6,n);
     cAlien[n]->Faze=cAlien[n]->Fire;
    }
   cAlien[n]->Coord.X+=cAlien[n]->Dx;
   cAlien[n]->Coord.Y+=cAlien[n]->Dy;
   cAlien[n]->Coord.Z+=cAlien[n]->Dz;
   float znewup=cAlien[n]->Coord.Z;//проверим столкновения со стенами,полом и потолком
   float znewdown=cAlien[n]->Coord.Z-cAlien[n]->Coord.Height;
   int sectornew=FindSector(cAlien[n]->Coord.X,cAlien[n]->Coord.Y);
   int upnew=sSector[sectornew].Up;
   int downnew=sSector[sectornew].Down;
   if (upnew<znewup)
    {
     cAlien[n]->Dz=-cAlien[n]->Dz;
     cAlien[n]->Coord.Z=upnew;
    }
   if (downnew>znewdown)
    {
     cAlien[n]->Dz=-cAlien[n]->Dz;
     cAlien[n]->Coord.Z=downnew+cAlien[n]->Coord.Height;
    }
   if (rnd(1000)>900 || cAlien[n]->Time==0 || Impact(cAlien[n]->Coord.X,cAlien[n]->Coord.Y,cAlien[n]->Coord.Z,cAlien[n]->Coord.Radius,cAlien[n]->Coord.Height,n))
    {
     cAlien[n]->Coord.Y=yold;
     cAlien[n]->Coord.X=xold;
     cAlien[n]->Coord.Z=zoldup;
     int angle=rnd(360);
     cAlien[n]->Dx=CosTable[angle]*3;
     cAlien[n]->Dy=SinTable[angle]*3;
     cAlien[n]->Dz=(rnd(200)-100)/30;
     cAlien[n]->Time=200;
    }
   cAlien[n]->Time--;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Цветовые эффекты                                                  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void ColorEffect(void)
{
 if (sPlayer.TimeSpacesuit==1000 || (sPlayer.TimeSpacesuit!=0 && sPlayer.TimeSpacesuit<=100 && sPlayer.TimeSpacesuit%5==0 && sPlayer.TimeSpacesuit%2==0)) cVideo.SetRGB_DAC(0.7,1,0.7);
 if ((sPlayer.TimeSpacesuit!=0 && sPlayer.TimeSpacesuit<=100 && sPlayer.TimeSpacesuit%5==0 && sPlayer.TimeSpacesuit%2!=0)) cVideo.SetRGB_DAC(1,1,1);
 if (sPlayer.TimeSpacesuit>0)
  {
   sPlayer.TimeSpacesuit--;
   if (sPlayer.TimeSpacesuit==0)
   if (sPlayer.TimeMegaSphere==0) cVideo.SetRGB_DAC(1,1,1);
                            else cVideo.SetRGB_DAC(0.7,0.7,1);
  }
 if (sPlayer.TimeMegaSphere==1000 || (sPlayer.TimeMegaSphere!=0 && sPlayer.TimeMegaSphere<=100 && sPlayer.TimeMegaSphere%5==0 && sPlayer.TimeMegaSphere%2==0)) cVideo.SetRGB_DAC(0.7,0.7,1);
 if ((sPlayer.TimeMegaSphere!=0 && sPlayer.TimeMegaSphere<=100 && sPlayer.TimeMegaSphere%5==0 && sPlayer.TimeMegaSphere%2!=0)) cVideo.SetRGB_DAC(1,1,1);
 if (sPlayer.TimeMegaSphere>0)
  {
   sPlayer.TimeMegaSphere--;
   if (sPlayer.TimeMegaSphere==0)
    {
     if (sPlayer.TimeSpacesuit==0) cVideo.SetRGB_DAC(1,1,1);
                             else cVideo.SetRGB_DAC(0.7,1,0.7);
    }
  }
 if (sPlayer.RedScreenOn==1)
  {
   sPlayer.RedScreen--;
   if (sPlayer.RedScreen==0)
    {
     if (sPlayer.TimeSpacesuit==0 && sPlayer.TimeMegaSphere==0) cVideo.SetRGB_DAC(1,1,1);
      else
       {
        if (sPlayer.TimeMegaSphere!=0) cVideo.SetRGB_DAC(0.7,0.7,1);
        if (sPlayer.TimeSpacesuit!=0) cVideo.SetRGB_DAC(0.7,1,0.7);
       }
     sPlayer.RedScreenOn=0;
    }
  }
 else
  {
   if (sPlayer.RedScreen!=0)
    {
     cVideo.SetRGB_DAC(1,0.7,0.7);
     sPlayer.RedScreenOn=1;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Анимация лица игрока                                              //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void AnimateFace(int t)
{
 if (sPlayer.Life<=0)
  {
   PutBar(sData.Bar,1,142,168);
   cVideo.ViewBar();
   return;
  }
 if (sPlayer.IDDQD!=0 || sPlayer.TimeMegaSphere!=0)
  {
   PutBar(sData.Bar,2,142,168);
   cVideo.ViewBar();
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
   if (sPlayer.Life>80) variant=0;
   if (sPlayer.Life>60 && sPlayer.Life<=80) variant=1;
   if (sPlayer.Life>40 && sPlayer.Life<=60) variant=2;
   if (sPlayer.Life>20 && sPlayer.Life<=40) variant=3;
   if (sPlayer.Life>0 && sPlayer.Life<=20) variant=4;
   PutBar(sData.Bar,face+variant*4+2,142,168);
   cVideo.ViewBar();
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Вывод сообщений                                                   //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void PutMessage(void)
{
 if (sPlayer.MessageNo==1) cVideo.Print(1,1,"PICKED UP A HEALTH BONUS.");
 if (sPlayer.MessageNo==2) cVideo.Print(1,1,"PICKED UP A CLIP.");
 if (sPlayer.MessageNo==3) cVideo.Print(1,1,"PICKED UP A 15 SHOTGUN SHELLS.");
 if (sPlayer.MessageNo==4) cVideo.Print(1,1,"PICKED UP A MEDIKIT.");
 if (sPlayer.MessageNo==5) cVideo.Print(1,1,"PICKED UP A SUPER MEDIKIT !");
 if (sPlayer.MessageNo==6) cVideo.Print(1,1,"INVULNERABILITY !");
 if (sPlayer.MessageNo==7) cVideo.Print(1,1,"SUPERCHARGE !");
 if (sPlayer.MessageNo==8) cVideo.Print(1,1,"PICKED UP A BOX OF BULLETS.");
 if (sPlayer.MessageNo==9) cVideo.Print(1,1,"YOU GOT THE SHOTGUN.");
 if (sPlayer.MessageNo==10) cVideo.Print(1,1,"YOU GOT THE CHAINGUN.");
 if (sPlayer.MessageNo==11) cVideo.Print(1,1,"YOU GOT THE PLASMAGUN.");
 if (sPlayer.MessageNo==12) cVideo.Print(1,1,"PICKED UP AN ENERGY CELL PACK.");
 if (sPlayer.MessageNo==13) cVideo.Print(1,1,"RADIATION SHEILDING SUIT.");
 if (sPlayer.MessageNo==14) cVideo.Print(1,1,"PICKED UP A COMPUTER MAP.");
 if (sPlayer.MessageNo==15) cVideo.Print(1,1,"GAME SAVED.");
 if (sPlayer.MessageNo==16)
  {
   char s[255];
   itoa(sPlayer.FPS,s,10);
   cVideo.Print(1,1,s);
  }
 if (sPlayer.MessageTime!=0)
  {
   sPlayer.MessageTime--;
   if (sPlayer.MessageTime==0) sPlayer.MessageNo=0;
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
 sPlayer.P_X=sPlayer.X*65536;
 sPlayer.P_Y=sPlayer.Y*65536;
 sector=FindSector(sPlayer.X,sPlayer.Y);
 if (sPlayer.Life!=-1)
  {
   Fallen(sector);
   SpecialSector(sector);
  }
 cVideo.ClearWindow();
 AnimateAll(sector);
 if (sPlayer.AutoMap==1) PutAutoMap();
  else
   {
    View();
    PutWeapon();
   }
 if (sPlayer.Life==-1) cVideo.Print(115,79,"GAME OVER");
 PutMessage();
 cVideo.ViewWindow();
 return(sector);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Функция управления от клавиатуры  и мыши                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int CKeyboardControl(int sector)
{
 int mButton,mX,mY;
 m_getpos(&mButton,&mX,&mY);
 int dx=mX-160;
 int dy=mY-100;
 if (dx!=0 || dy!=0) m_setpos(160,100);
 /*
 if (dx!=0)
  {
   sPlayer.Angle+=dx;
   if (sPlayer.Angle<0) sPlayer.Angle+=360;
   if (sPlayer.Angle>=360) sPlayer.Angle-=360;
  }
  */
 if (cKeyboard.GetPressKey(USE) || mButton==2)
  {
   int done=Use(sector);//выход найден
   if (done==1)
    {
     cVideo.ClearWindow();
     View();
     cVideo.ViewWindow();
     cVideo.Clear();
     RunningScreen();
     return(done);
    }
  }
 if (cKeyboard.GetPressKey(SIT))
  {
   sPlayer.SitZ+=5;
   if (sPlayer.SitZ>30) sPlayer.SitZ=30;
  }
 else
  {
   sPlayer.SitZ-=5;
   if (sPlayer.SitZ<0) sPlayer.SitZ=0;
   if (-sPlayer.SitZ+sPlayer.Z+20>sSector[sector].Up) sPlayer.SitZ=-sSector[sector].Up+sPlayer.Z+20;
  }
 if (cKeyboard.GetPressKey(MAP))
  {
   if (sPlayer.AutoMap==0) sPlayer.AutoMap=1;
                      else sPlayer.AutoMap=0;
   KeyTable[MAP]=0;
  }
 if (cKeyboard.GetPressKey(WEAPON0)) ChangeWeapon(0);
 if (cKeyboard.GetPressKey(WEAPON1)) ChangeWeapon(1);
 if (cKeyboard.GetPressKey(WEAPON2)) ChangeWeapon(2);
 if (cKeyboard.GetPressKey(WEAPON3)) ChangeWeapon(3);
 if (cKeyboard.GetPressKey(WEAPON4)) ChangeWeapon(4);
 if ((cKeyboard.GetPressKey(FIRE) || mButton==1) && sPlayer.AutoMap==0) Fire();
 if (cKeyboard.GetPressKey(UP)) MoveUp(0);
  else
   {
    if (sPlayer.Inerci>0)
     {
      MoveUp(1);
      sPlayer.Inerci-=2;
     }
   }
 if (cKeyboard.GetPressKey(DOWN)) MoveDown(0);
  else
   {
    if (sPlayer.Inerci<0)
     {
      MoveDown(1);
      sPlayer.Inerci+=2;
     }
   }
 if (cKeyboard.GetPressKey(REFRESH))
  {
   int t0=clock();
   sPlayer.D_Angle=2;
   sPlayer.Inerci=0;
   sPlayer.Dz=0;
   for(int angle=0;angle<180;angle++)
    {
     RotateRight();
     cVideo.ClearWindow();
     View();
     cVideo.ViewWindow();
    }
   int t1=clock();
   sPlayer.FPS=18000.0/(t1-t0);
   sPlayer.MessageNo=16;
   sPlayer.MessageTime=1000;
  }
 if (cKeyboard.GetPressKey(SLEFT)) MoveLeft();
 if (cKeyboard.GetPressKey(SRIGHT)) MoveRight();
 if (cKeyboard.GetPressKey(LEFT))
  {
   if (sPlayer.D_Angle>0) sPlayer.D_Angle=0;
   sPlayer.D_Angle--;
   if (sPlayer.D_Angle<-5) sPlayer.D_Angle=-5;
   RotateLeft();
  }
 if (cKeyboard.GetPressKey(RIGHT))
  {
   if (sPlayer.D_Angle<0) sPlayer.D_Angle=0;
   sPlayer.D_Angle++;
   if (sPlayer.D_Angle>5) sPlayer.D_Angle=5;
   RotateRight();
  }
 if (!(cKeyboard.GetPressKey(RIGHT) || cKeyboard.GetPressKey(LEFT))) sPlayer.D_Angle=0;
 if (!(cKeyboard.GetPressKey(UP) || cKeyboard.GetPressKey(DOWN) || dy!=0 || cKeyboard.GetPressKey(SRIGHT) || cKeyboard.GetPressKey(SLEFT)))
  {
   sPlayer.Pz=0;
   sPlayer.Dz=0;
  }
 else
  {
   if (sPlayer.Dz==0) sPlayer.Dz=1;
  }
 if (sPlayer.Pz>3) sPlayer.Dz=-sPlayer.Dz;
 if (sPlayer.Pz<-3) sPlayer.Dz=-sPlayer.Dz;
 return(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Главная функция управления.                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int Control(void)
{
 int sector;
 KeyTable[ESC]=0;
 sPlayer.Z=sSector[FindSector(sPlayer.X,sPlayer.Y)].Down+64;
 PutStatistik();
 m_setpos(160,100);
 while (1)
  {
   if (Timer/5==TimerUser/5) continue;
   Timer=TimerUser;
   sector=ViewAll();
   if (cKeyboard.GetPressKey(ESC)) return(0);
   if (sPlayer.Life==-1) continue;
   if (sPlayer.Life==0)//вы проиграли
    {
     sPlayer.SitZ=0;
     for(int n=0;n<48;n++)
      {
       while (Timer==TimerUser)
        {
         int r=0;//задержка
        }
       Timer=TimerUser;
       sPlayer.SitZ++;
       ViewAll();
      }
     sPlayer.Life=-1;//игрок в отключке
    }
   if (CKeyboardControl(sector)) return(1);
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Отвечает за обработку нажатия кнопки "использовать".              //
//W-номер сектора,в котором находиться игрок.                       //
//Возвращает 0-если это не конец пути.Иначе-1                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int Use(int w)
{
 if (sSector[w].Type==2)//переключатель
  {
   if (sSector[sSector[w].Sector].DZ==0 && sSector[sSector[w].Sector].Sector==0)
    {
     for(register int l=0;l<sInfo.SegmentNumber;l++)
      {
       if (sSegment[l].Type>=3 && sSegment[l].Type<=21 && (sSegment[l].Sector1==w || sSegment[l].Sector2==w))
        {
         if (DistanceOfVector(sSegment[l].X1,sSegment[l].Y1,sSegment[l].X2,sSegment[l].Y2))
          {
           sSegment[l].Type=sSegment[l].Type+1;
           sSector[sSector[w].Sector].Sector=1;
           sSector[sSector[w].Sector].Time=0;
           if (sSector[sSector[w].Sector].Type==3) sSector[sSector[w].Sector].DZ=-2;
           if (sSector[sSector[w].Sector].Type==5 || sSector[sSector[w].Sector].Type==8)
            sSector[sSector[w].Sector].DZ=(sSector[sSector[w].Sector].Down1-sSector[sSector[w].Sector].Down2)/abs((sSector[sSector[w].Sector].Down1-sSector[sSector[w].Sector].Down2))*2;
          }
        }
      }
    }
  }
 if (sSector[w].Type==7)//телепортация
  {
   sPlayer.X=sSector[w].Sector;
   sPlayer.Y=sSector[w].Down1;
   sPlayer.Z=sSector[FindSector(sPlayer.X,sPlayer.Y)].Down+64;
   sPlayer.P_X=sPlayer.X*65536;
   sPlayer.P_Y=sPlayer.Y*65536;
   LightingMinus();
   cVideo.ClearWindow();
   if (sPlayer.AutoMap==1) PutAutoMap();
    else
     {
      View();
      PutWeapon();
     }
   cVideo.ViewWindow();
   LightingPlus();
   if (sPlayer.TimeSpacesuit!=0) cVideo.SetRGB_DAC(0.7,1,0.7);
   if (sPlayer.TimeMegaSphere!=0) cVideo.SetRGB_DAC(0.7,0.7,1);
  }
 if (sSector[w].Type==6)//конец пути
  {
   for(register int l=0;l<sInfo.SegmentNumber;l++)//меняем текстуру переключателя
    {
     if (sSegment[l].Type>=3 && sSegment[l].Type<=21 && (sSegment[l].Sector1==w || sSegment[l].Sector2==w))
      {
       if (DistanceOfVector(sSegment[l].X1,sSegment[l].Y1,sSegment[l].X2,sSegment[l].Y2))
        {
         sSegment[l].Type=sSegment[l].Type+1;
         return(1);
        }
      }
    }
  }
 for(register int n=0;n<sInfo.SectorNumber;n++)
 {
  if (sSector[n].Type==1 || sSector[n].Type==4 || sSector[n].Type==5)//двери и платформы
   {
    for(int t=0;t<sSector[n].Segment-1;t++)
     {
      float x1=sSector[n].X[t];
      float y1=sSector[n].Y[t];
      float x2=sSector[n].X[t+1];
      float y2=sSector[n].Y[t+1];
      int l1=DistanceOfVector(x1,y1,x1,y2);
      int l2=DistanceOfVector(x1,y2,x2,y2);
      int l3=DistanceOfVector(x2,y2,x2,y1);
      int l4=DistanceOfVector(x2,y1,x1,y1);
      if (l1!=0 || l2!=0 || l3!=0 || l4!=0 || n==w)
       {
        if (sSector[n].Type==1 && sSector[n].DZ==0)
         {
          sSector[n].DZ=-5;
         }
        if ((sSector[n].Type==4 || (sSector[n].Sector==1 && sSector[n].Type==5)) && sSector[n].DZ==0)
         {
          sSector[n].DZ=5*(sSector[n].Down1-sSector[n].Down2)/abs((sSector[n].Down1-sSector[n].Down2));
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
 sPlayer.Angle+=sPlayer.D_Angle;
 if (cKeyboard.GetPressKey(SPEED)) sPlayer.Angle+=sPlayer.D_Angle;
 if (sPlayer.Angle<0) sPlayer.Angle+=360;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Поворот игрока направо.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void RotateRight(void)
{
 sPlayer.Angle+=sPlayer.D_Angle;
 if (cKeyboard.GetPressKey(SPEED)) sPlayer.Angle+=sPlayer.D_Angle;
 if (sPlayer.Angle>360) sPlayer.Angle-=360;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Движение игрока вперёд.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveUp(int type)
{
 float x=sPlayer.X;
 float y=sPlayer.Y;
 float ss=SinTable[sPlayer.Angle];
 float cs=CosTable[sPlayer.Angle];
 sPlayer.X+=ss*sPlayer.Inerci;
  sPlayer.Y+=cs*sPlayer.Inerci;
  if (cKeyboard.GetPressKey(SPEED))
   {
    sPlayer.X+=abs(sPlayer.Inerci)*ss;
    sPlayer.Y+=abs(sPlayer.Inerci)*cs;
   }
  int k=Impact(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ,20,64-sPlayer.SitZ,-1);
  if (k==0) ImpactsPlayerToThing(sPlayer.X,sPlayer.Y,sPlayer.Z,20,64);
  if (k<0)
   {
    sPlayer.X=x;
    sPlayer.Y=y;
   }
  if (k>0)//есть столкновение
   {
    SlideTraverse(x,y,sPlayer.X,sPlayer.Y);
    sPlayer.Inerci=0;
   }
 sPlayer.Pz+=sPlayer.Dz;
 if (type==0)
  {
   sPlayer.Inerci+=2;
   if (sPlayer.Inerci>10) sPlayer.Inerci=10;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Движение игрока назад.                                            //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveDown(int type)
{
 float x=sPlayer.X;
 float y=sPlayer.Y;
 float ss=SinTable[sPlayer.Angle];
 float cs=CosTable[sPlayer.Angle];
 sPlayer.X+=sPlayer.Inerci*ss;
 sPlayer.Y+=sPlayer.Inerci*cs;
 if (cKeyboard.GetPressKey(SPEED))
  {
   sPlayer.X+=sPlayer.Inerci*ss;
   sPlayer.Y+=sPlayer.Inerci*cs;
  }
 int k=Impact(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ,20,64-sPlayer.SitZ,-1);
 if (k==0) ImpactsPlayerToThing(sPlayer.X,sPlayer.Y,sPlayer.Z,20,64);
 if (k<0)
  {
   sPlayer.X=x;
   sPlayer.Y=y;
  }
 if (k>0)//есть столкновение
  {
   SlideTraverse(x,y,sPlayer.X,sPlayer.Y);
   sPlayer.Inerci=0;
  }
 sPlayer.Pz+=sPlayer.Dz;
 if (type==0)
  {
   sPlayer.Inerci-=2;
   if (sPlayer.Inerci<-10) sPlayer.Inerci=-10;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Движение игрока влево.                                            //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveLeft(void)
{
 float x=sPlayer.X;
 float y=sPlayer.Y;
 float ss=SinTable[sPlayer.Angle];
 float cs=CosTable[sPlayer.Angle];
 sPlayer.X-=10*cs;
 sPlayer.Y+=10*ss;
 if (cKeyboard.GetPressKey(SPEED))
  {
   sPlayer.X-=10*cs;
   sPlayer.Y+=10*ss;
  }
 int k=Impact(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ,20,64-sPlayer.SitZ,-1);
 if (k==0) ImpactsPlayerToThing(sPlayer.X,sPlayer.Y,sPlayer.Z,20,64);
 if (k!=0)
  {
   sPlayer.X=x;
   sPlayer.Y=y;
  }
 sPlayer.Pz+=sPlayer.Dz;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Движение игрока вправо.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void MoveRight(void)
{
 float x=sPlayer.X;
 float y=sPlayer.Y;
 float ss=SinTable[sPlayer.Angle];
 float cs=CosTable[sPlayer.Angle];
 sPlayer.X+=10*cs;
 sPlayer.Y-=10*ss;
 if (cKeyboard.GetPressKey(SPEED))
  {
   sPlayer.X+=10*cs;
   sPlayer.Y-=10*ss;
  }
 int k=Impact(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ,20,64-sPlayer.SitZ,-1);
 if (k==0) ImpactsPlayerToThing(sPlayer.X,sPlayer.Y,sPlayer.Z,20,64);
 if (k!=0)
  {
   sPlayer.X=x;
   sPlayer.Y=y;
  }
 sPlayer.Pz+=sPlayer.Dz;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Падение игрока.                                                   //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Fallen(int sector)
{
 int down=sSector[sector].Down;
 int bottom=sPlayer.Z-64;
 int dz=sPlayer.PV;
 if (down<bottom)
  {
   if (bottom-down<dz) dz=bottom-down;
   if (Impact(sPlayer.X,sPlayer.Y,sPlayer.Z-dz,20,64,-1)==0) sPlayer.Z-=dz;
  }
 else sPlayer.Z=down+64;
}
void PutWeapon(void)
{
 int i=sWeapon.WeaponNo;
 int j=sWeapon.Fire_Faze;
 int dw=sPlayer.Pz/2;
 if (i==0) PutStaticSprite(sWeapon.X[i][j],sWeapon.Y[i][j]-dw+sWeapon.Vy,sData.Weapon0,sWeapon.Faze[i][j]);
 if (i==1) PutStaticSprite(sWeapon.X[i][j],sWeapon.Y[i][j]-dw+sWeapon.Vy,sData.Weapon1,sWeapon.Faze[i][j]);
 if (i==2) PutStaticSprite(sWeapon.X[i][j],sWeapon.Y[i][j]-dw+sWeapon.Vy,sData.Weapon2,sWeapon.Faze[i][j]);
 if (i==3) PutStaticSprite(sWeapon.X[i][j],sWeapon.Y[i][j]-dw+sWeapon.Vy,sData.Weapon3,sWeapon.Faze[i][j]);
 if (i==4) PutStaticSprite(sWeapon.X[i][j],sWeapon.Y[i][j]-dw+sWeapon.Vy,sData.Weapon4,sWeapon.Faze[i][j]);
}
void AnimateWeapon(void)
{
 int i=sWeapon.WeaponNo;
 int j=sWeapon.Fire_Faze;
 if (j==0)
  {
   sWeapon.Vy+=sWeapon.Dy;
   if (sWeapon.Vy>70)
    {
     sWeapon.Dy=-8;
     sWeapon.WeaponNo=sWeapon.Weapon;//меняем оружие
     PutStatistik();
    }
   if (sWeapon.Vy==0 && sWeapon.Dy!=0) sWeapon.Dy=0;
  }
 if (j==0) return;//нечего анимировать
 sWeapon.Fire_Faze=j+1;
 if (sWeapon.Faze[i][j+1]==-1)
  {
   sWeapon.Fire_Faze=0;//закончили анимацию
   if (i!=0) sWeapon.Ammo[i]--;
   PutStatistik();
  }
}
void Fire(void)
{
 int i=sWeapon.WeaponNo;
 int j=sWeapon.Fire_Faze;
 if (j!=0) return;//ещё не закончился предыдущий выстрел
 if (sWeapon.Dy!=0 || sWeapon.Vy!=0) return;//оружие ещё не поднялось
 if (sWeapon.Ammo[i]==0)//патронов нет
  {
   //ищем другое оружие
   for(int n=4;n>=0;n--)
    {
     if (sWeapon.Ammo[n]>0 && sWeapon.Attendance[n]!=0)
      {
       sWeapon.Dy=8;
       sWeapon.Weapon=n;
       return;
      }
    }
  }
 if (i==0)//если это кулаки
  {
   sWeapon.Fire_Faze=1;//начали анимацию
   int minNo=-1;
   float mindist=10000000;
   float cs=CosTable[sPlayer.Angle];
   float ss=SinTable[sPlayer.Angle];
   for(int n=0;n<NowAlien;n++)
    {
     int z1=cAlien[n]->Coord.Z;
     int z2=cAlien[n]->Coord.Z-cAlien[n]->Coord.Height;
     if (z2>sPlayer.Z || z1<sPlayer.Z) continue;//не совпадают высоты
     float x1=cAlien[n]->Coord.X-sPlayer.X;
     float y1=cAlien[n]->Coord.Y-sPlayer.Y;
     float v=x1*ss+y1*cs;
     if (v<=1) continue;
     if (v>70) continue;
     register int u=x1*cs-y1*ss;
     int col1=160+160*(u-cAlien[n]->Coord.Radius/2)/v;
     int col2=160+160*(u+cAlien[n]->Coord.Radius/2)/v;
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
     cAlien[minNo]->Life-=25;
     if (cAlien[minNo]->Life<=0)
      {
       cAlien[minNo]->Life=0;
       cAlien[minNo]->Faze=cAlien[minNo]->Destroy;
      }
    }
   return;
  }
 //иначе посылаем снаряд
 if (i==1 || i==3 || i==4)//пистолет,пулемёт и плазмомёт создают один снаряд
  {
   int angle=sPlayer.Angle+rnd(4)-2;
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
   if (i==1) CreateNewGunFire(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ+10,ss,cs,dz+za,1000,10,i,-1);
   if (i==3) CreateNewGunFire(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ+10,ss,cs,dz+za,1000,15,i,-1);
   if (i==4) CreateNewPlasmaGunFire(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ+10,ss,cs,dz+za,40,50,i,-1);
  }
 if (i==2)//ружьё создаёт восемь снарядов
  {
   for(int n=0;n<8;n++)
    {
     int angle=sPlayer.Angle+rnd(10)-5;
     if (angle>=360) angle-=360;
     if (angle<0) angle+=360;
     float ss=SinTable[angle];
     float cs=CosTable[angle];
     angle=rnd(10)-5;
     if (angle>=360) angle-=360;
     if (angle<0) angle+=360;
     float dz=SinTable[angle];
     float za=FindAlien();
     CreateNewGunFire(sPlayer.X,sPlayer.Y,sPlayer.Z-sPlayer.SitZ+5,ss,cs,dz+za,1000,10,i,-1);
    }
  }
 sWeapon.Fire_Faze=1;//начали анимацию оружия
}
void ChangeWeapon(int number)
{
 int i=sWeapon.WeaponNo;
 if (sWeapon.Attendance[number]==0) return;//у нас нет такого оружия
 if (i==number) return;
 sWeapon.Dy=8;
 sWeapon.Weapon=number;
}
void AnimateMissile(void)
{
 int n;
 for(n=0;n<NowMissile;n++)
  {
   cMissile[n]->Faze++;
   if (cMissile[n]->Footage[cMissile[n]->Faze]==-1)
    {
     cMissile[n]->Faze=0;
     if (cMissile[n]->Speed==0)
      {
       DeleteMissile(n);
       continue;
      }
    }
   if (cMissile[n]->Speed==0) continue;
   float speed=cMissile[n]->Speed;
   float dx=cMissile[n]->Dx*speed;
   float dy=cMissile[n]->Dy*speed;
   float dz=cMissile[n]->Dz*sqrt(dx*dx+dy*dy);
   float x=cMissile[n]->X+dx;
   float y=cMissile[n]->Y+dy;
   float z=cMissile[n]->Z+dz;
   if (dx==0 && dy==0 && dz==0)
    {
     cMissile[n]->Speed=0;//делаем анимацию взрыва снаряда
     cMissile[n]->Faze=cMissile[n]->Destroy;
     continue;
    }
   float xo,yo;
   int object;
   int sw=CheckWay(cMissile[n]->X,cMissile[n]->Y,cMissile[n]->Z-cMissile[n]->Radius,x,y,z-cMissile[n]->Radius,cMissile[n]->Radius,2*cMissile[n]->Radius,cMissile[n]->Object,&xo,&yo,&object);
   if (sw!=0)
    {
     float dt=sqrt((cMissile[n]->X-xo)*(cMissile[n]->X-xo)+(cMissile[n]->Y-yo)*(cMissile[n]->Y-yo));
     cMissile[n]->Z=cMissile[n]->Z+cMissile[n]->Dz*(dt-cMissile[n]->Radius*2);
     cMissile[n]->X=xo-cMissile[n]->Dx*cMissile[n]->Radius*2;
     cMissile[n]->Y=yo-cMissile[n]->Dy*cMissile[n]->Radius*2;
     if (object>0)//снаряд попал во врага
      {
       int objectno=object-1;
       cAlien[objectno]->Active=1;//активизируем врага
       cAlien[objectno]->Life-=cMissile[n]->Power;
       if (cAlien[objectno]->Life<=0)
        {
         cAlien[objectno]->Life=0;
         cAlien[objectno]->Faze=cAlien[objectno]->Destroy;
        }
      }
     if (object==-1 && sPlayer.Life!=-1)//снаряд попал в игрока
      {
       if (sPlayer.IDDQD==0 && sPlayer.TimeMegaSphere==0) sPlayer.Life-=cMissile[n]->Power;
       if (sPlayer.Life<0) sPlayer.Life=0;
       PutStatistik();
       sPlayer.RedScreen=3;//число циклов красного экрана
       sPlayer.RedScreenOn=0;
       AnimateFace(2);
      }
     cMissile[n]->Speed=0;//делаем анимацию взрыва снаряда
     cMissile[n]->Faze=cMissile[n]->Destroy;
    }
   else
    {
     cMissile[n]->X=x;
     cMissile[n]->Y=y;
     cMissile[n]->Z=z;
    }
  }
}
void PutStatistik()
{
 int i=sWeapon.WeaponNo;
 char s[255];
 PutBar(sData.Bar,0,0,168);
 itoa(sWeapon.Ammo[i],s,10);
 cVideo.Print(13,180,s);
 itoa(sPlayer.Life,s,10);
 cVideo.Print(60,180,s);
 AnimateFace(1);
}
float FindAlien(void)
{
 int minNo=-1;
 float mindist=10000000;
 float cs=CosTable[sPlayer.Angle];
 float ss=SinTable[sPlayer.Angle];
 for(int n=0;n<NowAlien;n++)
  {
   if (cAlien[n]->Life==0) continue;
   if (cAlien[n]->Visible==0) continue;
   float x1=cAlien[n]->Coord.X-sPlayer.X;
   float y1=cAlien[n]->Coord.Y-sPlayer.Y;
   float v=x1*ss+y1*cs;
   if (v<=1) continue;
   if (v>1000) continue;
   register int u=x1*cs-y1*ss;
   int col1=160+160*(u-cAlien[n]->Coord.Radius/2)/v;
   int col2=160+160*(u+cAlien[n]->Coord.Radius/2)/v;
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
   float dz=((cAlien[minNo]->Coord.Z-cAlien[minNo]->Coord.Height/2)-(sPlayer.Z-sPlayer.SitZ+5))/mindist;
   return(dz);
  }
 return(0);
}
