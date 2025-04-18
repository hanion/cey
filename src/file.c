#pragma once
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "da.h"
#include <sys/stat.h>


bool read_entire_file(const char *path, StringBuilder *sb) {
	bool result = true;

	FILE *f = fopen(path, "rb");
	if (f == NULL) { result = false; goto defer; }
	if (fseek(f, 0, SEEK_END) < 0) { result = false; goto defer; }

#ifndef _WIN32
	long m = ftell(f);
#else
	long long m = _ftelli64(f);
#endif

	if (m < 0) { result = false; goto defer; }
	if (fseek(f, 0, SEEK_SET) < 0) { result = false; goto defer; }

	size_t new_count = sb->count + m;
	if (new_count > sb->capacity) {
		sb->items = realloc(sb->items, new_count);
		assert(sb->items != NULL);
		sb->capacity = new_count;
	}

	fread(sb->items + sb->count, m, 1, f);
	if (ferror(f)) { result = false; goto defer; }
	sb->count = new_count;

defer:
	if (!result) { printf("Could not read file %s: %s\n", path, strerror(errno)); }
	if (f) { fclose(f); }
	return result;
}

bool write_to_file(const char *path, StringBuilder *sb) {
	FILE *f = fopen(path, "wb");
	if (f == NULL) {
		printf("Could not open file for writing: %s\n", strerror(errno));
		return false;
	}

	size_t written = fwrite(sb->items, 1, sb->count, f);
	if (written != sb->count) {
		printf("Error writing to file: %s\n", strerror(errno));
		fclose(f);
		return false;
	}

	fclose(f);
	return true;
}


// recursively create directories in the path
void mkdirs_recursive(const char* path) {
	char tmp[512];
	size_t len = strlen(path);

	for (size_t i = 0; i < len; ++i) {
		if (path[i] == '/' && i > 0) {
			strncpy(tmp, path, i);
			tmp[i] = '\0';
			mkdir(tmp, 0755); // ignore failure, it'll fail if it already exists
		}
	}
}

// file_path must be null terminated
const char* get_filename(const char* file_path) {
	const char* last_slash = strrchr(file_path, '/');
	if (!last_slash) {
		return file_path;
	}
	return last_slash + 1;
}

// str must be null terminated
bool ends_with(const char* str, const char* w) {
	size_t len_str = strlen(str);
	size_t len_w = strlen(w);
	if (len_w > len_str) {
		return false;
	}
	return strcmp(str + len_str - len_w, w) == 0;
}

// filename must be null terminated
bool is_cey_file(const char* filename) {
	return ends_with(filename, ".cy");
}
