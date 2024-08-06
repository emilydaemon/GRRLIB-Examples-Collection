#pragma once
#include "Common.h"
#include <preview_particle_png.h>

/*===========================================
        GRRLIB (GX Version)
        Example code by Xane
        Ported to C by bor-real

        This example shows a basic particle
        engine creating a Smokebomb.
============================================*/

#include <grrlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ogc/lwp_watchdog.h>
#include <wiiuse/wpad.h>
#include <stdint.h>

#include "RGFX_Background_jpg.h"
#include "RGFX_Crosshair_png.h"
#include "RGFX_Smoke_png.h"
#include "RGFX_Font_png.h"

#define MAX_PARTICLES 1000

typedef enum {
    SMOKEBOMB
} Effect;

typedef struct {
    uint8_t id;
    float x, y;
    float sx, sy;
    uint16_t rot;
    uint8_t frame, framecnt, framedelay;
    uint8_t red, green, blue;
    float scale, alpha;
    float sscale, salpha;
    float scolor;
    GRRLIB_texImg *tex;
} Particle;

static GRRLIB_texImg *TextureList[4];
static Particle *ParticleList[MAX_PARTICLES];
static Particle *ParticleListTmp[MAX_PARTICLES];
static int ParticleListSize = 0;
static int ParticleListTmpSize = 0;

static void ExitGame();
static void createEffect(Effect id, int _x, int _y);
static void createParticle(uint8_t _id, int _x, int _y, float _scale, float _alpha, uint8_t _red, uint8_t _green, uint8_t _blue);
static int updateParticle(Particle *part);
static uint8_t CalculateFrameRate();
static uint8_t ClampVar8(float Value);

GRRLIB_texImg *GFX_Background;
GRRLIB_texImg *GFX_Crosshair;
GRRLIB_texImg *GFX_Smoke;
GRRLIB_texImg *GFX_Font;

static float RandomFloat() {
    return ((float)(rand() % 12) / 12.0f) - 0.5f;
}

void particle_main() {
    ir_t P1Mote;
    uint8_t FPS = 0;

    const uint16_t WinW = rmode->fbWidth;
    const uint16_t WinH = rmode->efbHeight;

    WPAD_SetIdleTimeout(60 * 10);
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    GFX_Background = GRRLIB_LoadTextureJPG(RGFX_Background_jpg);
    GFX_Crosshair  = GRRLIB_LoadTexturePNG(RGFX_Crosshair_png);
    GFX_Smoke      = GRRLIB_LoadTexturePNG(RGFX_Smoke_png);
    GFX_Font       = GRRLIB_LoadTexturePNG(RGFX_Font_png);
    GRRLIB_InitTileSet(GFX_Font, 8, 16, 32);

    GRRLIB_SetMidHandle(GFX_Crosshair, 1);
    GRRLIB_SetMidHandle(GFX_Smoke, 1);

    TextureList[0] = GFX_Background;
    TextureList[1] = GFX_Crosshair;
    TextureList[2] = GFX_Smoke;
    TextureList[3] = GFX_Font;

    while (1) {
        WPAD_ScanPads();
        uint32_t WPADKeyDown = WPAD_ButtonsDown(WPAD_CHAN_0);
        WPAD_SetVRes(WPAD_CHAN_0, WinW, WinH);
        WPAD_IR(WPAD_CHAN_0, &P1Mote);

        GRRLIB_SetBlend(GRRLIB_BLEND_ALPHA);

        int P1MX = P1Mote.sx - 150;
        int P1MY = P1Mote.sy - 150;

        GRRLIB_DrawImg(0, 0, GFX_Background, 0, 1, 1, RGBA(255, 255, 255, 255));

        if (ParticleListTmpSize > 0) {
            for (int i = 0; i < ParticleListTmpSize; i++) {
                if (ParticleListSize < MAX_PARTICLES) {
                    ParticleList[ParticleListSize++] = ParticleListTmp[i];
                } else {
                    free(ParticleListTmp[i]);
                }
            }
            ParticleListTmpSize = 0;
        }

        for (int i = 0; i < ParticleListSize; ) {
            if (updateParticle(ParticleList[i])) {
                GRRLIB_DrawImg(ParticleList[i]->x, ParticleList[i]->y, ParticleList[i]->tex, ParticleList[i]->rot, ParticleList[i]->scale, ParticleList[i]->scale, RGBA(ParticleList[i]->red, ParticleList[i]->green, ParticleList[i]->blue, ClampVar8(ParticleList[i]->alpha * 255)));
                i++;
            } else {
                free(ParticleList[i]);
                ParticleList[i] = ParticleList[--ParticleListSize];
            }
        }

        GRRLIB_DrawImg(P1MX, P1MY, GFX_Crosshair, 0, 1, 1, RGBA(255, 255, 255, 255));

        GRRLIB_Rectangle(28, 28, 292, 20, RGBA(0, 0, 0, 160), 1);
        GRRLIB_Printf(32, 32, GFX_Font, 0xFFFFFFFF, 1, "Point your Wii Remote on the screen.");
        GRRLIB_Rectangle(28, 48, 200, 16, RGBA(0, 0, 0, 160), 1);
        GRRLIB_Printf(32, 48, GFX_Font, 0xFFFFFFFF, 1, "Number of Particle: %d", ParticleListSize);
        GRRLIB_Rectangle(28, 64, 64, 16, RGBA(0, 0, 0, 160), 1);
        GRRLIB_Printf(32, 64, GFX_Font, 0xFFFFFFFF, 1, "FPS: %d", FPS);

        GRRLIB_Render();
        FPS = CalculateFrameRate();

        if (WPADKeyDown & WPAD_BUTTON_B) {
            createEffect(SMOKEBOMB, P1MX, P1MY);
        }
        if (WPADKeyDown & WPAD_BUTTON_HOME) {
            break;
        }
    }
    ExitGame();
}

static void createEffect(Effect id, int _x, int _y) {
    switch (id) {
        case SMOKEBOMB:
            for (uint8_t i = 0; i < 5; i++) {
                createParticle(1, _x + (RandomFloat() * 10), _y + (RandomFloat() * 10), (1.4f + (RandomFloat() * 0.20)), 1.0f, 64, 64, 64);
            }
            for (uint8_t i = 0; i < 20; i++) {
                createParticle(3, _x + (RandomFloat() * 50), _y + (RandomFloat() * 50), 1.25f, 1.5f, 92, 92, 92);
            }
            for (uint8_t i = 0; i < 5; i++) {
                uint8_t _ColorAdd = (RandomFloat() * 75);
                createParticle(2, _x + (RandomFloat() * 40), _y + (RandomFloat() * 40), (1.0f + (RandomFloat() * 0.20)), 1.0f, 128 + _ColorAdd, 128 + _ColorAdd, 128 + _ColorAdd);
            }
            break;
    }
}

static void createParticle(uint8_t _id, int _x, int _y, float _scale, float _alpha, uint8_t _red, uint8_t _green, uint8_t _blue) {
    Particle *part = (Particle *)malloc(sizeof(Particle));
    if (!part) return;

    part->id = _id;
    part->x = _x;
    part->y = _y;
    part->rot = rand() % 360;
    part->red = _red;
    part->green = _green;
    part->blue = _blue;
    part->scale = _scale;
    part->alpha = _alpha;

    part->tex = GFX_Smoke;
    part->sy = RandomFloat();
    part->sx = RandomFloat();
    part->sscale = 0.9985f;
    part->salpha = 0.985f;
    switch (part->id) {
        case 1:
            part->sy = RandomFloat() * 0.5f;
            part->sx = RandomFloat() * 0.5f;
            part->sscale = 0.999f;
            part->salpha = 0.992f;
            part->framedelay = 10;
            part->framecnt = 2;
            break;
        case 2:
            part->scolor = 0.98f;
            part->salpha = 0.95f;
            break;
        case 3:
            part->sy = RandomFloat() * 8.0f;
            part->sx = RandomFloat() * 8.0f;
            part->salpha = 0.85f;
            part->scolor = 0.95f;
            break;
    }
    ParticleListTmp[ParticleListTmpSize++] = part;
}

static int updateParticle(Particle *part) {
    if (part->alpha < 0.05f) { part->alpha -= 0.001f; }
    if (part->alpha < 0.1f) { part->alpha -= 0.001f; }

    part->x += part->sx;
    part->y += part->sy;
    part->scale *= part->sscale;
    part->alpha *= part->salpha;
    switch (part->id) {
        case 1:
            if (part->alpha < 0.25f) { part->alpha -= 0.001f; }
            if (part->framecnt == 0) {
                part->framecnt = 20;
                part->red -= 1;
                part->green -= 1;
                part->blue -= 1;
            }
            part->framecnt -= 1;
            break;
        case 2:
        case 3:
            part->red *= part->scolor;
            part->green *= part->scolor;
            part->blue *= part->scolor;
            break;
    }
    if (part->scale < 0 || part->alpha < 0) {
        return 0;
    }
    return 1;
}

static uint8_t ClampVar8(float Value) {
    Value = roundf(Value);
    if (Value < 0.0f) return 0;
    if (Value > 255.0f) return 255;
    return (uint8_t)Value;
}

static void FreeTextures() {
    for (int i = 0; i < 4; i++) {
        if (TextureList[i]) {
            GRRLIB_FreeTexture(TextureList[i]);
            TextureList[i] = NULL;
        }
    }
}

static void FreeParticles() {
    for (int i = 0; i < ParticleListSize; i++) {
        if (ParticleList[i]) {
            free(ParticleList[i]);
            ParticleList[i] = NULL;
        }
    }
    ParticleListSize = 0;

    for (int i = 0; i < ParticleListTmpSize; i++) {
        if (ParticleListTmp[i]) {
            free(ParticleListTmp[i]);
            ParticleListTmp[i] = NULL;
        }
    }
    ParticleListTmpSize = 0;
}

static void ExitGame() {
    FreeTextures();
    FreeParticles();
    ResetSettings(); 
}
