//
//  token.hpp
//  cs8_string_tokenizer
//
//  Created by Calvin Huang on 3/1/18.
//

#ifndef token_hpp
#define token_hpp

#include <stdio.h>
#include <string>
#include <iomanip>
using namespace std;

const int UNKNOWN_TYPE = -1;
const int ALPHA_TYPE = 0;
const int NUMBER_TYPE = 1;
const int PUNCTUATION_TYPE = 2;
const int SPACE_TYPE = 3;


class Token
{
public:
	Token() : _token(""), _type(UNKNOWN_TYPE) {};
	Token(string str, int type) : _type(type), _token(str) {};
	friend ostream& operator <<(ostream& outs, const Token& t);
	int type() const;
	string type_string() const;
	string token_str() const;
private:
	string _token;
	int _type;
};

string token_type_to_string(int type);

#endif /* token_hpp */
