#include "cpuemulator.h"

static void		print_ram(int *ram)
{
	int	last_written;
	int i;
	int ram_line;

	last_written = RAM_SIZE - 1;
	i = 0;
	ram_line = 1;
	printf("      ----> RAM STATUS (up to last non zero value)\n");
	while (ram[last_written] == 0)
		last_written--;
	while (i <= last_written)
	{
		if (i == SCREEN_START)
		{
			if (ram_line != 1)
				printf("\n");
			printf("      ---- SCREEN FLAG\n");
		}
		else if (i == KEYBOARD)
		{
			if (ram_line != 1)
				printf("\n");
			printf("      ---- KEYBOARD FLAG\n");
		}
		if (ram[i] || i == KEYBOARD)
		{
			printf("%5d: %7d | ", i, ram[i]);
			ram_line++;
		}
		if (ram_line % 5 == 0)
		{
			printf("\n");
			ram_line = 1;
		}
		i++;
	}
	printf("\n");
}

static int		*set_power_table(int max_power)
{
	int i;
	int *powers_table;

	powers_table = malloc(sizeof(int) * max_power);

	i = 1;
	powers_table[0] = 1;
	while (i++ < max_power)
		powers_table[i - 1] = powers_table[i - 2] * 2;
	return (powers_table);
}

static int		run_alu(int comp, int is_A, int regA, int regD, int *ram)
{
	int a_or_m;

	if (is_A)
		a_or_m = regA;
	else
		a_or_m = ram[regA];
	switch (comp)
	{
		case 42:
			return (0);
		case 63:
			return (1);
		case 58:
			return (-1);
		case 12:
			return (regD);
		case 48:
			return (a_or_m);
		case 13:
			return (~regD);
		case 49:
			return (~a_or_m);
		case 15:
			return (-regD);
		case 51:
			return (-a_or_m);
		case 31:
			return (regD + 1);
		case 55:
			return (a_or_m + 1);
		case 14:
			return (regD - 1);
		case 50:
			return (a_or_m - 1);
		case 2:
			return (regD + a_or_m);
		case 19:
			return (regD - a_or_m);
		case 7:
			return (a_or_m - regD);
		case 0:
			return (regD & a_or_m);
		case 21:
			return (regD | a_or_m);
		default:
			printf("Error: Unknown processor command\n");
			return (0);
	}

}

static size_t	do_jump(size_t pc, int out, int regA, int jump)
{
	switch (jump)
	{
		case 0:
			return (pc + 1);
		case 1:
			if (out > 0)
				return (regA);
			break ;
		case 2:
			if (out == 0)
				return (regA);
			break ;
		case 3:
			if (out >= 0)
				return (regA);
			break ;
		case 4:
			if (out < 0)
				return (regA);
			break ;
		case 5:
			if (out != 0)
				return (regA);
			break ;
		case 6:
			if (out <= 0)
				return (regA);
			break ;
		case 7:
			return (regA);
		default:
			return (pc + 1);
	}
	return (pc + 1);
}

void			run_computer(char *prog_name, size_t instr_nb, char **instr, int *ram)
{
	t_ui	*tui;
	int		instr_done;
	char	*instr_sel;
	c_instr	c_sel;
	int		*powers_table;
	int		regA;
	int		regD;
	int		alu_result;
	size_t	pc;
	int		kbr_key;
	int		last_kbr_key;

	pc = 0;
	instr_done = 0;
	last_kbr_key = -2;
	instr_sel = instr[pc];
	powers_table = set_power_table(INSTR_SIZE);
	if (!(tui = init_tui()))
	{
		errorExit(ERR_INIT_TUI);
		return ;
	}
	init_sidebar(tui, prog_name, instr_nb);
	while (1)
	{
		if ((kbr_key = getch()) == ERR)
		{
			kbr_key = 0;
			if (kbr_key != last_kbr_key)
			{
				mvwprintw(tui->keyboard, 1, 2, "PRESS A KEY ");
				last_kbr_key = kbr_key;
				ram[KEYBOARD] = kbr_key;
				wrefresh(tui->keyboard);
			}
		}
		else if (kbr_key != last_kbr_key)
		{
			mvwprintw(tui->keyboard, 1, 2, "CHAR:%2c %3d", kbr_key, kbr_key);
			last_kbr_key = kbr_key;
			ram[KEYBOARD] = kbr_key;
			wrefresh(tui->keyboard);
		}
		if (kbr_key == 27) // ESC or ALT keys
		{
			mvwprintw(tui->footer, 1, 1, "END OF PROGRAM: Forced exit with ESC");
			wrefresh(tui->footer);
			break ;
		}
		if (pc >= instr_nb)
		{
			mvwprintw(tui->footer, 1, 1, "Error: Program counter out of range (pc = %ld, instr_nb = %ld)\n", pc, instr_nb);
			wrefresh(tui->footer);
			break ;
		}
		instr_sel = instr[pc];
		if (instr_sel[0] == '0')
		{
			regA = bin_to_dec(instr_sel + 1, 15, powers_table);
			// printf("regA: %d\n", regA);
			pc++;
		}
		else
		{
			c_sel.is_A = instr_sel[3] - '0' == 0 ? 1 : 0;
			c_sel.comp = bin_to_dec(instr_sel + 4, 6, powers_table);
			c_sel.dest = bin_to_dec(instr_sel + 10, 3, powers_table);
			c_sel.jump = bin_to_dec(instr_sel + 13, 3, powers_table);

			if (!c_sel.is_A && (regA < 0 || regA >= RAM_SIZE))
			{
				mvwprintw(tui->footer, 1, 1, "Line %ld: Error: Try to access inlegal range for M", pc);
				wrefresh(tui->footer);
				break ;
			}
			// printf("is_A    |comp    |dest    |jump    |\n");
			// printf("%8d|%8d|%8d|%8d|\n",
			// 	c_sel.is_A,
			// 	c_sel.comp,
			// 	c_sel.dest,
			// 	c_sel.jump);
			alu_result = run_alu(c_sel.comp, c_sel.is_A, regA, regD, ram);
			if (c_sel.dest % 2 == 1)
			{
				if (!c_sel.is_A && (regA < 0 || regA >= RAM_SIZE))
				{
					mvwprintw(tui->footer, 1, 1, "Line %ld: Error: Try to access inlegal range for M", pc);
					wrefresh(tui->footer);
					break ;
				}
				ram[regA] = alu_result;
				if (regA >= SCREEN_START && regA <= SCREEN_END)
				{
					update_out_screen(tui->out_screen, regA, ram);
				}
			}
			if (c_sel.dest == 2 || c_sel.dest >= 6 || c_sel.dest == 3)
				regD = alu_result;
			if (c_sel.dest >= 4)
				regA = alu_result;

			if (c_sel.jump != 0)
			{
				if (regA == (int)pc - 1 && c_sel.jump == 7)
				{
					mvwprintw(tui->footer, 1, 1, "END OF PROGRAM: Infinite loop detected");
					wrefresh(tui->footer);
					break ;
				}
				pc = do_jump(pc, alu_result, regA, c_sel.jump);
			}
			else
				pc++;
		}
		instr_done++;
	}
	mvwprintw(tui->footer, 2, 1, "(press 'r' to see memory, 'q' to quit) - Instructions processed: %d", instr_done);
	wrefresh(tui->footer);
	while ((kbr_key = getch()) == ERR)
		continue ;
	endwin();
	free(powers_table);
	free(tui);
	if (kbr_key == 'r' || kbr_key == 'R')
		print_ram(ram);
}