#include "../Library/Typedefs.h"
#include "VGA_Text.h"
#include "port_io.h"
#include "../CPU/irq.h"
#include "../CPU/timer.h"
#include "../Library/string.h"


//Global for input return string
string inString;

bool onInput;

uint16_t inputStart;

bool fieldmode = false;

extern uint16_t CursorPos;

bool fieldExit = false;

bool entering = false;

int inputLength = 1;

extern int curLine;

#define elif else if
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

bool shift_pressed = false;
bool caps_lock = false;
void output(bool nextline = false)
{
  irq_uninstall_handler(1);
}

void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    //Get keyboard button pressed
    scancode = inb(0x60);

    if (scancode & 0x80)
    {
    	
    	//Detect if unhold shift
    	switch(scancode){
    		case 0xaa: shift_pressed = false; break;
    		
    	}
	
    }
    else
    {
        switch(scancode){
          case 0x4b: 
          if(onInput)
          {
            if(inputStart > CursorPos - 1) return;
            MoveCursorLR(-1);
          }
          break;
          case 0x4d:
            if(onInput)
            {
              if(!fieldmode)
              {
                if(inputStart + inputLength - 1 != CursorPos)
                  MoveCursorLR(1);
              }
              else if(fieldmode)
              {
                MoveCursorLR(1);
              }
            }
          break;
          case 0x48:
          if(onInput)
          {
            if(fieldmode)
            {
              if(CursorPos > 160)
                MoveCursorUD(-1);
            }
            else
              SetCursorPosRaw(inputStart);
          }
          break;
          case 0x50:
            if(onInput)
            {
              if(fieldmode)
              if(inputStart + inputLength + 80 < CursorPos)
                MoveCursorUD(1);
              else
                SetCursorPosRaw(inputStart + inputLength - 1);
            }
          break;
          case 0x2a: shift_pressed = true; break;
          case 0x3a: caps_lock = !caps_lock; break;
          case 0x1c: 
            if(!fieldmode)
            {
              onInput = false;
            }
            else if(fieldmode)
            {
              print("\n");
            }
          case 0x0E:
            printChar(kbdus[scancode], shift_pressed | caps_lock);
            if(inputStart < CursorPos)
              inputLength -= 1;
           break;
          break;
          break;
          default: 
          if(scancode == 0x0B && shift_pressed) {  printChar(')', 0);}
          elif(scancode == 0x02 && shift_pressed) {printChar('!', 0);}
          elif(scancode == 0x03 && shift_pressed) {printChar('@', 0);}
          elif(scancode == 0x04 && shift_pressed) {printChar('#', 0);}
          elif(scancode == 0x05 && shift_pressed) {printChar('$', 0);}
          elif(scancode == 0x06 && shift_pressed) {printChar('%', 0);}
          elif(scancode == 0x07 && shift_pressed) {printChar('^', 0);}
          elif(scancode == 0x08 && shift_pressed) {printChar('&', 0);}
          elif(scancode == 0x09 && shift_pressed) {printChar('*', 0);}
          elif(scancode == 0x0A && shift_pressed) {printChar('(', 0);}
          elif(scancode == 0x0C && shift_pressed) {printChar('_', 0);}
          elif(scancode == 0x27 && shift_pressed) {printChar(':', 0);}
          elif(scancode == 0x28 && shift_pressed) {printChar('\'', 0);}
          elif(scancode == 0x34 && shift_pressed) {printChar('>', 0);}
          elif(scancode == 0x33 && shift_pressed) {printChar('<', 0);}
          elif(scancode == 0x35 && shift_pressed) {printChar('?', 0);}
          elif(scancode == 0x2B && shift_pressed) {printChar('|', 0);}
          elif(scancode == 0x0D && shift_pressed) {printChar('+', 0);}
          elif(scancode == 0x29 && shift_pressed) {printChar('~', 0);}
          elif(scancode == 0x1A && shift_pressed) {printChar('{', 0);}
          elif(scancode == 0x1B && shift_pressed) {printChar('}', 0);}
          elif(scancode == 0x1D && scancode == 0x1F) {onInput = false;}
          else
            printChar(kbdus[scancode], shift_pressed | caps_lock);
          inputLength += 1;
          break;
        }
    }
}
//Input function
string input(char* message = "", string mode = "input")
{
  if(mode.find("input"))
  {
    fieldmode = false;
    printf(message);
  }
  else if(mode.find("field"))
  {
    ClearScreen();
    fieldmode = true;
    inputLength += length(message);
  }
  inString = "";
  onInput = true;
  inputStart = CursorPos;
  irq_install_handler(1, keyboard_handler);
  if(fieldmode)
    print(message);
  while(onInput);
  int inputEnd = CursorPos;
  char* c = (char*)(0xB8000 + 2 * inputStart);
  int i;
  for(i = 0; i < inputLength; i ++) inString[i] = *(c + 2 * i);
  inString[i] = 0;
  irq_uninstall_handler(1);
  Setline(curLine + 1);
  inputLength = 1;
  return inString;
}
void keyboard_install() {irq_install_handler(1, keyboard_handler);irq_uninstall_handler(1);}