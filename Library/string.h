#ifndef STRING_H
#define STRING_H
struct string {
    char* value;
	char& operator[](int index) { return value[index]; }
    string(char*);
    string() : value() {}
	int length();
    bool find(string t);
    //string substr(string s, int start, int end); //Under Development
    bool startswith(string s, string t);
    void append(char s2);
    void append(string str);
    void push_back();
    void clear();
};
//const char* reverse(const char* str, char* buffer, int len); //Under Development
int length(char* thing);
int length(const char* thing);

#endif