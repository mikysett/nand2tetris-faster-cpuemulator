# nand2tetris (faster) CPUEmulator
This tool reproduces in terminal the CPUEmulator given with the exercises of [nand2tetris.org](https://www.nand2tetris.org/).
It is not meant to replace the original CPUEmulator that has a lot more functionalities.
I created it only to test my assembler and VM in those limited cases where the original CPUEmulator is short.

## Extra functionalities
- Unthrottled instruction execution
- Customizable input delay (in ms) at program launch
- Count of instructions executed at the end of the program (useful for checking optimization on your VM)

## Notes about the terminal UI
- Monitors will usually be too small to have the 512*256 pixels represented on a 1:1 scale.
- You will find in `cpuemulator.h` two constants (`SCREEN_ZOOM_R` and `SCREEN_ZOOM_C`) that set the zoom out scale for the monitor (by default 4 and 2 respectively).
- This means that even if you have 4 rows in memory only one will be showed (black pixel if at least one of the four pixels in that columns is black).
- This is not so handy for screen feedbacks on I/O.
- Remember you may need to zoom out your terminal and make it full screen to see the program properly.

## Compiling the program
The program have a `Makefile` with a `make` rule. It also include my assembler and my VM. Those two can be replaced by yours: you'll need to put the source doe in `my_assembler` and `my_vm` directories and their makefile will be called by the parent directory.
The binaries should of those two programs should be named `assembler` and `VMtranslator`.

## Accepted arguments
You must pass an argument when calling the program, it can be:
1. A `directory` path -> the program will take all the .vm files and convert them to a .hack file.
2. A `.vm` file path -> it will convert this single file to a .hack file.
3. A `.asm` file path -> it will convert this single file to a .hack file.
4. A `.hack` file path -> it will run the CPUEmulator directly.

## Debug mode
In order to see verbose output in `cpuemulator.h`, `assembler.h` and `vmtranslator.h` switch `DEBUG_MODE` to 1.

### Warning
This little program was made in 3 days and not tested very extensively, bugs and unexpected behaviors are probably there.