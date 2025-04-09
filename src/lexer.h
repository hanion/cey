#pragma once
#include "string.h"
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
	TOKEN_END = 0,
	TOKEN_INVALID,
	TOKEN_DONT_CARE,
	TOKEN_PREPROC_END,
	TOKEN_COMMENT,
	TOKEN_LITERAL,
	TOKEN_INTEGER,
	TOKEN_SYMBOL,
} TokenType;

typedef struct {
	TokenType type;
	const char* text;
	size_t length;
} Token;

typedef struct {
	const char* content;
	size_t content_length;
	size_t cursor;
	bool preprocessor_mode;
	bool preprocessor_in_string;
} Lexer;



bool is_delimiter(char c);
bool is_operator(char c);
bool is_integer(char c);

bool is_symbol_start(char c);
bool is_symbol(char c);


Lexer lexer_new(StringBuilder sb);

char lexer_advance(Lexer* l);
bool lexer_match_next(Lexer* l, char c);
bool lexer_match_string(Lexer* l, const char* str, size_t len);
void lexer_trim_left(Lexer* l);
void lexer_skip_to_next_line(Lexer* l);


Token lexer_next(Lexer* l);
