#include "../Library/Typedefs.h"
#include <port_io.h>
#include "../Drivers/Keyboard.h"
#include "../Library/string.h"

#define VIDEO_MEMORY		(char*)0xB8000
#define VGA_WIDTH	80
#define elif else if
//thanks, OSDEV wiki, for being a thing.

extern int curMode;
extern int curColor;
extern int curLine; //Value of current cursor's line
extern int inputStart;
extern bool onInput;
extern int inputStart;
uint16_t CursorPos = 0; // Value of current position of the cursor

//Set Cursor's Position in raw
void SetCursorPosRaw(uint16_t pos){
	if(pos >= 0 && pos < 2000) {
		outb(0x3d4, 0x0f);
		outb(0x3d5, (uint8_t)(pos & 0xff));
		outb(0x3d4, 0x0e);
		outb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
		CursorPos = pos;
	}
	return;
}
//Set Cursor Position to given Line
void Setline(uint16_t pos){
	if(pos == 0)
	{
		SetCursorPosRaw(0);
		curLine = 0;
	}
	else
	{
		SetCursorPosRaw(80 * pos);
		curLine = pos;
	}
}
//Set Cursor's position in coordinates
void SetCursorPos(int x, int y){
	uint16_t pos;

			pos = y * VGA_WIDTH + x;
			SetCursorPosRaw(pos);
	return;
}

/*uint16_t GetCursorPos() //Commented for no particular reason, uncomment if needed
{
	uint16_t pos = 0;
	outb(0x3D4, 0x0F);
	pos |= inb(0x3D5);
	outb(0x3D4, 0x0E);
	pos |= ((uint16_t)inb(0x3D5)) << 8;
	return pos;
}
/*
void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}
*/

//Clean the whole screen
void ClearScreen(bool line1 = true/*If set cursor to first line*/, int col = 0x0f){
		for(int i = (int)VIDEO_MEMORY; i < (int)VIDEO_MEMORY + 4000; i += 1) {
			if(i % 2 == 0) *((char*)i) = 32;//Default is spaces
			else *((char*)i) = col;
		}
	if(line1)
		Setline(0);
}
//Color the whole screen to given color
void ColScreen(int col){
	for(int i = (int)VIDEO_MEMORY + 1; i < (int)VIDEO_MEMORY + 4000; i += 2)
		*((char*)i) = col;
}

//Set given line's color
void ColLine(int line, int col){
	for(int i = (int)VIDEO_MEMORY + VGA_WIDTH * 2 * line + 1;
	i < (int)VIDEO_MEMORY + VGA_WIDTH * 2 * (line + 1) + 1; i += 2)  *((char*)i) = col;
		
}
//Clear a given line
void ClrLine(int line){
	for(int i = (int)VIDEO_MEMORY + VGA_WIDTH * 2 * line;
	i < (int)VIDEO_MEMORY + VGA_WIDTH * 2 * (line + 1); i += 2)  *((char*)i) = 0; 	// CLR WITH 0s
		
}

//Basic message printing
void print(string s, int col = 7){	
	uint8_t* charPtr = reinterpret_cast<uint8_t*>(s.value);
	uint16_t i = CursorPos;
	int b = 0;
	do
	{
			if(curLine >= 25)
			{
				i = 0;
				ClearScreen();
				SetCursorPosRaw(0);
			}
		b++;
	}
	while(s[b] != 0);
	while(*charPtr != 0){
	switch (*charPtr) {
		case 10:
	  			i+= VGA_WIDTH - i % VGA_WIDTH;	
				curLine += 1;
			break;
		case 13:
			i -= i % VGA_WIDTH;
			break;
		default:
		*(VIDEO_MEMORY + i * 2) = *charPtr;
		*(VIDEO_MEMORY + i*2 + 1) = col;
		i++;
	}

	charPtr++;
	}
	SetCursorPosRaw(i);
	return;
}

//Message printing, but parameter is char*
void printf(const char* s, int col = 7){	
	uint8_t* charPtr = (uint8_t*)s;
	uint16_t i = CursorPos;
	int b = 0;
	do
	{
			if(curLine >= 25)
			{
				i = 0;
				ClearScreen();
				SetCursorPosRaw(0);
			}
		b++;
	}
	while(s[b] != 0);
	while(*charPtr != 0){
	switch (*charPtr) {
		case 10:
	  			i+= VGA_WIDTH - i % VGA_WIDTH;	
				curLine += 1;
			break;
		case 13:
			i -= i % VGA_WIDTH;
			break;
		default:
		*(VIDEO_MEMORY + i * 2) = *charPtr;
		*(VIDEO_MEMORY + i*2 + 1) = col;
		i++;
	}

	charPtr++;
	}
	SetCursorPosRaw(i);
	return;
}

//Print message with color
void printCol(string s, int col){
  uint8_t* charPtr = reinterpret_cast<uint8_t*>(s.value);
  uint16_t i = CursorPos;
  while(*charPtr != 0)
  {
	switch (*charPtr) {
	  case 10:	
	  			i+= VGA_WIDTH - i % VGA_WIDTH;
			break;
	  case 13:
			i -= i % VGA_WIDTH;
			break;
	  default:
	  *(VIDEO_MEMORY + i*2) = *charPtr;
	  *(VIDEO_MEMORY + i*2 + 1) = col;
	  i++;
	}

	charPtr++;
  }
  SetCursorPosRaw(i);
  return;
}
//Print message as a line
void println(string s, int col = 0)
{
	if(col == 0)
		print(s);
	else
		printCol(s, col);
	print("\n");
}

//Move Cursor Horizontally
void MoveCursorLR(int i){
		CursorPos += i;
		SetCursorPosRaw(CursorPos);
	return;
}

//Move Cursor Vertically
void MoveCursorUD(int i){
		CursorPos += VGA_WIDTH * i;
		SetCursorPosRaw(CursorPos);
	return;
}

void ScrollVMem(){
	int i;
	
	for(i = 0xB8000/2 + 1999 ; i >= CursorPos + 0xB8000/2; i--)
		*((char*)(i * 2)) = *((char*)(i * 2 - 2));
	return;
}

// Print message as charactor
void printChar(const char c, bool caps){
	int curLine = 1 + CursorPos / VGA_WIDTH;
	
	if(c == 8 || c == 10 || (c >= 32 && c <= 255)) {
	switch(c){
		case 10:					// newline
			CursorPos+=VGA_WIDTH - CursorPos % VGA_WIDTH;	
			break;
		case 8:						// backspace
			if(onInput)
			{
				if(inputStart < CursorPos)
					CursorPos--;
			}
			*(VIDEO_MEMORY + CursorPos * 2) = (char)0;
			break;
		case 127: 						// del
			if(CursorPos < 2000){
			*(VIDEO_MEMORY + CursorPos * 2 + 2) = (char)0;
			}
			break;
		default:
			ScrollVMem();
			if(c >= 97 && c <= 172 && caps) *(VIDEO_MEMORY + CursorPos * 2) = c - 32; // CAPS
			else if(c >= 48 && c <= 57 && caps){
				//caps numbers
			}
			else *(VIDEO_MEMORY + CursorPos * 2) = c;
			
			if(CursorPos < 2000)
				CursorPos++;
			
		}
	}
	
	SetCursorPosRaw(CursorPos);
	return;
}




