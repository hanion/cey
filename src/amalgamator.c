#include "lexer.h"
#include "lexer.c"
#include "file.c"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_PATH 1024
#define INITIAL_CAPACITY 1024
#define MAX_INPUTS 128


typedef struct {
	char **files;
	size_t count;
	size_t capacity;
} IncludedFiles;

IncludedFiles included_files_new() {
	IncludedFiles i = {
		.count = 0,
		.capacity = 64,
		.files = malloc(sizeof(char *) * 64)
	};
	return i;
}
void included_files_add(IncludedFiles *included_files, const char *file) {
	if (included_files->count == included_files->capacity) {
		included_files->capacity *= 2;
		included_files->files = realloc(included_files->files, sizeof(char *) * included_files->capacity);
	}
	included_files->files[included_files->count] = strdup(file);
	included_files->count++;
}
bool included_files_contains(IncludedFiles *included_files, const char *file) {
	for (size_t i = 0; i < included_files->count; i++) {
		if (strcmp(included_files->files[i], file) == 0) {
			return true;
		}
	}
	return false;
}

typedef struct {
	char *items;
	size_t count;
	size_t capacity;
} IncludeDirs;
IncludeDirs include_dirs_new() {
	IncludeDirs id = {
		.items = NULL, .count = 0, .capacity = 0
	};
	return id;
}

typedef struct {
	StringBuilder output;
	IncludedFiles included_files;
	char * source_files[MAX_INPUTS];
	char * include_dirs[MAX_INPUTS];
	size_t source_files_count;
	size_t include_dirs_count;
	const char* output_file;
} Amalgamator;

bool find_file_path(Amalgamator* a, char* filename, StringBuilder* out_fp) {
	if (access(filename, F_OK) == 0) {
		out_fp->items = filename;
		out_fp->count = strlen(filename);
		out_fp->capacity = out_fp->count;
		return true;
	}
	int including_from_dir = 0;
	while (access(out_fp->items, F_OK) != 0) {
		if (including_from_dir >= a->include_dirs_count) {
			return false;
		}
		const char* id = a->include_dirs[including_from_dir++];
		out_fp->count = 0;
		da_append_many(out_fp, id, strlen(id));
		da_append(out_fp, '/');
		da_append_many(out_fp, filename, strlen(filename) + 1); // +1 is the null terminator
	}
	return true;
}

bool process_file(Amalgamator* a, const char *file) {
	StringBuilder source = sb_new();
	if (!read_entire_file(file, &source)) {
		perror("read_entire_file error");
		return false;
	}

	Lexer lexer = lexer_new(source);
	Token token = lexer_next(&lexer);

	size_t cursor = 0;
	bool pack_tight = false;

	while (token.type != TOKEN_END && cursor < lexer.content_length) {
		if (!pack_tight) {
			// catch up to cursor
			while (cursor < lexer.content_length && &(lexer.content[cursor]) != token.text) {
				da_append(&a->output, lexer.content[cursor]);
				cursor++;
			}
		}

		if (lexer.preprocessor_mode && token.type == TOKEN_SYMBOL && strncmp(token.text, "#include", 8)==0) {
			// token is "#include"
			lexer_next(&lexer); // " or <
			Token include_text = lexer_next(&lexer); // the string

			// null terminate include_text.text
			StringBuilder filename = sb_new();
			da_append_many(&filename, include_text.text, include_text.length);
			da_append(&filename, '\0');

			StringBuilder found_fp = sb_new();
			bool included = false;
			if (find_file_path(a, filename.items, &found_fp)) {
				if (!included_files_contains(&a->included_files, found_fp.items)) {
					if (!process_file(a, found_fp.items)) {
						printf("failed to process file %.*s\n", (int)filename.count, filename.items);
						return false;
					}
					included_files_add(&a->included_files, found_fp.items);
				}
				included = true;
			}
			free(filename.items);

			// TODO: we are not handling '\' before newline anywhere
			// catch up to new line
			lexer_skip_until_new_line(&lexer);
			Token nl = lexer_next(&lexer);
			assert(nl.preproc_end && "we assumed newline would be the end of the include");
			while (cursor < lexer.content_length && &(lexer.content[cursor]) != nl.text) {
				if (!included) {
					da_append(&a->output, lexer.content[cursor]);
				}
				cursor++;
			}
		} else if (lexer.preprocessor_mode && token.type == TOKEN_SYMBOL && strncmp(token.text, "#pragma", 7)==0) {
			bool next_is_once = (strncmp(lexer_next(&lexer).text, "once", 4) == 0);
			if (next_is_once) {
				lexer_skip_until_new_line(&lexer);
				Token nl = lexer_next(&lexer);
				assert(nl.preproc_end && "we assumed newline would be the end of the pragma once");
				while (cursor < lexer.content_length && &(lexer.content[cursor]) != nl.text) {
					cursor++;
				}
			}
		} else {
			// any other token
			if (!pack_tight || token.type != TOKEN_NEWLINE) {
				da_append_many(&a->output, token.text, token.length);
			}
			cursor += token.length;
		}

		Token prev = token;
		token = lexer_next(&lexer);

		if (pack_tight) {
			if (prev.type == TOKEN_COMMENT || prev.preproc_end) {
				da_append(&a->output, '\n');
			}
			if (token.type == TOKEN_SYMBOL || token.type == TOKEN_INTEGER) {
				if (prev.type == TOKEN_SYMBOL) {
					da_append(&a->output, ' ');
				}
			}
		}
	}

	free(source.items);
	return true;
}

void print_usage() {
	printf("usage: amalgamator <source_files> -I <include_dirs> -o <output_file>\n");
}

int main(int argc, char *argv[]) {
	if (argc < 5) {
		print_usage();
		return 1;
	}

	Amalgamator a = {
		.output = sb_new(),
		.included_files = included_files_new(),
		.source_files_count = 0,
		.include_dirs_count = 0,
		.output_file = NULL
	};

	int opt;
	while ((opt = getopt(argc, argv, "I:o:")) != -1) {
		switch (opt) {
			case 'I':
				a.include_dirs[a.include_dirs_count++] = optarg;
				break;
			case 'o':
				a.output_file = optarg;
				break;
			default:
				print_usage();
				return 1;
		}
	}

	for (int i = optind; i < argc; i++) {
		a.source_files[a.source_files_count++] = argv[i];
	}

	if (a.source_files_count == 0 || a.output_file == NULL) {
		print_usage();
		return 1;
	}


	bool proc = process_file(&a, a.source_files[0]);
	if (!proc) {
		return 1;
	}

	mkdirs_recursive(a.output_file);
	if (!write_to_file(a.output_file, &a.output)) {
		perror("write_to_file error");
		free(a.output.items);
		return 1;
	}
	free(a.output.items);

	printf("amalgamation completed successfully.\n");
	return 0;
}

