#ifndef SMouse_H
#define SMouse_H

#include "common.h"

int m_init(void);
void m_show(void);
void m_hide(void);
void m_getpos(int *button,int *x,int *y);
void m_setpos(int x,int y);
void m_setsensitivity(int h,int v,int d);
void m_getsensitivity(int *h,int *v,int *d);
#endif