/* #include <tonc.h>
#include <maxmod.h>
#include <string.h>

#include "sprite.h"
#include "card.h"
#include "game.h"
#include "blind.h"
#include "joker.h"
#include "affine_background.h"
#include "graphic_utils.h"

// Graphics
#include "background_gfx.h"
#include "affine_background_gfx.h"

// Audio
#include "soundbank.h"
#include "soundbank_bin.h"
 */
#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#include <main.h>
#include <music.h>


//--------------------------------------------------------------------------
// G L O B A L S
//--------------------------------------------------------------------------

DISPENV disp[2];             // Double buffered DISPENV and DRAWENV
DRAWENV draw[2];
u_long ot[2][OTLEN];     // double ordering table of length 8 * 32 = 256 bits / 32 bytes
char primbuff[2][32768];     // double primitive buffer of length 32768 * 8 =  262.144 bits / 32,768 Kbytes
char *nextpri = primbuff[0]; // pointer to the next primitive in primbuff. Initially, points to the first bit of primbuff[0]
short db = 0;                // index of which buffer is used, values 0, 1


int main(void)
{
    printf("main started: \n");

    //init();
    Init();
    MusicInit();

    //main loop
	while(1)
    {
        printf("loop start: \n");

        ClearOTagR(ot[db], OTLEN);


        FntPrint("WELCOME TO BALATRO!");
        FntFlush(-1);

        UpdateScreen();

        /*         
        VBlankIntrWait();
        mmFrame();
		key_poll();
        update();
        draw(); */
    } //end main loop
    
    DeInitGame(); //de-innit game
	return 0;
}

void Init( void )
{
/*     irq_init(NULL);
    irq_add(II_VBLANK, mmVBlank);
    irq_add(II_HBLANK, affine_background_hblank);

    // Initialize text engine
    tte_init_se(0, BG_CBB(TTE_CBB) | BG_SBB(TTE_SBB), 0, CLR_WHITE, TTE_BIT_UNPACK_OFFSET, NULL, NULL);
    tte_erase_screen();
    tte_init_con();

    // TTE palette setup
    pal_bg_bank[TTE_YELLOW_PB][TTE_BIT_ON_CLR_IDX] = TEXT_CLR_YELLOW;
    pal_bg_bank[TTE_BLUE_PB][TTE_BIT_ON_CLR_IDX] = TEXT_CLR_BLUE; 
    pal_bg_bank[TTE_RED_PB][TTE_BIT_ON_CLR_IDX] = TEXT_CLR_RED; 
    pal_bg_bank[TTE_WHITE_PB][TTE_BIT_ON_CLR_IDX] = TEXT_CLR_WHITE;

    // Set up the video mode
    // BG0 is the TTE text layer
    REG_BG0CNT = BG_PRIO(0) | BG_CBB(TTE_CBB) | BG_SBB(TTE_SBB) | BG_4BPP;
    // BG1 is the main background layer
    REG_BG1CNT = BG_PRIO(1) | BG_CBB(MAIN_BG_CBB) | BG_SBB(MAIN_BG_SBB) | BG_8BPP;
	// BG2 is the affine background layer
    REG_BG2CNT = BG_PRIO(2) | BG_CBB(AFFINE_BG_CBB) | BG_SBB(AFFINE_BG_SBB) | BG_8BPP | BG_WRAP;

    int win1_left = 72;
    int win1_top = 44;
    int win1_right = 200;
    int win1_bottom = 128;

    int win2_left = 72;
    int win2_top = 0;
    int win2_right = 232;
    int win2_bottom = 44;

	REG_WIN0H = win1_left<<8 | win1_right;
	REG_WIN0V =  win1_top<<8 | win1_bottom;
	REG_WIN0CNT = WIN_ALL | WIN_BLD;
	REG_WINOUTCNT = WIN_ALL;

    REG_WIN1H = win2_left<<8 | win2_right;
    REG_WIN1V =  win2_top<<8 | win2_bottom;
    REG_WIN1CNT = WIN_ALL | WIN_BLD;

    REG_BLDCNT = BLD_BUILD(BLD_BG1, BLD_BG2, 1);

    REG_BLDALPHA = BLDA_BUILD(0, 13);

    REG_DISPCNT = DCNT_MODE1 | DCNT_OBJ_1D | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ | DCNT_WIN0 | DCNT_WIN1;

    // Initialize subsystems
    mmInitDefault((mm_addr)soundbank_bin, 12);
    mmStart(MOD_MAIN_THEME, MM_PLAY_LOOP);
    affine_background_init();
    sprite_init();
    card_init();
    blind_init();
    joker_init();
    game_init(); */

    int count;
     
     printf("Starting InitGame() \n");
    
    ResetGraph(0);
    SetDefDispEnv(&disp[0], 0, 0, SCREENXRES, SCREENYRES);
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[1], 0, 0, SCREENXRES, SCREENYRES);
    if (VMODE)
    {
        SetVideoMode(MODE_PAL);
        disp[0].screen.y += 8;
        disp[1].screen.y += 8;
        }
    SetDispMask(1);                 // Display on screen    
    setRGB0(&draw[0], 50, 50, 50);
    setRGB0(&draw[1], 50, 50, 50);
    draw[0].isbg = 1;
    draw[1].isbg = 1;
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    FntLoad(960, 0);
    FntOpen(MARGINX, SCREENYRES - MARGINY - FONTSIZE, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 280 );
    
    printf("Game setup is complete: \n");
}

void DeInitGame( void )
 {

      // set previous video mode
     //SetVideoMode( prev_mode );

     // current drawing is cancelled and the command queue is flushed
     ResetGraph(3);	   

     printf("Graphics flushed: \n");
     printf("Game now de-int: \n");
 
 }// end DeInitGame

/* void update()
{
    affine_background_update();
    game_update();
}

void draw()
{
    sprite_draw();
} */

void UpdateScreen( void )
 {
    int count;
    DrawSync(0);
    VSync(0);
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    DrawOTag(&ot[db][OTLEN - 1]);
    db = !db;
    nextpri = primbuff[db];
 }// end UpdateScreen 





