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
	{"yanlış", "false"},
	{"doğru", "true"},
	{"HİÇ", "NULL"},
	{"git", "goto"},
	{"bellekal", "malloc"},
	{"tekraral", "realloc"},
	{"bırak", "free"},
	{"bellkopy", "memcpy"},
	{"ip_kopy", "strcpy"},
	{"ip_karşılaştır", "strcmp"},
	{"ip_sınırlı_karşılaştır", "strncmp"},
	{"ip_son_karakter", "strrchr"},
	{"ip_kopyala_sınırlı", "strncpy"},
	{"ip_uzunluk", "strlen"},
	{"ana", "main"},
	{"çık", "exit"},
	{"yazdırf", "printf"},
	{"snyazdırf", "snprintf"},
	{"fyazdırf", "fprintf"},
	{"stdhata", "stderr"},
	{"hata_mesajı", "strerror"},
	{"hatano", "errno"},
	{"dizin_oluştur", "mkdir"},
	{"dyaz", "fwrite"},
	{"dkapat", "fclose"},
	{"daç", "fopen"},
	{"boyut_t", "size_t"},
	{"komut_yürüt", "execvp"},
	{"çatal", "fork"},
	{"beklepid", "waitpid"},
	{"sistem", "system"},
	{"doğrula", "assert"},
	{"boşluk_mu", "isspace"},
	{"harf_mi", "isalpha"},
	{"alfanümerik_mi", "isalnum"},
	{"DOSYA", "FILE"},
	{"hata_mesajı", "strerror"},
	{NULL, NULL}
};

KeywordMap pkmap[] = {
	{"#ekle", "#include"},
	{"#tanımla", "#define"},
	{"#tanımsil", "#undef"},
	{"#eğertanımlı", "#ifdef"},
	{"#değiltanımlı", "#ifndef"},
	{"#eğerson", "#endif"},
	{"#değilse", "#else"},
	{"#yönerge", "#pragma"},

	{"stdgç.b", "stdio.h"},
	{"stdtam.b", "stdint.h"},
	{"stdküt.b", "stdlib.h"},
	{"evrstd.b", "unistd.h"},
	{"hatano.b", "errno.h"},
	{"ckarakter.b", "ctype.h"},
	{"doğrulama.b", "assert.h"},
	{"stdmantık.b", "stdbool.h"},
	{"stdtan.b", "stddef.h"},

	{"sis/türler.b", "sys/types.h"},
	{"sis/bekle.b", "sys/wait.h"},
	{"sistem/durum.h", "sys/stat.h"},
	{"birkere", "once"},
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


const char* find_keyword_preproc(const char* word, size_t len) {
	for (int i = 0; pkmap[i].from != NULL; ++i) {
		if (strncmp(pkmap[i].from, word, len) == 0 && strlen(pkmap[i].from) == len) {
			return pkmap[i].to;
		}
	}
	return find_keyword(word, len);
}
const char* find_keyword_preprocr(const char* word, size_t len) {
	for (int i = 0; pkmap[i].to != NULL; ++i) {
		if (strncmp(pkmap[i].to, word, len) == 0 && strlen(pkmap[i].to) == len) {
			return pkmap[i].from;
		}
	}
	return find_keywordr(word, len);
}
