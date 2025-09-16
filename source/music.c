
#include <stdio.h>
#include <stdint.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
// CD library
#include <libcd.h>
// SPU library
#include <libspu.h>


#include <music.h>





// SPU attributes
SpuCommonAttr spuSettings;
// CD tracks 
int playing = -1;
int tracks[] = {2, 0};  // Track to play , 1 is data, 2 is music.wav, 3 is nothing. See isoconfig.xml

void MusicInit(void)
{
    printf("music init start: \n");

    int count = 0;
    int flip = 1;
    int ntoc;
    CdlLOC loc[100];
    CdInit();
    SpuInit();

    // Set master & CD volume to max
    spuSettings.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR | SPU_COMMON_CDVOLL | SPU_COMMON_CDVOLR | SPU_COMMON_CDMIX);
    // Master volume should be in range 0x0000 - 0x3fff
    spuSettings.mvol.left  = 0x3fff;
    spuSettings.mvol.right = 0x3fff;
    // Cd volume should be in range 0x0000 - 0x7fff
    spuSettings.cd.volume.left = 0x7fff;
    spuSettings.cd.volume.right = 0x7fff;
    // Enable CD input ON
    spuSettings.cd.mix = SPU_ON;
    // Apply settings
    SpuSetCommonAttr(&spuSettings);
    // Set transfer mode 
    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    // CD Playback setup
    // Play second audio track
    // Get CD TOC
    while ((ntoc = CdGetToc(loc)) == 0) { 		/* Read TOC */
        //FntPrint("No TOC found: please use CD-DA disc...\n");
        printf("No TOC found: please use CD-DA disc...\n");
    }
    // Prevent out of bound pos
    for (int i = 1; i < ntoc; i++) {
        CdIntToPos(CdPosToInt(&loc[i]) - 74, &loc[i]);
    }
    // Those array will hold the return values of the CD commands
    u_char param[4], result[8];
    // Set CD parameters ; Report Mode ON, CD-DA ON. See LibeOver47.pdf, p.188
    param[0] = CdlModeRept|CdlModeDA;	
    CdControlB (CdlSetmode, param, 0);	/* set mode */
    VSync (3);				/* wait three vsync times */
    // Play second track in toc array
    CdControlB (CdlPlay, (u_char *)&loc[3], 0);	/* play */
    printf("music playing\n");


}