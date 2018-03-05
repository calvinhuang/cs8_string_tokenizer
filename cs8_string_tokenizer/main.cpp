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
	cout << '\"' << s << '"' << "\n\n";
}

void print_test(const char*s, STokenizer &stk)
{
	print_string(s);
	stk.set_string(s);
	print_tokens(stk);
	cout << "\n   =======\n\n";
}

int main(int argc, const char * argv[]) {
	STokenizer stk;
	
	char s[] = "it was the night of october 17th. pi was still 3.14.";
	print_test(s, stk);
	
	char s2[] = "This is another test.\n\tIt has a line-break and tab.";
	print_test(s2, stk);
	
	char s3[] = "";
	print_test(s3, stk);
	
	char s4[] = "This ends with a number: 3.46.77";
	print_test(s4, stk);
	
	char s5[] = "This also ends with a number: 3333.3334,77.57";
	print_test(s5, stk);
	
	char s6[] = "What if we end on some spaces?			\n\t";
	print_test(s6, stk);
	
	char s7[] = ",+(,-)+:;\nWhat's going on!?\t###$#%$%";
	print_test(s7, stk);
	
	char s8[] = "alpha, 8.88, 01, 32, 666666, alpha ! . +";
	print_test(s8, stk);
	
	char s9[] = " \t\n \t\n \t\n \t\n";
	print_test(s9, stk);
	
    return 0;
}
