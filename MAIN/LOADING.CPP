#include "loading.h"

//============================================================================
SInfo sInfo;
SSegment *sSegment;
SSector *sSector;
SBSPNode *Node;
FILE *in;
//============================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загрузка спрайт файла.                                            //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
unsigned char* LoadSpriteFile(char *FileName)
{
 in=fopen(FileName,"r+b");
 unsigned char a,b,c,d;
 fread(&a,1,1,in);
 fread(&b,1,1,in);
 fread(&c,1,1,in);
 fread(&d,1,1,in);
 int len=a+b*256+c*65536+d*16777216;
 unsigned char *dpt=(unsigned char *)malloc(len+1);
 for(int n=0;n<len;n++)
  {
   fread(&a,1,1,in);
   dpt[n]=a;
  }
 return(dpt);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загрузка текстур.                                                 //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void LoadResurs(void)
{
 unsigned char a;
 register int n;
 int s,t;
 sData.Font=LoadSpriteFile("resurs\\font.spt");
//============================================================================
 cVideo.Clear();
 cVideo.Print(0,0,"LOADING TEXTURE MAP");
 cVideo.View();
 sData.Thing=LoadSpriteFile("resurs\\tiles\\thing.spt");
 sData.Alien=LoadSpriteFile("resurs\\tiles\\alien.spt");
 sData.Fire=LoadSpriteFile("resurs\\tiles\\fire.spt");
 sData.Menu=LoadSpriteFile("resurs\\tiles\\menu.spt");
 in =fopen("resurs\\data.pak","r+b");
 if (in!=NULL)
  {
   fread(&a,1,1,in);
   t=(int)a;
   s=((int)(a+1))*16384;
   sData.WallTexture1=(unsigned char*)malloc(s);
   sData.WallTexture2=(unsigned char*)malloc(s/2);
   sData.WallTexture3=(unsigned char*)malloc(s/4);
   sData.WallTexture4=(unsigned char*)malloc(s/8);
   sData.WallTexture5=(unsigned char*)malloc(s/16);
   sData.WallTexture6=(unsigned char*)malloc(s/32);
   sData.WallTexture7=(unsigned char*)malloc(s/64);
   sData.WallTexture8=(unsigned char*)malloc(s/128);
   for (n=0;n<t;n++)
    {
     fread(&a,1,1,in);//резервные данные
     fread(&a,1,1,in);
     fread(&a,1,1,in);
     fread(&a,1,1,in);
     fread(&a,1,1,in);
     fread(&a,1,1,in);
     fread(&a,1,1,in);
     for(int m=0;m<16384;m++)
      {
       fread(&a,1,1,in);
       *(sData.WallTexture1+n*16384+m)=a;
      }
    }
    fclose(in);
   }
 sData.Bar=LoadSpriteFile("resurs\\tiles\\bar.spt");
 sData.Weapon0=LoadSpriteFile("resurs\\tiles\\weapon0.spt");
 sData.Weapon1=LoadSpriteFile("resurs\\tiles\\weapon1.spt");
 sData.Weapon2=LoadSpriteFile("resurs\\tiles\\weapon2.spt");
 sData.Weapon3=LoadSpriteFile("resurs\\tiles\\weapon3.spt");
 sData.Weapon4=LoadSpriteFile("resurs\\tiles\\weapon4.spt");
 cVideo.Print(0,0,"LOADING TEXTURE MAP -OK.");
 in=fopen("resurs\\mipmap.crt","r+b");
 if (in==NULL)//нет такого файла
  {
   cVideo.Print(0,10,"CREATE MIPMAPPING MAP");
   cVideo.View();
   in=fopen("resurs\\mipmap.crt","w+b");
   MipMapping(in,t);
   fclose(in);
   cVideo.Print(0,10,"CREATE MIPMAPPING MAP -OK.");
   cVideo.View();
  }
 else
  {
   unsigned char a,b;
   fread(&a,1,1,in);
   fread(&b,1,1,in);
   int t1=(int)a+((int)b)*256;
   if (t1!=t) //это другой файл
    {
     cVideo.Print(0,10,"CREATE MIPMAPPING MAP");
     cVideo.View();
     in=fopen("resurs\\mipmap.crt","w+b");
     MipMapping(in,t);
     fclose(in);
     cVideo.Print(0,10,"CREATE MIPMAPPING MAP -OK.");
     cVideo.View();
     return;
    }
   else//загружаем пирамиду
    {
     cVideo.Print(0,10,"LOADING MIPMAPPING MAP");
     cVideo.View();
     int x,y;
     for(n=0;n<t;n++)
      {
       for(x=0;x<64;x++)
        for(y=0;y<64;y++)
         {
          fread(&a,1,1,in);
          sData.WallTexture2[n*4096+x+y*64]=a;
         }
       for(x=0;x<32;x++)
        for(y=0;y<32;y++)
        {
         fread(&a,1,1,in);
         sData.WallTexture3[n*1024+x+y*32]=a;
        }
       for(x=0;x<16;x++)
        for(y=0;y<16;y++)
         {
          fread(&a,1,1,in);
          sData.WallTexture4[n*256+x+y*16]=a;
         }
       for(x=0;x<8;x++)
        for(y=0;y<8;y++)
         {
          fread(&a,1,1,in);
          sData.WallTexture5[n*64+x+y*8]=a;
         }
       for(x=0;x<4;x++)
        for(y=0;y<4;y++)
         {
          fread(&a,1,1,in);
          sData.WallTexture6[n*16+x+y*4]=a;
         }
       for(x=0;x<2;x++)
        for(y=0;y<2;y++)
         {
          fread(&a,1,1,in);
          sData.WallTexture7[n*4+x+y*2]=a;
         }
        fread(&a,1,1,in);
        sData.WallTexture8[n]=a;
      }
     fclose(in);
     cVideo.Print(0,10,"LOADING MIPMAPPING MAP -OK");
     cVideo.View();
     return;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загрузка уровня и инициализация таблиц.                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int LoadGame(int level)
{
 FILE *in;
 unsigned char a,b,c,d;
 int n,m,k;
 in=GoToBlock((level-1)*3);
 if (in==NULL) return(1);//такого уровня нет
 Node=LoadTree(in);
 fread(&a,1,1,in);//длина блока
 fread(&b,1,1,in);
 fread(&c,1,1,in);
 fread(&d,1,1,in);
 //читаем предметы
 sInfo.ThingNumber=LoadInt(in);
 sInfo.TX=new int[sInfo.ThingNumber+1];
 sInfo.TY=new int[sInfo.ThingNumber+1];
 sInfo.TType=new int[sInfo.ThingNumber+1];
 for(n=0;n<sInfo.ThingNumber;n++)
  {
   sInfo.TX[n]=LoadInt(in);
   sInfo.TY[n]=LoadInt(in);
   sInfo.TType[n]=LoadInt(in);
  }
 fread(&a,1,1,in);//длина блока
 fread(&b,1,1,in);
 fread(&c,1,1,in);
 fread(&d,1,1,in);
 //читаем врагов
 sInfo.AlienNumber=LoadInt(in);
 sInfo.AX=new int[sInfo.AlienNumber+1];
 sInfo.AY=new int[sInfo.AlienNumber+1];
 sInfo.AType=new int[sInfo.AlienNumber+1];
 for(n=0;n<sInfo.AlienNumber;n++)
  {
   sInfo.AX[n]=LoadInt(in);
   sInfo.AY[n]=LoadInt(in);
   sInfo.AType[n]=LoadInt(in);
  }
 fclose(in);
 for (n=0;n<=360;n++)
  {
   CosTable[n]=cos(pi/180*n);
   SinTable[n]=sin(pi/180*n);
  }
  for(n=0;n<=360;n++)
   {
    sMaze.dcTable[n]=cos(pi/180*n)*65536;
    sMaze.dsTable[n]=sin(pi/180*n)*65536;
    sMaze.diTable[n]=sMaze.dcTable[n]/160;
    sMaze.djTable[n]=-sMaze.dsTable[n]/160;
   }
 short *sgm=(short *)malloc(sInfo.SegmentNumber*sizeof(short));
 for(int x=0;x<100;x++)//заполняем структуру BlockMap
  for(int y=0;y<100;y++)
   {
    int u=0;
    int X_B1=x*128-1;
    int X_B2=x*128+129;
    int Y_B1=y*128-1;
    int Y_B2=y*128+129;
    for(n=0;n<sInfo.SegmentNumber;n++)
    {
     int x1=sSegment[n].X1;
     int x2=sSegment[n].X2;
     int y1=sSegment[n].Y1;
     int y2=sSegment[n].Y2;
     if (x1<X_B1 && x2<X_B1) continue;
     if (x1>X_B2 && x2>X_B2) continue;
     if (y1<Y_B1 && y2<Y_B1) continue;
     if (y1>Y_B2 && y2>Y_B2) continue;
     sgm[u]=n;
     u++;
    }
    sMaze.BlockMap[y][x]=(short *)malloc((u+1)*sizeof(short));
    for(n=0;n<u;n++)
    {
     sMaze.BlockMap[y][x][n+1]=sgm[n];
    }
    sMaze.BlockMap[y][x][0]=u;
   }
 free(sgm);
 return(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает положение точки относительно прямой x1,y1-x2,y2        //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int Diagnostik(float x,float y,float x1,float y1,float x2,float y2)
{
 float ESP=0.01;
 float p=x*(y1-y2)-y*(x1-x2)+x1*y2-x2*y1;
 if (p>ESP) return(1);
 if (p<-ESP) return(-1);
 return(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает номер сектора,в котором находится объект с координатами//
// x,y или -1.                                                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int FindSector(float x,float y)
{
 int n,m;
 for(n=0;n<sInfo.SectorNumber;n++)
  {
   if (x<sSector[n].XB1 || x>sSector[n].XB2) continue;
   if (y<sSector[n].YB1 || y>sSector[n].YB2) continue;
   int s=1;
   int po=0;
   for(m=0;m<sSector[n].Segment-1;m++)
    {
     float x1=sSector[n].X[m];
     float y1=sSector[n].Y[m];
     float x2=sSector[n].X[m+1];
     float y2=sSector[n].Y[m+1];
     int p=Diagnostik(x,y,x1,y1,x2,y2);
     if (p<0) p=-1;
     if (p>0) p=1;
     if (p==0) continue;
     if (po==0) po=p;
     if (p!=po)
      {
       s=0;
       break;
      }
    }
   if (s!=0) return(n);
  }
 return(-1);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает 0 если два вектора ax,ay-bx,by и cx,cy-dx,dy           //
//не пересекаются.Иначе-1.                                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int CollizeLine (float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy)
{
 float z=(bx-ax)*(dy-cy)-(by-ay)*(dx-cx);
 if (z==0) return(0);
 float r=((ay-cy)*(dx-cx)-(ax-cx)*(dy-cy))/z;
 if (r>1 || r<0) return(0);
 float s=((ay-cy)*(bx-ax)-(ax-cx)*(by-ay))/z;
 if (!(s>1 || s<0)) return(1);
 return(0);
}
//===========================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Удаляет BSP дерево.                                               //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void FreeTree(SBSPNode *root)
{
 if (root->Split==0) return;
  if (root->Left!=NULL)
   {
    FreeTree(root->Left);
    free(root->Left);
   }
  if (root->Right!=NULL)
   {
    FreeTree(root->Right);
    free(root->Right);
   }
}
//=============================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загружает число Int-типа.                                         //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int LoadInt(FILE *f)
{
 unsigned char a,b,c,d;
 fread(&a,1,1,f);
 fread(&b,1,1,f);
 fread(&c,1,1,f);
 fread(&d,1,1,f);
 int t=a+b*256+c*65536+(d&127)*16777216;
 if (d&128) t=-t;
 return(t);
}
void SaveInt(FILE *f,int t)
{
 unsigned int t1=abs(t);
 unsigned char a,b,c,d;
 d=(unsigned char)(t1/16777216);
 t1=t1-d*16777216;
 c=(unsigned char)(t1/65536);
 t1=t1-c*65536;
 b=(unsigned char)(t1/256);
 t1=t1-(int)b*256;
 a=(unsigned char)t1;
 if (t<0) d=d|128;
 fwrite(&a,1,1,f);
 fwrite(&b,1,1,f);
 fwrite(&c,1,1,f);
 fwrite(&d,1,1,f);
}
void LoadThing(char *FileName)
{
 in=fopen(FileName,"r+b");
 sInfo.ThingNumber=LoadInt(in);
 sInfo.TX=new int[sInfo.ThingNumber+1];
 sInfo.TY=new int[sInfo.ThingNumber+1];
 sInfo.TType=new int[sInfo.ThingNumber+1];
 for(int n=0;n<sInfo.ThingNumber;n++)
  {
   sInfo.TX[n]=LoadInt(in);
   sInfo.TY[n]=LoadInt(in);
   sInfo.TType[n]=LoadInt(in);
  }
 fclose(in);
}
void LoadAlien(char *FileName)
{
 in=fopen(FileName,"r+b");
 sInfo.AlienNumber=LoadInt(in);
 sInfo.AX=new int[sInfo.AlienNumber+1];
 sInfo.AY=new int[sInfo.AlienNumber+1];
 sInfo.AType=new int[sInfo.AlienNumber+1];
 for(int n=0;n<sInfo.AlienNumber;n++)
  {
   sInfo.AX[n]=LoadInt(in);
   sInfo.AY[n]=LoadInt(in);
   sInfo.AType[n]=LoadInt(in);
  }
 fclose(in);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загружает BSP дерево и карту лабиринта.                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
SBSPNode* LoadTree(FILE *in)
{
 SBSPNode *Tree;
 unsigned char a,b;
 int n,m,t;
 sInfo.SegmentNumber=LoadInt(in);
 sSegment=(SSegment *)malloc(sInfo.SegmentNumber*sizeof(SSegment));
 for(n=0;n<sInfo.SegmentNumber;n++)//загрузили линии
  {
   sSegment[n].X1=LoadInt(in)*0.01;
   sSegment[n].Y1=LoadInt(in)*0.01;
   sSegment[n].X2=LoadInt(in)*0.01;
   sSegment[n].Y2=LoadInt(in)*0.01;
   sSegment[n].Offset=LoadInt(in)/100;
   float x1=sSegment[n].X1;
   float y1=sSegment[n].Y1;
   float x2=sSegment[n].X2;
   float y2=sSegment[n].Y2;
   sSegment[n].Len=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
   sSegment[n].Type=LoadInt(in);
   sSegment[n].Frontier=LoadInt(in);
   sSegment[n].Sector1=LoadInt(in);
   sSegment[n].Sector2=LoadInt(in);
   sSegment[n].Visible=0;//игрок ещё не видел
  }
 sInfo.SectorNumber=LoadInt(in);
 sSector=(SSector *)malloc(sInfo.SectorNumber*sizeof(SSector));
 for(n=0;n<sInfo.SectorNumber;n++)//загружаем сектора
  {
   sSector[n].XB1=LoadInt(in)*0.01;
   sSector[n].YB1=LoadInt(in)*0.01;
   sSector[n].XB2=LoadInt(in)*0.01;
   sSector[n].YB2=LoadInt(in)*0.01;
   sSector[n].Segment=LoadInt(in);
   for(int t=0;t<sSector[n].Segment;t++)
    {
     sSector[n].X[t]=LoadInt(in)*0.01;
     sSector[n].Y[t]=LoadInt(in)*0.01;
    }
   sSector[n].Type=LoadInt(in);
   sSector[n].Up=LoadInt(in);
   sSector[n].UpTexture=LoadInt(in);
   sSector[n].Down=LoadInt(in);
   sSector[n].DownTexture=LoadInt(in);
   if (sSector[n].Type==2) sSector[n].Sector=LoadInt(in);
   if (sSector[n].Type==4 || sSector[n].Type==5 || sSector[n].Type==8) sSector[n].Down1=LoadInt(in);
   if (sSector[n].Type==1 || sSector[n].Type==3)
    {
     sSector[n].Down1=sSector[n].Down;
     sSector[n].Down=sSector[n].Up;
     sSector[n].Sector=0;
    }
   if (sSector[n].Type==4 || sSector[n].Type==5 || sSector[n].Type==8)
    {
     sSector[n].Down2=sSector[n].Down;
     sSector[n].Sector=0;
    }
   if (sSector[n].Type==7)
    {
     sSector[n].Sector=LoadInt(in)*16;
     sSector[n].Down1=LoadInt(in)*16;
    }
   sSector[n].DZ=0;
   sSector[n].Time=0;
  }
 sInfo.X=LoadInt(in);
 sInfo.Y=LoadInt(in);
 Tree=LoadBSPTree(in,sSegment);
 return(Tree);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загружает ветвь дерева.Sgm-список сегментов.                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
SBSPNode* LoadBSPTree(FILE *file,SSegment *Sgm)
{
 SBSPNode* Root=new SBSPNode;
 char a;
 int n;
 fread(&a,1,1,file);
 Root->Split=a;
 if (a==0)
  {
   n=LoadInt(file);
   Root->Number=n;
   Root->X1=Sgm[n].X1;
   Root->Y1=Sgm[n].Y1;
   Root->X2=Sgm[n].X2;
   Root->Y2=Sgm[n].Y2;
   Root->Offset=Sgm[n].Offset;
   Root->Type=n;
   Root->Frontier=Sgm[n].Frontier;
   Root->Sector1=Sgm[n].Sector1;
   Root->Sector2=Sgm[n].Sector2;
   return(Root);
  }
 //узел
 n=LoadInt(file);
 Root->Number=n;
 Root->X1=Sgm[n].X1;
 Root->Y1=Sgm[n].Y1;
 Root->X2=Sgm[n].X2;
 Root->Y2=Sgm[n].Y2;
 Root->Offset=Sgm[n].Offset;
 Root->Type=n;
 Root->Frontier=Sgm[n].Frontier;
 Root->Sector1=Sgm[n].Sector1;
 Root->Sector2=Sgm[n].Sector2;
 Root->LX1=LoadInt(file)*0.01;
 Root->LY1=LoadInt(file)*0.01;
 Root->LX2=LoadInt(file)*0.01;
 Root->LY2=LoadInt(file)*0.01;
 Root->RX1=LoadInt(file)*0.01;
 Root->RY1=LoadInt(file)*0.01;
 Root->RX2=LoadInt(file)*0.01;
 Root->RY2=LoadInt(file)*0.01;
 fread(&a,1,1,file);
 if (a==0) Root->Left=NULL;
  else Root->Left=LoadBSPTree(file,Sgm);
 fread(&a,1,1,file);
 if (a==0) Root->Right=NULL;
  else Root->Right=LoadBSPTree(file,Sgm);
 return(Root);
}
int FindPaletteIndex(int r,int g,int b)
{
 int index,d;
 int mindist=1024;
 for(register short i=1;i<256;i++)
  {
   d=abs(r-cVideo.Pal[i*3])+abs(g-cVideo.Pal[i*3+1])+abs(b-cVideo.Pal[i*3+2]);
   if (d<mindist)
    {
     mindist=d;
     index=i;
    }
  }
 return(index);
}
void MipMapping(FILE *f,int t)
{
 //пирамидальное фильтрование
 register int x,y,n;
 unsigned char a;
 a=t%256;
 fwrite(&a,1,1,f);
 a=(int)t/256;
 fwrite(&a,1,1,f);
 //Level2
 for(n=0;n<t;n++)
  {
   for(x=0;x<64;x++)
    for(y=0;y<64;y++)
     {
      int index1=sData.WallTexture1[n*16384+x*2+y*2*128];
      int index2=sData.WallTexture1[n*16384+x*2+y*2*128+1];
      int index3=sData.WallTexture1[n*16384+x*2+y*2*128+1+128];
      int index4=sData.WallTexture1[n*16384+x*2+y*2*128+128];
      int r=(cVideo.Pal[index1*3]+cVideo.Pal[index2*3]+cVideo.Pal[index3*3]+cVideo.Pal[index4*3])/4;
      int g=(cVideo.Pal[index1*3+1]+cVideo.Pal[index2*3+1]+cVideo.Pal[index3*3+1]+cVideo.Pal[index4*3+1])/4;
      int b=(cVideo.Pal[index1*3+2]+cVideo.Pal[index2*3+2]+cVideo.Pal[index3*3+2]+cVideo.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      sData.WallTexture2[n*4096+x+y*64]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
 //Level3
   for(x=0;x<32;x++)
    for(y=0;y<32;y++)
     {
      int index1=sData.WallTexture2[n*4096+x*2+y*2*64];
      int index2=sData.WallTexture2[n*4096+x*2+y*2*64+1];
      int index3=sData.WallTexture2[n*4096+x*2+y*2*64+1+64];
      int index4=sData.WallTexture2[n*4096+x*2+y*2*64+64];
      int r=(cVideo.Pal[index1*3]+cVideo.Pal[index2*3]+cVideo.Pal[index3*3]+cVideo.Pal[index4*3])/4;
      int g=(cVideo.Pal[index1*3+1]+cVideo.Pal[index2*3+1]+cVideo.Pal[index3*3+1]+cVideo.Pal[index4*3+1])/4;
      int b=(cVideo.Pal[index1*3+2]+cVideo.Pal[index2*3+2]+cVideo.Pal[index3*3+2]+cVideo.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      sData.WallTexture3[n*1024+x+y*32]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
 //Level4
   for(x=0;x<16;x++)
    for(y=0;y<16;y++)
     {
      int index1=sData.WallTexture3[n*1024+x*2+y*2*32];
      int index2=sData.WallTexture3[n*1024+x*2+y*2*32+1];
      int index3=sData.WallTexture3[n*1024+x*2+y*2*32+1+32];
      int index4=sData.WallTexture3[n*1024+x*2+y*2*32+32];
      int r=(cVideo.Pal[index1*3]+cVideo.Pal[index2*3]+cVideo.Pal[index3*3]+cVideo.Pal[index4*3])/4;
      int g=(cVideo.Pal[index1*3+1]+cVideo.Pal[index2*3+1]+cVideo.Pal[index3*3+1]+cVideo.Pal[index4*3+1])/4;
      int b=(cVideo.Pal[index1*3+2]+cVideo.Pal[index2*3+2]+cVideo.Pal[index3*3+2]+cVideo.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      sData.WallTexture4[n*256+x+y*16]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
 //Level5
   for(x=0;x<8;x++)
    for(y=0;y<8;y++)
     {
      int index1=sData.WallTexture4[n*256+x*2+y*2*16];
      int index2=sData.WallTexture4[n*256+x*2+y*2*16+1];
      int index3=sData.WallTexture4[n*256+x*2+y*2*16+1+16];
      int index4=sData.WallTexture4[n*256+x*2+y*2*16+16];
      int r=(cVideo.Pal[index1*3]+cVideo.Pal[index2*3]+cVideo.Pal[index3*3]+cVideo.Pal[index4*3])/4;
      int g=(cVideo.Pal[index1*3+1]+cVideo.Pal[index2*3+1]+cVideo.Pal[index3*3+1]+cVideo.Pal[index4*3+1])/4;
      int b=(cVideo.Pal[index1*3+2]+cVideo.Pal[index2*3+2]+cVideo.Pal[index3*3+2]+cVideo.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      sData.WallTexture5[n*64+x+y*8]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
 //Level6
   for(x=0;x<4;x++)
    for(y=0;y<4;y++)
     {
      int index1=sData.WallTexture5[n*64+x*2+y*2*8];
      int index2=sData.WallTexture5[n*64+x*2+y*2*8+1];
      int index3=sData.WallTexture5[n*64+x*2+y*2*8+1+8];
      int index4=sData.WallTexture5[n*64+x*2+y*2*8+8];
      int r=(cVideo.Pal[index1*3]+cVideo.Pal[index2*3]+cVideo.Pal[index3*3]+cVideo.Pal[index4*3])/4;
      int g=(cVideo.Pal[index1*3+1]+cVideo.Pal[index2*3+1]+cVideo.Pal[index3*3+1]+cVideo.Pal[index4*3+1])/4;
      int b=(cVideo.Pal[index1*3+2]+cVideo.Pal[index2*3+2]+cVideo.Pal[index3*3+2]+cVideo.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      sData.WallTexture6[n*16+x+y*4]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
  //Level7
   for(x=0;x<2;x++)
    for(y=0;y<2;y++)
     {
      int index1=sData.WallTexture6[n*16+x*2+y*2*4];
      int index2=sData.WallTexture6[n*16+x*2+y*2*4+1];
      int index3=sData.WallTexture6[n*16+x*2+y*2*4+1+4];
      int index4=sData.WallTexture6[n*16+x*2+y*2*4+4];
      int r=(cVideo.Pal[index1*3]+cVideo.Pal[index2*3]+cVideo.Pal[index3*3]+cVideo.Pal[index4*3])/4;
      int g=(cVideo.Pal[index1*3+1]+cVideo.Pal[index2*3+1]+cVideo.Pal[index3*3+1]+cVideo.Pal[index4*3+1])/4;
      int b=(cVideo.Pal[index1*3+2]+cVideo.Pal[index2*3+2]+cVideo.Pal[index3*3+2]+cVideo.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      sData.WallTexture7[n*4+x+y*2]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
  //Level8
   for(x=0;x<1;x++)
    for(y=0;y<1;y++)
     {
      int index1=sData.WallTexture7[n*4+x*2+y*2*2];
      int index2=sData.WallTexture7[n*4+x*2+y*2*2+1];
      int index3=sData.WallTexture7[n*4+x*2+y*2*2+1+2];
      int index4=sData.WallTexture7[n*4+x*2+y*2*2+2];
      int r=(cVideo.Pal[index1*3]+cVideo.Pal[index2*3]+cVideo.Pal[index3*3]+cVideo.Pal[index4*3])/4;
      int g=(cVideo.Pal[index1*3+1]+cVideo.Pal[index2*3+1]+cVideo.Pal[index3*3+1]+cVideo.Pal[index4*3+1])/4;
      int b=(cVideo.Pal[index1*3+2]+cVideo.Pal[index2*3+2]+cVideo.Pal[index3*3+2]+cVideo.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      sData.WallTexture8[n+x+y*2]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Возвращает готовый к загрузке блок                                //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
FILE *GoToBlock(int number)
{
 unsigned char a,b,c,d;
 int n;
 in=fopen("resurs\\level.dat","r+b");
 fread(&a,1,1,in);//длина файла
 fread(&b,1,1,in);
 fread(&c,1,1,in);
 fread(&d,1,1,in);
 fread(&a,1,1,in);//число блоков
 fread(&b,1,1,in);
 int qb=a+b*256;
 if (qb<=number) return(NULL);//нет такого блока
 for(n=0;n<number;n++)//считываем все смещения до нужного
  {
   fread(&a,1,1,in);
   fread(&b,1,1,in);
   fread(&c,1,1,in);
   fread(&d,1,1,in);
  }
 //считываем смещения уровня
 fread(&a,1,1,in);
 fread(&b,1,1,in);
 fread(&c,1,1,in);
 fread(&d,1,1,in);
 int offset=a+b*256+c*65536+d*16777216;
 //считываем все данные до нужных
 for(n=0;n<offset-number*4-6;n++) fread(&a,1,1,in);
 fread(&a,1,1,in);//длина блока
 fread(&b,1,1,in);
 fread(&c,1,1,in);
 fread(&d,1,1,in);
 //дальше идут данные блока
 return(in);
}
