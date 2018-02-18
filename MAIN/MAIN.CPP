//=========================================================================//
//3-х мерный движок,основанный на BSP дереве.Принцип работы данной         //
//программы несущественным образом отличается от технологии DOOM`а.        //
//Создано и оформлено Дмитрием Труновым 27.05.2002.                        //
//Все права защищены.Никакая часть этой программы не может быть            //
//воспроизведена без разрешения автора.                                    //
//=========================================================================//

#include "common.h"
#include "video.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "loading.h"
#include "sprites.h"
#include "classes.h"
#include "special.h"
#include "automap.h"
#include "3dengine.h"
#include "control.h"
#include "menu.h"

extern CKeyboard cKeyboard;
extern CVideo cVideo;
extern SData sData;
extern SMouse sMouse;
extern SPlayer sPlayer;

//===========================================================================
void InitGame(void);
//===========================================================================
void main(void)
{
 InitGame();
 PutSprite(0,0,sData.Menu,0,0);
 PutSprite(0,0,sData.Menu,0,2);
 cVideo.PrintToCVideo(0,192," DMITRIY TRUNOW 18.06.2005 MODIFY 22.10.2015");
 Menu();
 cVideo.CloseVga();
 OldTimer();
 m_setsensitivity(sMouse.HSpeed,sMouse.VSpeed,sMouse.Double);
}
//===========================================================================
void InitGame(void)
{
 cVideo.InitVga();
 cVideo.Clear();
 InitPlayer();
 cVideo.LoadPalette("resurs\\game.pal");
 LoadResurs();
 StartUpTimer(140);
 m_init();
 int h,v,d;
 m_getsensitivity(&h,&v,&d);
 sMouse.HSpeed=h;
 sMouse.VSpeed=v;
 sMouse.Double=d;
 sPlayer.LoadPos=0;
 sPlayer.SavePos=0;
// m_setsensitivity(16,16,16);
}


