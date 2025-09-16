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
#include <libps.h>
#include <stdio.h>
#include <main.h>

//--------------------------------------------------------------------------
// G L O B A L S
//--------------------------------------------------------------------------

int output_buffer_index;            // buffer index
GsOT world_ordering_table[2];       // ordering table headers
GsOT_TAG ordering_table[2][1<<1];   // actual ordering tables
PACKET gpu_work_area[2][24000];     // GPU packet work area
u_char prev_mode;					// previous code
int fnt_id[9];						// font id


int main(void)
{
    printf("main started: \n");

    //init();
    Init();

    //main loop
	while(1)
    {
        printf("loop start: \n");

        FntPrint(fnt_id[0], (char*)"WELCOME TO BALATRO!");
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
    
  
     // all reset, the drawing environment and display are initialised
     ResetGraph(0);

    //This function MUST be called before using other libGS functions!
     GsInitGraph( SCREEN_WIDTH, SCREEN_HEIGHT,
                  GsOFSGPU|GsINTER, 0, 0 );


     // load in the font pattern
     FntLoad(960,256);
     printf("Fonts loaded: \n");

     fnt_id[0] = FntOpen(0,10,SCREEN_WIDTH, SCREEN_HEIGHT,0,80);
     fnt_id[1] = FntOpen(0,20,SCREEN_WIDTH, SCREEN_HEIGHT,0,80);
     fnt_id[2] = FntOpen(0,30,SCREEN_WIDTH, SCREEN_HEIGHT,0,80);
     fnt_id[3] = FntOpen(0,40,SCREEN_WIDTH, SCREEN_HEIGHT,0,80);

     fnt_id[4] = FntOpen(0,120,SCREEN_WIDTH, SCREEN_HEIGHT,0,80);
     fnt_id[5] = FntOpen(0,130,SCREEN_WIDTH, SCREEN_HEIGHT,0,80);
     fnt_id[6] = FntOpen(0,140,SCREEN_WIDTH, SCREEN_HEIGHT,0,80);
     fnt_id[7] = FntOpen(0,150,SCREEN_WIDTH, SCREEN_HEIGHT,0,80);
      
        // save current video mode
     prev_mode = GetVideoMode();

     // init graphic mode
     SetVideoMode( MODE_PAL );
     printf("Set video mode complete: \n");

             // double buffer definition
     GsDefDispBuff( 0, 0, 0, 0 );
     printf("Double buffer setup complete: \n");

     
     // set display output on t.v 
     GsDISPENV.screen.x = 10;
     GsDISPENV.screen.y = 18;
     GsDISPENV.screen.w = 255;
     GsDISPENV.screen.h = 255; 
     
     // set bg clear color and flag
     GsDRAWENV.r0 = 0x00;
     GsDRAWENV.g0 = 0x00;
     GsDRAWENV.b0 = 0x80;
     GsDRAWENV.isbg = 1;	  	
      
     // set up the ordering table handlers
     for( count=0; count < 2; count++ )
        {
          world_ordering_table[count].length = 1;
          world_ordering_table[count].org = ordering_table[count];
        }

     // initialises the ordering table
     GsClearOt( 0, 0, &world_ordering_table[output_buffer_index]);
     GsClearOt( 0, 0, &world_ordering_table[output_buffer_index+1]);
     printf("WOT is setup and complete: \n");
     printf("Game setup is complete: \n");
}

void DeInitGame( void )
 {

      // set previous video mode
     SetVideoMode( prev_mode );

     // current drawing is canvelled and the command queue is flushed
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

    // get the active buffer
    output_buffer_index = GsGetActiveBuff();

    // sets drawing command storage address
    GsSetWorkBase((PACKET*)gpu_work_area[output_buffer_index]);

    // initialises the ordering table
    GsClearOt(0, 0, &world_ordering_table[output_buffer_index]);

    // rendering done here
    
    for( count =0; count <8; count++ )
         FntFlush(fnt_id[count]);         
         
    //GsSortSprite(&sprite, &world_ordering_table[output_buffer_index], 0);           
        
    // wait for vertical synchronisation
    VSync(0);    // 0: blocking until vertical synch occurs

    // swap double buffers, (changes the display buffer and drawing buffer)
    GsSwapDispBuff(); 
       
    // start execution of the drawing command registered in OT
    GsDrawOt(&world_ordering_table[output_buffer_index]);

 }// end UpdateScreen 





