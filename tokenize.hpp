#pragma once

// Templated C++ implementation of the tokenize python module
// Written by Dominic Price 2021, see the LICENCE file
// 
// Tokenization help for Python programs
//
// tokenize(const std::istream& is) is a generator that breaks
// a stream of bytes into Python tokens. It decodes the bytes
// according to PEP-0263 for determining source file encoding.
//
// It accepts an input stream on which std::getline is called
// repeatedly to get the next line of input. It generates
// TokenInfo objects representing each token.
//
// It is designed to match the working of the Python
// tokenizer, except that it produces COMMENT tokens
// for comments and gives type OP for all operators. Additionally,
// all token lists start with an ENCODING toekn which tells you 
// which encoding was used to decode the bytes stream.


#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <algorithm>
#include <regex>
#include <optional>

namespace tokenize_py
{
	// from token import tok_name
	// print("enum Token\n{", )
	// print(",\n".join(f"\t{val} = {key}" for key, val in tok_name.items()))
	// print("};")
	enum Token
	{
		ENDMARKER = 0,
		NAME = 1,
		NUMBER = 2,
		STRING = 3,
		NEWLINE = 4,
		INDENT = 5,
		DEDENT = 6,
		LPAR = 7,
		RPAR = 8,
		LSQB = 9,
		RSQB = 10,
		COLON = 11,
		COMMA = 12,
		SEMI = 13,
		PLUS = 14,
		MINUS = 15,
		STAR = 16,
		SLASH = 17,
		VBAR = 18,
		AMPER = 19,
		LESS = 20,
		GREATER = 21,
		EQUAL = 22,
		DOT = 23,
		PERCENT = 24,
		LBRACE = 25,
		RBRACE = 26,
		EQEQUAL = 27,
		NOTEQUAL = 28,
		LESSEQUAL = 29,
		GREATEREQUAL = 30,
		TILDE = 31,
		CIRCUMFLEX = 32,
		LEFTSHIFT = 33,
		RIGHTSHIFT = 34,
		DOUBLESTAR = 35,
		PLUSEQUAL = 36,
		MINEQUAL = 37,
		STAREQUAL = 38,
		SLASHEQUAL = 39,
		PERCENTEQUAL = 40,
		AMPEREQUAL = 41,
		VBAREQUAL = 42,
		CIRCUMFLEXEQUAL = 43,
		LEFTSHIFTEQUAL = 44,
		RIGHTSHIFTEQUAL = 45,
		DOUBLESTAREQUAL = 46,
		DOUBLESLASH = 47,
		DOUBLESLASHEQUAL = 48,
		AT = 49,
		ATEQUAL = 50,
		RARROW = 51,
		ELLIPSIS = 52,
		COLONEQUAL = 53,
		OP = 54,
		AWAIT = 55,
		ASYNC = 56,
		TYPE_IGNORE = 57,
		TYPE_COMMENT = 58,
		ERRORTOKEN = 59,
		COMMENT = 60,
		NL = 61,
		ENCODING = 62,
		N_TOKENS = 63,
		NT_OFFSET = 256
	};

	std::string to_string(Token token);

	struct TokenInfo
	{
		TokenInfo(Token type,
			const std::string& token,
			std::pair<size_t, size_t> start,
			std::pair<size_t, size_t> end,
			const std::string& line);

		Token exact_type() const;
		bool is_empty() const;

		friend std::ostream& operator << (std::ostream& stream, const TokenInfo& tokinfo);

		Token type;
		std::string token;
		std::pair<size_t, size_t> start;
		std::pair<size_t, size_t> end;
		std::string line;
	};

	class TokenError : public std::runtime_error
	{
	public:
		TokenError(const std::string& msg, const std::pair<size_t, size_t>& pos);
	};

	class StopTokenizing : public std::exception {};

	// Produces a sequence of TokenInfo objects from an input stream. 
	std::vector<TokenInfo> tokenize(std::istream& stream);
	// FIXME: Always returns 'utf-8'
	std::pair<std::string, std::vector<std::string>> detect_encoding(std::istream& stream);
	// FIXME: Always returns 'utf-8'
	std::string get_normal_name(const std::string orig_enc);
	// Transform tokens back into Python source code
	std::string untokenize(const std::vector<TokenInfo>& iterable);
} // namespace tokenize