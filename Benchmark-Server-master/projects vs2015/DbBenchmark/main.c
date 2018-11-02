#include "main.h"

#include "mysql_interface.h"
#include "oracle_interface.h"

int main(int argc, char* argv[])
{
	
	/* Platform */
	SDL_Window *win;
	SDL_GLContext glContext;
	int win_width, win_height;
	int running = 1;

	

	/* GUI */
	struct nk_context *ctx;
	struct nk_colorf bg;
	
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
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL |SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	glContext = SDL_GL_CreateContext(win);
	SDL_GetWindowSize(win, &win_width, &win_height);
	SDL_ShowCursor(0);

	/* GUI */
	ctx = nk_sdl_init(win);

	{struct nk_font_atlas *atlas;
	nk_sdl_font_stash_begin(&atlas);
	
	struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "Roboto-Regular.ttf", 16, 0);

	nk_sdl_font_stash_end();
	nk_style_load_all_cursors(ctx, atlas->cursors);
	nk_style_set_font(ctx, &roboto->handle); }

	bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
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

			enum { EASY, HARD };
			static int op = EASY;
			static int property = 20;
			static int selected_item = 0;


			
			nk_layout_row_dynamic(ctx, 8, 1);
			nk_spacing(ctx, 2);

			nk_layout_row_dynamic(ctx, 30, 1);
			nk_label(ctx, "Database Type :", NK_TEXT_ALIGN_CENTERED);
			if (nk_combo_begin_label(ctx, database_name[selected_item], nk_vec2(nk_widget_width(ctx), 200))) {
				int i;
				nk_layout_row_dynamic(ctx, 25, 1);
				for (i = 0; i < sizeof(database_id) / sizeof(database_id[0]); ++i)
					if (nk_combo_item_label(ctx, database_name[i], NK_TEXT_LEFT)) {
						selected_item = i;
						printf("Settings style_id : %d \n", database_id[i]);
					}
				nk_combo_end(ctx);
			}


			// Pour les champs de selections de text : 
			static char input[9][64];
			static int text_len[9];
			static char box_buffer_read[512];
			static int box_len_read;
			static char box_buffer_write[512];
			static int box_len_write;
			static int checkbox_custom_script = 0;


			nk_layout_row_dynamic(ctx, 8, 1);
			nk_spacing(ctx, 2);

			nk_layout_row_dynamic(ctx, 30, 10);
			nk_label(ctx, "Hostname:", NK_TEXT_RIGHT);
			nk_edit_string(ctx, NK_EDIT_SIMPLE, input[0], &text_len[0], 64, nk_filter_default);

			nk_label(ctx, "Port:", NK_TEXT_RIGHT);
			nk_edit_string(ctx, NK_EDIT_SIMPLE, input[1], &text_len[1], 64, nk_filter_decimal);

			nk_label(ctx, "DB Name:", NK_TEXT_RIGHT);
			nk_edit_string(ctx, NK_EDIT_SIMPLE, input[2], &text_len[2], 64, nk_filter_default);

			nk_label(ctx, "User:", NK_TEXT_RIGHT);
			nk_edit_string(ctx, NK_EDIT_SIMPLE, input[3], &text_len[3], 64, nk_filter_default);

			nk_label(ctx, "Password:", NK_TEXT_RIGHT);
			{
				int i = 0;
				int old_len = text_len[4];
				char buffer[64];
				for (i = 0; i < text_len[4]; ++i) buffer[i] = '*';
				nk_edit_string(ctx, NK_EDIT_FIELD, buffer, &text_len[4], 64, nk_filter_default);
				if (old_len < text_len[4])
					memcpy(&input[4][old_len], &buffer[old_len], (nk_size)(text_len[4] - old_len));
			}



			nk_checkbox_label(ctx, "Custom script", &checkbox_custom_script);
			if (checkbox_custom_script)
			{
				nk_layout_row_static(ctx, 25, 330, 2);
				nk_label(ctx, "Read SQL:", NK_TEXT_CENTERED);
				nk_label(ctx, "Write SQL:", NK_TEXT_CENTERED);
				nk_layout_row_static(ctx, 180, 175, 4);
				nk_spacing(ctx, 1);
				nk_edit_string(ctx, NK_EDIT_BOX, box_buffer_read, &box_len_read, 512, nk_filter_default);
				nk_spacing(ctx, 1);
				nk_edit_string(ctx, NK_EDIT_BOX, box_buffer_write, &box_len_write, 512, nk_filter_default);
			}

		

			nk_layout_row_static(ctx, 50, 300, 2);
			nk_spacing(ctx, 1);
			if (nk_button_label(ctx, "Benchmark !"))
			{
				fprintf(stdout, "> benchmark pressed\n");
				fprintf(stdout, "Benchmark type : %s \n", database_name[selected_item]);
				fprintf(stdout, "Hostname : %s \n", input[0]);
				fprintf(stdout, "Port : %s \n", input[1]);
				fprintf(stdout, "DBName: %s \n", input[2]);
				fprintf(stdout, "User: %s \n", input[3]);
				fprintf(stdout, "Password: %s \n", input[4]);


				// Initialise
				if (database_name[selected_item] == "Oracle")
				{
					printf("> Init database for : Oracle \n");
					InitOracle(input[0], atoi(input[1]), input[2], input[3], input[4]);
					DoBenchmarkOracle();	
				}
				else if (database_name[selected_item] == "Mysql")
				{
					printf("> Init database for : MySql \n");
					InitMySql(input[0], atoi(input[1]) , input[2], input[3], input[4]);
					DoBenchmarkMySql();
				}
			

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