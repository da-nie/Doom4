void PutStaticSprite(int x,int y,unsigned char *wdp,int number);
void PutBar(unsigned char *wdp,int number,int x,int y);
void PutSprite(int x,int y,unsigned char *wdp,int number,int type);
//===========================================================================
void PutBar(unsigned char *wdp,int number,int x,int y)
{
 int ofs1=2+number*4;
 int offset=wdp[ofs1]+wdp[ofs1+1]*256+wdp[ofs1+2]*65536+wdp[ofs1+3]*16777216;
 wdp+=offset;
 int len=wdp[0]+256*wdp[1];
 int hgt=wdp[2]+256*wdp[3];
 offset=3;
 for(register int n=0;n<len;n++)
  for(register int m=0;m<hgt;m++)
   {
    offset++;
    if (m+y>199) continue;
    *(Video.Buffer+(m+y)*320+n+x)=wdp[offset];
   }
}
void PutStaticSprite(int x,int y,unsigned char *wdp,int number)
{
 int ofs1=2+number*4;
 int offset=wdp[ofs1]+wdp[ofs1+1]*256+wdp[ofs1+2]*65536+wdp[ofs1+3]*16777216;
 wdp+=offset;
 int len=wdp[0]+256*wdp[1];
 int hgt=wdp[2]+256*wdp[3];
 offset=3;
 for(int n=0;n<len;n++)
  for(int m=0;m<hgt;m++)
   {
    offset++;
    if (m+y>167) continue;
    if (wdp[offset]==117) continue;
    *(Video.Buffer+(m+y)*320+n+x)=wdp[offset];
   }
}
void PutSprite(int x,int y,unsigned char *wdp,int number,int type)
{
 char *vpt=(char *)0xa0000;
 int ofs1=2+number*4;
 int offset=wdp[ofs1]+wdp[ofs1+1]*256+wdp[ofs1+2]*65536+wdp[ofs1+3]*16777216;
 wdp+=offset;
 int len=wdp[0]+256*wdp[1];
 int hgt=wdp[2]+256*wdp[3];
 offset=3;
 if (type==2) vpt=Video.Buffer;
 for(int n=0;n<len;n++)
  for(int m=0;m<hgt;m++)
   {
    offset++;
    if (wdp[offset]==117 && type==1 ) continue;
    *(vpt+(m+y)*320+n+x)=wdp[offset];
   }
}
