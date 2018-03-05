//
//  token.cpp
//  cs8_string_tokenizer
//
//  Created by Calvin Huang on 3/1/18.
//

#include "token.hpp"

string Token::type_string() const
{
	return token_type_to_string(_type);
}

string Token::token_str() const
{
	return _token;
}

int Token::type() const
{
	return _type;
}

ostream& operator <<(ostream& outs, const Token& t)
{
	outs << t.token_str();
	return outs;
}

string token_type_to_string(int type)
{
	switch (type) {
		case ALPHA_TYPE:
			return "ALPHA";
		case NUMBER_TYPE:
			return "NUMBER";
		case SPACE_TYPE:
			return "SPACE";
		case PUNCTUATION_TYPE:
			return "PUNCTUATION";
		default:
			return "UNKNOWN";
	}
}
