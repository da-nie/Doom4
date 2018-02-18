//============================================================================
int LoadGame(int level);
unsigned char* LoadSpriteFile(char *FileName);
void LoadResurs(void);
int Diagnostik(float x,float y,float x1,float y1,float x2,float y2);
int FindSector(float x,float y);
int CollizeLine (float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy);
int FindPaletteIndex(int r,int g,int b);
void MipMapping(FILE *f,int t);
FILE *GoToBlock(int number);
//============================================================================
struct BSPNode
{
 BSPNode* Left;//левое поддерево
 BSPNode* Right;//правое поддерево
 int X1;
 int Y1;
 int X2;
 int Y2;
 int Offset;//смещение текстуры
 unsigned char Split;//что за линия (1-линия раздела,2-не рисуемая линия раздела,3-не рисуемая линия раздела с сектором,0-субсектор)
 unsigned char Frontier;//1-линия раздела секторов 0-нет
 int Sector1;//какой сектор (если Split=2);
 int Sector2;//если это линия раздела то тут два сектора от неё
 int Type;//текстура для линии раздела и субсектора
 int LX1,LX2,LY1,LY2;//левый описывающий прямоугольник
 int RX1,RX2,RY1,RY2;//правый описывающий прямоугольник
 int Number;
};
struct info
{
 int SectorNumber;
 int SegmentNumber;
 int ThingNumber;
 int AlienNumber;
 int X;
 int Y;
 int *TX;
 int *TY;
 int *TType;
 int *AX;
 int *AY;
 int *AType;
} Info;
struct segment
{
 int X1;
 int Y1;
 int X2;
 int Y2;
 int Offset;//смещение текстуры
 int Type;//текстура
 int Frontier;//линия раздела=1
 int Sector1;//сектора для линии раздела
 int Sector2;
 int Len;
 char Visible;//видел ли его игрок(нужно для карты)
} *Segment;
struct sector
{
 int XB1;
 int YB1;
 int XB2;
 int YB2;
 int Segment;
 int X[20];
 int Y[20];
 int Type;//тип сектора
 int Sector;//номер сектора для переключателя
 int Down1;//конечная высота пола для платформы и для двери
 int Down2;//старое значение высоты для платформы
 int UpTexture;
 int DownTexture;
 int Up;
 int Down;
 int DZ;
 int Time;
} *Sector;
BSPNode *Node;
void FreeTree(BSPNode *root);
int LoadInt(FILE *f);
void SaveInt(FILE *f,int t);
BSPNode* LoadTree(FILE *in);
BSPNode* LoadBSPTree(FILE *file,segment *Sgm);
//============================================================================
FILE *in;
//============================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загрузка спрайт файла.                                            //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
unsigned char* LoadSpriteFile(char *FileName)
{
 in =fopen(FileName,"r+b");
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
 Data.Font=LoadSpriteFile("resurs\\font.spt");
//============================================================================
 Video.Clear();
 Video.Print(0,0,"LOADING TEXTURE MAP");
 Video.View();
 Data.Thing=LoadSpriteFile("resurs\\tiles\\thing.spt");
 Data.Alien=LoadSpriteFile("resurs\\tiles\\alien.spt");
 Data.Fire=LoadSpriteFile("resurs\\tiles\\fire.spt");
 Data.Menu=LoadSpriteFile("resurs\\tiles\\menu.spt");
 in =fopen("resurs\\data.pak","r+b");
 if (in!=NULL)
  {
   fread(&a,1,1,in);
   t=(int)a;
   s=((int)(a+1))*16384;
   Data.WallTexture1=(unsigned char*)malloc(s);
   Data.WallTexture2=(unsigned char*)malloc(s/2);
   Data.WallTexture3=(unsigned char*)malloc(s/4);
   Data.WallTexture4=(unsigned char*)malloc(s/8);
   Data.WallTexture5=(unsigned char*)malloc(s/16);
   Data.WallTexture6=(unsigned char*)malloc(s/32);
   Data.WallTexture7=(unsigned char*)malloc(s/64);
   Data.WallTexture8=(unsigned char*)malloc(s/128);
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
       *(Data.WallTexture1+n*16384+m)=a;
      }
    }
    fclose(in);
   }
 Data.Bar=LoadSpriteFile("resurs\\tiles\\bar.spt");
 Data.Weapon0=LoadSpriteFile("resurs\\tiles\\weapon0.spt");
 Data.Weapon1=LoadSpriteFile("resurs\\tiles\\weapon1.spt");
 Data.Weapon2=LoadSpriteFile("resurs\\tiles\\weapon2.spt");
 Data.Weapon3=LoadSpriteFile("resurs\\tiles\\weapon3.spt");
 Data.Weapon4=LoadSpriteFile("resurs\\tiles\\weapon4.spt");
 Video.Print(0,0,"LOADING TEXTURE MAP -OK.");
 in=fopen("resurs\\mipmap.crt","r+b");
 if (in==NULL)//нет такого файла
  {
   Video.Print(0,10,"CREATE MIPMAPPING MAP");
   Video.View();
   in=fopen("resurs\\mipmap.crt","w+b");
   MipMapping(in,t);
   fclose(in);
   Video.Print(0,10,"CREATE MIPMAPPING MAP -OK.");
   Video.View();
  }
 else
  {
   unsigned char a,b;
   fread(&a,1,1,in);
   fread(&b,1,1,in);
   int t1=(int)a+((int)b)*256;
   if (t1!=t) //это другой файл
    {
     Video.Print(0,10,"CREATE MIPMAPPING MAP");
     Video.View();
     in=fopen("resurs\\mipmap.crt","w+b");
     MipMapping(in,t);
     fclose(in);
     Video.Print(0,10,"CREATE MIPMAPPING MAP -OK.");
     Video.View();
     return;
    }
   else//загружаем пирамиду
    {
     Video.Print(0,10,"LOADING MIPMAPPING MAP");
     Video.View();
     int x,y;
     for(n=0;n<t;n++)
      {
       for(x=0;x<64;x++)
        for(y=0;y<64;y++)
         {
          fread(&a,1,1,in);
          Data.WallTexture2[n*4096+x+y*64]=a;
         }
       for(x=0;x<32;x++)
        for(y=0;y<32;y++)
        {
         fread(&a,1,1,in);
         Data.WallTexture3[n*1024+x+y*32]=a;
        }
       for(x=0;x<16;x++)
        for(y=0;y<16;y++)
         {
          fread(&a,1,1,in);
          Data.WallTexture4[n*256+x+y*16]=a;
         }
       for(x=0;x<8;x++)
        for(y=0;y<8;y++)
         {
          fread(&a,1,1,in);
          Data.WallTexture5[n*64+x+y*8]=a;
         }
       for(x=0;x<4;x++)
        for(y=0;y<4;y++)
         {
          fread(&a,1,1,in);
          Data.WallTexture6[n*16+x+y*4]=a;
         }
       for(x=0;x<2;x++)
        for(y=0;y<2;y++)
         {
          fread(&a,1,1,in);
          Data.WallTexture7[n*4+x+y*2]=a;
         }
        fread(&a,1,1,in);
        Data.WallTexture8[n]=a;
      }
     fclose(in);
     Video.Print(0,10,"LOADING MIPMAPPING MAP -OK");
     Video.View();
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
 Info.ThingNumber=LoadInt(in);
 Info.TX=new int[Info.ThingNumber+1];
 Info.TY=new int[Info.ThingNumber+1];
 Info.TType=new int[Info.ThingNumber+1];
 for(n=0;n<Info.ThingNumber;n++)
  {
   Info.TX[n]=LoadInt(in);
   Info.TY[n]=LoadInt(in);
   Info.TType[n]=LoadInt(in);
  }
 fread(&a,1,1,in);//длина блока
 fread(&b,1,1,in);
 fread(&c,1,1,in);
 fread(&d,1,1,in);
 //читаем врагов
 Info.AlienNumber=LoadInt(in);
 Info.AX=new int[Info.AlienNumber+1];
 Info.AY=new int[Info.AlienNumber+1];
 Info.AType=new int[Info.AlienNumber+1];
 for(n=0;n<Info.AlienNumber;n++)
  {
   Info.AX[n]=LoadInt(in);
   Info.AY[n]=LoadInt(in);
   Info.AType[n]=LoadInt(in);
  }
 fclose(in);
 for (n=0;n<=360;n++)
  {
   CosTable[n]=cos(pi/180*n);
   SinTable[n]=sin(pi/180*n);
  }
  for(n=0;n<=360;n++)
   {
    Maze.dcTable[n]=cos(pi/180*n)*65536;
    Maze.dsTable[n]=sin(pi/180*n)*65536;
    Maze.diTable[n]=Maze.dcTable[n]/160;
    Maze.djTable[n]=-Maze.dsTable[n]/160;
   }
 short *sgm=(short *)malloc(Info.SegmentNumber*sizeof(short));
 for(int x=0;x<100;x++)//заполняем структуру BlockMap
  for(int y=0;y<100;y++)
   {
    int u=0;
    int X_B1=x*128-1;
    int X_B2=x*128+129;
    int Y_B1=y*128-1;
    int Y_B2=y*128+129;
    for(n=0;n<Info.SegmentNumber;n++)
    {
     int x1=Segment[n].X1;
     int x2=Segment[n].X2;
     int y1=Segment[n].Y1;
     int y2=Segment[n].Y2;
     if (x1<X_B1 && x2<X_B1) continue;
     if (x1>X_B2 && x2>X_B2) continue;
     if (y1<Y_B1 && y2<Y_B1) continue;
     if (y1>Y_B2 && y2>Y_B2) continue;
     sgm[u]=n;
     u++;
    }
    Maze.BlockMap[y][x]=(short *)malloc((u+1)*sizeof(short));
    for(n=0;n<u;n++)
    {
     Maze.BlockMap[y][x][n+1]=sgm[n];
    }
    Maze.BlockMap[y][x][0]=u;
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
 for(n=0;n<Info.SectorNumber;n++)
  {
   if (x<Sector[n].XB1 || x>Sector[n].XB2) continue;
   if (y<Sector[n].YB1 || y>Sector[n].YB2) continue;
   int s=1;
   int po=0;
   for(m=0;m<Sector[n].Segment-1;m++)
    {
     float x1=Sector[n].X[m];
     float y1=Sector[n].Y[m];
     float x2=Sector[n].X[m+1];
     float y2=Sector[n].Y[m+1];
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
void FreeTree(BSPNode *root)
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
 Info.ThingNumber=LoadInt(in);
 Info.TX=new int[Info.ThingNumber+1];
 Info.TY=new int[Info.ThingNumber+1];
 Info.TType=new int[Info.ThingNumber+1];
 for(int n=0;n<Info.ThingNumber;n++)
  {
   Info.TX[n]=LoadInt(in);
   Info.TY[n]=LoadInt(in);
   Info.TType[n]=LoadInt(in);
  }
 fclose(in);
}
void LoadAlien(char *FileName)
{
 in=fopen(FileName,"r+b");
 Info.AlienNumber=LoadInt(in);
 Info.AX=new int[Info.AlienNumber+1];
 Info.AY=new int[Info.AlienNumber+1];
 Info.AType=new int[Info.AlienNumber+1];
 for(int n=0;n<Info.AlienNumber;n++)
  {
   Info.AX[n]=LoadInt(in);
   Info.AY[n]=LoadInt(in);
   Info.AType[n]=LoadInt(in);
  }
 fclose(in);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загружает BSP дерево и карту лабиринта.                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
BSPNode* LoadTree(FILE *in)
{
 BSPNode *Tree;
 unsigned char a,b;
 int n,m,t;
 Info.SegmentNumber=LoadInt(in);
 Segment=(segment *)malloc(Info.SegmentNumber*sizeof(segment));
 for(n=0;n<Info.SegmentNumber;n++)//загрузили линии
  {
   Segment[n].X1=LoadInt(in)*0.01;
   Segment[n].Y1=LoadInt(in)*0.01;
   Segment[n].X2=LoadInt(in)*0.01;
   Segment[n].Y2=LoadInt(in)*0.01;
   Segment[n].Offset=LoadInt(in)/100;
   float x1=Segment[n].X1;
   float y1=Segment[n].Y1;
   float x2=Segment[n].X2;
   float y2=Segment[n].Y2;
   Segment[n].Len=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
   Segment[n].Type=LoadInt(in);
   Segment[n].Frontier=LoadInt(in);
   Segment[n].Sector1=LoadInt(in);
   Segment[n].Sector2=LoadInt(in);
   Segment[n].Visible=0;//игрок ещё не видел
  }
 Info.SectorNumber=LoadInt(in);
 Sector=(sector *)malloc(Info.SectorNumber*sizeof(sector));
 for(n=0;n<Info.SectorNumber;n++)//загружаем сектора
  {
   Sector[n].XB1=LoadInt(in)*0.01;
   Sector[n].YB1=LoadInt(in)*0.01;
   Sector[n].XB2=LoadInt(in)*0.01;
   Sector[n].YB2=LoadInt(in)*0.01;
   Sector[n].Segment=LoadInt(in);
   for(int t=0;t<Sector[n].Segment;t++)
    {
     Sector[n].X[t]=LoadInt(in)*0.01;
     Sector[n].Y[t]=LoadInt(in)*0.01;
    }
   Sector[n].Type=LoadInt(in);
   Sector[n].Up=LoadInt(in);
   Sector[n].UpTexture=LoadInt(in);
   Sector[n].Down=LoadInt(in);
   Sector[n].DownTexture=LoadInt(in);
   if (Sector[n].Type==2) Sector[n].Sector=LoadInt(in);
   if (Sector[n].Type==4 || Sector[n].Type==5 || Sector[n].Type==8) Sector[n].Down1=LoadInt(in);
   if (Sector[n].Type==1 || Sector[n].Type==3)
    {
     Sector[n].Down1=Sector[n].Down;
     Sector[n].Down=Sector[n].Up;
     Sector[n].Sector=0;
    }
   if (Sector[n].Type==4 || Sector[n].Type==5 || Sector[n].Type==8)
    {
     Sector[n].Down2=Sector[n].Down;
     Sector[n].Sector=0;
    }
   if (Sector[n].Type==7)
    {
     Sector[n].Sector=LoadInt(in)*16;
     Sector[n].Down1=LoadInt(in)*16;
    }
   Sector[n].DZ=0;
   Sector[n].Time=0;
  }
 Info.X=LoadInt(in);
 Info.Y=LoadInt(in);
 Tree=LoadBSPTree(in,Segment);
 return(Tree);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загружает ветвь дерева.Sgm-список сегментов.                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
BSPNode* LoadBSPTree(FILE *file,segment *Sgm)
{
 BSPNode* Root=new BSPNode;
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
   d=abs(r-Video.Pal[i*3])+abs(g-Video.Pal[i*3+1])+abs(b-Video.Pal[i*3+2]);
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
      int index1=Data.WallTexture1[n*16384+x*2+y*2*128];
      int index2=Data.WallTexture1[n*16384+x*2+y*2*128+1];
      int index3=Data.WallTexture1[n*16384+x*2+y*2*128+1+128];
      int index4=Data.WallTexture1[n*16384+x*2+y*2*128+128];
      int r=(Video.Pal[index1*3]+Video.Pal[index2*3]+Video.Pal[index3*3]+Video.Pal[index4*3])/4;
      int g=(Video.Pal[index1*3+1]+Video.Pal[index2*3+1]+Video.Pal[index3*3+1]+Video.Pal[index4*3+1])/4;
      int b=(Video.Pal[index1*3+2]+Video.Pal[index2*3+2]+Video.Pal[index3*3+2]+Video.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      Data.WallTexture2[n*4096+x+y*64]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
 //Level3
   for(x=0;x<32;x++)
    for(y=0;y<32;y++)
     {
      int index1=Data.WallTexture2[n*4096+x*2+y*2*64];
      int index2=Data.WallTexture2[n*4096+x*2+y*2*64+1];
      int index3=Data.WallTexture2[n*4096+x*2+y*2*64+1+64];
      int index4=Data.WallTexture2[n*4096+x*2+y*2*64+64];
      int r=(Video.Pal[index1*3]+Video.Pal[index2*3]+Video.Pal[index3*3]+Video.Pal[index4*3])/4;
      int g=(Video.Pal[index1*3+1]+Video.Pal[index2*3+1]+Video.Pal[index3*3+1]+Video.Pal[index4*3+1])/4;
      int b=(Video.Pal[index1*3+2]+Video.Pal[index2*3+2]+Video.Pal[index3*3+2]+Video.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      Data.WallTexture3[n*1024+x+y*32]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
 //Level4
   for(x=0;x<16;x++)
    for(y=0;y<16;y++)
     {
      int index1=Data.WallTexture3[n*1024+x*2+y*2*32];
      int index2=Data.WallTexture3[n*1024+x*2+y*2*32+1];
      int index3=Data.WallTexture3[n*1024+x*2+y*2*32+1+32];
      int index4=Data.WallTexture3[n*1024+x*2+y*2*32+32];
      int r=(Video.Pal[index1*3]+Video.Pal[index2*3]+Video.Pal[index3*3]+Video.Pal[index4*3])/4;
      int g=(Video.Pal[index1*3+1]+Video.Pal[index2*3+1]+Video.Pal[index3*3+1]+Video.Pal[index4*3+1])/4;
      int b=(Video.Pal[index1*3+2]+Video.Pal[index2*3+2]+Video.Pal[index3*3+2]+Video.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      Data.WallTexture4[n*256+x+y*16]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
 //Level5
   for(x=0;x<8;x++)
    for(y=0;y<8;y++)
     {
      int index1=Data.WallTexture4[n*256+x*2+y*2*16];
      int index2=Data.WallTexture4[n*256+x*2+y*2*16+1];
      int index3=Data.WallTexture4[n*256+x*2+y*2*16+1+16];
      int index4=Data.WallTexture4[n*256+x*2+y*2*16+16];
      int r=(Video.Pal[index1*3]+Video.Pal[index2*3]+Video.Pal[index3*3]+Video.Pal[index4*3])/4;
      int g=(Video.Pal[index1*3+1]+Video.Pal[index2*3+1]+Video.Pal[index3*3+1]+Video.Pal[index4*3+1])/4;
      int b=(Video.Pal[index1*3+2]+Video.Pal[index2*3+2]+Video.Pal[index3*3+2]+Video.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      Data.WallTexture5[n*64+x+y*8]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
 //Level6
   for(x=0;x<4;x++)
    for(y=0;y<4;y++)
     {
      int index1=Data.WallTexture5[n*64+x*2+y*2*8];
      int index2=Data.WallTexture5[n*64+x*2+y*2*8+1];
      int index3=Data.WallTexture5[n*64+x*2+y*2*8+1+8];
      int index4=Data.WallTexture5[n*64+x*2+y*2*8+8];
      int r=(Video.Pal[index1*3]+Video.Pal[index2*3]+Video.Pal[index3*3]+Video.Pal[index4*3])/4;
      int g=(Video.Pal[index1*3+1]+Video.Pal[index2*3+1]+Video.Pal[index3*3+1]+Video.Pal[index4*3+1])/4;
      int b=(Video.Pal[index1*3+2]+Video.Pal[index2*3+2]+Video.Pal[index3*3+2]+Video.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      Data.WallTexture6[n*16+x+y*4]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
  //Level7
   for(x=0;x<2;x++)
    for(y=0;y<2;y++)
     {
      int index1=Data.WallTexture6[n*16+x*2+y*2*4];
      int index2=Data.WallTexture6[n*16+x*2+y*2*4+1];
      int index3=Data.WallTexture6[n*16+x*2+y*2*4+1+4];
      int index4=Data.WallTexture6[n*16+x*2+y*2*4+4];
      int r=(Video.Pal[index1*3]+Video.Pal[index2*3]+Video.Pal[index3*3]+Video.Pal[index4*3])/4;
      int g=(Video.Pal[index1*3+1]+Video.Pal[index2*3+1]+Video.Pal[index3*3+1]+Video.Pal[index4*3+1])/4;
      int b=(Video.Pal[index1*3+2]+Video.Pal[index2*3+2]+Video.Pal[index3*3+2]+Video.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      Data.WallTexture7[n*4+x+y*2]=index;
      a=index;
      fwrite(&a,1,1,f);
     }
  //Level8
   for(x=0;x<1;x++)
    for(y=0;y<1;y++)
     {
      int index1=Data.WallTexture7[n*4+x*2+y*2*2];
      int index2=Data.WallTexture7[n*4+x*2+y*2*2+1];
      int index3=Data.WallTexture7[n*4+x*2+y*2*2+1+2];
      int index4=Data.WallTexture7[n*4+x*2+y*2*2+2];
      int r=(Video.Pal[index1*3]+Video.Pal[index2*3]+Video.Pal[index3*3]+Video.Pal[index4*3])/4;
      int g=(Video.Pal[index1*3+1]+Video.Pal[index2*3+1]+Video.Pal[index3*3+1]+Video.Pal[index4*3+1])/4;
      int b=(Video.Pal[index1*3+2]+Video.Pal[index2*3+2]+Video.Pal[index3*3+2]+Video.Pal[index4*3+2])/4;
      int index=FindPaletteIndex(r,g,b);
      Data.WallTexture8[n+x+y*2]=index;
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
