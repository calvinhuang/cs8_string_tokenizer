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
	set_table_cell(_table, SPACE_START + 1, (int) ' ', SPACE_START + 1);
	// specify success states
	set_table_success(_table, SPACE_START, false);
	set_table_success(_table, SPACE_START + 1);
	
	// UNKNOWN TOKEN MACHINE
	set_table_row(_table, UNKNOWN_START, 1, 255, UNKNOWN_START + 1);
	set_table_success(_table, UNKNOWN_START, false);
	set_table_success(_table, UNKNOWN_START + 1);
	
//	for (int i = 0; i < MAX_ROWS; i++)
//	{
//		std::cout << start_state_to_string(i) << ": " << _table[i][0] << "\n";
//	}
}

bool STokenizer::get_token(int start_state, string& token)
{
	int last_state = start_state;
	token = "";
	while (start_state > -1 && _pos < MAX_BUFFER)
	{
//		std::cout << "start_state: " << start_state_to_string(start_state) << "\n";
//		std::cout << "position: " << _pos << "\n";
		int ascii = _buffer[_pos];
		if (ascii == 0) {
			// end of string
//			std::cout << "end of string (" << ascii << " at " << _pos << ")\n";
			_pos = MAX_BUFFER;
			break;
		}
//		std::cout << "ascii: " << ascii << "\n";
		int next_state = _table[start_state][ascii];
//		std::cout << "next_state: " << next_state << "\n";
		if (next_state > -1)
		{
			token += _buffer[_pos];
			_pos++;
//			std::cout << start_state_to_string(start_state) << ": next_state valid; increased position to " << _pos << ".\n";
		}
		last_state = start_state;
		start_state = next_state;
	}
//	std::cout << "last state: " << last_state << ":" << _table[last_state][0] << "\n";
	if (_table[last_state][0] > 0)
	{
//		std::cout << "known token: '" << token << "'\n";
		return true;
	}
//	std::cout << "no token recognized. position not incremented.\n";
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
	else
	{
		s.get_token(UNKNOWN_START, token);
		t = Token(token, UNKNOWN_TYPE);
	}
//	std::cout << token << " >> Token\n";
	
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

void STokenizer::set_string(char *str)
{
	strcpy(_buffer, str);
	_pos = 0;
}

bool STokenizer::more() {
	return _pos < MAX_BUFFER;
}

bool STokenizer::done() {
	return !more();
}
