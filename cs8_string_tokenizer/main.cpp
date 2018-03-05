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
	while (stk.more()) {
		stk>>t;
		cout << setw(10) << t.type_string() << setw(10) << "|" << t << "|" << endl;
		t = Token();
	}
}

int main(int argc, const char * argv[]) {
	char s[] = "it was the night of october 17th. pi was still 3.14.";
	STokenizer stk(s);
	print_tokens(stk);
	cout << "\n   =======\n\n";
	char s2[] = "This is another test.\nIt has line-breaks.";
	stk.set_string(s2);
	print_tokens(stk);
    return 0;
}
