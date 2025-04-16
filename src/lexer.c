#include "lexer.h"
#include <assert.h>
#include <ctype.h>

bool is_delimiter(char c) {
	return (c == '+' || c == '-'
		|| c == '*' || c == '/' || c == ','
		|| c == ';' || c == '%' || c == '>'
		|| c == '<' || c == '=' || c == '('
		|| c == ')' || c == '[' || c == ']'
		|| c == '{' || c == '}');
}

bool is_operator(char c) {
	return (c == '+' || c == '-' || c == '*'
		|| c == '/' || c == '>' || c == '<'
		|| c == '=');
}

bool is_integer(char c) {
	return (c >= '0' && c <= '9');
}

bool is_symbol_start(char c) {
	return (unsigned char)c >= 128 || isalpha(c) || c == '_';
}

bool is_symbol(char c) {
	return (unsigned char)c >= 128 || isalnum(c) || c == '_';
}


Lexer lexer_new(StringBuilder sb) {
	Lexer lexer = {
		.content = sb.items,
		.content_length = sb.count,
		.cursor = 0,
		.preprocessor_mode = false,
		.preprocessor_in_string = false
	};
	return lexer;
}

char lexer_advance(Lexer* l) {
	assert(l->cursor < l->content_length);
	char c = l->content[l->cursor];
	l->cursor++;
	return c;
}

bool lexer_match(Lexer* l, char c) {
	if (l->cursor < l->content_length) {
		return c == l->content[l->cursor];
	}
	return false;
}

bool lexer_match_next(Lexer* l, char c) {
	if (l->cursor + 1 < l->content_length) {
		return c == l->content[l->cursor + 1];
	}
	return false;
}

bool lexer_match_string(Lexer* l, const char* str, size_t len) {
	size_t i = 0;
	while (l->cursor+i < l->content_length && i < len) {
		if (l->content[l->cursor+i] != str[i]) {
			return false;
		}
		++i;
	}
	return true;
}

void lexer_trim_left(Lexer* l) {
	while (l->cursor < l->content_length && isspace(l->content[l->cursor])) {
		lexer_advance(l);
	}
}
void lexer_skip_to_next_line(Lexer* l) {
	while (l->cursor < l->content_length && l->content[l->cursor] != '\n') {
		lexer_advance(l);
	}
}


Token lexer_next(Lexer* l) {
	lexer_trim_left(l);

	Token token = {
		.type = TOKEN_END,
		.text = &l->content[l->cursor],
		.length = 0
	};

	if (l->cursor >= l->content_length) {
		return token;
	}


	if (lexer_match(l, '/')) {
		size_t start = l->cursor;
		if (lexer_match_next(l, '/')) {
			lexer_skip_to_next_line(l);
			token.length = l->cursor-start;
			token.type = TOKEN_COMMENT;
			return token;
		}
	}

	if (lexer_match(l, '#')) {
		lexer_advance(l);
		l->preprocessor_mode = true;
		// 'ekle'
		Token next = lexer_next(l);
		// add '#'
		next.text = token.text;
		next.length++;
		return next;
	}

	if (l->preprocessor_mode) {
		if (lexer_match(l, '>')) {
			lexer_advance(l);
			l->preprocessor_mode = false;
			token.type = TOKEN_PREPROC_END;
			token.length = 1;
			return token;
		}
		if (lexer_match(l, '"')) {
			lexer_advance(l);
			if (!l->preprocessor_in_string) {
				l->preprocessor_in_string = true;
				token.type = TOKEN_DONT_CARE;
				token.length = 1;
				return token;
			}
			l->preprocessor_in_string = false;
			l->preprocessor_mode = false;
			token.type = TOKEN_PREPROC_END;
			token.length = 1;
			return token;
		}
	}

	if (!l->preprocessor_mode && lexer_match(l, '"')) {
		lexer_advance(l);
		token.type = TOKEN_LITERAL;
		token.length++;
		while (l->cursor < l->content_length) {
			if (lexer_match(l, '\n') || lexer_match(l, '"')) {
				break;
			}
			token.length++;
			l->cursor++; ;
		}
		return token;
	}

	if (is_symbol_start(l->content[l->cursor])) {
		token.type = TOKEN_SYMBOL;
		while (l->cursor < l->content_length) {
			if (!is_symbol(l->content[l->cursor])) {
				if (l->preprocessor_mode && lexer_match(l, '.')) {
					// the '.' in #ekle <stdgç.b>
				} else {
					break;
				}
			}
			l->cursor++;
			token.length++;
		}
		return token;
	}

	char c = l->content[l->cursor];
	if (is_delimiter(c) || is_operator(c) || is_integer(c)) {
		token.type = is_integer(c) ? TOKEN_INTEGER : TOKEN_DONT_CARE;
		while (l->cursor < l->content_length) {
			if (lexer_match(l, '/') && lexer_match_next(l, '/')) {
				break;
			}
			char current = l->content[l->cursor];
			if (!is_delimiter(current) && !is_operator(current) && !is_integer(current)) {
				break;
			}
			l->cursor++;
			token.length++;
		}
		return token;
	}

	// unrecognized string
	l->cursor++;
	token.length = 1;
	token.type = TOKEN_INVALID;
	return token;
}


