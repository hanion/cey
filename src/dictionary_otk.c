#include "lexer.h"
#include <string.h>

typedef struct {
	const char* from;
	const char* to;
} KeywordMap;

// 𐱅𐰇𐰼𐰜
// https://www.turkbitig.com
KeywordMap kmap[] = {
	{"𐰆𐱃𐰆", "auto"},
	{"𐰶𐰃𐰺", "break"},
	{"𐰢𐰀𐰦𐰶", "bool"},
	{"𐰑𐰆𐰺𐰢", "case"},
	{"𐰴𐰀𐰺", "char"},
	{"𐰽𐰀𐰋𐰃𐱅", "const"},
	{"𐰓𐰀𐰉𐰢", "continue"},
	{"𐰉𐰀𐰺𐰽𐰀𐰖𐰃𐰞𐰀𐰣", "default"},
	{"𐰖𐰀𐰯", "do"},
	{"𐰲𐰃𐰯𐱅𐰀", "double"},
	{"𐰓𐰀𐰏𐰃𐰠𐰾𐰀", "else"},
	{"𐰽𐰃𐰺𐰀𐰞𐰃", "enum"},
	{"𐰑𐰃𐱁𐰀𐰺𐰑𐰀𐰣", "extern"},
	{"𐰴𐰀𐰖𐰣", "float"},
	{"𐰃𐰲𐰤", "for"},
	{"𐰏𐰃𐱅", "goto"},
	{"𐰀𐰏𐰼", "if"},
	{"𐱃𐰀𐰢", "int"},
	{"𐰆𐰔𐰣", "long"},
	{"𐰴𐰀𐰖𐰃𐱃", "register"},
	{"𐰓𐰇𐰦𐰼", "return"},
	{"𐰶𐰃𐰽𐰀", "short"},
	{"𐰃𐱁𐰀𐰺𐱅𐰠𐰃", "signed"},
	{"𐰉𐰆𐰖𐱃", "sizeof"},
	{"𐰽𐱃𐰀𐱅𐰃𐰚", "static"},
	{"𐰖𐰀𐰯𐰃", "struct"},
	{"𐰾𐰀𐰲", "switch"},
	{"𐱅𐰇𐰼𐱃𐰀𐰣𐰃𐰢𐰞𐰀", "typedef"},
	{"𐰋𐰃𐰼𐰠𐰀𐱁𐰃𐰢", "union"},
	{"𐰃𐱁𐰀𐰺𐱅𐰾𐰃𐰔", "unsigned"},
	{"𐰉𐰆𐱁𐰞𐰸", "void"},
	{"𐰆𐰖𐰣𐰀𐰴", "volatile"},
	{"𐰃𐰚𐰀𐰤", "while"},
	{"𐰖𐰀𐰣𐰞𐰃𐱁", "false"},
	{"𐰑𐰆𐰍𐰺𐰆", "true"},
	{"𐰚𐰃𐰲", "NULL"},
	{"𐰋𐰀𐰠𐰠𐰀𐰴𐰞", "malloc"},
	{"𐱅𐰀𐰚𐰺𐰀𐰺𐰞", "realloc"},
	{"𐰉𐰃𐰺𐰀𐰴", "free"},
	{"𐰋𐰀𐰠𐰠𐰸𐰆𐰯𐰖", "memcpy"},
	{"𐰃𐰯_𐰸𐰆𐰯𐰖", "strcpy"},
	{"𐰃𐰯_𐰴𐰀𐰺𐱁𐰃𐰞𐰀𐱁𐱃𐰃𐰺", "strcmp"},
	{"𐰃𐰯_𐰽𐰃𐰣𐰺𐰞𐰃_𐰴𐰀𐰺𐱁𐰃𐰞𐰀𐱁𐱃𐰃𐰺", "strncmp"},
	{"𐰃𐰯_𐰽𐰆𐰣_𐰴𐰀𐰺𐰴𐱅𐰀𐰼", "strrchr"},
	{"𐰃𐰯_𐰸𐰆𐰯𐰖𐰀𐰞𐰀_𐰽𐰃𐰣𐰺𐰞𐰃", "strncpy"},
	{"𐰃𐰯_𐰆𐰔𐰣𐰞𐰸", "strlen"},
	{"𐰀𐰣𐰀", "main"},
	{"𐰲𐰶", "exit"},
	{"𐰖𐰀𐰔𐰑𐰃𐰺𐰯", "printf"},
	{"𐰽𐰪𐰀𐰔𐰑𐰃𐰺𐰯", "snprintf"},
	{"𐰯𐰖𐰀𐰔𐰑𐰃𐰺𐰯", "fprintf"},
	{"𐰽𐱃𐰑𐰴𐰀𐱃𐰀", "stderr"},
	{"𐰴𐰀𐱃𐰀_𐰢𐰀𐰾𐰲𐰃", "strerror"},
	{"𐰴𐰀𐱃𐰣𐰆", "errno"},
	{"𐰓𐰃𐰔𐰤_𐰆𐰞𐱁𐱃𐰆𐰺", "mkdir"},
	{"𐰑𐰖𐰀𐰔", "fwrite"},
	{"𐰑𐰴𐰀𐰯𐱃", "fclose"},
	{"𐰑𐰀𐰲", "fopen"},
	{"𐰉𐰆𐰖𐱃_𐱃", "size_t"},
	{"𐰸𐰆𐰢𐱃_𐰘𐰇𐰼𐱅", "execvp"},
	{"𐰲𐰀𐱃𐰞", "fork"},
	{"𐰋𐰀𐰚𐰠𐰀𐰯𐰃𐰓", "waitpid"},
	{"𐰾𐰃𐰾𐱅𐰀𐰢", "system"},
	{"𐰑𐰆𐰍𐰺𐰆𐰞𐰀", "assert"},
	{"𐰉𐰆𐱁𐰞𐰸_𐰢𐰆", "isspace"},
	{"𐰴𐰀𐰺𐰯_𐰢𐰃", "isalpha"},
	{"𐰀𐰞𐰯𐰀𐰣𐰇𐰢𐰀𐰼𐰃𐰚_𐰢𐰃", "isalnum"},
	{"𐰑𐰆𐰽𐰖𐰀", "FILE"},
	{"𐰴𐰀𐱃𐰀_𐰢𐰀𐰾𐰲𐰃", "strerror"},
	{NULL, NULL}
};

KeywordMap pkmap[] = {
	{"#𐰀𐰚𐰠𐰀", "#include"},
	{"#𐱃𐰀𐰣𐰃𐰢𐰞𐰀", "#define"},
	{"#𐱃𐰀𐰣𐰃𐰢𐰾𐰃𐰠", "#undef"},
	{"#𐰀𐰏𐰼𐱃𐰀𐰣𐰃𐰢𐰞𐰃", "#ifdef"},
	{"#𐰓𐰀𐰏𐰃𐰡𐰀𐰣𐰃𐰢𐰞𐰃", "#ifndef"},
	{"#𐰀𐰏𐰼𐰽𐰆𐰣", "#endif"},
	{"#𐰓𐰀𐰏𐰃𐰠𐰾𐰀", "#else"},
	{"#𐰘𐰇𐰤𐰀𐰼𐰏𐰀", "#pragma"},

	{"𐰽𐱃𐰑𐰍𐰲.𐰉", "stdio.h"},
	{"𐰽𐱃𐰑𐱃𐰀𐰢.𐰉", "stdint.h"},
	{"𐰽𐱃𐰑𐰚𐰇𐱅.𐰉", "stdlib.h"},
	{"𐰀𐰋𐰼𐰾𐱅𐰓.𐰋", "unistd.h"},
	{"𐰴𐰀𐱃𐰣𐰆.𐰉", "errno.h"},
	{"𐰲𐰴𐰀𐰺𐰴𐱅𐰀𐰼.𐰋", "ctype.h"},
	{"𐰑𐰆𐰍𐰺𐰆𐰞𐰀𐰢𐰀.𐰉", "assert.h"},
	{"𐰽𐱃𐰑𐰢𐰀𐰦𐰶.𐰉", "stdbool.h"},
	{"𐰽𐱃𐰑𐱃𐰀𐰣.𐰉", "stddef.h"},
	{"𐰃𐰯.𐰋", "string.h"},

	{"𐰾𐰃𐰾/𐱅𐰇𐰼𐰠𐰀𐰼.𐰉", "sys/types.h"},
	{"𐰾𐰃𐰾/𐰋𐰀𐰚𐰠𐰀.𐰉", "sys/wait.h"},
	{"𐰾𐰃𐰾𐱅𐰀𐰢/𐰑𐰆𐰺𐰢.𐰉", "sys/stat.h"},
	{"𐰋𐰃𐰼𐰚𐰀𐰼𐰀", "once"},
	{NULL, NULL}
};

const char* find_keyword(const char* word, size_t len) {
	for (int i = 0; kmap[i].from != NULL; ++i) {
		if (strlen(kmap[i].from) == len && strncmp(kmap[i].from, word, len) == 0) {
			return kmap[i].to;
		}
	}
	return NULL;
}

const char* find_keywordr(const char* word, size_t len) {
	for (int i = 0; kmap[i].to != NULL; ++i) {
		if (strlen(kmap[i].to) == len && strncmp(kmap[i].to, word, len) == 0) {
			return kmap[i].from;
		}
	}
	return NULL;
}


const char* find_keyword_preproc(const char* word, size_t len) {
	for (int i = 0; pkmap[i].from != NULL; ++i) {
		if (strlen(pkmap[i].from) == len && strncmp(pkmap[i].from, word, len) == 0) {
			return pkmap[i].to;
		}
	}
	return find_keyword(word, len);
}
const char* find_keyword_preprocr(const char* word, size_t len) {
	for (int i = 0; pkmap[i].to != NULL; ++i) {
		if (strlen(pkmap[i].to) == len && strncmp(pkmap[i].to, word, len) == 0) {
			return pkmap[i].from;
		}
	}
	return find_keywordr(word, len);
}

const char* find_token(Token* token, bool preproc, bool reverse) {
	if (preproc) {
		if (reverse) return find_keyword_preprocr(token->text.items, token->text.count);
		else         return find_keyword_preproc (token->text.items, token->text.count);
	} else {
		if (reverse) return find_keywordr(token->text.items, token->text.count);
		else         return find_keyword (token->text.items, token->text.count);
	}
}
