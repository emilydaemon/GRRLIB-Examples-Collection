#pragma once
#include "Common.h"
#include <preview_ttf_png.h>

/*===========================================
        TrueType Font demo
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>   // Needed for gettime and ticks_to_millisecs

// Font
#include "FreeMonoBold_ttf.h"


void ttf_main() {
    bool ShowFPS = false;

    // Load the font from memory
    GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
    // Create an empty texture to store a copy of the screen
    GRRLIB_texImg *CopiedImg = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);

    // Fill a table with characters
    u32 i, n = 0;
    wchar_t charTable[460];
    for(i=33; i<=126; i++) { // 0 to 93
        charTable[n++] = i;
    }
    for(i=161; i<=518; i++) { // 94 to 451
        charTable[n++] = i;
    }
    for(i=9824; i<=9831; i++) { // 452 to 459
        charTable[n++] = i;
    }

    // Seed the random-number generator with current time so that
    // the numbers will be different every time we run.
    srand(time(NULL));

    wchar_t Letter[2] = L""; // A character + terminal NULL

    // To have a cool effect anti-aliasing is turned on
    GRRLIB_Settings.antialias = true;

    // Black background
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Screen2Texture(0, 0, CopiedImg, false);

    // Loop forever
    while(1) {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
        Letter[0] = charTable[rand() % 459];
        GRRLIB_PrintfTTFW(rand() % rmode->fbWidth - 50,
                         rand() % rmode->efbHeight - 50,
                         myFont,
                         Letter,
                         rand() % 180 + 20,
                         ((rand() % 0xFFFFFF) << 8) | 0xFF);
        GRRLIB_Screen2Texture(0, 0, CopiedImg, false);

        if(ShowFPS == true) {
            char FPS[255];
            snprintf(FPS, sizeof(FPS), "Current FPS: %d", CalculateFrameRate());
            GRRLIB_PrintfTTF(500+1, 25+1, myFont, FPS, 12, 0x000000FF);
            GRRLIB_PrintfTTF(500, 25, myFont, FPS, 12, 0xFFFFFFFF);
        }

        WPAD_ScanPads();  // Scan the Wii Remotes

        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) {
            break;
        }
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A) {
            GRRLIB_Screen2Texture(0, 0, CopiedImg, false);
        }
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_B) {
            ShowFPS = !ShowFPS;
        }
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_1 && WPAD_ButtonsHeld(0) & WPAD_BUTTON_2) {
            WPAD_Rumble(0, true);  // Rumble on
            ScreenShot();    // Needs to be after GRRLIB_Render()
            WPAD_Rumble(0, false); // Rumble off
        }

        GRRLIB_Render();  // Render the frame buffer to the TV
    }

    ResetSettings();

    GRRLIB_FreeTexture(CopiedImg);
    GRRLIB_FreeTTF(myFont);
}