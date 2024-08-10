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

void GRRLIB_PrintfTTF_WordWrap(f32 xpos, f32 ypos, GRRLIB_ttfFont *font, u32 color, f32 font_size, int max_length, const char *text, ...) {
    char buffer[1024];
    char wrapped_text[2048];
    va_list args;
    va_start(args, text);
    vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);

    wrap_text(buffer, wrapped_text, max_length);

    char *line_start = wrapped_text;
    char *line_end = wrapped_text;
    f32 current_ypos = ypos;

    while (*line_end) {
        while (*line_end && *line_end != '\n') {
            line_end++;
        }

        char temp_char = *line_end;
        *line_end = '\0';

        GRRLIB_PrintfTTF(xpos, current_ypos, font, line_start, font_size, color);

        *line_end = temp_char;
        if (*line_end == '\n') {
            line_end++;
        }
        line_start = line_end;
        current_ypos += font_size;
    }
}

#endif
