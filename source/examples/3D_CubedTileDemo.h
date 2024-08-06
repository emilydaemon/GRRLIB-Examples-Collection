#pragma once
#include "Common.h"
#include "Map.h"
#include <preview_3D_CubedTileDemo_png.h>

/*===========================================
    NoNameNo CubedTileDemo
    A sample code to show how to use Dynamic Texturing
    Have a look a TileDemo sources for diff ;)
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <wiiuse/wpad.h>

#include "tile1_png.h"
#include "perso_png.h"
#include "bg_png.h"
#include "nonameno_png.h"

void ThreeD_CubedTileDemo_main() {
    int startx=0, starty=0;
    int x, y;
    int dirx=0, diry=0;
    int cptx=0, cpty=0;
    int bgx=-32, bgy=-32;
    float idperso=0;
    float sinnonameno=0;
    float camZ=1400.0f;
    float a=0;

    GRRLIB_Settings.antialias = false;
    GRRLIB_ClipDrawing(0, 0, rmode->fbWidth, rmode->efbHeight);
    GRRLIB_texImg *tex_tile1 = GRRLIB_LoadTexture(tile1_png);
    GRRLIB_InitTileSet(tex_tile1, TileMap1Width, TileMap1Height, 0);
    GRRLIB_texImg *tex_perso = GRRLIB_LoadTexture(perso_png);
    GRRLIB_InitTileSet(tex_perso, 64, 64, 0);
    GRRLIB_texImg *tex_bg = GRRLIB_LoadTexture(bg_png);
    GRRLIB_texImg *tex_nonameno = GRRLIB_LoadTexture(nonameno_png);
    GRRLIB_InitTileSet(tex_nonameno, 32, 32, 0);

    GRRLIB_texImg *tex_screen = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);

    GRRLIB_SetBackgroundColour(0, 0, 0, 255);

    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)  break;
        if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_PLUS)  camZ+=20.0f;
        if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_MINUS)  camZ-=20.0f;

        if((dirx==0) && (diry==0)) {
            if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT) {
                diry=-4;
                idperso=15;
            }
            else if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT) {
                diry=4;
                idperso=15;
            }
            else if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN) {
                dirx=-4;
                idperso=1;
            }
            else if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP) {
                dirx=4;
                idperso=8;
            }
        }

        if((dirx==0) && (diry==0)) {
            idperso=0;
        }

        if(((Map1Info[9+starty][10+startx]==1) || (Map1Info[9+starty][11+startx]==1)) || ((Map1Info[9+starty][10+startx]==43521) || (Map1Info[9+starty][11+startx]==43521))) {
        }
        else {
            dirx=0;
            diry=-4;
        }

        if(dirx<0) {
            if((Map1Info[7+starty][12+startx]==8) || (Map1Info[7+starty][12+startx]==2) || (Map1Info[8+starty][12+startx]==8) || (Map1Info[8+starty][12+startx]==2)) {
                dirx=0;
            }
            else {
                idperso++;
                if(idperso>7)
                    idperso=1;
            }
        }

        if(dirx>0) {
            if((Map1Info[7+starty][9+startx]==8) || (Map1Info[7+starty][9+startx]==2) || (Map1Info[8+starty][9+startx]==8) || (Map1Info[8+starty][9+startx]==2)) {
                dirx=0;
            }
            else {
                idperso++;
                if(idperso>14)
                    idperso=8;
           }
        }

        if(diry<0) {
            if((Map1Info[9+starty][10+startx]==1) || (Map1Info[9+starty][11+startx]==1)) {
                diry=0;
            }
        }

        if(diry!=0) {
            idperso++;
            if(idperso>22)
                idperso=16;
        }

        cptx+=dirx;
        if(dirx>0)
            bgx++;
        else if(dirx<0)
            bgx--;
        if((bgx>-1) || (bgx<-63))
            bgx=-32;

        if(cptx==32) {
            cptx=0;
            dirx=0;
            startx--;
        }
        else if(cptx==-32) {
            cptx=0;
            dirx=0;
            startx++;
        }

        cpty+=diry;
        if(diry>0)
            bgy++;
        else if(diry<0)
            bgy--;
        if((bgy>-1) || (bgy<-63))
            bgy=-32;

        if(cpty==32) {
            cpty=0;
            diry=0;
            starty--;
        }
        else if(cpty==-32) {
            cpty=0;
            if(((Map1Info[9+starty][10+startx]==1) || (Map1Info[9+starty][11+startx]==1)) || ((Map1Info[9+starty][10+startx]==43521) || (Map1Info[9+starty][11+startx]==43521))) {
                diry=0;
            }
            starty++;
        }
        GRRLIB_DrawImg(bgx, bgy, tex_bg, 0, 1, 1, 0xFFFFFFFF);

        for(y=0; y<=17; y++) {
            for(x=0; x<=21; x++) {
                if(Map1Data[y+starty][x+startx] != 0) {
                    GRRLIB_DrawTile(x*TileMap1Width+cptx-TileMap1Width,y*TileMap1Height+cpty-TileMap1Height,tex_tile1,0,1,1,0xFFFFFFFF,Map1Data[y+starty][x+startx]-1);
                }
            }
        }
        GRRLIB_DrawTile(TileMap1Width*9,TileMap1Height*6,tex_perso,0,1,1,0xFFFFFFFF,(int)idperso);

        const float oldsinnonameno=sinnonameno;
        for(int i=0; i<8; i++) {
            GRRLIB_DrawTile(TileMap1Width*(6+i),(TileMap1Height*10)+sin(sinnonameno)*64,tex_nonameno,0,1,1,0xFFFFFFFF,i);
            sinnonameno+=0.4F;
        }
        sinnonameno=oldsinnonameno+0.08F;

        GRRLIB_Screen2Texture(0, 0, tex_screen, GX_TRUE);


        GRRLIB_Camera3dSettings(0.0f,0.0f,camZ, 0,1,0, 0,0,0);
        GRRLIB_3dMode(0.1,3000,45,1,0);
        GRRLIB_SetTexture(tex_screen, FALSE);
        GRRLIB_ObjectView(0,0,0, a,a*2,a*3,1,1,1);
        GX_Begin(GX_QUADS, GX_VTXFMT0, 16);
            GX_Position3f32(-rmode->fbWidth/2,rmode->efbHeight/2,rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(rmode->fbWidth/2,rmode->efbHeight/2,rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(rmode->fbWidth/2,-rmode->efbHeight/2,rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(-rmode->fbWidth/2,-rmode->efbHeight/2,rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);

            GX_Position3f32(rmode->fbWidth/2,rmode->efbHeight/2,-rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(-rmode->fbWidth/2,rmode->efbHeight/2,-rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(-rmode->fbWidth/2,-rmode->efbHeight/2,-rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(rmode->fbWidth/2,-rmode->efbHeight/2,-rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);

            GX_Position3f32(rmode->fbWidth/2,rmode->efbHeight/2,rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(rmode->fbWidth/2,rmode->efbHeight/2,-rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(rmode->fbWidth/2,-rmode->efbHeight/2,-rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(rmode->fbWidth/2,-rmode->efbHeight/2,rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);

            GX_Position3f32(-rmode->fbWidth/2,rmode->efbHeight/2,-rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(-rmode->fbWidth/2,rmode->efbHeight/2,rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(-rmode->fbWidth/2,-rmode->efbHeight/2,rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(-rmode->fbWidth/2,-rmode->efbHeight/2,-rmode->fbWidth/2);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);
        GX_End();

        a+=0.2f;

        GRRLIB_Render();
    }

    GRRLIB_FreeTexture(tex_tile1);
    GRRLIB_FreeTexture(tex_perso);
    GRRLIB_FreeTexture(tex_bg);
    GRRLIB_FreeTexture(tex_nonameno);
    GRRLIB_FreeTexture(tex_screen);
    
    ResetSettings();
}
