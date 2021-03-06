#include "3dengine.h"
//===========================================================================
short TopLine[330];
short BottomLine[330];
short st;//�᫮ ���������� �⮫�殢 ��࠭�
//===========================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//�����頥� 1 �᫨ ����뢠�騩 ��אַ㣮�쭨� �����.����-0.        //
//tree-㪠��⥫� ��⢨ ��ॢ�,type-⨯ ��אַ㣮�쭨��(LEFT,RIGHT).  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
int SeenBox(SBSPNode* tree,int type)
{
 if (tree->Split!=1) return(1);
 float ss=SinTable[sPlayer.Angle];
 float cs=CosTable[sPlayer.Angle];
 float x1,y1,x2,y2;
 if (type==LEFT)//�஢�ਬ ��������� �����ॢ�
  {
   x1=tree->LX1-sPlayer.X;
   y1=tree->LY1-sPlayer.Y;
   x2=tree->LX2-sPlayer.X;
   y2=tree->LY2-sPlayer.Y;
  }
 if (type==RIGHT)
  {
   x1=tree->RX1-sPlayer.X;
   y1=tree->RY1-sPlayer.Y;
   x2=tree->RX2-sPlayer.X;
   y2=tree->RY2-sPlayer.Y;
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
 if (v1<=0 || v2<=0 || v3<=0 || v4<=0) return(1);//�� �஢����
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
//���ᮢ뢠�� ����ਭ� �� BSP ��ॢ�.                             //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
inline void DrawBSPTree(SBSPNode* tree)
{
 if (st>=320) return;
 int p;
 if (tree->Split==0)
  {
   DrawSubSector(tree);
   return;
  }
 p=Diagnostik(sPlayer.X,sPlayer.Y,tree->X1,tree->Y1,tree->X2,tree->Y2);
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
//���ᮢ��� ᥣ���� �⥭�.                                      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void DrawSubSector(SBSPNode* root)
{
 float x1=root->X1;
 float x2=root->X2;
 float y1=root->Y1;
 float y2=root->Y2;
 int p=Diagnostik(sPlayer.X,sPlayer.Y,x1,y1,x2,y2);
 float cs=CosTable[sPlayer.Angle];
 float ss=SinTable[sPlayer.Angle];
 float c1,c2;
 x1=root->X1-sPlayer.X;
 x2=root->X2-sPlayer.X;
 y1=root->Y1-sPlayer.Y;
 y2=root->Y2-sPlayer.Y;
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
   offset=128-(sSegment[root->Type].Len+offset);
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
 if (root->Frontier==0)//�� �� ����� ࠧ����
  {
   t=sqrt((v1-vo1)*(v1-vo1)+(u1-uo1)*(u1-uo1))+offset;
   z=vo1*0.01;
   float vz1=1/vo1;
   float vt1=t/vo1;
   float t1=84-(sSector[root->Sector1].Up-sPlayer.Z+sPlayer.Pz+sPlayer.SitZ)/z;
   float b1=84-(sSector[root->Sector1].Down-sPlayer.Z+sPlayer.Pz+sPlayer.SitZ)/z;
   float dc=c2-c1+1;
   t=sqrt((vo2-v1)*(vo2-v1)+(uo2-u1)*(uo2-u1))+offset;
   z=vo2*0.01;
   float vz2=1/vo2;
   float vt2=t/vo2;
   float t2=84-(sSector[root->Sector1].Up-sPlayer.Z+sPlayer.Pz+sPlayer.SitZ)/z;
   float b2=84-(sSector[root->Sector1].Down-sPlayer.Z+sPlayer.Pz+sPlayer.SitZ)/z;
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
        sSegment[root->Number].Visible=1;
       }
     z=1.0/vz1;
     if (z<1) continue;
     t=vt1/vz1;
     float ys=t1;
     float ys1=b1;
     DrawSimpleLine(z,c,ys,ys1,t,sSector[root->Sector1].Up-sSector[root->Sector1].Down,root->Type,sSegment[root->Type].Type,root->Sector1);
    }
  }
 else
  {
   int sd1,sd2,su1,su2;
   short sectors=0;
   if (p<0)//��ப �ࠢ�
    {
     sd1=sSector[root->Sector1].Down;
     sd2=sSector[root->Sector2].Down;
     su1=sSector[root->Sector2].Up;
     su2=sSector[root->Sector1].Up;
     sectors=root->Sector1;
    }
   else
    {
     sd1=sSector[root->Sector2].Down;
     sd2=sSector[root->Sector1].Down;
     su1=sSector[root->Sector1].Up;
     su2=sSector[root->Sector2].Up;
     sectors=root->Sector2;
    }
   if (sd1>=su1) su1=sd1;
   t=sqrt((v1-vo1)*(v1-vo1)+(u1-uo1)*(u1-uo1))+offset;
   z=vo1*0.01;
   float vz1=1/vo1;
   float vt1=t/vo1;
   float oz=-sPlayer.Z+sPlayer.Pz+sPlayer.SitZ;
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
        sSegment[root->Number].Visible=1;
       }
     z=1.0/vz1;
     if (z<1) continue;
     t=vt1/vz1;
     float ys=tu1;
     float ys1=bu1;
     DrawUpperLine(z,c,ys,ys1,t,hgtup,root->Type,sSegment[root->Type].Type,sectors);
     ys=td1;
     ys1=bd1;
     DrawLowerLine(z,c,ys,ys1,t,hgtdown,root->Type,sSegment[root->Type].Type,sectors);
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
 unsigned char *rtexture=texture*16384+sData.WallTexture1+offset;
 if (ky>=2 && ky<4)
  {
   dy=dy/2;
   yt=yt/2;
   r=63;
   offset=((int)(offsett&127)/2)<<6;
   rtexture=texture*4096+sData.WallTexture2+offset;
  }
 if (ky>=4 && ky<8)
  {
   dy=dy/4;
   yt=yt/4;
   r=31;
   offset=((int)(offsett&127)/4)<<5;
   rtexture=texture*1024+sData.WallTexture3+offset;
  }
 if (ky>=8 && ky<16)
  {
   dy=dy/8;
   yt=yt/8;
   r=15;
   offset=((int)(offsett&127)/8)<<4;
   rtexture=texture*256+sData.WallTexture4+offset;
  }
 if (ky>=16 && ky<32)
  {
   dy=dy/16;
   yt=yt/16;
   r=7;
   offset=((int)(offsett&127)/16)<<3;
   rtexture=texture*64+sData.WallTexture5+offset;
  }
 if (ky>=32 && ky<64)
  {
   dy=dy/32;
   yt=yt/32;
   r=3;
   offset=((int)(offsett&127)/32)<<2;
   rtexture=texture*16+sData.WallTexture6+offset;
  }
 if (ky>=64 && ky<128)
  {
   dy=dy/64;
   yt=yt/64;
   r=1;
   offset=((int)(offsett&127)/64)<<1;
   rtexture=texture*4+sData.WallTexture7+offset;
  }
 if (ky>=128)
  {
   dy=dy/128;
   yt=yt/128;
   r=1;
   rtexture=texture+sData.WallTexture8;
  }
 *dy_=dy;
 *yt_=yt;
 *mask=r;
 return(rtexture);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//����� �⮫��� ������⭮� �⥭�.                                  //
//z-���न��� z �⥭�,xs-�⮫��� ��࠭�,Top-������,Bottom-������  //
//���न��� �஥�権 �⥭� �� ��࠭�,u-ᬥ饭�� ⥪�����,height-   //
//���� �⥭�,o-����� ᥪ��,� ���஬� �ਭ������� �⥭�         //
//texture-㪠��⥫� �� ����� ⥪����� �⥭�.                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void DrawSimpleLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short sector)
{
 //������塞 ����� ����� ��㡨��
 register int pt=sZLines[xs].Pointer;
 sZLines[xs].Begin_Y[pt]=TopLine[xs];
 sZLines[xs].End_Y[pt]=BottomLine[xs];
 sZLines[xs].Z_Value[pt]=z;
 sZLines[xs].Pointer=pt+1;
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
   DrawFloorLine(xs,TopLine[xs],Top-1,sector);
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
   DrawFloorLine(xs,Bottom,BottomLine[xs]-1,sector);
   BottomLine[xs]=Bottom;
  }
 else Bottom=BottomLine[xs];
 TopLine[xs]=Bottom;
 BottomLine[xs]=Top;
 vpt=cVideo.Buffer+xs+((int)Top)*320;
 int ky=dy>>20;//�����樥�� ᦠ��
 int mask;
 unsigned char *texturet=FindTexture(ky,u,texture,&mask,&dy,&yt);
 st++;
 yt-=dy*(Top-(int)Top);
 for(n=Top;n<=Bottom;n++,vpt+=320,yt+=dy) *(vpt)=*(texturet+((yt>>20)&mask));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//����� �⮫��� ������ �⥭�.                                      //
//z-���न��� z �⥭�,xs-�⮫��� ��࠭�,Top-������,Bottom-������  //
//���न��� �஥�権 �⥭� �� ��࠭�,u-ᬥ饭�� ⥪�����,height-   //
//���� �⥭�,o-����� ᥪ��,� ���஬� �ਭ������� �⥭�         //
//texture-㪠��⥫� �� ����� ⥪����� �⥭�.                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
inline void DrawLowerLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short sector)
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
 if (Bottom<BottomLine[xs]) DrawFloorLine(xs,Bottom,BottomLine[xs]-1,sector);
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
   vpt=cVideo.Buffer+xs+((int)Top)*320;
   int pt=sZLines[xs].Pointer;
   sZLines[xs].Begin_Y[pt]=Top;
   sZLines[xs].End_Y[pt]=BottomLine[xs];
   sZLines[xs].Z_Value[pt]=z;
   sZLines[xs].Pointer=pt+1;
   int ky=dy>>20;//�����樥�� ᦠ��
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
     int pt=sZLines[xs].Pointer;
     sZLines[xs].Begin_Y[pt]=Bottom;
     sZLines[xs].End_Y[pt]=BottomLine[xs];
     sZLines[xs].Z_Value[pt]=z;
     sZLines[xs].Pointer=pt+1;
     BottomLine[xs]=Bottom;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//����� �⮫��� ���孥� �⥭�.                                     //
//z-���न��� z �⥭�,xs-�⮫��� ��࠭�,Top-������,Bottom-������  //
//���न��� �஥�権 �⥭� �� ��࠭�,u-ᬥ饭�� ⥪�����,height-   //
//���� �⥭�,o-����� ᥪ��,� ���஬� �ਭ������� �⥭�         //
//texture-㪠��⥫� �� ����� ⥪����� �⥭�.                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
inline void DrawUpperLine(int z,short xs,float Top,float Bottom,short u,int height,int o,int texture,short sector)
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
 if (Top>TopLine[xs]) DrawFloorLine(xs,TopLine[xs],Top-1,sector);
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
   vpt=cVideo.Buffer+xs+((int)Top)*320;
   int pt=sZLines[xs].Pointer;
   sZLines[xs].Begin_Y[pt]=TopLine[xs];
   sZLines[xs].End_Y[pt]=Bottom;
   sZLines[xs].Z_Value[pt]=z;
   sZLines[xs].Pointer=pt+1;
   int ky=dy>>20;//�����樥�� ᦠ��
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
     int pt=sZLines[xs].Pointer;
     sZLines[xs].Begin_Y[pt]=TopLine[xs];
     sZLines[xs].End_Y[pt]=Top;
     sZLines[xs].Z_Value[pt]=z;
     sZLines[xs].Pointer=pt+1;
     TopLine[xs]=Top;
    }
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//����� ���� � ��⮫�� ᥪ�஢                                    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void DrawFloor_Flow(void)
{
 register int row,col;
 int pt,n;
 int px=sPlayer.P_X;
 int py=sPlayer.P_Y;
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
 register unsigned char *vptup=cVideo.Buffer+26560;
 register unsigned char *vptdown=cVideo.Buffer+26880;
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
       texturefloor=(sSector[floor].DownTexture<<14)+sData.WallTexture1;
       Z_SectorsFloor=(-sSector[floor].Down+sPlayer.Z-sPlayer.Pz-sPlayer.SitZ)*100;
       dist=Z_SectorsFloor/(row+1);
       didown=dist*sMaze.diTable[sPlayer.Angle];
       djdown=dist*sMaze.djTable[sPlayer.Angle];
       i0down=dist*sMaze.dsTable[sPlayer.Angle]+px-didown*(159-col);
       j0down=dist*sMaze.dcTable[sPlayer.Angle]+py-djdown*(159-col);
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
       textureflow=(sSector[fup].UpTexture<<14)+sData.WallTexture1;
       Z_SectorsFlow=(sSector[fup].Up-sPlayer.Z+sPlayer.Pz+sPlayer.SitZ)*100;
       dist=Z_SectorsFlow/(row+1);
       diup=dist*sMaze.diTable[sPlayer.Angle];
       djup=dist*sMaze.djTable[sPlayer.Angle];
       i0up=dist*sMaze.dsTable[sPlayer.Angle]+px-diup*(159-col);
       j0up=dist*sMaze.dcTable[sPlayer.Angle]+py-djup*(159-col);
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
//���ᯥ稢��� �����⮢�� � ���ᮢ�� � ���ᮢ�� ����ਭ�.      //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void View(void)
{
 st=0;
 register int n,m;
 ClearFloorBuffer();
 for(n=0;n<320;n++)
  {
   //���樠����஢��� ����� ��ਧ���
   TopLine[n]=0;
   BottomLine[n]=167;
   //���樠����஢��� ����� ����� ��㡨��
   sZLines[n].Pointer=0;//� ᯨ᪥ ��� �����
  }
 DrawBSPTree(Node);//��㥬 ᥣ�����
 DrawFloor_Flow();//��㥬 ���� � ��⮫��
 int *necess=new int[1+NowMissile+NowThing+NowAlien];//����㥬 �।����,�ࠣ�� � ᭠���
 int *znecess=new int[1+NowMissile+NowThing+NowAlien];
 float cs=CosTable[sPlayer.Angle];
 float ss=SinTable[sPlayer.Angle];
 int x1,y1;
 int no=0;
 //������塞 � ⠡���� ���஢�� ᭠���
 for(n=0;n<NowMissile;n++)
  {
   x1=cMissile[n]->X-sPlayer.X;
   y1=cMissile[n]->Y-sPlayer.Y;
   int z=x1*ss+y1*cs;
   if (!(z<7 || z>6400))
    {
     int x=x1*cs-y1*ss;
     int col1=160+160*(x-cMissile[n]->Radius)/z;
     int col2=160+160*(x+cMissile[n]->Radius)/z;
     if (!(col1<0 && col2<0 || col1>319 && col2>319))
      {
       necess[no]=n;
       znecess[no]=z;
       no++;
      }
    }
  }
 //������塞 �।����
 for(n=0;n<NowThing;n++)
  {
   x1=cThing[n]->Coord.X-sPlayer.X;
   y1=cThing[n]->Coord.Y-sPlayer.Y;
   int z=x1*ss+y1*cs;
   if (!(z<7 || z>6400))
    {
     int x=x1*cs-y1*ss;
     int col1=160+160*(x-cThing[n]->Coord.Radius)/z;
     int col2=160+160*(x+cThing[n]->Coord.Radius)/z;
     if (!(col1<0 && col2<0 || col1>319 && col2>319))
      {
       necess[no]=n+NowMissile;
       znecess[no]=z;
       no++;
      }
    }
  }
 //������塞 �ࠣ��
 for(n=0;n<NowAlien;n++)
  {
   x1=cAlien[n]->Coord.X-sPlayer.X;
   y1=cAlien[n]->Coord.Y-sPlayer.Y;
   int z=x1*ss+y1*cs;
   if (!(z<7 || z>6400))
    {
     int x=x1*cs-y1*ss;
     int col1=160+160*(x-cAlien[n]->Coord.Radius)/z;
     int col2=160+160*(x+cAlien[n]->Coord.Radius)/z;
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
   if (necess[n]<NowMissile && necess[n]>=0) cMissile[necess[n]]->Put();//�뢮��� ᭠���
   if (necess[n]>=NowMissile && necess[n]<NowThing+NowMissile) cThing[necess[n]-NowMissile]->Put();//�뢮��� �।����
   if (necess[n]>=NowThing+NowMissile) cAlien[necess[n]-NowMissile-NowThing]->Put();//�뢮��� �ࠣ��
  }
 delete(necess);
 delete(znecess);
}
