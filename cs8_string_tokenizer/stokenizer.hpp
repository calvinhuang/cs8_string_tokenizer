//
//  stokenizer.hpp
//  cs8_string_tokenizer
//
//  Created by Calvin Huang on 2/27/18.
//

#ifndef stokenizer_hpp
#define stokenizer_hpp

#include <stdio.h>
#include <string>
#include "token.hpp"
using namespace std;

const size_t MAX_COLUMNS = 256;
const size_t MAX_ROWS = 10;
const size_t MAX_BUFFER = 102400;

const int NUMBER_START = 0;
const int ALPHA_START = 4;
const int SPACE_START = 6;
const int UNKNOWN_START = 8;


class STokenizer
{
public:
	STokenizer();
	STokenizer(char str[]);
	bool done();            //true: there are no more tokens
	bool more();            //true: there are more tokens
	//-- big three --
	
	//---------------
	//extract one token (very similar to the way cin >> works)
	friend STokenizer& operator >> (STokenizer& s, Token& t);
	
	//set a new string as the input string
	void set_string(char str[]);
	bool get_token(int start_state, string& token);
	
private:
	//create table for all the tokens we will recognize
	//                      (e.g. doubles, words, etc.)
	void make_table(int _table[][MAX_COLUMNS]);
	
	void init_table(int _table[][MAX_COLUMNS]);
	void set_table_row(int _table[][MAX_COLUMNS], int state, int col_start, int col_stop, int next_state);
	void set_table_cell(int _table[][MAX_COLUMNS], int state, int col, int next_state);
	void set_table_success(int _table[][MAX_COLUMNS], int state, bool success=true);
	
	//extract the longest string that match
	//     one of the acceptable token types
	
	//---------------------------------
	char _buffer[MAX_BUFFER];       //input string
	int _pos;                       //current position in the string
	bool _done = false;				//whether there are any tokens left to parse
	static int _table[MAX_ROWS][MAX_COLUMNS];
	
};

string start_state_to_string(int start_state);

#endif /* stokenizer_hpp */
