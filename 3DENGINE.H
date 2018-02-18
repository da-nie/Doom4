//===========================================================================
short TopLine[330];
short BottomLine[330];
short st;//число заполненых столбцов экрана
//===========================================================================
int SeenBox(BSPNode* tree,int type);
inline void DrawBSPTree(BSPNode* tree);
void DrawSubSector(BSPNode* root);
void DrawFloorLine(int x,int y1,int y2,short f_texture);
unsigned char *FindTexture(int ky,int offsett,int texture,int *mask,int *dy_,int *yt_);
void DrawSimpleLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short Sector);
inline void DrawLowerLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short Sector);
inline void DrawUpperLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short Sector);
void DrawFloor_Flow(void);
void View(void);
//===========================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает 1 если описывающий прямоугольник виден.Иначе-0.        //
//tree-указатель ветви дерева,type-тип прямоугольника(LEFT,RIGHT).  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int SeenBox(BSPNode* tree,int type)
{
 if (tree->Split!=1) return(1);
 float ss=SinTable[Player.Angle];
 float cs=CosTable[Player.Angle];
 float x1,y1,x2,y2;
 if (type==LEFT)//проверим видимость поддерева
  {
   x1=tree->LX1-Player.X;
   y1=tree->LY1-Player.Y;
   x2=tree->LX2-Player.X;
   y2=tree->LY2-Player.Y;
  }
 if (type==RIGHT)
  {
   x1=tree->RX1-Player.X;
   y1=tree->RY1-Player.Y;
   x2=tree->RX2-Player.X;
   y2=tree->RY2-Player.Y;
  }
 float x1ss=x1*ss;
 float x2ss=x2*ss;
 float y1ss=y1*ss;
 float y2ss=y2*ss;
 float x1cs=x1*cs;
 float x2cs=x2*cs;
 float y1cs=y1*cs;
 float y2cs=y2*cs;
 float v1=x1ss+y1cs;
 float v2=x2ss+y2cs;
 float v3=x1ss+y2cs;
 float v4=x2ss+y1cs;
 if (v1<=1 && v2<=1 && v3<=1 && v4<=1) return(0);
 if (v1<=0 || v2<=0 || v3<=0 || v4<=0) return(1);//не проверить
 float u1=x1cs-y1ss;
 float u2=x2cs-y2ss;
 float u3=x1cs-y2ss;
 float u4=x2cs-y1ss;
 float xs1,xs2,xs3,xs4;
 xs1=160+160*u1/v1;
 xs2=160+160*u2/v2;
 xs3=160+160*u3/v3;
 xs4=160+160*u4/v4;
 if (xs1<0 && xs2<0 && xs3<0 && xs4<0) return(0);
 if (xs1>319 && xs2>319 && xs3>319 && xs4>319) return(0);
 return(1);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Прорисовывает лабиринт по BSP дереву.                             //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
inline void DrawBSPTree(BSPNode* tree)
{
 if (st>=320) return;
 int p;
 if (tree->Split==0)
  {
   DrawSubSector(tree);
   return;
  }
 p=Diagnostik(Player.X,Player.Y,tree->X1,tree->Y1,tree->X2,tree->Y2);
 if (p<=0)
  {
   if (tree->Right!=NULL)
    {
     if (SeenBox(tree,RIGHT)) DrawBSPTree(tree->Right);
    }
   DrawSubSector(tree);
   if (tree->Left!=NULL)
    {
     if (SeenBox(tree,LEFT)) DrawBSPTree(tree->Left);
    }
   return;
  }
 else
  {
   if (tree->Left!=NULL)
    {
     if (SeenBox(tree,LEFT)) DrawBSPTree(tree->Left);
    }
   DrawSubSector(tree);
   if (tree->Right!=NULL)
    {
     if (SeenBox(tree,RIGHT)) DrawBSPTree(tree->Right);
    }
   return;
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Прорисовывыет сегмент стены.                                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void DrawSubSector(BSPNode* root)
{
 float x1=root->X1;
 float x2=root->X2;
 float y1=root->Y1;
 float y2=root->Y2;
 int p=Diagnostik(Player.X,Player.Y,x1,y1,x2,y2);
 float cs=CosTable[Player.Angle];
 float ss=SinTable[Player.Angle];
 float c1,c2;
 x1=root->X1-Player.X;
 x2=root->X2-Player.X;
 y1=root->Y1-Player.Y;
 y2=root->Y2-Player.Y;
 float v1=x1*ss+y1*cs;
 float v2=x2*ss+y2*cs;
 if (v1<1 && v2<1) return;
 float u1=x1*cs-y1*ss;
 float u2=x2*cs-y2*ss;
 float vo1=v1;
 float vo2=v2;
 float uo1=u1;
 float uo2=u2;
 if (v1<1)
  {
   uo1+=(1-v1)*(u2-u1)/(v2-v1);
   vo1=1;
  }
 if (v2<1)
  {
   uo2+=(1-v2)*(u2-u1)/(v2-v1);
   vo2=1;
  }
 float col1=160+160*uo1/vo1;
 float col2=160+160*uo2/vo2;
 int offset=root->Offset%128;
 if (col2<col1)
  {
   offset=128-(Segment[root->Type].Len+offset);
   c1=col2;
   c2=col1;
   float a=u1;
   u1=u2;
   u2=a;
   a=v1;
   v1=v2;
   v2=a;
   a=uo1;
   uo1=uo2;
   uo2=a;
   a=vo1;
   vo1=vo2;
   vo2=a;
  }
 else
  {
   c1=col1;
   c2=col2;
  }
 if (c2<0 || c1>319) return;
 if (c1==c2) return;
 int t;
 float z;
 if (root->Frontier==0)//это не линия раздела
  {
   t=sqrt((v1-vo1)*(v1-vo1)+(u1-uo1)*(u1-uo1))+offset;
   z=vo1*0.01;
   float vz1=1/vo1;
   float vt1=t/vo1;
   float t1=84-(Sector[root->Sector1].Up-Player.Z+Player.Pz+Player.SitZ)/z;
   float b1=84-(Sector[root->Sector1].Down-Player.Z+Player.Pz+Player.SitZ)/z;
   float dc=c2-c1+1;
   t=sqrt((vo2-v1)*(vo2-v1)+(uo2-u1)*(uo2-u1))+offset;
   z=vo2*0.01;
   float vz2=1/vo2;
   float vt2=t/vo2;
   float t2=84-(Sector[root->Sector1].Up-Player.Z+Player.Pz+Player.SitZ)/z;
   float b2=84-(Sector[root->Sector1].Down-Player.Z+Player.Pz+Player.SitZ)/z;
   float dtop=(t2-t1)/dc;
   float dbottom=(b2-b1)/dc;
   float dz=(vz2-vz1)/dc;
   float dt=(vt2-vt1)/dc;
   if (c1<0)
    {
     t1-=dtop*c1;
     b1-=dbottom*c1;
     vz1-=dz*c1;
     vt1-=dt*c1;
     c1=0;
    }
   if (c2>319) c2=319;
   for (register int c=c1;c<=c2;c++,t1+=dtop,b1+=dbottom,vz1+=dz,vt1+=dt)
    {
     if (TopLine[c]>=BottomLine[c]) continue;
      else
       {
        Segment[root->Number].Visible=1;
       }
     z=1.0/vz1;
     if (z<1) continue;
     t=vt1/vz1;
     float ys=t1;
     float ys1=b1;
     DrawSimpleLine(z,c,ys,ys1,t,Sector[root->Sector1].Up-Sector[root->Sector1].Down,root->Type,Segment[root->Type].Type,root->Sector1);
    }
  }
 else
  {
   int sd1,sd2,su1,su2;
   short Sectors=0;
   if (p<0)//игрок справа
    {
     sd1=Sector[root->Sector1].Down;
     sd2=Sector[root->Sector2].Down;
     su1=Sector[root->Sector2].Up;
     su2=Sector[root->Sector1].Up;
     Sectors=root->Sector1;
    }
   else
    {
     sd1=Sector[root->Sector2].Down;
     sd2=Sector[root->Sector1].Down;
     su1=Sector[root->Sector1].Up;
     su2=Sector[root->Sector2].Up;
     Sectors=root->Sector2;
    }
   if (sd1>=su1) su1=sd1;
   t=sqrt((v1-vo1)*(v1-vo1)+(u1-uo1)*(u1-uo1))+offset;
   z=vo1*0.01;
   float vz1=1/vo1;
   float vt1=t/vo1;
   float oz=-Player.Z+Player.Pz+Player.SitZ;
   float tu1=84-(su2+oz)/z;
   float bu1=84-(su1+oz)/z;
   float td1=84-(sd2+oz)/z;
   float bd1=84-(sd1+oz)/z;
   float dc=c2-c1+1;
   t=sqrt((vo2-v1)*(vo2-v1)+(uo2-u1)*(uo2-u1))+offset;
   z=vo2*0.01;
   float vz2=1/vo2;
   float vt2=t/vo2;
   float tu2=84-(su2+oz)/z;
   float bu2=84-(su1+oz)/z;
   float td2=84-(sd2+oz)/z;
   float bd2=84-(sd1+oz)/z;
   float dutop=(tu2-tu1)/dc;
   float dubottom=(bu2-bu1)/dc;
   float ddtop=(td2-td1)/dc;
   float ddbottom=(bd2-bd1)/dc;
   float dz=(vz2-vz1)/dc;
   float dt=(vt2-vt1)/dc;
   if (c1<0)
    {
     tu1-=dutop*c1;
     bu1-=dubottom*c1;
     td1-=ddtop*c1;
     bd1-=ddbottom*c1;
     vz1-=dz*c1;
     vt1-=dt*c1;
     c1=0;
    }
   if (c2>319) c2=319;
   int hgtup=abs(su2-su1);
   int hgtdown=abs(sd2-sd1);
   for (register int c=c1;c<c2;c++,tu1+=dutop,bu1+=dubottom,td1+=ddtop,bd1+=ddbottom,vz1+=dz,vt1+=dt)
    {
     if (TopLine[c]>=BottomLine[c]) continue;
      else
       {
        Segment[root->Number].Visible=1;
       }
     z=1.0/vz1;
     if (z<1) continue;
     t=vt1/vz1;
     float ys=tu1;
     float ys1=bu1;
     DrawUpperLine(z,c,ys,ys1,t,hgtup,root->Type,Segment[root->Type].Type,Sectors);
     ys=td1;
     ys1=bd1;
     DrawLowerLine(z,c,ys,ys1,t,hgtdown,root->Type,Segment[root->Type].Type,Sectors);
    }
  }
}
void DrawFloorLine(int x,int y1,int y2,short f_texture)
{
 f_texture++;
 register int n;
 short *ptr=FloorBuffer+y1*320+x;
 for(n=y1;n<=y2;n++,ptr+=320) *ptr=f_texture;
}
unsigned char *FindTexture(int ky,int offsett,int texture,int *mask,int *dy_,int *yt_)
{
 int dy=*dy_;
 int yt=*yt_;
 int r=127;
 int offset=(offsett&127)<<7;
 unsigned char *rtexture=texture*16384+Data.WallTexture1+offset;
 if (ky>=2 && ky<4)
  {
   dy=dy/2;
   yt=yt/2;
   r=63;
   offset=((int)(offsett&127)/2)<<6;
   rtexture=texture*4096+Data.WallTexture2+offset;
  }
 if (ky>=4 && ky<8)
  {
   dy=dy/4;
   yt=yt/4;
   r=31;
   offset=((int)(offsett&127)/4)<<5;
   rtexture=texture*1024+Data.WallTexture3+offset;
  }
 if (ky>=8 && ky<16)
  {
   dy=dy/8;
   yt=yt/8;
   r=15;
   offset=((int)(offsett&127)/8)<<4;
   rtexture=texture*256+Data.WallTexture4+offset;
  }
 if (ky>=16 && ky<32)
  {
   dy=dy/16;
   yt=yt/16;
   r=7;
   offset=((int)(offsett&127)/16)<<3;
   rtexture=texture*64+Data.WallTexture5+offset;
  }
 if (ky>=32 && ky<64)
  {
   dy=dy/32;
   yt=yt/32;
   r=3;
   offset=((int)(offsett&127)/32)<<2;
   rtexture=texture*16+Data.WallTexture6+offset;
  }
 if (ky>=64 && ky<128)
  {
   dy=dy/64;
   yt=yt/64;
   r=1;
   offset=((int)(offsett&127)/64)<<1;
   rtexture=texture*4+Data.WallTexture7+offset;
  }
 if (ky>=128)
  {
   dy=dy/128;
   yt=yt/128;
   r=1;
   rtexture=texture+Data.WallTexture8;
  }
 *dy_=dy;
 *yt_=yt;
 *mask=r;
 return(rtexture);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Рисует столбец монолитной стены.                                  //
//z-координата z стены,xs-столбец экрана,Top-верхняя,Bottom-нижняя  //
//координата проекций стены на экране,u-смещение текстуры,height-   //
//высота стены,o-номер сектора,к которому принадлежит стена         //
//texture-указатель на данные текстуры стены.                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void DrawSimpleLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short Sector)
{
 //заполняем линии буффера глубины
 register int pt=Z_Lines[xs].Pointer;
 Z_Lines[xs].Begin_Y[pt]=TopLine[xs];
 Z_Lines[xs].End_Y[pt]=BottomLine[xs];
 Z_Lines[xs].Z_Value[pt]=z;
 Z_Lines[xs].Pointer=pt+1;
 register int offset;
 register int j;
 register int n;
 register unsigned char *vpt;
 register int ys1,ys;
 int yt,dy;
 dy=0;
 yt=0;
 dy=(height*1048576)/(Bottom-Top+1);
 if (Bottom>BottomLine[xs]) Bottom=BottomLine[xs];
 if (Top>BottomLine[xs])
  {
   j=BottomLine[xs];
   yt=dy*(j-Top);
   Top=j;
  }
 if (Bottom<TopLine[xs]) Bottom=TopLine[xs];
 if (Top>=TopLine[xs])
  {
   DrawFloorLine(xs,TopLine[xs],Top-1,Sector);
   TopLine[xs]=Top;
  }
  else
   {
    j=TopLine[xs];
    yt=dy*(j-Top);
    Top=j;
   }
 if (Bottom<=BottomLine[xs])
  {
   DrawFloorLine(xs,Bottom,BottomLine[xs]-1,Sector);
   BottomLine[xs]=Bottom;
  }
 else Bottom=BottomLine[xs];
 TopLine[xs]=Bottom;
 BottomLine[xs]=Top;
 vpt=Video.Buffer+xs+((int)Top)*320;
 int ky=dy>>20;//коэффициент сжатия
 int mask;
 unsigned char *texturet=FindTexture(ky,u,texture,&mask,&dy,&yt);
 st++;
 yt-=dy*(Top-(int)Top);
 for(n=Top;n<=Bottom;n++,vpt+=320,yt+=dy) *(vpt)=*(texturet+((yt>>20)&mask));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Рисует столбец нижней стены.                                      //
//z-координата z стены,xs-столбец экрана,Top-верхняя,Bottom-нижняя  //
//координата проекций стены на экране,u-смещение текстуры,height-   //
//высота стены,o-номер сектора,к которому принадлежит стена         //
//texture-указатель на данные текстуры стены.                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
inline void DrawLowerLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short Sector)
{
 register int offset;
 register int j;
 register int n;
 register char *vpt;
 register int ys1,ys;
 int yt,dy;
 dy=0;
 yt=0;
 float Bottom_Top=(Bottom-Top+1);
 if (Top<TopLine[xs]) st++;
 if (Bottom<TopLine[xs]) Bottom=TopLine[xs];
 if (Bottom<BottomLine[xs]) DrawFloorLine(xs,Bottom,BottomLine[xs]-1,Sector);
 else Bottom=BottomLine[xs];
 if (Top<Bottom)
  {
   dy=1048576*height/Bottom_Top;
   if (Top<TopLine[xs])
    {
     j=TopLine[xs];
     yt=dy*(j-Top);
     Top=j;
    }
   vpt=Video.Buffer+xs+((int)Top)*320;
   int pt=Z_Lines[xs].Pointer;
   Z_Lines[xs].Begin_Y[pt]=Top;
   Z_Lines[xs].End_Y[pt]=BottomLine[xs];
   Z_Lines[xs].Z_Value[pt]=z;
   Z_Lines[xs].Pointer=pt+1;
   int ky=dy>>20;//коэффициент сжатия
   int mask;
   unsigned char *texturet=FindTexture(ky,u,texture,&mask,&dy,&yt);
   yt-=dy*(Top-(int)Top);
   for(n=Top;n<=Bottom;n++,vpt+=320,yt+=dy) *(vpt)=*(texturet+((yt>>20)&mask));
   if (Top<BottomLine[xs]) BottomLine[xs]=Top;
  }
 else
  {
   if (Bottom<BottomLine[xs])
    {
     int pt=Z_Lines[xs].Pointer;
     Z_Lines[xs].Begin_Y[pt]=Bottom;
     Z_Lines[xs].End_Y[pt]=BottomLine[xs];
     Z_Lines[xs].Z_Value[pt]=z;
     Z_Lines[xs].Pointer=pt+1;
     BottomLine[xs]=Bottom;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Рисует столбец верхней стены.                                     //
//z-координата z стены,xs-столбец экрана,Top-верхняя,Bottom-нижняя  //
//координата проекций стены на экране,u-смещение текстуры,height-   //
//высота стены,o-номер сектора,к которому принадлежит стена         //
//texture-указатель на данные текстуры стены.                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
inline void DrawUpperLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short Sector)
{
 register int offset;
 register int j;
 register int n;
 register char *vpt;
 register int ys1,ys;
 int yt,dy;
 dy=0;
 yt=0;
 float Bottom_Top=(Bottom-Top+1);
 if (Bottom>BottomLine[xs]) st++;
 if (Top>BottomLine[xs]) Top=BottomLine[xs];
 if (Top>TopLine[xs]) DrawFloorLine(xs,TopLine[xs],Top-1,Sector);
 if (Bottom>BottomLine[xs]) Bottom=BottomLine[xs];
 if (Top<Bottom)
  {
   dy=1048576*height/Bottom_Top;
   if (Top<=TopLine[xs])
    {
     j=TopLine[xs];
     yt=dy*(j-Top);
     Top=j;
    }
   vpt=Video.Buffer+xs+((int)Top)*320;
   int pt=Z_Lines[xs].Pointer;
   Z_Lines[xs].Begin_Y[pt]=TopLine[xs];
   Z_Lines[xs].End_Y[pt]=Bottom;
   Z_Lines[xs].Z_Value[pt]=z;
   Z_Lines[xs].Pointer=pt+1;
   int ky=dy>>20;//коэффициент сжатия
   int mask;
   unsigned char *texturet=FindTexture(ky,u,texture,&mask,&dy,&yt);
   yt-=dy*(Top-(int)Top);
   for(n=Top;n<=Bottom;n++,vpt+=320,yt+=dy) *(vpt)=*(texturet+((yt>>20)&mask));
   if (Bottom>TopLine[xs]) TopLine[xs]=Bottom;
  }
 else
  {
   if (Top>TopLine[xs])
    {
     int pt=Z_Lines[xs].Pointer;
     Z_Lines[xs].Begin_Y[pt]=TopLine[xs];
     Z_Lines[xs].End_Y[pt]=Top;
     Z_Lines[xs].Z_Value[pt]=z;
     Z_Lines[xs].Pointer=pt+1;
     TopLine[xs]=Top;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Рисует полы и потолки секторов                                    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void DrawFloor_Flow(void)
{
register int row,col;
 int pt,n;
 int px=Player.P_X;
 int py=Player.P_Y;
 int floor;
 int flow;
 int Z_SectorsFloor;
 int Z_SectorsFlow;
 int dist;
 register unsigned char *texturefloor;
 register unsigned char *textureflow;
 register int diup,djup;
 register int i0up,j0up;
 register int didown,djdown;
 register int i0down,j0down;
 register unsigned char *vptup=Video.Buffer+26560;
 register unsigned char *vptdown=Video.Buffer+26880;
 register short *fbufferup=FloorBuffer+26560;
 register short *fbufferdown=FloorBuffer+26560;
 for(row=0;row<=83;row++,vptup-=640,fbufferup-=640)
  {
   floor=-1;
   flow=-1;
   diup=0;
   djup=0;
   j0up=0;
   i0up=0;
   didown=0;
   djdown=0;
   j0down=0;
   i0down=0;
   for(col=0;col<320;col++,i0up+=diup,j0up+=djup,i0down+=didown,j0down+=djdown,vptdown++,vptup++,fbufferup++,fbufferdown++)
    {
     short fup=*fbufferup-1;
     short fdown=*fbufferdown-1;
     if (fdown==-1 && fup==-1) continue;
     if (floor!=fdown && fdown!=-1)
      {
       floor=fdown;
       texturefloor=(Sector[floor].DownTexture<<14)+Data.WallTexture1;
       Z_SectorsFloor=(-Sector[floor].Down+Player.Z-Player.Pz-Player.SitZ)*100;
       dist=Z_SectorsFloor/(row+1);
       didown=dist*Maze.diTable[Player.Angle];
       djdown=dist*Maze.djTable[Player.Angle];
       i0down=dist*Maze.dsTable[Player.Angle]+px-didown*(159-col);
       j0down=dist*Maze.dcTable[Player.Angle]+py-djdown*(159-col);
      }
     if (fdown!=-1)
      {
       int i=(i0down>>9)&16256;
       int j=(j0down>>16)&127;
       *(vptdown)=*(texturefloor+i+j);
      }
     if (flow!=fup && fup!=-1)
      {
       flow=fup;
       textureflow=(Sector[fup].UpTexture<<14)+Data.WallTexture1;
       Z_SectorsFlow=(Sector[fup].Up-Player.Z+Player.Pz+Player.SitZ)*100;
       dist=Z_SectorsFlow/(row+1);
       diup=dist*Maze.diTable[Player.Angle];
       djup=dist*Maze.djTable[Player.Angle];
       i0up=dist*Maze.dsTable[Player.Angle]+px-diup*(159-col);
       j0up=dist*Maze.dcTable[Player.Angle]+py-djup*(159-col);
      }
     if (fup!=-1)
      {
       int i=(i0up>>9)&16256;
       int j=(j0up>>16)&127;
       *(vptup)=*(textureflow+i+j);
      }
    }
  }
}
void ClearFloorBuffer(void)
{
 unsigned char *dpt=(unsigned char *) FloorBuffer;
 *(dpt)=0;
 *(dpt+1)=0;
 *(dpt+2)=0;
 *(dpt+3)=0;
 #pragma aux clearfloorbuffer parm[]=\
 "mov esi,dpt"\
 "mov ecx,26881"\
 "mov eax,ecx"\
 "mov edi,dpt"\
 "add edi,4"\
 "rep movsd"\
 modify [esi edi ecx eax];
 clearfloorbuffer();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Обеспечивает подготовку к прорисовке и прорисовку лабиринта.      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void View(void)
{
 st=0;
 register int n,m;
 ClearFloorBuffer();
 for(n=0;n<320;n++)
  {
   //инициализировали линии горизонта
   TopLine[n]=0;
   BottomLine[n]=167;
   //инициализировали линии буффера глубины
   Z_Lines[n].Pointer=0;//в списке нет линий
  }
 DrawBSPTree(Node);//рисуем сегменты
 DrawFloor_Flow();//рисуем полы и потолки
 int *necess=new int[1+NowMissile+NowThing+NowAlien];//сортируем предметы,врагов и снаряды
 int *znecess=new int[1+NowMissile+NowThing+NowAlien];
 float cs=CosTable[Player.Angle];
 float ss=SinTable[Player.Angle];
 int x1,y1;
 int no=0;
 //добавляем в таблицу сортировки снаряды
 for(n=0;n<NowMissile;n++)
  {
   x1=Missile[n]->X-Player.X;
   y1=Missile[n]->Y-Player.Y;
   int z=x1*ss+y1*cs;
   if (!(z<7 || z>6400))
    {
     int x=x1*cs-y1*ss;
     int col1=160+160*(x-Missile[n]->Radius)/z;
     int col2=160+160*(x+Missile[n]->Radius)/z;
     if (!(col1<0 && col2<0 || col1>319 && col2>319))
      {
       necess[no]=n;
       znecess[no]=z;
       no++;
      }
    }
  }
 //добавляем предметы
 for(n=0;n<NowThing;n++)
  {
   x1=Thing[n]->Coord.X-Player.X;
   y1=Thing[n]->Coord.Y-Player.Y;
   int z=x1*ss+y1*cs;
   if (!(z<7 || z>6400))
    {
     int x=x1*cs-y1*ss;
     int col1=160+160*(x-Thing[n]->Coord.Radius)/z;
     int col2=160+160*(x+Thing[n]->Coord.Radius)/z;
     if (!(col1<0 && col2<0 || col1>319 && col2>319))
      {
       necess[no]=n+NowMissile;
       znecess[no]=z;
       no++;
      }
    }
  }
 //добавляем врагов
 for(n=0;n<NowAlien;n++)
  {
   x1=Alien[n]->Coord.X-Player.X;
   y1=Alien[n]->Coord.Y-Player.Y;
   int z=x1*ss+y1*cs;
   if (!(z<7 || z>6400))
    {
     int x=x1*cs-y1*ss;
     int col1=160+160*(x-Alien[n]->Coord.Radius)/z;
     int col2=160+160*(x+Alien[n]->Coord.Radius)/z;
     if (!(col1<0 && col2<0 || col1>319 && col2>319))
      {
       necess[no]=n+NowThing+NowMissile;
       znecess[no]=z;
       no++;
      }
    }
  }
 for(n=0;n<no;n++)
  for(m=0;m<no-1;m++)
   {
    if (znecess[m+1]>znecess[m])
     {
      int r=znecess[m];
      znecess[m]=znecess[m+1];
      znecess[m+1]=r;
      r=necess[m];
      necess[m]=necess[m+1];
      necess[m+1]=r;
     }
   }
 for(n=0;n<no;n++)
  {
   if (necess[n]<NowMissile && necess[n]>=0) Missile[necess[n]]->Put();//выводим снаряды
   if (necess[n]>=NowMissile && necess[n]<NowThing+NowMissile) Thing[necess[n]-NowMissile]->Put();//выводим предметы
   if (necess[n]>=NowThing+NowMissile) Alien[necess[n]-NowMissile-NowThing]->Put();//выводим врагов
  }
 delete(necess);
 delete(znecess);
}
