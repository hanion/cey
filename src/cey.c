#include "compiler.c"

#define DEFAULT_CC "gcc" // or "clang"

int main(int argc, char** argv) {
	char* cc_args[argc+3];
	int cc_argc = 0;

	char* to_compile[argc+3];
	int to_compile_count = 0;

	cc_args[cc_argc++] = DEFAULT_CC;
	cc_args[cc_argc++] = "-x";
	cc_args[cc_argc++] = "c";

	Options op = options_new_default();

	bool parsing_cey_args = false;

	for (int i = 1; i < argc; i++) {
		char* arg = argv[i];
		if (!parsing_cey_args && strcmp(arg, "--") == 0) {
			parsing_cey_args = true;
			continue;
		}

		if (parsing_cey_args) {
			if (strncmp(arg, "--cc=", 5) == 0) {
				op.cc_override = arg + 5;
			} else if (strncmp(arg, "--pack",6) == 0) {
				op.pack_tight = true;
			} else {
				fprintf(stderr, "unknown cey flag: %s\n", arg);
				exit(1);
			}
			continue;
		}

		if (is_cey_file(arg)) {
			to_compile[to_compile_count++] = arg;
		} else {
			cc_args[cc_argc++] = arg;
		}
	}

	if (to_compile_count < 0) {
		fprintf(stderr, "no source file provided\n");
		exit(1);
	} else {
		StringBuilder sb_arg = sb_new();
		while (--to_compile_count >= 0) {
			char* arg = to_compile[to_compile_count];
			sb_arg.count = 0;
			da_append_many(&sb_arg, INTERMEDIATE_DIR, strlen(INTERMEDIATE_DIR));
			const char* filename = get_filename(arg);
			da_append_many(&sb_arg, filename, strlen(filename));
			da_append(&sb_arg, '\0');
			// heap-copy the string since sb_arg will change
			cc_args[cc_argc] = strdup(sb_arg.items);
			bool result = compile_to_c(arg, cc_args[cc_argc], op);
			cc_argc++;
			if (!result) {
				fprintf(stderr, "compilation failed for %s\n", arg);
				// should we cleanup?
				exit(1);
			}
		}
		free(sb_arg.items);
	}

	if (op.cc_override) {
		cc_args[0] = op.cc_override;
	}

	cc_args[cc_argc] = NULL;

	printf("executing: ");
	for (int i = 0; i < cc_argc; ++i) {
		printf("%s ", cc_args[i]);
	}
	printf("\n");

	execvp(cc_args[0], cc_args);

	return 0;
}
