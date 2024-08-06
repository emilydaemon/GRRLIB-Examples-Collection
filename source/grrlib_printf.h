#ifndef GRRLIB_PRINTF_H
#define GRRLIB_PRINTF_H

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <grrlib.h>

// Function to wrap text into lines of a given maximum length
void wrap_text(const char* text, char* wrapped_text, int max_length) {
    int len = strlen(text);
    int pos = 0, line_start = 0;

    while (line_start < len) {
        // Find the position of the last space within the max_length
        int wrap_pos = line_start + max_length;
        if (wrap_pos >= len) {
            wrap_pos = len;
        } else {
            while (wrap_pos > line_start && text[wrap_pos] != ' ') {
                wrap_pos--;
            }
            if (wrap_pos == line_start) {
                wrap_pos = line_start + max_length;
            }
        }

        // Copy the line to the wrapped_text buffer
        for (int i = line_start; i < wrap_pos; i++) {
            wrapped_text[pos++] = text[i];
        }

        // Add a newline character if not at the end
        if (wrap_pos < len) {
            wrapped_text[pos++] = '\n';
        }

        // Move to the start of the next line
        line_start = wrap_pos;
        while (line_start < len && text[line_start] == ' ') {
            line_start++;
        }
    }

    wrapped_text[pos] = '\0'; // Null-terminate the wrapped text
}

// Modified GRRLIB_Printf function using word wrapping
void GRRLIB_Printf_WordWrap(const f32 xpos, const f32 ypos,
                            const GRRLIB_texImg *tex, const u32 color,
                            const f32 zoom, const int max_length, const char *text, ...) {
    if (tex == NULL || tex->data == NULL) {
        return;
    }

    char tmp[1024];
    char wrapped_text[2048];
    const f32 offset = tex->tilew * zoom;

    f32 current_x = xpos;
    f32 current_y = ypos;

    va_list argp;
    va_start(argp, text);
    vsnprintf(tmp, sizeof(tmp), text, argp);
    va_end(argp);

    // Wrap the text
    wrap_text(tmp, wrapped_text, max_length);

    // Draw the wrapped text
    for (int i = 0; wrapped_text[i] != '\0'; i++) {
        if (wrapped_text[i] == '\n') {
            current_x = xpos;
            current_y += tex->tileh * zoom;
            continue;
        }
        GRRLIB_DrawTile(current_x, current_y, tex, 0, zoom, zoom, color,
                        wrapped_text[i] - tex->tilestart);
        current_x += offset;
    }
}

void GRRLIB_Printf_Newline(const f32 xpos, const f32 ypos,
                           const GRRLIB_texImg *tex, const u32 color,
                           const f32 zoom, const char *text, ...) {
    if (tex == NULL || tex->data == NULL) {
        return;
    }

    char tmp[1024];
    const f32 offset = tex->tilew * zoom;
    const f32 lineHeight = tex->tileh * zoom;

    va_list argp;
    va_start(argp, text);
    const int size = vsnprintf(tmp, sizeof(tmp), text, argp);
    va_end(argp);

    f32 currentX = xpos;
    f32 currentY = ypos;

    for (int i = 0; i < size; i++) {
        if (tmp[i] == '\n') {
            currentX = xpos;
            currentY += lineHeight;
        } else {
            GRRLIB_DrawTile(currentX, currentY, tex, 0, zoom, zoom, color,
                            tmp[i] - tex->tilestart);
            currentX += offset;
        }
    }
}

void GRRLIB_Putchar(const f32 xpos, const f32 ypos,
                    const GRRLIB_texImg *tex, const u32 color,
                    const f32 zoom, const char character) {
    if (tex == NULL || tex->data == NULL) {
        return;
    }

    GRRLIB_DrawTile(xpos, ypos, tex, 0, zoom, zoom, color, character - tex->tilestart);
}

#endif
