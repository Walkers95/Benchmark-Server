#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 476

#include <SDL.h>
#include <SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL2_IMPLEMENTATION
#include "Library\nuklear.h"

#include "Library\nuklear_sdl_gl2.h"



int style_id[] = {
	0,
	1,
	2
};

const char* style_name[] = {
	"Oracle",
	"Mysql",
	"MongoDB",
};

void SetStyle(struct nk_context *ctx)
{
	struct nk_color table[NK_COLOR_COUNT];
	table[NK_COLOR_TEXT] = nk_rgba(20, 20, 20, 255);
	table[NK_COLOR_WINDOW] = nk_rgba(202, 212, 214, 215);
	table[NK_COLOR_HEADER] = nk_rgba(137, 182, 224, 220);
	table[NK_COLOR_BORDER] = nk_rgba(140, 159, 173, 255);
	table[NK_COLOR_BUTTON] = nk_rgba(137, 182, 224, 255);
	table[NK_COLOR_BUTTON_HOVER] = nk_rgba(142, 187, 229, 255);
	table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(147, 192, 234, 255);
	table[NK_COLOR_TOGGLE] = nk_rgba(177, 210, 210, 255);
	table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(182, 215, 215, 255);
	table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(137, 182, 224, 255);
	table[NK_COLOR_SELECT] = nk_rgba(177, 210, 210, 255);
	table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(137, 182, 224, 255);
	table[NK_COLOR_SLIDER] = nk_rgba(177, 210, 210, 255);
	table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(137, 182, 224, 245);
	table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(142, 188, 229, 255);
	table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(147, 193, 234, 255);
	table[NK_COLOR_PROPERTY] = nk_rgba(210, 210, 210, 255);
	table[NK_COLOR_EDIT] = nk_rgba(210, 210, 210, 225);
	table[NK_COLOR_EDIT_CURSOR] = nk_rgba(20, 20, 20, 255);
	table[NK_COLOR_COMBO] = nk_rgba(210, 210, 210, 255);
	table[NK_COLOR_CHART] = nk_rgba(210, 210, 210, 255);
	table[NK_COLOR_CHART_COLOR] = nk_rgba(137, 182, 224, 255);
	table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
	table[NK_COLOR_SCROLLBAR] = nk_rgba(190, 200, 200, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
	table[NK_COLOR_TAB_HEADER] = nk_rgba(156, 193, 220, 255);
	nk_style_from_table(ctx, table);
}