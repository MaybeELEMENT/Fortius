#ifndef VGA_TEXT_H
#define VGA_TEXT_H
#include "../Library/string.h"
void SetCursorPosRaw(uint16_t pos);
void Setline(uint16_t pos);
void SetCursorPos(int x, int y);
//uint16_t GetCursorPos();
//void disable_cursor();
//void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void ClearScreen(bool line1 = true, int col = 0x0f);
void print(string s, int col = 7);
void printChar(const char c, bool caps);
void MoveCursorLR(int i);
void printf(const char* s, int col = 7);
void MoveCursorUD(int i);
void ColLine(int line, int col);
void printCol(string s, int col);
void ClrLine(int line);
void ColScreen(int col);
void println(string s, int col = 0);

#endif
