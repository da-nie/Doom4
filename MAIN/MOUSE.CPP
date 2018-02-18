#include "mouse.h"

//===========================================================================
int m_init(void)
{
 REGS regs;
 regs.w.ax=0x0000;
 int386(0x33,&regs,&regs);
 return(regs.w.ax);
}
void m_show(void)
{
 REGS regs;
 regs.w.ax=0x0001;
 int386(0x33,&regs,&regs);
}
void m_hide(void)
{
 REGS regs;
 regs.w.ax=0x0002;
 int386(0x33,&regs,&regs);
}
void m_getpos(int *button,int *x,int *y)
{
 REGS regs;
 regs.w.ax=0x0003;
 int386(0x33,&regs,&regs);
 *x=regs.w.cx;
 *y=regs.w.dx;
 *button=regs.w.bx;
}
void m_setpos(int x,int y)
{
 REGS regs;
 regs.w.ax=0x0004;
 regs.w.cx=x;
 regs.w.dx=y;
 int386(0x33,&regs,&regs);
}
void m_setsensitivity(int h,int v,int d)
{
 REGS regs;
 regs.w.ax=0x001A;
 regs.w.cx=v;
 regs.w.bx=h;
 regs.w.dx=d;
 int386(0x33,&regs,&regs);
}
void m_getsensitivity(int *h,int *v,int *d)
{
 REGS regs;
 regs.w.ax=0x001B;
 int386(0x33,&regs,&regs);
 *v=regs.w.cx;
 *h=regs.w.bx;
 *d=regs.w.dx;
}
