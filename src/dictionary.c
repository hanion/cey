#include <string.h>

typedef struct {
	const char* from;
	const char* to;
} KeywordMap;

KeywordMap kmap[] = {
	{"oto", "auto"},
	{"kır", "break"},
	{"mantık", "bool"},
	{"durum", "case"},
	{"kar", "char"},
	{"sabit", "const"},
	{"devam", "continue"},
	{"varsayılan", "default"},
	{"yap", "do"},
	{"çifte", "double"},
	{"değilse", "else"},
	{"sıralı", "enum"},
	{"dışardan", "extern"},
	{"kayan", "float"},
	{"için", "for"},
	{"git", "goto"},
	{"eğer", "if"},
	{"tam", "int"},
	{"uzun", "long"},
	{"kayıt", "register"},
	{"döndür", "return"},
	{"kısa", "short"},
	{"işaretli", "signed"},
	{"boyut", "sizeof"},
	{"statik", "static"},
	{"yapı", "struct"},
	{"seç", "switch"},
	{"türtanımla", "typedef"},
	{"birleşim", "union"},
	{"işaretsiz", "unsigned"},
	{"boşluk", "void"},
	{"oynak", "volatile"},
	{"iken", "while"},

	{"#ekle", "#include"},
	{"#tanımla", "#define"},
	{"#tanımsil", "#undef"},
	{"#eğertanımlı", "#ifdef"},
	{"#değiltanımlı", "#ifndef"},
	{"#eğerson", "#endif"},

	{"stdgç.b", "stdio.h"},
	{"stdtam.b", "stdint.h"},
	{"stdgç", "stdio.h"},
	{"stdtam", "stdint.h"},
	{"ip", "string"},
	{"evrstd", "unistd"},

	{"bellekal", "malloc"},
	{"tekraral", "realloc"},
	{"bırak", "free"},
	{"bellkopy", "memcpy"},
	{"ipkopy", "strcpy"},

	{"yazdırf", "printf"},
	{"ana", "main"},
	{"çık", "exit"},

	{NULL, NULL}
};

const char* find_keyword(const char* word, size_t len) {
	for (int i = 0; kmap[i].from != NULL; ++i) {
		if (strncmp(kmap[i].from, word, len) == 0 && strlen(kmap[i].from) == len) {
			return kmap[i].to;
		}
	}
	return NULL;
}

const char* find_keywordr(const char* word, size_t len) {
	for (int i = 0; kmap[i].to != NULL; ++i) {
		if (strncmp(kmap[i].to, word, len) == 0 && strlen(kmap[i].to) == len) {
			return kmap[i].from;
		}
	}
	return NULL;
}

