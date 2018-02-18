//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Класс обработки экрана.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class Screen
{
 public:
  unsigned char *Buffer;
  unsigned char Pal[800];
  void InitVga(void);
  void CloseVga(void);
  void ClearWindow(void);
  void Clear(void);
  void ViewWindow(void);
  void ViewBar(void);
  void View(void);
  void SetRGB_DAC(float dr,float dg,float db);
  void LoadPalette(char *filename);
  int PutSymbol(int x,int y,char symbol,int type);
  void Print(int x,int y,char *Text);
  void PrintToScreen(int x,int y,char *Text);
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Инициализирует видеорежим.                                        //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Screen::InitVga(void)
{
 Buffer=(unsigned char *)malloc(64000);
 _setvideomode(_MRES256COLOR);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Восстанавливает текстовый режим.                                  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Screen::CloseVga(void)
{
 free(Buffer);
 _setvideomode(_TEXTC80);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Очищает окно в буффере.                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Screen::ClearWindow(void)
{
 unsigned char *dpt=Buffer;
 *(dpt)=0;
 *(dpt+1)=0;
 *(dpt+2)=0;
 *(dpt+3)=0;
 #pragma aux clearwindow parm[]=\
 "mov esi,dpt"\
 "mov ecx,13439"\
 "mov eax,ecx"\
 "mov edi,dpt"\
 "add edi,4"\
 "rep movsd"\
 modify [esi edi ecx eax];
 clearwindow();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Очищает буффер.                                                   //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Screen::Clear(void)
{
 unsigned char *dpt=Buffer;
 *(dpt)=0;
 *(dpt+1)=0;
 *(dpt+2)=0;
 *(dpt+3)=0;
 #pragma aux clearscreen parm[]=\
 "mov esi,dpt"\
 "mov ecx,15999"\
 "mov eax,ecx"\
 "mov edi,dpt"\
 "add edi,4"\
 "rep movsd"\
 modify [esi edi ecx eax];
 clearscreen();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Делает видимым окно на экране.                                    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Screen::ViewWindow(void)
{
// while((inp(0x3DA)&0x08)==0);//ждать развёртку
 unsigned char *dpt=Buffer;
 #pragma aux putwindow parm[]=\
 "mov esi,dpt"\
 "mov ecx,13440"\
 "mov eax,ecx"\
 "mov edi,0xa0000"\
 "rep movsd"\
 modify [esi edi ecx eax];
 putwindow();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Делает видимым статус панель на экране                            //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Screen::ViewBar(void)
{
 //while((inp(0x3DA)&0x08)==0);//ждать развёртку
 unsigned char *dpt=Buffer+53760;
 #pragma aux putbar parm[]=\
 "mov esi,dpt"\
 "mov ecx,2560"\
 "mov eax,ecx"\
 "mov edi,0xad200"\
 "rep movsd"\
 modify [esi edi ecx eax];
 putbar();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Делает видимым весь буффер на экране.                             //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Screen::View(void)
{
// while((inp(0x3DA)&0x08)==0);//ждать развёртку
 unsigned char *dpt=Buffer;
 #pragma aux putscreen parm[]=\
 "mov esi,dpt"\
 "mov ecx,16000"\
 "mov eax,ecx"\
 "mov edi,0xa0000"\
 "rep movsd"\
 modify [esi edi ecx eax];
 putscreen();
}
void Screen::SetRGB_DAC(float dr,float dg,float db)
{
 outp(0x3c8,0);
 for(register int n=0;n<256;n++)
  {
   outp(0x3c9,Pal[n*3]*dr);
   outp(0x3c9,Pal[n*3+1]*dg);
   outp(0x3c9,Pal[n*3+2]*db);
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Загрузка палитры с именем filename.                               //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Screen::LoadPalette(char *filename)
{
 FILE *in;
 unsigned char r,g,b;
 register int n;
 in =fopen(filename,"r+b");
 if (in!=NULL)
  {
  for (n=0;n<256;n++)
   {
    fread(&b,1,1,in);
    fread(&g,1,1,in);
    fread(&r,1,1,in);
    Pal[n*3]=r;
    Pal[n*3+1]=g;
    Pal[n*3+2]=b;
   }
   fclose(in);
   SetRGB_DAC(1,1,1);
  }
}
int Screen::PutSymbol(int x,int y,char symbol,int type)
{
 int sprite=-1;
 char *wdp=Data.Font;
//Data.Font
 if (symbol>='0' && symbol<='9') sprite=symbol-48;
 if (symbol>='A' && symbol<='Z') sprite=10+symbol-65;
 if (symbol==' ') return(5);
 if (symbol=='(') sprite=36;
 if (symbol==')') sprite=37;
 if (symbol=='`') sprite=39;
 if (symbol==';') sprite=40;
 if (symbol==',') sprite=41;
 if (symbol=='.') sprite=42;
 if (symbol=='_') sprite=43;
 if (symbol=='-') sprite=44;
 if (symbol=='=') sprite=45;
 if (symbol=='!') sprite=46;

 if (sprite==-1) return(0);
 int ofs1=2+sprite*4;
 int offset=wdp[ofs1]+wdp[ofs1+1]*256+wdp[ofs1+2]*65536+wdp[ofs1+3]*16777216;
 wdp+=offset;
 int len=wdp[0]+256*wdp[1];
 int hgt=wdp[2]+256*wdp[3];
 offset=3;
 char *vpt=Buffer;
 if (type==0) vpt=(char *)0xa0000;
 for(int n=0;n<len;n++)
  for(int m=0;m<hgt;m++)
   {
    offset++;
    if (n+x<0 || n+x>319) continue;
    if (wdp[offset]==117) continue;
    *(vpt+(m+y)*320+n+x)=wdp[offset];
   }
 return(len);
}
void Screen::Print(int x,int y,char *Text)
{
 int n;
 unsigned char s;
 for (n=0;n<strlen(Text);n++)
  {
   s=(unsigned char)(Text[n]);
   x+=PutSymbol(x,y,s,1);
  }
}
void Screen::PrintToScreen(int x,int y,char *Text)
{
 int n;
 unsigned char s;
 for (n=0;n<strlen(Text);n++)
  {
   s=(unsigned char)(Text[n]);
   x+=PutSymbol(x,y,s,0);
  }
}
