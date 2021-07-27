#include "../Library/Typedefs.h"
#include "../Drivers/VGA_Text.h"
#include "../Library/mem.h"
#include "../CPU/idt.h"
#include "../CPU/isr.h"
#include "../CPU/irq.h"
#include "../CPU/timer.h"
#include "../Drivers/Keyboard.h"
#include "../Library/string.h"
int curLine;
extern uint16_t CursorPos;
extern bool onInput;
string username;
string password;
void Terminal()
{
	println("Fortius v0.1");
	while(true)
	{	
		string cmd = input("Terminal> ");
		if(cmd.find("help"))
		{
			println("Commands list:");
			println("clr -------- Clear Screen");
		}
		//else if(startswith(cmd, "color")) println("String start with 'color'!");
		else if(cmd.find("test")) println("Hello World!");
		else if(cmd.find("clr"))
		{
			ClearScreen();
		}
		else if(cmd.length() == 0);
		else
		{
			println("Command not found.");
		}
	}
}
void login(bool clearScrn)
{
    while(true)
	{
		username = input("Username: ");
		if(username.find("root"))
		{
			while(true)
			{
				password = input("Password: ");
				if(password.find("admin"))
				{
                    if(clearScrn) ClearScreen();
					return;
				}
				else if(password.length() == 0) break;
				else
					println("Password incorrect.");
			}
		}
		else
			println("Account doesn't exists.");
	}
}

extern "C" void main(){
	//asm volatile("1: jmp 1b");
	println("[ ... ] Getting basic system Ready...");
	idt_install();
	isrs_install();
	irq_install(); // interrupt request
	asm volatile ("sti");
	timer_install();
	Setline(0);
	println("[ OK ] All basic system are now ready.");
	Setline(1);
	keyboard_install();
	println("[ ... ] Installing Keyboard Drivers...");
	Setline(1);
	println("[ OK ] Keyboard has been installed.   ");
	Setline(2);
	login(false);
	Terminal();
}


