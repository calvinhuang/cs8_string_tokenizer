//
//  stokenizer.cpp
//  cs8_string_tokenizer
//
//  Created by Calvin Huang on 2/27/18.
//

#include "stokenizer.hpp"
#include <iostream>

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS] = { { } };

string start_state_to_string(int start_state)
{
	switch (start_state) {
		case NUMBER_START:
		case NUMBER_START + 1:
		case NUMBER_START + 2:
		case NUMBER_START + 3:
			return "NUMBER";
		case ALPHA_START:
		case ALPHA_START + 1:
			return "ALPHA";
		case SPACE_START:
		case SPACE_START + 1:
			return "SPACE";
		case PUNCTUATION_START:
		case PUNCTUATION_START + 1:
			return "PUNCTUATION";
		default:
			return "UNKNOWN";
	}
}

void STokenizer::init_table(int _table[][MAX_COLUMNS])
{
	for (int state = 0; state < MAX_ROWS; ++state)
	{
		for (int ascii = 0; ascii < MAX_COLUMNS; ++ascii)
		{
			_table[state][ascii] = -1;
		}
	}
}

void STokenizer::set_table_cell(int _table[][MAX_COLUMNS], int state, int col, int next_state)
{
	_table[state][col] = next_state;
}

void STokenizer::set_table_row(int _table[][MAX_COLUMNS], int state, int col_start, int col_stop, int next_state)
{
	for (int ascii = col_start; ascii <= col_stop; ++ascii)
	{
		set_table_cell(_table, state, ascii, next_state);
	}
}

void STokenizer::set_table_success(int _table[][MAX_COLUMNS], int state, bool success)
{
	_table[state][0] = success ? 1 : 0;
}

void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
	init_table(_table);
	
	// NUMBER TOKEN MACHINE
	// state 0 and 1
	for (int state = NUMBER_START; state < NUMBER_START + 2; ++state)
	{
		set_table_row(_table, state, (int) '0', (int) '9', NUMBER_START + 1);
		set_table_cell(_table, state, (int) '.', NUMBER_START + 2);
	}
	set_table_cell(_table, NUMBER_START, (int) '.', -1);
	// state 2 and 3
	for (int state = NUMBER_START + 2; state < NUMBER_START + 4; ++state)
	{
		set_table_row(_table, state, (int) '0', (int) '9', NUMBER_START + 3);
	}
	// specify which states are success states
	set_table_success(_table, NUMBER_START, false);
	set_table_success(_table, NUMBER_START + 1);
	set_table_success(_table, NUMBER_START + 2, false);
	set_table_success(_table, NUMBER_START + 3);
	
	// ALPHA TOKEN MACHINE
	set_table_row(_table, ALPHA_START, (int) 'a', (int) 'z', ALPHA_START + 1);
	set_table_row(_table, ALPHA_START, (int) 'A', (int) 'Z', ALPHA_START + 1);
	set_table_row(_table, ALPHA_START + 1, (int) 'a', (int) 'z', ALPHA_START + 1);
	set_table_row(_table, ALPHA_START + 1, (int) 'A', (int) 'Z', ALPHA_START + 1);
	// specify success states
	set_table_success(_table, ALPHA_START, false);
	set_table_success(_table, ALPHA_START + 1);
	
	// SPACE TOKEN MACHINE
	set_table_cell(_table, SPACE_START, (int) ' ', SPACE_START + 1);
	set_table_cell(_table, SPACE_START, (int) '\n', SPACE_START + 1);
	set_table_cell(_table, SPACE_START, (int) '\t', SPACE_START + 1);
	set_table_cell(_table, SPACE_START + 1, (int) ' ', SPACE_START + 1);
	set_table_cell(_table, SPACE_START + 1, (int) '\n', SPACE_START + 1);
	set_table_cell(_table, SPACE_START + 1, (int) '\t', SPACE_START + 1);
	// specify success states
	set_table_success(_table, SPACE_START, false);
	set_table_success(_table, SPACE_START + 1);
	set_table_success(_table, ALPHA_START + 1);
	
	// PUNCTUATION TOKEN MACHINE
	set_table_cell(_table, PUNCTUATION_START, (int) '.', PUNCTUATION_START + 1);
	set_table_cell(_table, PUNCTUATION_START, (int) ',', PUNCTUATION_START + 1);
	set_table_cell(_table, PUNCTUATION_START, (int) ';', PUNCTUATION_START + 1);
	set_table_cell(_table, PUNCTUATION_START, (int) ':', PUNCTUATION_START + 1);
	set_table_cell(_table, PUNCTUATION_START, (int) '"', PUNCTUATION_START + 1);
	set_table_cell(_table, PUNCTUATION_START, (int) '?', PUNCTUATION_START + 1);
	set_table_cell(_table, PUNCTUATION_START, (int) '!', PUNCTUATION_START + 1);
	set_table_cell(_table, PUNCTUATION_START, (int) '(', PUNCTUATION_START + 1);
	set_table_cell(_table, PUNCTUATION_START, (int) ')', PUNCTUATION_START + 1);
	// specify success states
	set_table_success(_table, PUNCTUATION_START, false);
	set_table_success(_table, PUNCTUATION_START + 1);
	
	// UNKNOWN TOKEN MACHINE
	set_table_row(_table, UNKNOWN_START, 1, 255, UNKNOWN_START + 1);
	set_table_success(_table, UNKNOWN_START, false);
	set_table_success(_table, UNKNOWN_START + 1);
}

bool STokenizer::get_token(int start_state, string& token)
{
	int last_state = start_state;
	token = "";
	while (start_state > -1 && !done())
	{
		int ascii = _buffer[_pos];
		int next_state = -1;
		if (ascii == 0)
		{
			// end of string
			if (token.length() == 0)
			{
				// immediate read of null means no more tokens
				// otherwise, let the next token read attempt trigger done state
				_pos = MAX_BUFFER;
				break;
			}
		}
		else
		{
			next_state = _table[start_state][ascii];
		}
		
		if (next_state > -1)
		{
			// transition is valid; append character to token
			token += _buffer[_pos];
			_pos++;
		}
		last_state = start_state;
		start_state = next_state;
	}
	if (_table[last_state][0] > 0)
	{
		// there was a success state, so a valid token was found
		return true;
	}
	// never entered a success state, so no token was found
	return false;
}

STokenizer& operator >> (STokenizer& s, Token& t)
{
	string token;
	if (s.get_token(NUMBER_START, token))
	{
		t = Token(token, NUMBER_TYPE);
	}
	else if (s.get_token(ALPHA_START, token))
	{
		t = Token(token, ALPHA_TYPE);
	}
	else if (s.get_token(SPACE_START, token))
	{
		t = Token(token, SPACE_TYPE);
	}
	else if (s.get_token(PUNCTUATION_START, token))
	{
		t = Token(token, PUNCTUATION_TYPE);
	}
	else
	{
		s.get_token(UNKNOWN_START, token);
		t = Token(token, UNKNOWN_TYPE);
	}
	
	return s;
}

STokenizer::STokenizer(char str[])
{
	STokenizer();
	set_string(str);
}

STokenizer::STokenizer()
{
	make_table(_table);
}

void STokenizer::set_string(const char *str)
{
	strcpy(_buffer, str);
	_pos = 0;
}

bool STokenizer::more() {
	return !done();
}

bool STokenizer::done() {
	return _pos >= MAX_BUFFER;
}
