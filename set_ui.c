#include "cpuemulator.h"

static void	init_out_screen(WINDOW *out_screen)
{
	char def_screen[SCREEN_COLUMNS + 1];
	int i;

	i = 0;
	while (i < SCREEN_COLUMNS)
	{
		def_screen[i] = '0';
		i++;
	}
	def_screen[SCREEN_COLUMNS] = '\0';
	i = 0;
	while (i < SCREEN_ROWS)
	{
		wmove(out_screen, 1 + i, 1);
		wprintw(out_screen, "%s", def_screen);
		i++;
	}
}

void		update_out_screen(WINDOW *out_screen, int regA, int *ram)
{
	static const int	px_per_int = (INSTR_SIZE / SCREEN_ZOOM_C);
	static const int	s_columns = SCREEN_COLUMNS;
	static const int	int_per_row = FULL_SCREEN_COL / INSTR_SIZE;
	char 	full_pixels[px_per_int + 1];
	int		px_to_show;
	int		px_mask;
	int		row;
	int		column;
	int		i;

	column = row = (regA - SCREEN_START);
	row /= (s_columns / px_per_int);
	row /= SCREEN_ZOOM_R;
	column %= (s_columns / px_per_int);
	column *= px_per_int;

	// Merge of X rows pixels together
	i = 1;
	px_to_show = ram[SCREEN_START + (row * int_per_row)];
	while (i < SCREEN_ZOOM_R)
	{
		px_to_show = px_to_show | ram[SCREEN_START + ((row + i) * int_per_row)];
		i++;
	}
	// Set the mask
	px_mask = 1;
	i = 1;
	while (i < SCREEN_ZOOM_C)
	{
		px_mask = (px_mask << 1) + 1;
		i++;
	}

	// Calculate each individual pixel
	full_pixels[px_per_int] = '\0';
	i = 0;
	while (i < px_per_int)
	{
		if ((px_to_show & px_mask) == 0)
			full_pixels[px_per_int - i - 1] = '0';
		else
			full_pixels[px_per_int - i - 1] = ' ';
		px_mask = px_mask << SCREEN_ZOOM_C;
		i++;
	}
	mvwprintw(out_screen, row + 1, column + 1, "%s", full_pixels);
	wrefresh(out_screen);
}

void		init_sidebar(t_ui *tui, char *prog_name, size_t instr_nb)
{
	int s_zoom_r = SCREEN_ZOOM_R;
	int s_zoom_c = SCREEN_ZOOM_C;

	mvwprintw(tui->sidebar, 1, 1, "PROGRAM:\n%14s\n", prog_name);
	wprintw(tui->sidebar, " INSTRUCTIONS: %9ld\n", instr_nb);
	wprintw(tui->sidebar, " DELAY: %14ldms\n", tui->time_delay);
	wprintw(tui->sidebar, " ROWS ZOOM: %12ld\n", s_zoom_r);
	wprintw(tui->sidebar, " COLUMNS ZOOM: %9ld\n", s_zoom_c);
	wrefresh(tui->sidebar);
}

t_ui		*init_tui(void)
{
	t_ui	*tui;
	int		time_delay;

	if (!(tui = malloc(sizeof(t_ui))))
	{
		errorExit(-1);
		return (NULL);
	}
	printf("Input delay (in ms, default is 0): ");
	scanf("%d", &time_delay);
	if (time_delay < 0)
		tui->time_delay = 0;
	else
		tui->time_delay = time_delay;

	initscr();
	noecho();
	timeout(tui->time_delay);
	keypad(stdscr, TRUE);

	tui->out_screen = newwin(SCREEN_ROWS + 2, SCREEN_COLUMNS + 2, 0, 0);
	tui->sidebar = newwin(SCREEN_ROWS + 2, 25, 0, SCREEN_COLUMNS + 2);
	tui->footer = newwin(4, SCREEN_COLUMNS - 14, SCREEN_ROWS + 2, 0);
	tui->keyboard = newwin(4, 16, SCREEN_ROWS + 2, SCREEN_COLUMNS + 2 - 16);

	box(tui->out_screen, 0, 0);
	box(tui->footer, 0, 0);
	box(tui->sidebar, 0, 0);
	box(tui->keyboard, 0, 0);

	refresh();
	init_out_screen(tui->out_screen);
	mvwprintw(tui->footer, 1, 1, "PROGRAM EXECUTING...");
	wrefresh(tui->out_screen);
	wrefresh(tui->sidebar);
	wrefresh(tui->footer);
	wrefresh(tui->keyboard);

	return (tui);
}
