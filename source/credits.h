#ifndef CREDITS_H
#define CREDITS_H

#include <grrlib.h>
#include <credits_logo_png.h>
#include <credits_text_png.h>

void credits() {
    GRRLIB_texImg *credits_text = GRRLIB_LoadTexture(credits_text_png);
    GRRLIB_texImg *credits_logo = GRRLIB_LoadTexture(credits_logo_png);

    f32 scrollPos = 0.0f;

    while(1) {
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_1) break;

        GRRLIB_DrawImg(0, 480 - scrollPos, credits_text, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(0, 0, credits_logo, 0, 1, 1, 0xFFFFFFFF);

        scrollPos += 0.5f;
        if (scrollPos > (credits_text->h + credits_logo->h)) scrollPos = 0;

        GRRLIB_Render();
    }

    GRRLIB_FreeTexture(credits_logo);
    GRRLIB_FreeTexture(credits_text);
}

#endif