#include "timer.h"

int TimerUser;
int counsec,secs,mins,hours;
//===========================================================================
void (__interrupt __far *tmoldinter)();
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//��ࠡ��稪 ���뢠��� ⠩���.                                    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void interrupt far new_timer()
{
 TimerUser++;
 counsec++;
 if (counsec==140)
  {
   counsec=0;
   secs++;
   if (secs==60)
    {
     secs=0;
     mins++;
     if (mins==60)
      {
       mins=0;
       hours++;
       if (hours==24)
        {
         hours=0;
        }
      }
    }
  }
 if (!(TimerUser%15)) tmoldinter();
 outp(0x20,0x20);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//���樠������� ⠩���.freq-����.                               //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void StartUpTimer(int freq)
{
 tmoldinter=_dos_getvect(8);
 _dos_setvect(8,new_timer);
 SetTimer(1193180/freq);
 TimerUser=0;
 counsec=0;
 secs=0;
 mins=0;
 hours=0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//��⠭���� ����� ⠩���.                                        //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void SetTimer(unsigned int val)
{
 outp(0x43,0x36);
 outp(0x40,val&255);
 outp(0x40,(val>>8)&255);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//����⠭�������� �⠭����� ��ࠡ��稪 ⠩���.                   //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void OldTimer(void)
{
 SetTimer(1193180/(0.24));
 _dos_setvect(8,tmoldinter);
}


