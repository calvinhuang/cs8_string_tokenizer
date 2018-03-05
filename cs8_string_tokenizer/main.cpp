//
//  main.cpp
//  cs8_string_tokenizer
//
//  Created by Calvin Huang on 2/27/18.
//

#include <iostream>
#include "stokenizer.hpp"
#include <string>
#include <iomanip>
using namespace std;

void print_tokens(STokenizer stk)
{
	Token t;
	t = Token();
	stk>>t;
	while (stk.more()) {
		cout << setw(10) << t.type_string() << setw(10) << "|" << t << "|" << endl;
		stk>>t;
	}
}

void print_string(const char* s)
{
	cout << "string: \"" << s << '"' << "\n";
}

int main(int argc, const char * argv[]) {
	char s[] = "it was the night of october 17th. pi was still 3.14.";
	print_string(s);
	STokenizer stk(s);
	print_tokens(stk);
	cout << "\n   =======\n\n";
	
	char s2[] = "This is another test.\n\tIt has a line-break and tab.";
	print_string(s2);
	stk.set_string(s2);
	print_tokens(stk);
	cout << "\n   =======\n\n";
	
	char s3[] = "";
	print_string(s3);
	stk.set_string(s3);
	print_tokens(stk);
    return 0;
}
