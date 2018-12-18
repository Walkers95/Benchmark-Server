
#include "Render.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512

// SDL Include
#include "../Library\SDL2-2.0.8\include\SDL.h"
#include "../Library\SDL2-2.0.8\include\SDL_opengl.h"
#include "../Library/SDL2-2.0.8/include/SDL_thread.h"

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
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL  | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	glContext = SDL_GL_CreateContext(win);
	SDL_GetWindowSize(win, &win_width, &win_height);


	/* GUI */
	ctx = nk_sdl_init(win);

	{struct nk_font_atlas *atlas;
	nk_sdl_font_stash_begin(&atlas);

	struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "Fonts/Roboto-Regular.ttf", 16, 0);

	nk_sdl_font_stash_end();
	nk_style_load_all_cursors(ctx, atlas->cursors);
	nk_style_set_font(ctx, &roboto->handle); }

	SetStyle(ctx);
	
	bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
}


static int selected_tab = 0;
static int is_logged_in = 0;

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
		
		/* GUI */
		if (nk_begin(ctx, "Demo", nk_rect(0, 0, win_width, win_height), NK_WINDOW_BACKGROUND  ))
		{

			nk_layout_row_dynamic(ctx, 10, 1);
			nk_spacing(ctx, 1);


			if (!is_logged_in)
			{
				DrawLoginTab();
			}
			else
			{

				nk_layout_row_dynamic(ctx, 30, 4);
				if (nk_button_label(ctx, "Configuration"))
				{
					selected_tab = 0;
				}
				if (nk_button_label(ctx, "My account"))
				{
					selected_tab = 1;
				}
				if (nk_button_label(ctx, "Console"))
				{
					selected_tab = 2;
				}
				if (nk_button_label(ctx, "Results Chart"))
				{
					selected_tab = 3;
				}

				nk_layout_row_dynamic(ctx, 8, 1);
				nk_spacing(ctx, 2);


				// Configuration Draw
				if (selected_tab == 0)
				{
					if (SDL_ShowCursor(0));
					DrawConfigurationTab(ctx);
				}

				// account Draw
				if (selected_tab == 1)
				{
					if (!SDL_ShowCursor(1));
					DrawAccountTab();
				}

				// Console Draw
				if (selected_tab == 2)
				{
					if (!SDL_ShowCursor(1));
					DrawConsoleTab(ctx);
				}

				// Results Draw
				if (selected_tab == 3)
				{
					if (SDL_ShowCursor(0));
					DrawResultsTab(ctx);
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

int *selectedBenchmark = NULL;
int show_message_error = 0;

void DrawLoginTab() 
{
	if (SDL_ShowCursor(0));

	static char input[2][64];
	static int text_len[2];
	

	nk_layout_row_dynamic(ctx, (WINDOW_HEIGHT / 9), 1);
	nk_spacing(ctx, 1);

	if (show_message_error)
	{
		nk_layout_row_dynamic(ctx, 30, 1);
		nk_labelf_colored(ctx, NK_TEXT_CENTERED, nk_rgb(255, 0, 0), "Wrong user or password !");
	}

	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "User :", NK_TEXT_CENTERED);

	nk_layout_row_dynamic(ctx, 25, 3);
	nk_spacing(ctx, 1);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, input[0], &text_len[0], 64, nk_filter_default);
	nk_spacing(ctx, 1);

	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Password :", NK_TEXT_CENTERED);

	nk_layout_row_dynamic(ctx, 25, 3);
	nk_spacing(ctx, 1);
	int i = 0;
	int old_len = text_len[1];
	char buffer[64];
	for (i = 0; i < text_len[1]; ++i) buffer[i] = '*';
	nk_edit_string(ctx, NK_EDIT_FIELD, buffer, &text_len[1], 64, nk_filter_default);
	if (old_len < text_len[1])
		memcpy(&input[1][old_len], &buffer[old_len], (nk_size)(text_len[1] - old_len));
	nk_spacing(ctx, 1);


	nk_layout_row_dynamic(ctx, 30, 3);
	nk_spacing(ctx, 1);
	if (nk_button_label(ctx, "Login"))
	{
		struct database_login_params *db_login = Malloc(sizeof(struct database_login_params));
		db_login->user = Malloc(255);
		db_login->password = Malloc(255);

		strncpy(db_login->user, input[0], text_len[0]);
		strncpy(db_login->password, input[1], text_len[1]);


		if (LoginUser(db_login))
		{
			is_logged_in = 1;
			selectedBenchmark = Malloc(sizeof(int) * 999 );
		}
		else
		{
			show_message_error = 1;
		}

		free(db_login);
		
	}
	nk_spacing(ctx, 1);
}




struct database_benchmark_params db_param_buffer;
struct database_benchmark_params *db_Param;

void DrawConfigurationTab()
{
	// Pour les champs de selections de text : 
	static char input[5][64];
	static int text_len[5];
	static char box_buffer_read[512];
	static int box_len_read;
	static char box_buffer_write[512];
	static int box_len_write;
	static int checkbox_threads = 0;
	static int checkbox_ping = 0;
	static int checkbox_custom_script = 0;
	static int selected_database = 0;
	static int request_number = 0;


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
		nk_layout_row_dynamic(ctx, 25, 2);
		nk_label(ctx, "Read SQL file : ", NK_TEXT_CENTERED);
		nk_label(ctx, "Write SQL file : ", NK_TEXT_CENTERED);

		nk_layout_row_dynamic(ctx, 25, 2);
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
			}
		nk_combo_end(ctx);
	}


	nk_layout_row_dynamic(ctx, 8, 1);
	nk_spacing(ctx, 2);

	nk_layout_row_dynamic(ctx, 30, 10);
	nk_label(ctx, "Hostname :", NK_TEXT_CENTERED);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, input[0], &text_len[0], 64, nk_filter_default);

	nk_label(ctx, "Port :", NK_TEXT_CENTERED);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, input[1], &text_len[1], 64, nk_filter_decimal);

	nk_label(ctx, "Database :", NK_TEXT_CENTERED);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, input[2], &text_len[2], 64, nk_filter_default);

	nk_label(ctx, "User :", NK_TEXT_CENTERED);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, input[3], &text_len[3], 64, nk_filter_default);

	nk_label(ctx, "Password :", NK_TEXT_CENTERED);
	{
		int i = 0;
		int old_len = text_len[4];
		char buffer[64];
		for (i = 0; i < text_len[4]; ++i) buffer[i] = '*';
		nk_edit_string(ctx, NK_EDIT_FIELD, buffer, &text_len[4], 64, nk_filter_default);
		if (old_len < text_len[4])
			memcpy(&input[4][old_len], &buffer[old_len], (nk_size)(text_len[4] - old_len));
	}

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_spacing(ctx, 1);


	nk_labelf(ctx, NK_TEXT_CENTERED, "Number of request : %d ", request_number);
	nk_layout_row_dynamic(ctx, 25, 1);
	nk_progress(ctx, &request_number, 1000, 1);

	nk_layout_row_dynamic(ctx, 15, 1);
	nk_spacing(ctx, 1);

	nk_layout_row_dynamic(ctx, 30, 3);
	nk_spacing(ctx, 1);
	if (nk_button_label(ctx, "Benchmark !"))
	{
		selected_tab = 2;

		// Debug
		strcpy(input[0], "localhost"); //www.db4free.net
		strcpy(input[1], "0");
		strcpy(input[2], "testdb");			// database
		strcpy(input[3], "root");
		strcpy(input[4], "");

		system("cls");
		fprintf(stdout, "> benchmark pressed\n");
		fprintf(stdout, "Benchmark type : %s \n", database_name[selected_database]);
		fprintf(stdout, "Hostname : %s \n", input[0]);
		fprintf(stdout, "Port : %s \n", input[1]);
		fprintf(stdout, "Database : %s \n", input[2]);
		fprintf(stdout, "User: %s \n", input[3]);
		fprintf(stdout, "Password: %s \n", input[4]);
		fprintf(stdout, "ping comp: %d \n", checkbox_ping);
		fprintf(stdout, "request number : %d \n", request_number);
		fprintf(stdout, "custom script : %d \n", checkbox_custom_script);
		fprintf(stdout, "multi threads : %d \n", checkbox_threads);
		

		// Initialisation de la structure avec les parametres
		db_param_buffer.hostname = input[0];
		db_param_buffer.port = atoi(input[1]);
		db_param_buffer.database = input[2];
		db_param_buffer.user = input[3];
		db_param_buffer.password = input[4];
		db_param_buffer.pingCompensation = checkbox_ping;
		db_param_buffer.request_number = request_number;
		db_param_buffer.custom_script = checkbox_custom_script;
		db_param_buffer.multi_threads = checkbox_threads;

		if (checkbox_custom_script)
		{
			db_param_buffer.script_read = "SELECT 1";// LoadTextFromFile(box_buffer_read);
			db_param_buffer.scrit_write = "INSERT INTO testtable(int_test, text_test) VALUES(2,'nn')";// LoadTextFromFile(box_buffer_write);
		}
		
		db_Param = Malloc(sizeof(db_Param));
		db_Param = &db_param_buffer;
		
		StartBenchmarkThread(db_Param, database_name[selected_database]);

	}
	nk_spacing(ctx, 1);
}

int selected_item = -1;

void DrawAccountTab()
{

	if (selected_item != -1)
	{
		nk_layout_row_dynamic(ctx, 25, 3);
		if (nk_button_label(ctx, "Download"))
		{
			struct database_current_results* current_results = Malloc(sizeof(struct database_current_results));
			current_results->request_number = GetUserBenchmarkData()[selected_item]->db_param->request_number;
			current_results->results = GetResultsUserData(GetUserBenchmarkData()[selected_item]->id);
			SaveJsonBenchmarkResults(FormatJsonBenchmarkResults(current_results));
			printf("Downloading %d benchmark \n", selected_item);
		}

		if (nk_button_label(ctx, "View"))
		{
			struct database_current_results* current_results = Malloc(sizeof(struct database_current_results));
			current_results->request_number = GetUserBenchmarkData()[selected_item]->db_param->request_number;
			current_results->results = GetResultsUserData(GetUserBenchmarkData()[selected_item]->id);
			current_results->score = GetResultsUserScore(GetUserBenchmarkData()[selected_item]->id);
			SetCurrentResults(current_results);
			printf("Viewing %d benchmark \n", selected_item);
			selected_tab = 3;
		}

		if (nk_button_label(ctx, "Delete"))
		{
			DeleteUserBenchmark(GetUserBenchmarkData()[selected_item]->id);
			UpdateUserBenchmarkData();
			printf("Deleting %d benchmark \n", GetUserBenchmarkData()[selected_item]->id);
		}

	}


	nk_layout_row_dynamic(ctx, 5, 1);
	nk_spacing(ctx, 1);

	// Peut pas scroller
	nk_layout_row_static(ctx, WINDOW_HEIGHT * 0.67, WINDOW_WIDTH / 2.02, 2);
	if (nk_group_begin_titled(ctx, "Group_Without_Border", "Your benchmark :", NK_WINDOW_TITLE)) {
		

		nk_layout_row_dynamic(ctx, 25, 1);
		for (int i = 0; i < GetUserBenchmarkCount(); i++)
		{
			char buffer[255];
			struct database_user_records* user_records = GetUserBenchmarkData()[i];
			// ajouter score
			sprintf(buffer, "ID(%d) %s : %s (%s,%d,%s,%s) with %d requests", 
				user_records->id,
				user_records->date,
				user_records->databaseType,
				user_records->db_param->hostname,
				user_records->db_param->port,
				user_records->db_param->user,
				user_records->db_param->database,
				user_records->db_param->request_number
				);

			if (nk_selectable_label(ctx, buffer, NK_TEXT_ALIGN_LEFT, &selectedBenchmark[i]))
			{
				printf("Select item %d \n", i);
				selected_item = i;

				memset(selectedBenchmark, 0x00, GetUserBenchmarkCount() * sizeof(int));
				selectedBenchmark[i] = 1;
			}
			
		}

		nk_layout_row_static(ctx, WINDOW_HEIGHT * 20, WINDOW_WIDTH * 0.7, 1);
		nk_group_end(ctx);
	}

	
	if (nk_group_begin_titled(ctx, "Group_Without_Border", "Benchmark results :", NK_WINDOW_TITLE)) {


		if (selected_item != -1)
		{
			struct database_user_records* records = GetUserBenchmarkData()[selected_item];
			
			int request_number = GetUserBenchmarkData()[selected_item]->db_param->request_number;
			double** results = GetResultsUserData(GetUserBenchmarkData()[selected_item]->id);

		
			for (int i=0;i<request_number;i++)
			{
				nk_layout_row_dynamic(ctx, 25, 1);
				char label[255];
				sprintf(label, "Read : %lf  | Write : %lf ", results[1][i], results[0][i]);

				nk_label(ctx, label, NK_TEXT_ALIGN_LEFT);
				
			}
			
		}
		
		nk_layout_row_static(ctx, WINDOW_HEIGHT * 20, WINDOW_WIDTH * 0.7, 1);
		nk_group_end(ctx);
	}
	
	
}

void DrawConsoleTab()
{

	char *box_buffer_read;
	int box_len_read;

	// Get data from console
	struct console_data *consData = GetConsoleData();

	static struct nk_color color;
	switch (consData->type)
	{
	case C_ERROR:
		color = (struct nk_color){ 255,0,0,255 };
		break;
	case C_DEBUG:
		color = (struct nk_color) { 255, 255, 0, 255 };
		break;
	case C_SUCCESS:
		color = (struct nk_color) { 0, 255, 0, 255 };
		break;
	}

	box_buffer_read = consData->text;
	box_len_read = consData->length;


	nk_layout_row_dynamic(ctx, WINDOW_HEIGHT * 0.8, 1);
	if (nk_group_begin_titled(ctx, "Group_Without_Border","Output console :", NK_WINDOW_TITLE)) {
		nk_layout_row_static(ctx, WINDOW_HEIGHT * 20, WINDOW_WIDTH * 0.8, 1);
		nk_label_colored_multiline(ctx, box_buffer_read, color);
		nk_group_end(ctx);
	}
}



void DrawResultsTab()
{

	int i;
	struct nk_rect bounds;

	struct database_current_results* current_results = GetCurrentResults();

	if (current_results != NULL)
	{

		nk_layout_row_dynamic(ctx, 25, 3);
		nk_label_colored(ctx, "Read", NK_TEXT_CENTERED, nk_rgb(0, 131, 255));
		nk_labelf_colored(ctx, NK_TEXT_CENTERED, nk_rgb(255, 255, 0), "Score : %.0f ", current_results->score);
		nk_label_colored(ctx, "Write", NK_TEXT_CENTERED, nk_rgb(255, 0, 0));


		double write_selection = 0.00f;
		double read_selection = 0.00f;


		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
			printf("chartMinValue : %lf \n", current_results->minValue);


		/* mixed colored chart */
		nk_layout_row_dynamic(ctx, WINDOW_HEIGHT * 0.7, 1);
		bounds = nk_widget_bounds(ctx);
		if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0, 0), nk_rgb(150, 0, 0), current_results->request_number, current_results->minValue, current_results->maxValue)) { // Change  0.0f, 10.0f to 0.00f, GetMaxValue
			nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 131, 255), nk_rgb(0, 0, 140), current_results->request_number, current_results->minValue, current_results->maxValue);
			nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 255, 0), nk_rgb(0, 150, 0), current_results->request_number, current_results->minValue, current_results->maxValue);
			for (i = 0; i < current_results->request_number; ++i) {

				nk_flags resWrite = nk_chart_push_slot(ctx, current_results->results[0][i], 0);
				nk_flags resRead = nk_chart_push_slot(ctx, current_results->results[1][i], 1);

				if (resWrite && NK_CHART_CLICKED)
				{
					write_selection = current_results->results[0][i];
				}

				if (resRead && NK_CHART_CLICKED)
				{
					read_selection = current_results->results[1][i];
				}
			}
		}

		nk_chart_end(ctx);

		if (read_selection != 0.00f || write_selection != 0.00f)
		{
			nk_tooltipf(ctx, "Value: %lf ms", read_selection == 0.00f ? write_selection : read_selection);
		}

	}
	else
	{
		nk_label(ctx, "No benchmark has been launched yet !", NK_TEXT_CENTERED);
	}

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