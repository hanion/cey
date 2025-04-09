#include "string.h"
#include "dictionary.c"
#include "lexer.c"
#include "file.c"

#define SOURCE "./src/main.cy"
#define RESULT "./build/result.c"

int main(void) {
	StringBuilder source = {
		.items = NULL, .count = 0, .capacity = 0
	};
	read_entire_file(SOURCE, &source);


	StringBuilder result = {
		.items = NULL, .count = 0, .capacity = 0
	};

	Lexer lexer = lexer_new(source);
	Token token = lexer_next(&lexer);

	TokenType prev = TOKEN_DONT_CARE;
	while (token.type != TOKEN_END) {
		if (token.type == TOKEN_SYMBOL || token.type == TOKEN_INTEGER) {
			if (prev == TOKEN_SYMBOL) {
				da_append(&result, ' ');
			}
		}

		if (token.type == TOKEN_SYMBOL) {
			const char* to = find_keyword(token.text, token.length);
			if (to) {
				da_append_many(&result, to, strlen(to));
			} else {
				da_append_many(&result, token.text, token.length);
			}
		} else {
			da_append_many(&result, token.text, token.length);
		}

		if (token.type == TOKEN_COMMENT || token.type == TOKEN_PREPROC_END) {
			da_append(&result, '\n');
		}
		prev = token.type;
		token = lexer_next(&lexer);
	}
	write_to_file(RESULT, &result);

	free(source.items);
	free(result.items);
	return 0;
}
