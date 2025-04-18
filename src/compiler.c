#include "lexer.h"
#include "lexer.c"
#include "file.c"
#include "dictionary.c"
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
	char* cc_override;
	bool pack_tight;
	bool from_c_to_cy;
	bool retain_intermediate;
} Options;

Options options_new_default() {
	Options op = {
		.cc_override = NULL,
		.pack_tight = false,
		.from_c_to_cy = false,
		.retain_intermediate = false,
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

	size_t cursor = 0;
	while (token.type != TOKEN_END) {
		if (!options.pack_tight) {
			while (&(lexer.content[cursor]) != token.text) {
				da_append(&output, lexer.content[cursor]);
				cursor++;
			}
		}

		if (token.type == TOKEN_SYMBOL) {
			const char* to = NULL;
			if (lexer.preprocessor_mode) {
				to = options.from_c_to_cy ? find_keyword_preprocr(token.text, token.length) : find_keyword_preproc(token.text, token.length);
			} else {
				to = options.from_c_to_cy ? find_keywordr(token.text, token.length) : find_keyword(token.text, token.length);
			}
			da_append_many(&output, to ? to : token.text, to ? strlen(to) : token.length);
		} else {
			if (!options.pack_tight || token.type != TOKEN_NEWLINE) {
				da_append_many(&output, token.text, token.length);
			}
		}

		cursor += token.length;
		Token prev = token;
		token = lexer_next(&lexer);

		if (options.pack_tight) {
			if (prev.type == TOKEN_COMMENT || prev.preproc_end) {
				da_append(&output, '\n');
			}
			if (token.type == TOKEN_SYMBOL || token.type == TOKEN_INTEGER) {
				if (prev.type == TOKEN_SYMBOL) {
					da_append(&output, ' ');
				}
			}
		}
	}

	mkdirs_recursive(output_path);
	if (!write_to_file(output_path, &output)) { result = false; goto defer; }

defer:
	free(source.items);
	free(output.items);
	return result;
}

