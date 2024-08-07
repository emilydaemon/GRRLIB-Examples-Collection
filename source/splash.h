#ifndef SPLASH_H
#define SPLASH_H

#include <grrlib.h>
#include <math.h>
#include "Logo.h"

bool Splash() {
    int i = 0;

    float angle = 0;
    float rotSpeed = 0.1f;
    const float maxRotSpeed = 24.0f;

    int FadeInTransparency = 0;
    int FadeOutTransparency = 0;

    float flyup = 0.0f;

    GRRLIB_texImg *blur_buffer = GRRLIB_CreateEmptyTexture(640, 480);

    GRRLIB_Settings.antialias = true;

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
    GRRLIB_Camera3dSettings(0.0f,0.0f,13.0f, 0,1,0, 0,0,0);

    while(1) {
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_A) {return false;}

        GRRLIB_3dMode(0.1,1000,45,0,0);
        GRRLIB_FillScreen(0x000000FF);
        GRRLIB_ObjectView(0,flyup,-60, 1,angle,1, 1,1,1);
        GX_Begin(GX_TRIANGLES, GX_VTXFMT0, logoNbFace * 3);
        for(int i=0; i<logoNbFace*3; i+=3) {
            u32 col;
            if(i<=(246*3*2))
                col=0xFFFF00FF;
            else
                col=0xAAAA00FF;

            GX_Position3f32(logoPos[logoFac[i][0]-1].x,logoPos[logoFac[i][0]-1].y,logoPos[logoFac[i][0]-1].z);
            GX_Color1u32(col);

            GX_Position3f32(logoPos[logoFac[i+1][0]-1].x,logoPos[logoFac[i+1][0]-1].y,logoPos[logoFac[i+1][0]-1].z);
            GX_Color1u32(col);

            GX_Position3f32(logoPos[logoFac[i+2][0]-1].x,logoPos[logoFac[i+2][0]-1].y,logoPos[logoFac[i+2][0]-1].z);
            GX_Color1u32(col);
        }
        
        angle += rotSpeed;
        if (i < 400) rotSpeed = maxRotSpeed * pow((float)i / 400, 2);   
        i++;

        GRRLIB_2dMode();

	GRRLIB_DrawImg(0, 0, blur_buffer, 0, 1, 1, 0xFFFFFFAA); // first frame will be black... Whatever.

        if(FadeInTransparency < 256) {
            GRRLIB_Rectangle(0,0, rmode->fbWidth, rmode->efbHeight, 0x000000FF - FadeInTransparency, 1);
            FadeInTransparency += 2;   
        }

        if (i > 400 && FadeOutTransparency < 256) {
            GRRLIB_Rectangle(0, 0, rmode->fbWidth, rmode->efbHeight, 0xFFFFFF00 + FadeOutTransparency, 1);
            FadeOutTransparency += 5;
            flyup += 5;
        }

        if (FadeOutTransparency > 256) break;

        GRRLIB_CompoEnd(0, 0, blur_buffer);
	GRRLIB_DrawImg(0, 0, blur_buffer, 0, 1, 1, 0xFFFFFFFF);

        GRRLIB_Render();
    }

    return true;
}

#endif 
