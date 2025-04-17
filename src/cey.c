#include <sys/types.h>
#include <sys/wait.h>
#include "compiler.c"

#define DEFAULT_CC "gcc" // or "clang"
#define PATH_MAX 256

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
			} else if (strncmp(arg, "--yec",5) == 0) {
				op.from_c_to_cy = true;
				op.retain_intermediate = true;
			} else if (strncmp(arg, "--int",5) == 0) {
				op.retain_intermediate = true;
			} else {
				fprintf(stderr, "[ERROR] unknown cey flag: %s\n", arg);
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

	if (to_compile_count <= 0) {
		fprintf(stderr, "[ERROR] no source file provided\n");
		exit(1);
	}

	char tmp_template[] = "/tmp/cey_tmp_XXXXXX";
	char* tmp_dir = mkdtemp(tmp_template);

	for (int i = 0; i < to_compile_count; i++) {
		const char* file = to_compile[i];
		char tmp_file[PATH_MAX];
		snprintf(tmp_file, sizeof(tmp_file), "%s/%s", tmp_dir, get_filename(file));
		cc_args[cc_argc] = strdup(tmp_file);

		if (!compile_to_c(file, cc_args[cc_argc], op)) {
			fprintf(stderr, "[ERROR] compilation failed for %s\n", file);
			exit(1);
		}
		cc_argc++;
	}

	if (op.from_c_to_cy) {
		return 0;
	}

	if (op.cc_override) {
		cc_args[0] = op.cc_override;
	}

	cc_args[cc_argc] = NULL;

	printf("[INFO] executing: ");
	for (int i = 0; i < cc_argc; ++i) {
		printf("%s ", cc_args[i]);
	}
	printf("\n");

	pid_t pid = fork();
	if (pid == 0) {
		execvp(cc_args[0], cc_args);
		exit(1);
	} else {
		int status;
		waitpid(pid, &status, 0);
		if (!op.retain_intermediate) {
			char cmd[256];
			snprintf(cmd, sizeof(cmd), "rm -rf %s", tmp_dir);
			system(cmd);
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			exit(WEXITSTATUS(status));
		}
	}

	// NOTE: intentionally not freeing, less clutter
	return 0;
}
