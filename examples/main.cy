#ekle "stdgç.b"
#ekle <stdtam.b>
#ekle <stdküt.b>
#ekle <evrstd.b>
//#ekle <_123-"\">;

sabit kar* mesaj = "merhaba dunya";

tam ana(boşluk) {
	tam* dizi = (tam*) bellekal(5 * boyut(tam));//yorum testi

	eğer (dizi == 0) {
		yazdırf("Bellek alınamadı\n");
		çık(1); 
	}

	için (tam i = 0; i < 5; i++) {
		dizi[i] = i * 2;
	}

	yazdırf("Başlangıç dizisi:\n");
	için (tam i = 0; i < 5; i++) {
		yazdırf("%d ", dizi[i]);
	}
	yazdırf("\n");

	dizi = (tam*) tekraral(dizi, 15 * boyut(tam));
	eğer (dizi == 0) {
		yazdırf("Bellek yeniden alınamadı\n");
		çık(1);
	}

	için (tam i = 5; i < 15; i++) {
		dizi[i] = i * 2;
	}

	yazdırf("Genişletilmiş dizi:\n");
	için (tam i = 0; i < 15; i++) {
		yazdırf("%d ", dizi[i]);
	}
	yazdırf("\n");

	bırak(dizi);
	döndür 0;
}
