#include "compiler.c"

void print_usage() {
	printf("[INFO] usage: yec <source_file> <destination_file>\n");
}

int main(int argc, char** argv) {
	if (argc != 3) {
		print_usage();
		exit(1);
	}

	if (access(argv[1], F_OK) != 0) {
		fprintf(stderr, "[ERROR] source file does not exist: '%s'\n", argv[1]);
		print_usage();
		exit(2);
	}

	if (access(argv[2], F_OK) == 0) {
		fprintf(stderr, "[ERROR] destination file already exists: '%s'\n", argv[2]);
		print_usage();
		exit(3);
	}

	Options op = options_new_default();
	op.from_c_to_cy = true;

	return !compile_to_c(argv[1], argv[2], op);
}
