#include <cassert>
#include "tokenize.hpp"

namespace tokenize_py
{

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
				s.replace(start_pos, from.size(), to);
				start_pos = s.find(from, start_pos + to.size());
			}
			return s;
		}


		bool starts_with(const std::string& s, const std::string& prefix)
		{
			return s.rfind(prefix, 0) == 0;
		}


		bool ends_with(const std::string& s, const std::string& postfix)
		{
			auto pos = s.size() - postfix.size();
			return s.find(postfix, pos) == pos;
		}

		std::string rstrip(std::string s, const std::string& chars)
		{
			while (!s.empty() && chars.find(s.back()) != std::string::npos)
				s.pop_back();
			return s;
		}

		template <typename Container = std::initializer_list<std::string>>
		std::string join(const std::string& delim, const Container& choices)
		{
			std::string res;
			if (choices.size() == 0)
				return res;
			for (size_t i = 0; i < choices.size() - 1; ++i)
				res += *(choices.begin() + i) + delim;
			return res + *(choices.begin() + choices.size() - 1);
		}
	}

	// from token import tok_name, EXACT_TOKEN_TYPES
	// print("const std::unordered_map<std::string, Token> exact_token_types = { ", end='')
	// print(", ".join(f'{{ "{key}", {tok_name[val]} }}' for key, val in EXACT_TOKEN_TYPES.items()), end = ' };\n')
	const std::unordered_map<std::string, Token> exact_token_types = { { "!=", NOTEQUAL }, { "%", PERCENT }, { "%=", PERCENTEQUAL }, { "&", AMPER }, { "&=", AMPEREQUAL }, { "(", LPAR }, { ")", RPAR }, { "*", STAR }, { "**", DOUBLESTAR }, { "**=", DOUBLESTAREQUAL }, { "*=", STAREQUAL }, { "+", PLUS }, { "+=", PLUSEQUAL }, { ",", COMMA }, { "-", MINUS }, { "-=", MINEQUAL }, { "->", RARROW }, { ".", DOT }, { "...", ELLIPSIS }, { "/", SLASH }, { "//", DOUBLESLASH }, { "//=", DOUBLESLASHEQUAL }, { "/=", SLASHEQUAL }, { ":", COLON }, { ":=", COLONEQUAL }, { ";", SEMI }, { "<", LESS }, { "<<", LEFTSHIFT }, { "<<=", LEFTSHIFTEQUAL }, { "<=", LESSEQUAL }, { "=", EQUAL }, { "==", EQEQUAL }, { ">", GREATER }, { ">=", GREATEREQUAL }, { ">>", RIGHTSHIFT }, { ">>=", RIGHTSHIFTEQUAL }, { "@", AT }, { "@=", ATEQUAL }, { "[", LSQB }, { "]", RSQB }, { "^", CIRCUMFLEX }, { "^=", CIRCUMFLEXEQUAL }, { "{", LBRACE }, { "|", VBAR }, { "|=", VBAREQUAL }, { "}", RBRACE }, { "~", TILDE } };

	// from token import tok_name
	// print("std::string to_string(Token token)\n{")
	// print("\tswitch (token) { ", end='')
	// print(" ".join(f'case {tok}: return "{tok}";' for tok in tok_name.values()), end=' ')
	// print(f'default: throw std::runtime_error("Unrecognised token " + std::to_string(token)); }}')
	// print("}")
	std::string to_string(Token token)
	{
        switch (token) { case ENDMARKER: return "ENDMARKER"; case NAME: return "NAME"; case NUMBER: return "NUMBER"; case STRING: return "STRING"; case NEWLINE: return "NEWLINE"; case INDENT: return "INDENT"; case DEDENT: return "DEDENT"; case LPAR: return "LPAR"; case RPAR: return "RPAR"; case LSQB: return "LSQB"; case RSQB: return "RSQB"; case COLON: return "COLON"; case COMMA: return "COMMA"; case SEMI: return "SEMI"; case PLUS: return "PLUS"; case MINUS: return "MINUS"; case STAR: return "STAR"; case SLASH: return "SLASH"; case VBAR: return "VBAR"; case AMPER: return "AMPER"; case LESS: return "LESS"; case GREATER: return "GREATER"; case EQUAL: return "EQUAL"; case DOT: return "DOT"; case PERCENT: return "PERCENT"; case LBRACE: return "LBRACE"; case RBRACE: return "RBRACE"; case EQEQUAL: return "EQEQUAL"; case NOTEQUAL: return "NOTEQUAL"; case LESSEQUAL: return "LESSEQUAL"; case GREATEREQUAL: return "GREATEREQUAL"; case TILDE: return "TILDE"; case CIRCUMFLEX: return "CIRCUMFLEX"; case LEFTSHIFT: return "LEFTSHIFT"; case RIGHTSHIFT: return "RIGHTSHIFT"; case DOUBLESTAR: return "DOUBLESTAR"; case PLUSEQUAL: return "PLUSEQUAL"; case MINEQUAL: return "MINEQUAL"; case STAREQUAL: return "STAREQUAL"; case SLASHEQUAL: return "SLASHEQUAL"; case PERCENTEQUAL: return "PERCENTEQUAL"; case AMPEREQUAL: return "AMPEREQUAL"; case VBAREQUAL: return "VBAREQUAL"; case CIRCUMFLEXEQUAL: return "CIRCUMFLEXEQUAL"; case LEFTSHIFTEQUAL: return "LEFTSHIFTEQUAL"; case RIGHTSHIFTEQUAL: return "RIGHTSHIFTEQUAL"; case DOUBLESTAREQUAL: return "DOUBLESTAREQUAL"; case DOUBLESLASH: return "DOUBLESLASH"; case DOUBLESLASHEQUAL: return "DOUBLESLASHEQUAL"; case AT: return "AT"; case ATEQUAL: return "ATEQUAL"; case RARROW: return "RARROW"; case ELLIPSIS: return "ELLIPSIS"; case COLONEQUAL: return "COLONEQUAL"; case OP: return "OP"; case AWAIT: return "AWAIT"; case ASYNC: return "ASYNC"; case TYPE_IGNORE: return "TYPE_IGNORE"; case TYPE_COMMENT: return "TYPE_COMMENT"; case ERRORTOKEN: return "ERRORTOKEN"; case COMMENT: return "COMMENT"; case NL: return "NL"; case ENCODING: return "ENCODING"; case N_TOKENS: return "N_TOKENS"; case NT_OFFSET: return "NT_OFFSET"; default: throw std::runtime_error("Unrecognised token " + std::to_string(token)); }
	}

	// from tokenize import PseudoToken, endpats, single_quoted, triple_quoted
	// delim = "esc"
	// rs = f'R"{delim}('
	// re = f'){delim}"'
	// print(f'const std::string PseudoToken = {rs}{PseudoToken}{re};')
	const std::string PseudoToken = R"esc([ \f\t]*((\\\r?\n|\Z|#[^\r\n]*|((|U|R|F|RF|rF|f|Rb|fR|b|bR|BR|B|Fr|rB|fr|Rf|RB|rb|u|rf|r|br|Br|FR)'''|(|U|R|F|RF|rF|f|Rb|fR|b|bR|BR|B|Fr|rB|fr|Rf|RB|rb|u|rf|r|br|Br|FR)"""))|(([0-9](?:_?[0-9])*[jJ]|(([0-9](?:_?[0-9])*\.(?:[0-9](?:_?[0-9])*)?|\.[0-9](?:_?[0-9])*)([eE][-+]?[0-9](?:_?[0-9])*)?|[0-9](?:_?[0-9])*[eE][-+]?[0-9](?:_?[0-9])*)[jJ])|(([0-9](?:_?[0-9])*\.(?:[0-9](?:_?[0-9])*)?|\.[0-9](?:_?[0-9])*)([eE][-+]?[0-9](?:_?[0-9])*)?|[0-9](?:_?[0-9])*[eE][-+]?[0-9](?:_?[0-9])*)|(0[xX](?:_?[0-9a-fA-F])+|0[bB](?:_?[01])+|0[oO](?:_?[0-7])+|(?:0(?:_?0)*|[1-9](?:_?[0-9])*)))|(\r?\n|(\~|\}|\|=|\||\{|\^=|\^|\]|\[|@=|@|>>=|>>|>=|>|==|=|<=|<<=|<<|<|;|:=|:|/=|//=|//|/|\.\.\.|\.|\->|\-=|\-|,|\+=|\+|\*=|\*\*=|\*\*|\*|\)|\(|\&=|\&|%=|%|!=))|((|U|R|F|RF|rF|f|Rb|fR|b|bR|BR|B|Fr|rB|fr|Rf|RB|rb|u|rf|r|br|Br|FR)'[^\n'\\]*(?:\\.[^\n'\\]*)*('|\\\r?\n)|(|U|R|F|RF|rF|f|Rb|fR|b|bR|BR|B|Fr|rB|fr|Rf|RB|rb|u|rf|r|br|Br|FR)"[^\n"\\]*(?:\\.[^\n"\\]*)*("|\\\r?\n))|\w+))esc";
	// print('const std::unordered_map<std::string, std::string> endpats = {', end='')
	// print(', '.join(f'{{ {rs}{key}{re}, {rs}{val}{re} }}' for key, val in endpats.items()), end='')
	// print('};')
	const std::unordered_map<std::string, std::string> endpats = { { R"esc(')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(U')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(U")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(U''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(U""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(R')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(R")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(R''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(R""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(F')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(F")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(F''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(F""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(RF')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(RF")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(RF''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(RF""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(rF')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(rF")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(rF''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(rF""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(f')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(f")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(f''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(f""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(Rb')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(Rb")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(Rb''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(Rb""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(fR')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(fR")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(fR''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(fR""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(b')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(b")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(b''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(b""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(bR')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(bR")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(bR''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(bR""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(BR')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(BR")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(BR''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(BR""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(B')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(B")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(B''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(B""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(Fr')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(Fr")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(Fr''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(Fr""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(rB')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(rB")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(rB''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(rB""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(fr')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(fr")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(fr''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(fr""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(Rf')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(Rf")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(Rf''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(Rf""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(RB')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(RB")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(RB''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(RB""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(rb')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(rb")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(rb''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(rb""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(u')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(u")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(u''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(u""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(rf')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(rf")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(rf''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(rf""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(r')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(r")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(r''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(r""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(br')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(br")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(br''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(br""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(Br')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(Br")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(Br''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(Br""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" }, { R"esc(FR')esc", R"esc([^'\\]*(?:\\.[^'\\]*)*')esc" }, { R"esc(FR")esc", R"esc([^"\\]*(?:\\.[^"\\]*)*")esc" }, { R"esc(FR''')esc", R"esc([^'\\]*(?:(?:\\.|'(?!''))[^'\\]*)*''')esc" }, { R"esc(FR""")esc", R"esc([^"\\]*(?:(?:\\.|"(?!""))[^"\\]*)*""")esc" } };
	// print('const std::unordered_set<std::string> single_quoted = {', end='')
	// print(', '.join(f'{rs}{val}{re}' for val in single_quoted), end='')
	// print('};')
	const std::unordered_set<std::string> single_quoted = { R"esc(f")esc", R"esc(B")esc", R"esc(RB")esc", R"esc(rB")esc", R"esc(Rf")esc", R"esc(RF")esc", R"esc(rb')esc", R"esc(Br')esc", R"esc(Br")esc", R"esc(Fr')esc", R"esc(u')esc", R"esc(r')esc", R"esc(u")esc", R"esc(rF')esc", R"esc(')esc", R"esc(F')esc", R"esc(F")esc", R"esc(")esc", R"esc(R")esc", R"esc(rF")esc", R"esc(fR")esc", R"esc(bR")esc", R"esc(RF')esc", R"esc(Fr")esc", R"esc(fr")esc", R"esc(FR')esc", R"esc(r")esc", R"esc(Rb")esc", R"esc(Rb')esc", R"esc(bR')esc", R"esc(br')esc", R"esc(fr')esc", R"esc(R')esc", R"esc(b")esc", R"esc(BR")esc", R"esc(rf')esc", R"esc(f')esc", R"esc(br")esc", R"esc(B')esc", R"esc(rb")esc", R"esc(b')esc", R"esc(RB')esc", R"esc(BR')esc", R"esc(rB')esc", R"esc(FR")esc", R"esc(fR')esc", R"esc(U')esc", R"esc(rf")esc", R"esc(U")esc", R"esc(Rf')esc" };
	// print('const std::unordered_set<std::string> triple_quoted = {', end='')
	// print(', '.join(f'{rs}{val}{re}' for val in triple_quoted), end='')
	// print('};')
	const std::unordered_set<std::string> triple_quoted = { R"esc(br''')esc", R"esc(Br""")esc", R"esc(rB""")esc", R"esc(F""")esc", R"esc(RB''')esc", R"esc(b""")esc", R"esc(rF""")esc", R"esc(Rb""")esc", R"esc(fR""")esc", R"esc(Rb''')esc", R"esc(bR''')esc", R"esc(B""")esc", R"esc(rB''')esc", R"esc(u''')esc", R"esc(Br''')esc", R"esc(br""")esc", R"esc(f''')esc", R"esc(fR''')esc", R"esc(R""")esc", R"esc(rf""")esc", R"esc(RB""")esc", R"esc(Rf''')esc", R"esc(rf''')esc", R"esc(Rf""")esc", R"esc(''')esc", R"esc(rb""")esc", R"esc(b''')esc", R"esc(BR''')esc", R"esc(Fr""")esc", R"esc(fr''')esc", R"esc(bR""")esc", R"esc(r""")esc", R"esc(f""")esc", R"esc(B''')esc", R"esc(Fr''')esc", R"esc(""")esc", R"esc(FR''')esc", R"esc(BR""")esc", R"esc(RF""")esc", R"esc(FR""")esc", R"esc(rF''')esc", R"esc(F''')esc", R"esc(U''')esc", R"esc(rb''')esc", R"esc(u""")esc", R"esc(r''')esc", R"esc(U""")esc", R"esc(RF''')esc", R"esc(fr""")esc", R"esc(R''')esc" };

	// FIXME: Not valid for Python3 identifier which are extended to include unicode
	const std::string Identifier = R"esc([_a-z]\w*$)esc";

	const size_t tabsize = 8;

	TokenInfo::TokenInfo(Token type,
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

	Token TokenInfo::exact_type() const
	{
		if (type == OP && exact_token_types.find(token) != exact_token_types.end())
			return exact_token_types.at(token);
		return type;
	}

	bool TokenInfo::is_empty() const
	{
		return line.empty();
	}

	std::ostream& operator << (std::ostream& stream, const TokenInfo& tokinfo)
	{
		return stream
			<< "[type=" << to_string(tokinfo.type)
			<< ", token=\"" << tokinfo.token << "\""
			<< ", start=(" << tokinfo.start.first << ", " << tokinfo.start.second << ")"
			<< ", end=(" << tokinfo.end.first << ", " << tokinfo.end.second << ")"
			<< ", line=\"" << tokinfo.line << "\"]";
	}

	TokenError::TokenError(const std::string& msg, const std::pair<size_t, size_t>& pos)
		: std::runtime_error(msg + ": (" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")")
	{

	}

	class Untokenizer
	{
	public:
		Untokenizer();

		void add_whitespace(const std::pair<size_t, size_t>& start);
		std::string untokenize(const std::vector<TokenInfo>& iter);
		void compat(const TokenInfo& token, const std::vector<TokenInfo>& iter);

	private:
		std::vector<std::string> tokens;
		size_t prev_row, prev_col;
		std::string encoding;
	};

	Untokenizer::Untokenizer()
		: prev_row(1)
		, prev_col(0)
	{

	}

	void Untokenizer::add_whitespace(const std::pair<size_t, size_t>& start)
	{
		using namespace helpers;

		size_t row = start.first;
		size_t col = start.second;

		if (row < prev_row || row == prev_row && col < prev_col) {
			throw std::range_error("start (" + std::to_string(row) + ", " +
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

	std::string Untokenizer::untokenize(const std::vector<TokenInfo>& iter)
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
			else if (t.type == NEWLINE || t.type == NL) {
				startline = true;
			}
			else if (startline && !indents.empty()) {
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
			if (t.type == NEWLINE || t.type == NL) {
				++prev_row;
				prev_col = 0;
			}
		}
		return helpers::join("", tokens);
	}

	void Untokenizer::compat(const TokenInfo& token, const std::vector<TokenInfo>& iter)
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

	std::pair<std::string, std::vector<std::string>> detect_encoding(std::istream& stream)
	{
		return { "utf-8", {} };
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
		std::vector<size_t> indents = { 0 };
		std::string endprog;
		std::pair<size_t, size_t> strstart;

		tokens.emplace_back(ENCODING, "utf-8", std::pair<size_t, size_t>(0, 0), std::pair<size_t, size_t>(0, 0), "");

		std::string last_line = "";
		std::string line = "";

		while (std::getline(stream, line)) {
			lnum += 1;
			size_t pos = 0;
			size_t max = line.size();

			if (!contstr.empty()) {
				if (line.empty())
					throw TokenError("EOF in multi-line string", strstart);
				std::smatch endmatch;
				if (std::regex_match(line, endmatch, std::regex(endprog))) {
					size_t end = pos = endmatch.position() + endmatch.length();
					tokens.emplace_back(STRING,
						contstr + line.substr(0, end),
						strstart,
						std::pair<size_t, size_t>(lnum, end),
						*contline + line);
					contstr = "";
					needcont = 0;
					contline.reset();
				}
				else if (needcont &&
					!helpers::ends_with(line, "\\\n") &&
					!helpers::ends_with(line, "\\\r\n")) {
					tokens.emplace_back(ERRORTOKEN,
						contstr + line,
						strstart,
						std::pair<size_t, size_t>(lnum, line.size()),
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
						auto comment_token = helpers::rstrip(line.substr(pos), "\r\n");
						tokens.emplace_back(COMMENT,
							comment_token,
							std::make_pair(lnum, pos),
							std::make_pair(lnum, pos + comment_token.size()),
							line);
						pos += comment_token.size();
					}
					tokens.emplace_back(NL,
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
					if (std::find(indents.begin(), indents.end(), column) == indents.end()) 
						throw std::runtime_error("unindent does not match any outer indentation level");
						indents.pop_back();
					tokens.emplace_back(DEDENT,
						"",
						std::make_pair(lnum, pos),
						std::make_pair(lnum, pos),
						line);
				}
			}
			else {
				if (line.empty())
					throw TokenError("EOF in multi-line statement", std::pair<size_t, size_t>(lnum, 0));
				continued = 0;
			}

			while (pos < max) {
				std::match_results<std::string::iterator> pseudomatch;
				if (std::regex_match(line.begin() + pos, line.end(), pseudomatch, std::regex(PseudoToken))) {
					size_t start = std::distance(line.begin(), pseudomatch[1].first);
					size_t end = std::distance(line.begin(), pseudomatch[1].second);;
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
						assert(!helpers::ends_with(token, "\n"));
						tokens.emplace_back(COMMENT, token, spos, epos, line);
					}
					else if (triple_quoted.find(token) != triple_quoted.end()) {
						endprog = endpats.at(token);
						std::match_results<std::string::iterator> endmatch;
						if (std::regex_match(line.begin() + pos, line.end(), endmatch, std::regex(endprog))) {
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
					else if (single_quoted.find(token) != single_quoted.cend() ||
							single_quoted.find(token.substr(0, 2)) != single_quoted.cend() ||
							single_quoted.find(token.substr(0, 3)) != single_quoted.cend()) {
						if (token.back() == '\n') {
							strstart = std::make_pair(lnum, start);
							if (endpats.find(std::string(1, initial)) != endpats.end())
								endprog = endpats.at(std::string(1, initial));
							else if (endpats.find(token.substr(1, 2)) != endpats.end())
								endprog = endpats.at(token.substr(1, 2));
							else
								endprog = endpats.at(token.substr(2, 3));
							contstr = line.substr(start);
							needcont = 1;
							break;
						}
						else {
							tokens.emplace_back(STRING, token, spos, epos, line);
						}
					}
					else if (std::regex_match(std::string(1, initial), std::regex(Identifier))) {
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
					tokens.emplace_back(ERRORTOKEN,
						std::string(1, line[pos]),
						std::make_pair(lnum, pos),
						std::make_pair(lnum, pos + 1),
						line);
					++pos;
				}
			}
			last_line = line;
		}

		if (!last_line.empty() && last_line.back() != '\r' && last_line.back() != '\n')
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

		return tokens;
	}

}