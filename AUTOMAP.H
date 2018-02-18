void Line(int x1,int y1,int x2,int y2,char color);
void putpixel(int x,int y,char color);
void PutAutoMap(void);
//============================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Рисует линию x1,y1-x2,y2 цвета color.                             //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Line(int x1,int y1,int x2,int y2,char color)
{
 if (x1<0 && x2<0) return;
 if (y1<0 && y2<0) return;
 if (x1>319 && x2>319) return;
 if (y1>167 && y2>167) return;
 int dx=abs(x2-x1);
 int dy=abs(y2-y1);
 int sx=x2>=x1?1:-1;
 int sy=y2>=y1?1:-1;
 if (dy<=dx)
  {
   int d=(dy<<1)-dx;
   int d1=dy<<1;
   int d2=(dy-dx)<<1;
   putpixel(x1,y1,color);
   for(int x=x1+sx,y=y1,i=1;i<=dx;i++,x+=sx)
    {
     if (d>0)
      {
       d+=d2;
       y+=sy;
      }
     else d+=d1;
     putpixel(x,y,color);
    }
  }
 else
  {
   int d=(dx<<1)-dy;
   int d1=dx<<1;
   int d2=(dx-dy)<<1;
   putpixel(x1,y1,color);
   for(int x=x1,y=y1+sy,i=1;i<=dy;i++,y+=sy)
    {
     if (d>0)
      {
       d+=d2;
       x+=sx;
      }
     else d+=d1;
     putpixel(x,y,color);
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Ставит точку x,y цвета color.                                     //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void putpixel(int x,int y,char color)
{
 if (x<0) return;
 if (y<0) return;
 if (x>319) return;
 if (y>167) return;
 *(Video.Buffer+x+y*320)=color;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Обеспечивает вывод карты.                                         //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void PutAutoMap(void)
{
 int n,m;
 int x1,y1,x2,y2;
 for(n=0;n<Info.SegmentNumber;n++)
  {
   x1=Segment[n].X1-Player.X;
   y1=Segment[n].Y1-Player.Y;
   x2=Segment[n].X2-Player.X;
   y2=Segment[n].Y2-Player.Y;
   if (Segment[n].Visible==1)
    {
     if (Segment[n].Frontier!=0)
      {
       int up1=Sector[Segment[n].Sector1].Up;
       int up2=Sector[Segment[n].Sector2].Up;
       int down1=Sector[Segment[n].Sector1].Down;
       int down2=Sector[Segment[n].Sector2].Down;
       if (up1==up2 && down1==down2) continue;
       Line(x1/20+160,84-y1/20,x2/20+160,84-y2/20,161);
      }
     else Line(x1/20+160,84-y1/20,x2/20+160,84-y2/20,177);

    }
  }
 int a=360-Player.Angle;
 if (a<0) a+=360;
 Line(160,84,160-5*SinTable[a],84-5*CosTable[a],232);
}

