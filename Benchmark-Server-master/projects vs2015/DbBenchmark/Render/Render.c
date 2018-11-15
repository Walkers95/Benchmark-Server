#include "Render.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 476

// SDL Include
#include "../Library\SDL2-2.0.8\include\SDL.h"
#include "../Library\SDL2-2.0.8\include\SDL_opengl.h"

// Include for Nuklear
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL2_IMPLEMENTATION
#include "../Library\nuklear.h"
#include "../Library\nuklear_sdl_gl2.h"


int database_id[] = {
	0,
	1,
	2
};

const char* database_name[] = {
	"Oracle",
	"Mysql",
	"MongoDB",
};

/* Platform */
SDL_Window *win;
SDL_GLContext glContext;
int win_width, win_height;
int running = 1;

/* GUI */
struct nk_context *ctx;
struct nk_colorf bg;

void InitialiseRender()
{

	/* SDL setup */
	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	win = SDL_CreateWindow("Demo",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	glContext = SDL_GL_CreateContext(win);
	SDL_GetWindowSize(win, &win_width, &win_height);
	SDL_ShowCursor(0);

	/* GUI */
	ctx = nk_sdl_init(win);

	{struct nk_font_atlas *atlas;
	nk_sdl_font_stash_begin(&atlas);

	struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "Fonts/Roboto-Regular.ttf", 16, 0);

	nk_sdl_font_stash_end();
	nk_style_load_all_cursors(ctx, atlas->cursors);
	nk_style_set_font(ctx, &roboto->handle); }

	bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
}

void Render()
{

	while (running)
	{
		/* Input */
		SDL_Event evt;
		nk_input_begin(ctx);
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) goto cleanup;
			nk_sdl_handle_event(&evt);
		}
		nk_input_end(ctx);
		SetStyle(ctx);
		/* GUI */
		if (nk_begin(ctx, "Demo", nk_rect(0, 0, win_width, win_height), NK_WINDOW_TITLE))
		{

			static int selected_tab = 0;

			nk_layout_row_dynamic(ctx, 10, 1);
			nk_spacing(ctx, 1);

			nk_layout_row_dynamic(ctx, 30, 3);
			if (nk_button_label(ctx, "Configuration"))
			{
				selected_tab = 0;
			}
			if (nk_button_label(ctx, "Console"))
			{
				selected_tab = 1;
			}
			if (nk_button_label(ctx, "Results Chart"))
			{
				selected_tab = 2;
			}

			nk_layout_row_dynamic(ctx, 8, 1);
			nk_spacing(ctx, 2);


			// Configuration Draw
			if (selected_tab == 0)
			{
				DrawConfigurationTab(ctx);
			}

			// Chart Draw
			if (selected_tab == 1)
			{
				DrawConsoleTab(ctx);
			}

			// Other Draw
			if (selected_tab == 2)
			{
				DrawChartTab(ctx);
			}


		}
		nk_end(ctx);

		/* Draw */
		SDL_GetWindowSize(win, &win_width, &win_height);
		glViewport(0, 0, win_width, win_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(bg.r, bg.g, bg.b, bg.a);
		/* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
		* with blending, scissor, face culling, depth test and viewport and
		* defaults everything back into a default state.
		* Make sure to either a.) save and restore or b.) reset your own state after
		* rendering the UI. */
		nk_sdl_render(NK_ANTI_ALIASING_ON);
		SDL_GL_SwapWindow(win);

	}

cleanup:
	nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

void DrawConfigurationTab()
{
	// Pour les champs de selections de text : 
	static char input[4][64];
	static int text_len[4];
	static char box_buffer_read[512];
	static int box_len_read;
	static char box_buffer_write[512];
	static int box_len_write;
	static int checkbox_threads = 0;
	static int checkbox_ping = 0;
	static int checkbox_custom_script = 0;
	static int selected_database = 0;


	nk_layout_row_dynamic(ctx, 15, 7);
	nk_spacing(ctx, 1);
	nk_checkbox_label(ctx, "Custom script", &checkbox_custom_script);
	nk_spacing(ctx, 1);
	nk_checkbox_label(ctx, "Multi-Threads", &checkbox_threads);
	nk_spacing(ctx, 1);
	nk_checkbox_label(ctx, "Ping Compensation", &checkbox_ping);
	nk_spacing(ctx, 1);

	if (checkbox_custom_script)
	{
		nk_layout_row_static(ctx, 25, WINDOW_WIDTH/3, 2);
		nk_label(ctx, "Read SQL file : ", NK_TEXT_LEFT);
		nk_label(ctx, "Write SQL file : ", NK_TEXT_LEFT);

		nk_layout_row_static(ctx, 25, WINDOW_WIDTH / 3, 2);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, box_buffer_read, &box_len_read, 512, nk_filter_default);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, box_buffer_write, &box_len_write, 512, nk_filter_default);
	}


	nk_layout_row_dynamic(ctx, 4, 1);
	nk_spacing(ctx, 2);


	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Database Type :", NK_TEXT_ALIGN_LEFT);
	if (nk_combo_begin_label(ctx, database_name[selected_database], nk_vec2(nk_widget_width(ctx), 200))) {
		int i;
		nk_layout_row_dynamic(ctx, 25, 1);
		for (i = 0; i < sizeof(database_id) / sizeof(database_id[0]); ++i)
			if (nk_combo_item_label(ctx, database_name[i], NK_TEXT_LEFT)) {
				selected_database = i;
				printf("Settings style_id : %d \n", database_id[i]);
			}
		nk_combo_end(ctx);
	}


	nk_layout_row_dynamic(ctx, 8, 1);
	nk_spacing(ctx, 2);

	nk_layout_row_dynamic(ctx, 30, 8);
	nk_label(ctx, "Hostname :", NK_TEXT_CENTERED);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, input[0], &text_len[0], 64, nk_filter_default);

	nk_label(ctx, "Port :", NK_TEXT_CENTERED);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, input[1], &text_len[1], 64, nk_filter_decimal);

	nk_label(ctx, "User :", NK_TEXT_CENTERED);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, input[2], &text_len[2], 64, nk_filter_default);

	nk_label(ctx, "Password :", NK_TEXT_CENTERED);
	{
		int i = 0;
		int old_len = text_len[3];
		char buffer[64];
		for (i = 0; i < text_len[3]; ++i) buffer[i] = '*';
		nk_edit_string(ctx, NK_EDIT_FIELD, buffer, &text_len[3], 64, nk_filter_default);
		if (old_len < text_len[3])
			memcpy(&input[3][old_len], &buffer[old_len], (nk_size)(text_len[3] - old_len));
	}

	nk_layout_row_dynamic(ctx, 15, 1);
	nk_spacing(ctx, 1);

	nk_layout_row_dynamic(ctx, 30, 3);
	nk_spacing(ctx, 1);
	if (nk_button_label(ctx, "Benchmark !"))
	{
		strcpy(input[0], "localhost");
		strcpy(input[2], "root");

		fprintf(stdout, "> benchmark pressed\n");
		fprintf(stdout, "Benchmark type : %s \n", database_name[selected_database]);
		fprintf(stdout, "Hostname : %s \n", input[0]);
		fprintf(stdout, "Port : %s \n", input[1]);
		fprintf(stdout, "User: %s \n", input[2]);
		fprintf(stdout, "Password: %s \n", input[3]);

		// Initialisation de la structure avec les parametres
		struct database_params db_param = {
			.hostname = input[0],
			.port = atoi(input[1]),
			.user = input[2],
			.password = input[3]
		};

		// Initialise
		if (database_name[selected_database] == "Oracle")
		{
			printf("> Init database for : Oracle \n");
			InitOracle(&db_param);
			DoBenchmarkOracle();
		}
		else if (database_name[selected_database] == "Mysql")
		{
			printf("> Init database for : MySql \n");
			InitMySql(&db_param);
			DoBenchmarkMySql();
		}
	}
	nk_spacing(ctx, 1);
}

void DrawChartTab()
{
	nk_label(ctx, "No benchmark has been launched yet !", NK_TEXT_CENTERED);

	float id = 0;
	int i;
	struct nk_rect bounds;
	float step = (2 * 3.141592654f) / 32;

	/* mixed colored chart */
	nk_layout_row_dynamic(ctx, WINDOW_HEIGHT * 0.6, 1);
	bounds = nk_widget_bounds(ctx);
	if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0, 0), nk_rgb(150, 0, 0), 32, 0.0f, 1.0f)) {
		nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 0, 255), nk_rgb(0, 0, 150), 32, -1.0f, 1.0f);
		nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 255, 0), nk_rgb(0, 150, 0), 32, -1.0f, 1.0f);
		for (id = 0, i = 0; i < 32; ++i) {
			nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
			nk_chart_push_slot(ctx, (float)cos(id), 1);
			nk_chart_push_slot(ctx, (float)sin(id), 2);
			id += step;
		}
	}
	nk_chart_end(ctx);
}


void DrawConsoleTab()
{
	nk_label(ctx, "Console In dev !", NK_TEXT_CENTERED);

	static char box_buffer_read[512];
	static int box_len_read;

	nk_layout_row_dynamic(ctx, WINDOW_HEIGHT * 0.6, 1);
	nk_edit_string(ctx, NK_EDIT_BOX, box_buffer_read, &box_len_read, 512, nk_filter_everything);
}





void SetStyle()
{
	struct nk_color table[NK_COLOR_COUNT];
	table[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
	table[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
	table[NK_COLOR_HEADER] = nk_rgba(51, 51, 56, 220);
	table[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
	table[NK_COLOR_BUTTON] = nk_rgba(48, 83, 111, 255);
	table[NK_COLOR_BUTTON_HOVER] = nk_rgba(58, 93, 121, 255);
	table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(63, 98, 126, 255);
	table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
	table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
	table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
	table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
	table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
	table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
	table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 83, 111, 245);
	table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
	table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
	table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
	table[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
	table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
	table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
	table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
	table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
	table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
	table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
	table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
	table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
	nk_style_from_table(ctx, table);
}