#include <grrlib.h>
#include <string.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include <dpad_hint_right_png.h>
#include <FreeMonoBold_ttf.h>

#include "splash.h"
#include "credits.h"
#include "examples.h"
#include "grrlib_printf.h"

void Menu(bool showFadeIn) {
    int page = 0;
    int FadeInTransparency = 0;
    const int prvBorder = 2;
    
    GRRLIB_ttfFont *tex_font = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
    
    GRRLIB_texImg *tex_dpad_hint_right = GRRLIB_LoadTexture(dpad_hint_right_png);
    
    GRRLIB_texImg *tex_dpad_hint_left  = GRRLIB_CreateEmptyTexture(tex_dpad_hint_right->w, tex_dpad_hint_right->h);
    GRRLIB_BMFX_FlipH(tex_dpad_hint_right, tex_dpad_hint_left);
    GRRLIB_FlushTex(tex_dpad_hint_left);

    Examples_LoadPreviewTextures();

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

        const char *str_name = "GRRLIB Examples Collection 0.1.1";
        const char *str_credits = "Press (1) for credits";

        GRRLIB_PrintfTTF(3,3, tex_font, str_name, 16, 0x000000FF);
        GRRLIB_PrintfTTF(640 - GRRLIB_WidthTTF(tex_font, str_credits, 16) - 3,3, tex_font, str_credits, 16, 0x000000FF);

        GRRLIB_PrintfTTF((640 - GRRLIB_WidthTTF(tex_font, examples[page].title, 48)) / 2, 60, tex_font, examples[page].title, 48, 0x000000FF);
        
        GRRLIB_DrawImg(25, 405, tex_dpad_hint_left, 0, 0.5, 0.5, 0xFFFFFFFF);
        GRRLIB_DrawImg(565, 405, tex_dpad_hint_right, 0, 0.5, 0.5, 0xFFFFFFFF);
       
        GRRLIB_Rectangle(160 - prvBorder, 150 - prvBorder, examples[page].preview->w + (prvBorder * 2), examples[page].preview->h + (prvBorder * 2), 0x333333FF, 1);
        GRRLIB_DrawImg(160, 150, examples[page].preview, 0, 1, 1, 0xFFFFFFFF);
        
        GRRLIB_PrintfTTF_WordWrap(100, 480-96, tex_font, 0x000000FF, 16, 440, examples[page].description);

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
    GRRLIB_FreeTTF(tex_font);

    GRRLIB_Exit();
}


int main(int argc, char **argv) {
    GRRLIB_Init();
    WPAD_Init();

    bool showFadeIn = Splash();
    Menu(showFadeIn);
    exit(0);
}
