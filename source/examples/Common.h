// these are identical functions used in multiple examples
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <ogc/lwp_watchdog.h>

/**
 * This function calculates the number of frames we render each second.
 * @return The number of frames per second.
 */
static u8 CalculateFrameRate(void) {
    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    const u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}

/**
 * Create a PNG screenshot on the root of the SD card with a timestamp.
 * @return bool true=everything worked, false=problems occurred.
 */
static bool ScreenShot(void) {
    char path[255];
    time_t now = time(NULL);
    struct tm *ti = localtime(&now);
    snprintf(path, sizeof(path), "sd:/grrlib_ttf_%d%02d%02d%02d%02d%02d.png",
        ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
    return GRRLIB_ScrShot(path);
}

void ResetSettings() {
    GRRLIB_Settings.antialias = false;
    GRRLIB_SetBackgroundColour(250, 250, 250, 255);
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
}

#endif