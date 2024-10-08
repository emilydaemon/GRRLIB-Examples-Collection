#pragma once
#include "Common.h"
#include <preview_3D_sample4_png.h>

/*===========================================
    NoNameNo Simple 3D object rotating ;)
    object have to be triangulated to be used
    with this source.
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>

#include "../Logo.h"

void ThreeD_sampleFour_main() {
    float a=0;

    GRRLIB_Settings.antialias = true;

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
    GRRLIB_Camera3dSettings(0.0f,0.0f,13.0f, 0,1,0, 0,0,0);

    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) break;


        GRRLIB_3dMode(0.1,1000,45,0,0);
        GRRLIB_ObjectView(0,0,-30, a,a*2,a*3,1,1,1);
        GX_Begin(GX_TRIANGLES, GX_VTXFMT0, logoNbFace * 3);
        for(int i=0; i<logoNbFace*3; i+=3) {
            u32 col;
            if(i<=(246*3*2))
                col=0xFFFFFFFF;
            else
                col=0xAAAAAAFF;

            GX_Position3f32(logoPos[logoFac[i][0]-1].x,logoPos[logoFac[i][0]-1].y,logoPos[logoFac[i][0]-1].z);
            GX_Color1u32(col);

            GX_Position3f32(logoPos[logoFac[i+1][0]-1].x,logoPos[logoFac[i+1][0]-1].y,logoPos[logoFac[i+1][0]-1].z);
            GX_Color1u32(col);

            GX_Position3f32(logoPos[logoFac[i+2][0]-1].x,logoPos[logoFac[i+2][0]-1].y,logoPos[logoFac[i+2][0]-1].z);
            GX_Color1u32(col);
        }
        GX_End();

        a+=0.5f;
        GRRLIB_Render();
    }
    ResetSettings();
}
