#ifndef CPUEMULATOR_H
# define CPUEMULATOR_H

# define DEBUG_MODE		0
# if DEBUG_MODE == 1
#  define DEBUG_CODE(x)	x
# else
#  define DEBUG_CODE(x)
# endif

# define INSTR_SIZE		16
# define RAM_SIZE		32768
# define ROM_MAX_SIZE	32768
# define SCREEN_START	16384
# define SCREEN_END		24575
# define KEYBOARD		24576

# define SCREEN_ZOOM_R		4
# define SCREEN_ZOOM_C		2
# define FULL_SCREEN_ROWS	256
# define FULL_SCREEN_COL	512
# define SCREEN_ROWS		FULL_SCREEN_ROWS / SCREEN_ZOOM_R
# define SCREEN_COLUMNS		FULL_SCREEN_COL / SCREEN_ZOOM_C

# define ERR_NO_ARG		0
# define ERR_OPEN_FAIL	1
# define ERR_FILE_EMPTY	2
# define ERR_NO_INSTR	3
# define ERR_INSTR_SIZE	4
# define ERR_INSTR		5
# define ERR_MAX_ROM	6
# define ERR_PC_OUT		7
# define ERR_BAD_M		8
# define ERR_INIT_TUI	9

# include <unistd.h>
# include <libgen.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <dirent.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct	s_c_instr
{
	int is_A;
	int comp;
	int dest;
	int jump;
}				c_instr;

typedef struct	s_tui
{
	WINDOW	*out_screen;
	WINDOW	*sidebar;
	WINDOW	*footer;
	WINDOW	*keyboard;
	int		time_delay;
}				t_ui;

int			errorExit(int err);
char		*create_hack_file(char *full_path);
char		**ft_split(char const *s, char c);
void		ft_free_split(char **split_result);
char		**set_instructions(FILE *fd);
void		init_computer(char **instr, char *prog_name);
void		run_computer(char *prog_name, size_t instr_nb, char **instr, int *ram);
int			bin_to_dec(char *str, size_t nb_digits, int *powers_table);

t_ui		*init_tui(void);
void		init_sidebar(t_ui *tui, char *prog_name, size_t instr_nb);
void		update_out_screen(WINDOW *out_screen, int regA, int *ram);

DEBUG_CODE(
void		ft_print_instructions(char **instructions);
)

#endif