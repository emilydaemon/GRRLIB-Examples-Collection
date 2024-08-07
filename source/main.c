#include <grrlib.h>
#include <string.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include <dpad_hint_right_png.h>
#include <Letter_Gothic_Std_14_Bold_png.h>

#include "splash.h"
#include "credits.h"
#include "examples.h"
#include "grrlib_printf.h"

void Menu(bool showFadeIn) {
    int page = 0;
    int FadeInTransparency = 0;
    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(Letter_Gothic_Std_14_Bold_png);
    GRRLIB_InitTileSet(tex_font, 11, 24, 32);
    
    GRRLIB_texImg *tex_dpad_hint_right = GRRLIB_LoadTexture(dpad_hint_right_png);
    
    GRRLIB_texImg *tex_dpad_hint_left  = GRRLIB_CreateEmptyTexture(tex_dpad_hint_right->w, tex_dpad_hint_right->h);
    GRRLIB_BMFX_FlipH(tex_dpad_hint_right, tex_dpad_hint_left);
    GRRLIB_FlushTex(tex_dpad_hint_left);

    Examples_LoadTextures();


    GRRLIB_SetBackgroundColour(250, 250, 250, 255);
    GRRLIB_Settings.antialias = false;

    bool exitMenu = false;

    while(!exitMenu) {
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exitMenu = true;
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_1) credits();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_LEFT) {
            page--;
            if (page < 0) page = sizeof(examples) / sizeof(examples[0]) - 1;
        }
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_RIGHT) {
            page++;
            if (page >= sizeof(examples) / sizeof(examples[0])) page = 0;
        }

        GRRLIB_Printf(3,3, tex_font, 0x000000FF, 0.8f, "GRRLIB Examples Collection 0.1");
        GRRLIB_Printf(640 - (8.8*22) - 3,3, tex_font, 0x000000FF, 0.8f, "Press (1) for credits");

        GRRLIB_Printf((640 - (strlen(examples[page].title) * 22) ) / 2, 60, tex_font, 0x000000FF, 2, examples[page].title);
        
        GRRLIB_DrawImg(25, 405, tex_dpad_hint_left, 0, 0.5, 0.5, 0xFFFFFFFF);
        GRRLIB_DrawImg(565, 405, tex_dpad_hint_right, 0, 0.5, 0.5, 0xFFFFFFFF);
       
        GRRLIB_DrawImg(160, 150, examples[page].preview, 0, 1, 1, 0xFFFFFFFF);
        
        GRRLIB_Printf_WordWrap(100, 480-96, tex_font, 0x000000FF, 1, 40, examples[page].description);

        // draw some circles to show which page were on

        const f32 spacing = 20.0f;
        const f32 radius = 5.0f;
        const f32 startX = (640 - (sizeof(examples) / sizeof(examples[0]) * spacing)) / 2 + radius;

        for(int i = 0; i < sizeof(examples) / sizeof(examples[0]); i++) {
            u32 color = (i == page) ? 0x444444FF : 0xAAAAAAFF;
            GRRLIB_Circle(startX + i * spacing, 470, radius, color, 1);
        }

        if (FadeInTransparency < 256 && showFadeIn) {
            GRRLIB_Rectangle(0, 0, rmode->fbWidth, rmode->efbHeight, 0xFFFFFFFF - FadeInTransparency, 1);
            FadeInTransparency += 5;
        }

        GRRLIB_Render();
        
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A && examples[page].entry != NULL) {
            showFadeIn = false;
            examples[page].entry();
        }
    }

    GRRLIB_FreeTexture(tex_dpad_hint_right);
    GRRLIB_FreeTexture(tex_dpad_hint_left);
    GRRLIB_FreeTexture(tex_font);

    GRRLIB_Exit();
}


int main(int argc, char **argv) {
    GRRLIB_Init();
    WPAD_Init();

    bool showFadeIn = Splash();
    Menu(showFadeIn);
    exit(0);
}
