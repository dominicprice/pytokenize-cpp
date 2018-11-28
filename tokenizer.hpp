#pragma once

/*
Tokenization help for Python programs

tokenize(const std::istream& is) is a generator that breaks
a stream of bytes into Python tokens. It decodes the bytes
according to PEP-0263 for determining source file encoding.

It accepts an input stream on which std::getline is called
repeatedly to get the next line of input. It generates
5-tuples with these members:
	the token type
	the token
	the starting (row, col) indices of the token
	the ending (row, col) indices of the token
	the original line

It is designed to matcht he working of the Python
tokenizer exactly, except that it produces COMMENT tokens
for comments and gives type OP for all operators. Additionally,
all token lists start with an ENCODING toekn which tells you which encoding
was used to decode the bytes stream.
*/

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <algorithm>
#include <regex>
#include <optional>


namespace helpers
{
	std::string repeat(const std::string& s, size_t n)
	{
		std::string ret;
		while (n--)
			ret += s;
		return ret;
	}

	std::string to_lower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
	}

	std::string to_upper(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		return s;
	}

	std::string replace(std::string s, const std::string& from, const std::string& to)
	{
		size_t start_pos = s.find(from);
		while (start_pos != std::string::npos) {
			str.replace(start_pos, from.size(), to);
			start_pos = s.find(from, start_pos + to.size());
		}
		return s;
	}

	bool starts_with(const std::string& outer, const std::string& inner)
	{
		return outer.compare(0, inner.size(), inner) == 0;
	}

	bool ends_with(const std::string& outer, const std::string& inner)
	{
		return outer.compare(outer.size() - 3, inner.size(), inner);
	}
}



enum Token : int
{
	ENDMARKER,
	NAME,
	NUMBER,
	STRING,
	NEWLINE,
	INDENT,
	DEDENT,
	LPAR,
	RPAR,
	LSQB,
	RSQB,
	COLON,
	COMMA,
	SEMI,
	PLUS,
	MINUS,
	STAR,
	SLASH,
	VBAR,
	AMPER,
	LESS,
	GREATER,
	EQUAL,
	DOT,
	PERCENT,
	LBRACE,
	RBRACE,
	EQEQUAL,
	NOTEQUAL,
	LESSEQUAL,
	GREATEREQUAL,
	TILDE,
	CIRCUMFLEX,
	LEFTSHIFT,
	RIGHTSHIFT,
	DOUBLESTAR,
	PLUSEQUAL,
	MINUSEQUAL,
	STAREQUAL,
	SLASHEQUAL,
	PERCENTEQUAL,
	AMPEREQUAL,
	VBAREQUAL,
	CIRCUMFLEXEQUAL,
	LEFTSHIFTEQUAL,
	RIGHTSHIFTEQUAL,
	DOUBLESTAREQUAL,
	DOUBLESLASH,
	DOUBLESLASHEQUAL,
	AT,
	ATEQUAL,
	RARROW,
	ELLIPSIS,
	OP,
	AWAIT,
	ASYNC,
	ERRORTOKEN,
	N_TOKENS,

	ENCODING,
	NL
};

const std::unordered_map<std::string, Token> exact_token_types = {
	{ "(", LPAR },
	{ ")", RPAR },
	{ "[", LSQB },
	{ "]", RSQB },
	{ ":", COLON },
	{ ",", COMMA },
	{ ";", SEMI },
	{ "+", PLUS },
	{ "-", MINUS },
	{ "*", STAR },
	{ "/", SLASH },
	{ "|", VBAR },
	{ "&", AMPER },
	{ "<", LESS },
	{ ">", GREATER },
	{ "=", EQUAL },
	{ ".", DOT },
	{ "%", PERCENT },
	{ "{", LBRACE },
	{ "}", RBRACE },
	{ "==", EQEQUAL },
	{ "!=", NOTEQUAL },
	{ "<=", LESSEQUAL },
	{ ">=", GREATEREQUAL },
	{ "~", TILDE },
	{ "^", CIRCUMFLEX },
	{ "<<", LEFTSHIFT },
	{ ">>", RIGHTSHIFT },
	{ "**", DOUBLESTAR },
	{ "+=", PLUSEQUAL },
	{ "-=", MINUSEQUAL },
	{ "*=", STAREQUAL },
	{ "/=", SLASHEQUAL },
	{ "%=", PERCENTEQUAL },
	{ "&=", AMPEREQUAL },
	{ "|=", VBAREQUAL },
	{ "^=", CIRCUMFLEXEQUAL },
	{ "<<=", LEFTSHIFTEQUAL },
	{ ">>=", RIGHTSHIFTEQUAL },
	{ "**=", DOUBLESTAREQUAL },
	{ "//", DOUBLESLASH },
	{ "//=", DOUBLESLASHEQUAL },
	{ "...", ELLIPSIS },
	{ "->", RARROW },
	{ "@", AT },
	{ "@=", ATEQUAL }
};

struct TokenInfo
{
	TokenInfo(Token type,
			  const std::string& token,
			  std::pair<size_t, size_t> start,
			  std::pair<size_t, size_t> end,
			  const std::string& line)
	: type(type)
	, token(token)
	, start(start)
	, end(end)
	, line(line)
	{

	}

	Token exact_type() const
	{
		if (type == OP && exact_token_types.find(token) != exact_token_types.end()) 
			return exact_token_types.at(token);
		return type;
	}

	bool is_empty() const
	{
		return line == "";
	}

	Token type;
	std::string token;
	std::pair<size_t, size_t> start;
	std::pair<size_t, size_t> end;
	std::string line;
};

inline std::string group(const std::string& choices)
{
	return '(' + choices + ')';
}

template <typename... Strings>
std::string group(const std::string& a, const std::string& b, const Strings&... rest)
{
	return group(a + '|' + b, rest...);
}
std::string group(const std::vector<std::string>& v)
{
	std::string ret = "(";
	for (size_t i = 0; i < v.size() - 1; ++i)
		ret += v[i] + "|";
	return ret + v.back() + ")";
}

template <typename... Strings>
std::string any(const Strings&... strings)
{
	return group(strings...) + '*';
}

template <typename... Strings>
std::string maybe(const Strings&... strings)
{
	return group(strings...) + '?';
}

const std::string Whitespace = R"([ \f\t]*)";
const std::string Comment = R"(#[^\r\n]*)";
const std::string Ignore = Whitespace + any(R"(\\\r?\n)") + Whitespace + maybe(Comment);
const std::string Name = R"(\w+)";
const std::string Identifier = R"(^[^\d\W]\w*\Z)";

const std::string Hexnumber = R"(0[xX](?:_?[0-9a-fA-F])+)";
const std::string Binnumber = R"(0[bB](?:_?[01])+)";
const std::string Octnumber = R"(0[oO](?:_?[0-7])+)";
const std::string Decnumber = R"(?:0(?:_?0)*|[1-9](?:_?[0-9])*)";
const std::string Intnumber = group(Hexnumber, Binnumber, Octnumber, Decnumber);
const std::string Exponent = R"([eE][-+]?[0-9](?:_?[0-9])*)";
const std::string Pointfloat = group(
								R"([0-9](?:_?[0-9])*\.(?:[0-9](?:_?[0-9])*)?)",
								R"(\.[0-9](?:_?[0-9])*)") + maybe(Exponent);
const std::string Expfloat = R"([0-9](?:_?[0-9])*)" + Exponent;
const std::string Floatnumber = group(Pointfloat, Expfloat);
const std::string Imagnumber = group(R"([0-9](?:_?[0-9])*[jJ])", Floatnumber + R"([jJ])");
const std::string Number = group(Imagnumber, Floatnumber, Intnumber);

const std::vector<std::string> all_string_prefixes = {
	"", "fR", "r", "Rb", "b", "RB", "f", "BR", "rb", "F", 
	"Br", "B", "R", "br", "u", "fr", "U", "rB", "Rf", "bR", 
	"FR", "rF", "Fr", "rf", "RF"};

const std::string StringPrefix = group(all_string_prefixes);

const std::string Single = R"([^'\\]*(?:\\.[^'\\]*)*')";
const std::string Double = R"([^"\\]*(?:\\.[^"\\]*)*")";
const std::string Single3 = R"([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')";
const std::string Double3 = R"([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")";
const std::string Triple = group(StringPrefix + R"(''')", StringPrefix + R"(""")");

const std::string String = group(
							StringPrefix + R"('[^\n'\\]*(?:\\.[^\n'\\]*)*')",
							StringPrefix + R"("[^\n"\\]*(?:\\.[^\n"\\]*)*")");

const std::string Operator = group(
								R"(\*\*=?)",
								R"(>>=?)",
								R"(<<=?)", R"(!=)",
								R"(//=?", r"->)",
								R"([+\-*/%&@|^=<>]=?)",
								R"(~)");

const std::string Bracket = R"([][(){}])";
const std::string Special = group(R"(\r?\n)", R"(\.\.\.)", R"([:;.,@])");
const std::string Funny = group(Operator, Bracket, Special);

const std::string PlainToken = group(Number, Funny, String, Name);
const std::string Token = Ignore + PlainToken;

const std::string ContStr = group(
					StringPrefix + R"('[^\n'\\]*(?:\\.[^\n'\\]*)*)" + group("'", R"(\\\r?\n)"),
                	StringPrefix + R"("[^\n"\\]*(?:\\.[^\n"\\]*)*)" + group(R"(")", R"(\\\r?\n)"));

const std::string PseudoExtras = group(R"(\\\r?\n|\Z)", Comment, Triple);
const std::string PseudoToken = Whitespace + group(PseudoExtras, Number, Funny, ContStr, Name);

std::unordered_map<std::string, std::string> generate_endpats()
{
	std::unordered_map<std::string, std::string> endpats;
	for (const auto& prefix : all_string_prefixes) {
		endpats[prefix + "'"] = Single;
		endpats[prefix + "\""] = Double;
		endpats[prefix + "'''"] = Single3;
		endpats[prefix + "\"\"\""] = Double3;
	}
	return endpats;
}

const auto endpats = generate_endpats();

std::unordered_set<std::string> generate_single_quoted()
{
	std::unordered_set<std::string> single_quoted;
	for (const auto& prefix : all_string_prefixes) {
		single_quoted.insert(prefix + "\"");
		single_quoted.insert(prefix + "'");
	}
	return single_quoted;
}

const auto single_quoted = generate_single_quoted();

std::unordered_set<std::string> generate_triple_quoted()
{
	std::unordered_set<std::string> triple_quoted;
	for (const auto& prefix : all_string_prefixes) {
		triple_quoted.insert(prefix + "\"\"\"");
		triple_quoted.insert(prefix + "'''");
	}
	return triple_quoted;
}

const auto triple_quoted = generate_triple_quoted();

const size_t tabsize = 8;

class TokenError : public std::exception {};
class StopTokenizing : public std::exception {};

class Untokenizer
{
public:
	Untokenizer()
		: tokens()
		, prev_row(1)
		, prev_col(0)
		, encoding("")
	{

	}

	void add_whitespace(const std::pair<size_t, size_t>& start)
	{
		size_t row = start.first;
		size_t col = start.second;

		if (row < prev_row || row == prev_row && col < prev_col) {
			throw std::invalid_argument("start (" + std::to_string(row) + ", " +
										std::to_string(col) + ") precedes previous"
										" end (" + std::to_string(prev_row) + ", " +
										std::to_string(prev_col) + ")");
		}

		size_t row_offset = row - prev_row;
		if (row_offset) {
			tokens.push_back(repeat("\\\n", row_offset));
			prev_col = 0;
		}
		size_t col_offset = col - prev_col;
		if (col_offset)
			tokens.push_back(repeat(" ", col_offset));
	}

	std::string untokenize(const std::vector<TokenInfo>& iter)
	{
		std::vector<std::string> indents;
		bool startline = false;
		for (const auto& t : iter) {
			if (t.is_empty()) {
				compat(t, iter);
				break;
			}
			if (t.type == ENCODING) {
				encoding = t.token;
				continue;
			}
			if (t.type == ENDMARKER) {
				break;
			}
			if (t.type == INDENT) {
				indents.push_back(t.token);
				continue;
			}
			else if (t.type == DEDENT) {
				indents.pop_back();
				prev_row = t.end.first;
				prev_col = t.end.second;
				continue;
			}
			else if (t.type == NEWLINE or t.type == NL) {
				startline = true;
			}
			else if (startline and !indents.empty()) {
				auto indent = indents.back();
				if (t.start.second >= indent.size()) {
					tokens.push_back(indent);
					prev_col = indent.size();
				}
				startline = false;
			}
			add_whitespace(t.start);
			tokens.push_back(t.token);
			prev_row = t.end.first;
			prev_col = t.end.second;
			if (t.type == NEWLINE or t.type == NL) {
				++prev_row;
				prev_col = 0;
			}
		}
		std::string ret;
		for (const auto& token : tokens)
			ret += token;
		return ret;
	}

	void compat(const TokenInfo& token, const std::vector<TokenInfo>& iter)
	{
		std::vector<std::string> indents;
		bool startline = (token.type == NEWLINE || token.type == NL);
		bool prevstring = false;

		std::vector<TokenInfo> chain;
		chain.push_back(token);
		chain.insert(chain.end(), iter.begin(), iter.end());

		for (const auto& tok : chain) {
			auto toknum = tok.type;
			auto tokval = tok.token;
			if (toknum == ENCODING) {
				encoding = tokval;
				continue;
			}
			if (toknum == NAME || toknum == NUMBER) {
				tokval += " ";
			}

			if (toknum == STRING) {
				if (prevstring)
					tokval = ' ' + tokval;
				prevstring = true;
			}
			else
				prevstring = false;

			if (toknum == INDENT) {
				indents.push_back(tokval);
				continue;
			}
			else if (toknum == DEDENT) {
				indents.pop_back();
				continue;
			}
			else if (toknum == NEWLINE || toknum == NL) {
				startline = true;
			}
			else if (startline && !indents.empty()) {
				tokens.push_back(indents.back());
				startline = false;
			}
			tokens.push_back(tokval);
		}
	}

private:
	std::vector<std::string> tokens;
	size_t prev_row, prev_col;
	std::string encoding;
};

std::string untokenize(const std::vector<TokenInfo>& iterable)
{
	Untokenizer ut;
	auto out = ut.untokenize(iterable);
	return out;
}

std::string get_normal_name(const std::string orig_enc)
{
	return "utf-8";
}

std::string detect_encoding(std::istream& stream)
{
	return "utf-8";
}

std::vector<TokenInfo> tokenize(std::istream& stream)
{
	std::vector<TokenInfo> tokens;
	size_t lnum = 0;
	size_t parenlev = 0;
	size_t continued = 0;
	std::string numchars = "0123456789";
	std::string contstr = "";
	size_t needcont = 0;
	std::optional<std::string> contline;
	std::vector<size_t> indents = 0;
	std::string endprog;
	std::pair<size_t, size_t> strstart;

	tokens.emplace_back(ENCODING, "utf-8", (0, 0), (0, 0), '');

	std::string last_line = "";
	std::string line = "";

	while (std::getline(stream, line)) {
		lnum += 1
		size_t pos = 0;
		size_t max = line.size();

		if (!contstr.empty()) {
			if (line.empty())
				raise TokenError{};
			std::smatch endmatch;
			if (std::regex_match(line, sm, std::regex(endprog))) {
				size_t end = pos = endmatch.position() + endmatch.length();
				tokens.emplace_back(STRING,
									contstr + line.substr(0, end),
									strstart,
									std::make_pair(lnum, end),
									*contline + line);
				contstr = "";
				needcont = 0;
				contline.reset();
			}
			else if (needcont &&
					 !helpers::ends_with(line, "\\\n") &&
					 !helpers::ends_with(line, "\\\r\n") {
				tokens.emplace_back(ERRORTOKEN,
									contstr + line,
									strstart,
									std::make_pair(lnum, line.size()),
									*contline);
				contstr = "";
				contline.reset();
				continue;
			}
			else {
				contstr += line;
				*contline += line;
				continue;
			}
		}
		else if (parenlev == 0 && !continued) {
			if (line.empty())
				break;
			size_t column = 0;
			while (pos < max) {
				if (line[pos] == ' ')
					++column;
				else if (line[pos] == '\t')
					column = (column / tabsize + 1) * tabsize;
				else if (line[pos] == '\f')
					column = 0;
				else
					break;
				++pos;
			}
			if (pos == max)
				break;

			if (line[pos] == '#' || line[pos] == '\r' || line[pos] == '\n') {
				if (line[pos] == '#') {
					tokens.emplace_back(COMMENT,
										comment_token,
										std::make_pair(lnum, pos),
										std::make_pair(lnum, pos + comment_token.size()),
										line);
					pos += comment_token.size();
				}
				tokens.emplaec_back(NL,
									line.substr(pos),
									std::make_pair(lnum, pos),
									std::make_pair(lnum, line.size()),
									line);
				continue;
			}
			if (column > indents.back()) {
				indents.push_back(column);
				tokens.emplace_back(INDENT,
									line.substr(0, pos),
									std::make_pair(lnum, 0),
									std::make_pair(lnum, pos),
									line);
			}
			while (column < indents.back()) {
				if (std::find(indents.begin(), indents.end(), column) == indents.end()) {
					throw std::runtime_error("unindent does not match any outer indentation level");
				indents.pop_back();
				tokens.push_back(DEDENT,
								"",
								std::make_pair(lnum, pos),
								std::make_pair(lnum, pos),
								line);
				}
			}
		}
		else {
			if (line.empty())
				throw std::runtime_error("EOF in multi-line statement");
			continued = 0;
		}

		while (pos < max) {
			std::smatch psuedomatch;
			if (std::regex_match(line.substr(pos), psuedomatch, std::regex(PseudoToken))) {
				size_t start = sm.position();
				size_t end = start + length();
				if (start == end)
					continue;
				auto spos = std::make_pair(lnum, start);
				auto epos = std::make_pair(lnum, end);
				auto pos = end;
				auto token = line.substr(start, end - start);
				auto initial = line[start];
				if (numchars.find(initial) != std::string::npos ||
					(initial == '.' && token != "." && token != "...")) {
					tokens.emplace_back(NUMBER, token, spos, epos, line);
				}
				else if (initial == '\r' || initial == '\n') {
					if (parenlev > 0)
						tokens.emplace_back(NL, token, spos, epos, line);
					else
						tokens.emplace_back(NEWLINE, token, spos, epos, line);
				}
				else if (initial == '#') {
					tokens.emplace_back(COMMENT, token, spos, epos, line);
				}
				else if (triple_quoted.find(token) != std::string::npos) {
					endprog = endpats[token];
					std::smatch endmatch;
					if (std::regex_match(line.substr(pos), endmatch, std::regex(endprog)) {
						pos = endmatch.position() + endmatch.length();
						token = line.substr(start, pos - start);
						tokens.emplace_back(STRING, token, spos, std::make_pair(lnum, pos), line);
					}
					else {
						strstart = std::make_pair(lnum, start);
						contstr = line.substr(start);
						contline = line;
						break;
					}
				}
				else if (single_quoted.find(token) != std::string::npos ||
						single_quoted.find(token.substr(0, 2) != std::string::npos ||
						single_quoted.find(token.substr(0, 3) != std::string::npos) {
					if (token.back() == '\n') {
						strstart = std::make_pair(lnum, start);
						if (endpats.find(std::string(initial) != endpats.end())
							endprog = endpats[std::string(initial)];
						else if (endpats.find(token.substr(1, 2) != endpats.end())
							endprog = endpats[token.substr(1, 2)];
						else
							endprog = endpats[token.substr(2, 3)];
						contstr = line.substr(start);
						needcont = 1;
						break;
					}
					else {
						tokens.emplace_back(STRING, token, spos, epos, line);
					}
				}
				else if (std::regex_match(std::string(1, initial), std::regex(Identifier)) {
					tokens.emplace_back(NAME, token, spos, epos, line);
				}
				else if (initial == '\\') {
					continued = 1;
				}
				else {
					if (initial == '(' || initial == '[' || initial == '{') {
						++parenlev;
					}
					else if (initial == ')' || initial == ']' || initial == '}') {
						--parenlev;
					}
					tokens.emplace_back(OP, token, spos, epos, line);
				}
			}
			else {
				tokens.emplace_back(ERRORTOKEN
									std::string(1, line_pos),
									std::make_pair(lnum, pos),
									std::make_pair(lnum, pos+1),
									line);
				++pos;
			}
		}
		last_line = line;
	}

	if (last_line && last_line.back() != '\r' && last_line.back() != '\n'
		tokens.emplace_back(NEWLINE,
							"",
							std::make_pair(lnum - 1, last_line.size()),
							std::make_pair(lnum - 1, last_line.size() + 1),
							"");
	for (size_t i = 1; i < indents.size(); ++i)
		tokens.emplace_back(DEDENT,
							"",
							std::make_pair(lnum, 0),
							std::make_pair(lnum, 0),
							"");
	tokens.emplace_back(ENDMARKER,
						"",
						std::make_pair(lnum, 0),
						std::make_pair(lnum, 0),
						"");
}
