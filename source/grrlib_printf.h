#ifndef GRRLIB_PRINTF_H
#define GRRLIB_PRINTF_H

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <grrlib.h>

void GRRLIB_PrintfTTF_WordWrap(f32 xpos, f32 ypos, GRRLIB_ttfFont *font, u32 color, f32 font_size, u32 max_pixel_width, const char *format, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    const char *text = buffer;
    const char *line_start = text;
    f32 current_ypos = ypos;

    while (*line_start) {
        const char *line_end = line_start;
        const char *word_end = line_start;
        u32 line_width = 0;
        int is_first_word = 1;

        while (*word_end && *word_end != '\n') {
            while (*word_end == ' ') {
                word_end++;
            }

            const char *word_start = word_end;

            while (*word_end && *word_end != ' ' && *word_end != '\n') {
                word_end++;
            }

            size_t word_len = word_end - word_start;
            if (word_len == 0) {
                break;
            }

            char word_buffer[256];
            if (word_len >= sizeof(word_buffer)) {
                word_len = sizeof(word_buffer) - 1;
            }
            strncpy(word_buffer, word_start, word_len);
            word_buffer[word_len] = '\0';

            u32 word_width = GRRLIB_WidthTTF(font, word_buffer, font_size);
            u32 space_width = GRRLIB_WidthTTF(font, " ", font_size);

            if (is_first_word) {
                if (word_width > max_pixel_width) {
                    line_end = word_end;
                    line_width = word_width;
                } else {
                    line_width = word_width;
                    line_end = word_end;
                }
                is_first_word = 0;
            } else {
                if (line_width + space_width + word_width <= max_pixel_width) {
                    line_width += space_width + word_width;
                    line_end = word_end;
                } else {
                    break;
                }
            }

            while (*word_end == ' ') {
                word_end++;
            }
        }

        if (*word_end == '\n') {
            line_end = word_end;
        }

        size_t line_len = line_end - line_start;
        if (line_len >= sizeof(buffer)) {
            line_len = sizeof(buffer) - 1;
        }
        char line_buffer[1024];
        strncpy(line_buffer, line_start, line_len);
        line_buffer[line_len] = '\0';

        char *trim_ptr = line_buffer + line_len - 1;
        while (trim_ptr >= line_buffer && *trim_ptr == ' ') {
            *trim_ptr = '\0';
            trim_ptr--;
        }

        GRRLIB_PrintfTTF(xpos, current_ypos, font, line_buffer, font_size, color);
        current_ypos += font_size;

        if (*line_end == '\n') {
            line_start = line_end + 1;
        } else {
            line_start = line_end;
        }

        while (*line_start == ' ') {
            line_start++;
        }
    }
}

#endif
