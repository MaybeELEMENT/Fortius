#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "../Library/string.h"
void keyboard_handler(struct regs *r);
void output(bool nextline = false);
string input(char* message = "", string mode = "input");
void keyboard_install();

#endif
