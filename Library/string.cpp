#include "string.h"
#include "Typedefs.h"
string::string(char* x) : value{x} {}
	template<typename intype>
	string to_string(intype var)
	{

	}
	int string::length(){
		int res;
		for(res = 0; value[res] != 0; res++);
		return res;
	}
	bool string::find(string t){
		int vlen;
		for(vlen = 0; value[vlen] != 0; vlen++);
		string g = vlen > t.length()? value : t;
		
		bool res = true;
		int i = 0;
		do{
			if(value[i] != t[i] || i > 80){
				res = false;
				break;
			}
			else i++;
		}
		while(g[i] != 0);
		return res;
	}
	/*const char* reverse(const char* str, char* buffer, int len){
		int i;
		for (i = 0; i <= len; i++){
			buffer[i] = str[len - i];
		}
		buffer[i] = '\0';
		return buffer;
	}*/
	//Get length of given char*
	int length(char* thing){
		int res;
		for(res = 0; thing[res] != 0; res++);
		return res;
	}
	//Get length of given const char*
	int length(const char* thing){
		int res;
		for(res = 0; thing[res] != 0; res++);
		return res;
	}
	//Substr under development
	/*string string::substr(string s, int start, int end)
	{
		string rturn;
		int max = start + end;
		if(s[start] == 0) return "Invalid start value";
		if(s.length() < end) return "Invalid end value.";
		int i = start;
		do
		{
			rturn[i] = s[i];
			i++;
		}
		while (i < end);
		return rturn;
	}*/

	//Check if string start with given string
	bool string::startswith(string s, string t)
	{
		int lengths = s.length();
		int lengtht = t.length();
		for(int i = 0; i != lengtht; i++) {
			if(s[i] != t[i]) return false;
		}
		return true;
	}

	//Append character to string
	void string::append(char c)
	{
		int lengthi;
		for(lengthi = 0; value[lengthi] != 0; lengthi++);
		value[lengthi] = c;
	}
	void string::append(string str)
	{
		int lengthstr;
		int ilol;
		for(ilol = 0; value[ilol] != 0; ilol++);
		for(lengthstr = 0; value[lengthstr] != 0; lengthstr++)
		{
			value[ilol + lengthstr] = str[lengthstr];
		}
	}
	
	//Remove last character
	void string::push_back()
	{
		int lengthi;
		for(lengthi = 0; value[lengthi] != 0; lengthi++);
		lengthi -= 1;
		value[lengthi] = 0;
	}

	//Clear the string
	void string::clear()
	{
		int lengthi;
		for(lengthi = 0; value[lengthi] != 0; lengthi++)
		{
			value[lengthi] = 0;
		}
	}