//===========================спецэфекты=======================================
void RunningScreen(void);
void LightingPlus(void);
void LightingMinus(void);
int rnd(int max);
//============================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Стекание экрана.                                                  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void RunningScreen(void)
{
 unsigned int oldtimer=TimerUser;
 unsigned char *vpt=(unsigned char*)(0xa0000);
 int n,k,m;
 short y[80];
 for(n=0;n<80;n++)
  {
   y[n]=0;
  }
 for(n=0;n<199;n++)
  {
   while (TimerUser/4==oldtimer/4);
   oldtimer=TimerUser;
   k=0;
   for(m=0;m<80;m++)
    {
     int y1=y[m];
     int y2=y[m]+20;
     if (y2>199) y2=199;
     int dy=rnd(15)+1;
     for(int z=199;z>y1+dy;z--)
      for(int x=m*4;x<m*4+4;x++)
       *(vpt+z*320+x)=*(vpt+(z-dy)*320+x);
     for(z=y1;z<=y2;z++)
      for(int x=m*4;x<m*4+4;x++)
       *(vpt+z*320+x)=*(Video.Buffer+z*320+x);
     if (y[m]<199) y[m]=y[m]+dy;
     if (y[m]>199) y[m]=199;
     if (y[m]==199) k++;
     if (k==80) break;
    }
   if (k==80) break;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Экран проявляется.                                                //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void LightingPlus(void)
{
 float r=0;
 int oldtimer=TimerUser;
 for(int n=0;n<=100;n++,r+=0.01)
  {
   while (TimerUser==oldtimer);
   oldtimer=TimerUser;
   Video.SetRGB_DAC(r,r,r);
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Экран гаснет.                                                     //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void LightingMinus(void)
{
 float r=1;
 int oldtimer=TimerUser;
 for(int n=0;n<=100;n++,r-=0.01)
  {
   while (TimerUser==oldtimer);
   oldtimer=TimerUser;
   Video.SetRGB_DAC(r,r,r);
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Генератор случайных цыфр                                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int rnd(int max)
{
 int i=((max*rand())/32768);
 return(i);
}
