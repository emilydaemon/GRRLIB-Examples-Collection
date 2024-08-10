#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <stddef.h>
#include <grrlib.h>

#include <preview_none_png.h>

#include "examples/3D_CubedTileDemo.h"
#include "examples/3D_Light1.h"
#include "examples/3D_Light2.h"
#include "examples/3D_Light3.h"
#include "examples/3D_Light4.h"
#include "examples/3D_sample1.h"
#include "examples/3D_sample2.h"
#include "examples/3D_sample3.h"
#include "examples/3D_sample4.h"
#include "examples/3D_sample5.h"
#include "examples/basic_drawing.h"
#include "examples/bitmap_fx.h"
#include "examples/blending.h"
#include "examples/compositing.h"
#include "examples/funsin.h"
#include "examples/NEED_GRRLIB_FUNCTION.h"
#include "examples/particle.h"
#include "examples/TileDemo.h"
#include "examples/ttf.h"

typedef void (*entryPoint)(void);

// Define the struct
typedef struct {
    char *title;
    char *description;
    entryPoint entry;
    GRRLIB_texImg *preview;
} Example;

Example examples[] = {
    {"3D_CubedTileDemo", "3D_CubedTileDemo is TileDemo but it's a rotating cube.", 
    ThreeD_CubedTileDemo_main},
    {"3D_Light1", "3D_Light1 uses a rotating torus and two orbs to show GRRLIB lighting.", 
    ThreeD_LightOne_main},
    {"3D_Light2", "3D_Light2 uses 8 lit balls around the vertices of a rotating cube to demonstrate advanced GRRLIB lighting.", 
    ThreeD_LightTwo_main},
    {"3D_Light3", "3D_Light3 has cones and a cube with a configurable shinyness value.", 
    ThreeD_LightThree_main},
    {"3D_Light4", "3D_Light4 uses moving red and light spotlights to show the spot lighting capabilities of GRRLIB.", 
    ThreeD_LightFour_main},
    {"3D_sample1", "3D_sample1 has a rotating 3D cube as an example of basic 3D rendering.", 
    ThreeD_sampleOne_main},
    {"3D_sample2", "3D_sample2 has a rotating 3D cube but with textures.", 
    ThreeD_sampleTwo_main},
    {"3D_sample3", "3D_sample3 has a textured cube with a sine wave distortion effect.", 
    ThreeD_sampleThree_main},
    {"3D_sample4", "3D_sample4 displays a spinning 3D model of the GRRLIB logo.", 
    ThreeD_sampleFour_main},
    {"3D_sample5", "3D_sample5 has 8 rotating cubes with flat shading in a ring.", 
    ThreeD_sampleFive_main},
    {"basic_drawing", "basic_drawing shows the text, sprite, and image rendering of GRRLIB on multiple pages.", 
    basic_drawing_main},
    {"bitmap_fx", "bitmap_fx shows the various effects you can apply to bitmaps on multiple pages.", 
    bitmap_fx_main},
    {"blending", "blending demonstrates GRRLIB's various blending modes with a blob you can move with the Wii Remote.", 
    blending_main},
    {"compositing", "compositing uses GRRLIB's compositing modes to change the colors of text in a circle.", 
    compositing_main},
    {"funsin", "funsin displays a dynamic sinusoidal wave pattern.", 
    funsin_main},
    {"NEED_GRRLIB_FUNCTION", "NEED_GRRLIB_FUNCTION shows effects using a 3D torus, including diffuse, specular, and environmental mapping.", 
    NEED_GRRLIB_FUNCTION_main},
    {"particle", "particle lets you shoot smoke particles at the screen using the Wii remote.", 
    particle_main},
    {"TileDemo", "TileDemo demonstrates basic tile-based graphics with character movement and background rendering.", 
    TileDemo_main},
    {"ttf", "ttf shows off the TTF loading features of GRRLIB with a random display of letters and colors!", 
    ttf_main}
};

// Array of texture data pointers
const u8 *ExamplesTextures[] = {
    preview_3D_CubedTileDemo_png,
    preview_3D_Light1_png,
    preview_3D_Light2_png,
    preview_3D_Light3_png,
    preview_3D_Light4_png,
    preview_3D_sample1_png,
    preview_3D_sample2_png,
    preview_3D_sample3_png,
    preview_3D_sample4_png,
    preview_3D_sample5_png,
    preview_basic_drawing_png,
    preview_bitmap_fx_png,
    preview_blending_png,
    preview_compositing_png,
    preview_funsin_png,
    preview_NEED_GRRLIB_FUNCTION_png,
    preview_particle_png,
    preview_TileDemo_png,
    preview_ttf_png,
};

// Function to initialize the previews
void Examples_LoadPreviewTextures() {
    for (size_t i = 0; i < sizeof(examples) / sizeof(Example); i++) {
        if (i < sizeof(ExamplesTextures) / sizeof(ExamplesTextures[0]) && ExamplesTextures[i] != NULL) {
            examples[i].preview = GRRLIB_LoadTexture(ExamplesTextures[i]);
        } else {
            examples[i].preview = GRRLIB_LoadTexture(preview_none_png);
        }
    }
}
#endif // EXAMPLES_H
