#include "lexer.h"
#include "string.h"
#include "lexer.c"
#include "file.c"
#include "dictionary.c"
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>


#define INTERMEDIATE_DIR "./build/int/"

typedef struct {
	char* cc_override;
	bool pack_tight;
} Options;

Options options_new_default() {
	Options op = {
		.cc_override = NULL,
		.pack_tight = false,
	};
	return op;
}

// file_path   must be null terminated
// output_path must be null terminated
bool compile_to_c(const char* file_path, const char* output_path, Options options) {
	bool result = true;
	StringBuilder source = sb_new();
	StringBuilder output = sb_new();

	if (!read_entire_file(file_path, &source)) { result = false; goto defer; }


	Lexer lexer = lexer_new(source);
	Token token = lexer_next(&lexer);

	TokenType prev = TOKEN_DONT_CARE;
	while (token.type != TOKEN_END) {
		if (token.type == TOKEN_SYMBOL || token.type == TOKEN_INTEGER) {
			if (prev == TOKEN_SYMBOL) {
				da_append(&output, ' ');
			}
		}

		if (token.type == TOKEN_SYMBOL) {
			const char* to = find_keyword(token.text, token.length);
			if (to) {
				da_append_many(&output, to, strlen(to));
			} else {
				da_append_many(&output, token.text, token.length);
			}
		} else {
			da_append_many(&output, token.text, token.length);
		}

		if (token.type == TOKEN_COMMENT || token.type == TOKEN_PREPROC_END) {
			da_append(&output, '\n');
		}
		prev = token.type;
		token = lexer_next(&lexer);
	}

	mkdirs_recursive(output_path);
	if (!write_to_file(output_path, &output)) { result = false; goto defer; }

defer:
	free(source.items);
	free(output.items);
	return result;
}

