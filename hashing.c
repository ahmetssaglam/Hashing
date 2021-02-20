/* 
@file
BLM3021 2020-2021 GUZ ODEV-2
Bu programda M elemanli bir hash tablosu dosyadan okunarak, input dosyalarindan hash tablosuna kelimeler eklenme modulu ve tabloda kelime arama modulu gerceklenmistir.
Programdan cikilirken guncel hash tablosu, okundugu dosyaya geri yazilir.

@author
Ýsim: Ahmet Said SAÐLAM
Öðrenci No: 17011501
Tarih: 01.12.2020
E-Mail: l1117501@std.yildiz.edu.tr
Compiler: TDM-GCC 4.9.2 64 bit-Release
IDE: DEV-C++ (version 5.11)
Ýþletim Sistemi: Windows 10 Pro 64 bit
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>
#define MM 996 //double hashing hesabinda, iki numarali keyi hesaplarken kullanilacak olan sayi
#define M 997 //hash tablosunun boyutu
#define BUFFER_SIZE 10000 //txt dosyadan alýnan satýrýn saklanacaðý bufferin boyutu
#define TEXT_NAME_SIZE 20 // max file ismi uzunlugu
#define AYRAC " "	//kelimelerin ayrilacagi delim ifadesi
#define HORNER_NUMBER 31	//horner numarasi hesaplanirken kullanilcak olan asal sayi
#define WORD_SIZE 20	//alinan bir kelimenin maksimum uzunlugu
#define TEXT_NUMBER 20 //en fazla kac adet text eklenebilir tutan makro
#define HASH_TXT "17011501.txt"	//hash tablosunun kaydedildigi dosyanin ismi

//hash tablosunun her bir gozunde tutalacak olan yapi
typedef struct node {
	int text_count; //kelimenin kac adet dosyada gectigini tutan degisken
	float loadFactor;	//tablonun doluluk orani
	char word[WORD_SIZE]; //tutulan kelime
	char texts[TEXT_NUMBER][TEXT_NAME_SIZE]; //kelimenin gectigi textleri tutan matris
	
} node;

//icine verilen kelimenin horner sayisini donduren fonksiyon
long long getHorner(char *word) {
	long long key = 0L; //kelimenin sayisal key karþýlýðýný tutacak olan degisken
	int i; //dongu degiskeni
	//kelimenin horner sayisi hesaplanir
	for(i = 0; i < strlen(word); i++) {
		key = (long long) (HORNER_NUMBER * key + (word[i] - 'a' + 1));
	}
	return (long long) key;
	
}

//dosyadan okunan satirdaki kelimeyi alip tabloya ekleyen fonksiyon
int insertTable(char *word, char *file_name, int *total_word_count, struct node *hash_table) {
	int i = 0; //adres hesabinda adim sayisi
	long long key = getHorner(word);	//kelimenin horner sayisi
	int adr;	//kelimenin tablodaki adresi
	
	//adres double probing yontemi ile hesaplanir
	int h1_key = key % M;
	int h2_key = 1 + (key % MM);
	adr = (h1_key + (i * h2_key)) % M;
	i++;	//adim sayisi olasi bir yeni hesaplama icin guncellenir
	
	//printf("\n\n %s , adres: %d\n\n",word,adr);
	//system("pause");
	
	//load factor 0.8'in ustundeyse uyari verilir
	if(hash_table[0].loadFactor > 0.8 && hash_table[0].loadFactor < 1) {
		printf("UYARI. Load Factor sinirina yaklasiyorsunuz!\nLoad Factor : %.2f\n",hash_table[0].loadFactor);
	}
	//tablo dolduysa uyari verilir
	if(hash_table[0].loadFactor >= 1) {
		printf("UYARI. Load Factor sinirini gectiniz!\nLoad Factor : %.2f\n",hash_table[0].loadFactor);
		
		//eger kelime daha onceden tabloda var mi diye tablo gezilir
		while(i <= M && strcmp(hash_table[adr].word, word) != 0) {
			adr = (h1_key + (i * h2_key)) % M;
			i++;
		}
		//kelime tabloda mevcutsa
		if(strcmp(hash_table[adr].word, word) == 0) {
			int j = 0;
			//bir kelime ayný dosyada birden fazla kez gecebilir. sadece farkli dosyada gecti ise dosya ismini tabloda ilgili yere eklemelidir
			//dosya isimlerini kontrol edip tekrar olmamasýný saglayan while dongusu ve hemen pesinden gelen if kosulu
			while(strcmp(hash_table[adr].texts[j], file_name) != 0 && j < hash_table[adr].text_count ) {
				j++;
			}
			//kelimenin gectigi dosya, kelimenin dosyalarini tutan matrise eklenmemisse eklenir
			if(j >= hash_table[adr].text_count) {
				hash_table[adr].text_count++; //text sayisini arttir
				strcpy(hash_table[adr].texts[(hash_table[adr].text_count - 1)], file_name); //text ismini structa kopyala
			}
			
			return -1;
		}
		//kelime tabloda mevcut degilse, tabloya eklenemedigi uyarisi verilir
		else {	
			printf("%s kelimesi tabloya eklenemedi\n",word); //bilgilendirme printi
			return 2; //fonksiyondan cikilir
		}		
	}
	//kelimenin adresi ilk aramada bossa kelime eklenmemis demektir
	if(hash_table[adr].text_count == 0) {		
		struct node newnode; //yeni bir node tanimlanir
		strcpy(newnode.word, word);	//kelime yeni node atanir
		
		//printf("\n\n %s , adres: %d\n\n",word,adr);
		//system("pause");
		
		strcpy(newnode.texts[0], file_name); //file name yeni node atanir
		
		newnode.text_count = 1;	//node'un file countu guncellenir
		
		hash_table[adr] = newnode;	//tabloda ilgili adres node'a esitlenir
		*total_word_count = *total_word_count + 1;	//toplam kelime sayisi arttirilir
		hash_table[0].loadFactor = (float) *total_word_count / (float) M; //yeni load factor hesaplanip tabloda saklanir
		printf("%s kelimesi hash tablosuna eklendi\n",hash_table[adr].word);	//bilgilendirme printleri yazilir
		printf("Load Factor : %.3f\n",hash_table[0].loadFactor);

		return 1;	//fonksiyondan cikilir
	}
	else {
		//ilk aramada hash tablosunun gözü doluysa kelime kontrolü yapýlarak ve gerekirse yeni adres hesaplanarak ilerlenir
		while(hash_table[adr].text_count != 0 && strcmp(hash_table[adr].word, word) != 0) {
			adr = (h1_key + (i * h2_key)) % M;
			i++;
		}
		//whiledan kelime ile karsilasildigi icin cikildiysa, dosya ismi eklenmek uzere if kosuluna girilir
		if(strcmp(hash_table[adr].word, word) == 0) {
			int j = 0;
			//bir kelime ayný dosyada birden fazla kez gecebilir. sadece farkli dosyada gecti ise dosya ismini tabloda ilgili yere eklemelidir
			//dosya isimlerini kontrol edip tekrar olmamasýný saglayan while dongusu ve hemen pesinden gelen if kosulu
			while(strcmp(hash_table[adr].texts[j], file_name) != 0 && j < hash_table[adr].text_count ) {
				j++;
			}
			//kelime bu dosyada ilk kez geciyorsa tabloya dosya ismi eklenir
			if(j >= hash_table[adr].text_count) {
				hash_table[adr].text_count++; //text sayisini arttir
				strcpy(hash_table[adr].texts[(hash_table[adr].text_count - 1)], file_name); //text ismini structa kopyala
			}
			
			return -1; //fonksiyondan cikilir
		}
		//whiledan bos adrese gelindigi icin cikildiysa
		else {
			//yeni bir node tanimlanir ve veriler node kopyalanir
			struct node newnode;
			strcpy(newnode.word, word);
			strcpy(newnode.texts[0], file_name);
			newnode.text_count = 1;
		
			hash_table[adr] = newnode; //olusturulan node tabloda ilgili adrese atilir
			*total_word_count = *total_word_count + 1; //tablodaki kelime sayisi guncellenir
			hash_table[0].loadFactor =(float) *total_word_count / (float) M; //load factor hesaplanir
			printf("%s kelimesi hash tablosuna eklendi\n",hash_table[adr].word); //bilgilendirme printleri atilir
			printf("Load Factor : %.3f\n",hash_table[0].loadFactor);
			//printf("count : %d\n",*total_word_count);
			
			return 1; //fonksiyondan cikilir
		}
	}
}



//dosyadan veriyi satir satir okuyan fonksiyon
int readInputFile(char *file_name, int *total_word_count, struct node *hash_table) {
	FILE *inputFile; //input file 'ý açmaya yarayan file pointer
	char ch; //dosyadan karakterler okunup bu degiskene aktarilir(satir sayisini hesaplamak icin)
	char *buffer = (char*) calloc(BUFFER_SIZE,sizeof(char)); //dosyadan alinan satirin tutuldugu buffer
	char *word  = (char*) calloc (WORD_SIZE,sizeof(char));//satirdan parcalanip alinan kelime
	char *org_word = (char*) calloc (WORD_SIZE,sizeof(char));//orjinal kelimeyi tutar
	int line_count = 0; //dosyadaki satir sayisini tutan degisken
	int i, j; //dongu degiskeni
	int isExist; //kelime hash table'da var mi yok mu donus degerini tutan degisken
	//int wordCount = total_word_count;
	
	if((inputFile = fopen(file_name,"r")) == NULL) {
		printf("Dosya okunmak icin acilamadi!\n");
		return 1;
	}
	else {
		//dosyadaki satir sayisini hesaplayan do-while bloðu
		do
        {
        	ch = fgetc(inputFile); //karakter oku
        	//new line ise line_count'u 1 arttýr
       	 	if (ch == '\n') {
       	 		line_count++;
			} 
        } while (ch != EOF); //dosya sonuna gelene kadar
        rewind(inputFile); //dosyada basa don
        line_count++; //line count son haline guncellenir
        
        //dosyadan veriler satir satir okunur ve isleme alinir
        for(i = 0; i < line_count; i++) {
        	fgets(buffer,BUFFER_SIZE * sizeof(char),inputFile); //satýrý dosyadan buffer'a al
        	buffer[strlen(buffer)] = '\0'; //bufferin sinirini belirle
        	printf("---------------------------------------------------\n");
        	printf("Okunan satir : %s\n\n",buffer);
        	system("PAUSE");
        	printf("\n");
        	word = strtok(buffer, AYRAC); //satirdaki ilk kelime alinir
        	word[strlen(word)] = '\0'; //kelime sinirini belirle
        	strcpy(org_word,word); //alinan orjinal kelime org_word'de saklanir
        	//case insensitive durum saglanmasi icin kelimedeki butun harfler kucuk harfe cevrilir
        	for(j = 0; j <strlen(word); j++) {
        		word[j] = tolower(word[j]);
			}
//			printf("word : %s\n",word);
//        	system("PAUSE");
			isExist = insertTable(word,file_name,total_word_count,hash_table); //kelime hash tablosuna eklenmek uzere insertTable fonksiyonu cagirilir
			//donus degeri kontrol edilir
			if(isExist == -1) {
				printf("%s kelimesi zaten tabloda mevcut!\n\n",org_word); //kelime hash tablosunda mevcutsa blgilendirme printi atilir
			}
//			else if(isExist == 1) {
//				//printf("%s kelimesi eklendi!\n\n",org_word);
//			}
        	
        	word = strtok(NULL, AYRAC); //satirdaki sonraki kelime alinir
   			//satirdaki diger kelimeler parcalara ayrilarak ayristirilir
 			while( word != NULL ) {
 				//satir sonuna gelindiyse new line karakter maskelenir
				if(word[strlen(word)-1] == '\n') {
					word[strlen(word)-1] = '\0';
				}
      			strcpy(org_word,word); //alinan orjinal kelime org_word'de saklanir
        		//case insensitive durum saglanmasi icin kelimedeki butun harfler kucuk harfe cevrilir
        		for(j = 0; j <strlen(word); j++) {
        			word[j] = tolower(word[j]);
				}
				word[strlen(word)] = '\0';	//kelime siniri belirlenir
//				printf("word : %s, size %d\n",word,strlen(word));
//        		system("PAUSE");
				isExist = insertTable(word,file_name,total_word_count,hash_table); //kelime hash tablosuna eklenmek uzere insertTable fonksiyonu cagirilir
				if(isExist == -1) {
					printf("%s kelimesi zaten tabloda mevcut!\n\n",org_word); //kelime hash tablosunda mevcutsa blgilendirme printi atilir
				}
//				else if(isExist == 1) {
//					//printf("%s kelimesi eklendi!\n\n",org_word);
//				}
      			word = strtok(NULL, AYRAC); //satirdaki diger kelimeler alinir
   			}
        	
		}
        fclose(inputFile); //dosya kapatilir
	}
	//free islemleri
	free(buffer);
	free(word);
	free(org_word);
	return 0; //fonksiyondan cikilir
}

//icine verilen kelimeyi hash tablosunda arayan fonksiyon
void searchHash (struct node *hash_table) {
	char *word = (char*) calloc(WORD_SIZE, sizeof(char)); //kullanicadan alinan ve tabloada aranacak olan kelime
	int cont = 1; //yeni arama olup olmayacagini kontrol eden degisken
	int i = 0; //kelimenin tablodaki adresi double probinge gore hesaplanirken adim sayisini tutan degisken
	long long key; //kelimenin horner methoduna gore key degeri
	int adr; //kelimenin tablodaki adresi
	int j;	//dongu degiskeni
	while(cont) {
		printf("\nAramak istediginiz kelimeyi giriniz : \n");
		scanf("%s",word);	//aranmak istenen kelime kullanicidan alinir
		word[strlen(word)] = '\0';	//kelime siniri belirlenir
		//kelime case insensitive'lik saglanmasi icin kucuk harflere cevirilir
		for(j = 0; j <strlen(word); j++) {
        	word[j] = tolower(word[j]);
		}
//		printf("kontrol!!! kelime : %s\n",word);
//		system("pause");
		printf("\n");
		key = (long long) getHorner(word);	//horner sayisi alinir
		
		//kelimenin tablodaki adresi double probing yontemiyle hesaplanir
		int h1_key = key % M;
		int h2_key = 1 + (key % MM);
		adr = (h1_key + (i * h2_key)) % M;
		i++;  //adim sayisi guncellenir
		//kelime tabloda mevcut degilse ilgili gozde bulunan struct'ýn text_count degeri 0 demektir.
		if(hash_table[adr].text_count == 0) {
			printf("Kelime tabloda mevcut degil!\nArama islemi %d adimda tamamlanmistir.\n\nYeni arama yapmak icin 1'e, cikmak icin 0'a basiniz.\n",i);
			scanf("%d",&cont);
		}
		//kelimenin tabloda mevcut olma ihtimalinde
		else {
			//kelime ile karsilasana kadar veya bos goz gorene kadar donen while
			while(hash_table[adr].text_count != 0 && strcmp(hash_table[adr].word, word) != 0) {
				adr = (h1_key + (i * h2_key)) % M;	//adres degeri her adýmda guncellenir
				i++;	//adim sayisi guncellenir
			}
			//kelime ile karsilasildigi  icin while'dan cikilmissa kelime tabloda mevcuttur
			if(strcmp(hash_table[adr].word, word) == 0) {
				printf("Kelime tabloda mevcut!\nKelime toplamda %d adet dosyada gecmistir.\nKelimenin gectigi dosyalar : \n",hash_table[adr].text_count);
				//kelimenin gectigi dosyalari ekrana yazdiran for
				for(j = 0; j < hash_table[adr].text_count; j++) {
					printf("%d. dosyanin ismi : %s\n",(j+1),hash_table[adr].texts[j]);
				}
				printf("Arama islemi %d adimda tamamlanmistir.\n",i);
				printf("\nYeni arama yapmak icin 1'e, cikmak icin 0'a basiniz.\n");
				scanf("%d",&cont);
			}
			//bos goze gelindigi icin whiledan cikilmissa kelime tabloda mevcut degildir
			else {
				printf("Kelime tabloda mevcut degil!\nArama islemi %d adimda tamamlanmistir.\nYeni arama yapmak icin 1'e, cikmak icin 0'a basiniz.\n",i);
				scanf("%d",&cont);	//yeni arama yapilacak mi bilgisi kullanicidan alinir
			}
			
		}
		//yeni aramalar icin word'u temizleyen for dongusu
		for(j = 0; j < WORD_SIZE; j++) {
			word[j] = '\0';
		}
		i = 0; //yeni aramada adim sayisi 0'dan baslatilir
	}
	free(word);	 //free islemi
}


int main() {
	
	FILE *table_file;	//hashtable'ýn okunacaðý file icin file pointer
	char *file_name = (char*) calloc(TEXT_NAME_SIZE,sizeof(char));	//input dosyasinin adi
	struct node hashTable[M]; //hash table
	int total_word_count = 0;	//tablodaki kelime sayisini tutan degisken
	int control;	//secim kontrolu - dosyadan kelime okuma veya tablodan kelime arama
	int kontrol;	//input file'in okunup okunamadiðinin kontrolunu tutan degisken
	//int i;
	
//	//tablo 0'dan olusturuldugunda ilklendirme foru
//	for(i = 0; i < M; i++) {
//		hashTable[i].text_count = 0;
//		hashTable[i].loadFactor = 0;
//	}
	
	//hash tablosunun oldugu dosya acilir
    if ((table_file = fopen(HASH_TXT, "rb")) == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    //hash tablosu dosyadan okunur
    else {
    	fread(hashTable, sizeof(struct node) * M, 1, table_file);	//tabloyu oku
        fclose(table_file);	//dosyayi kapat
	}
	total_word_count =  hashTable[0].loadFactor *(float) M;
	printf("Hash Table basariyla dosyadan okundu!\n\nTablo boyutu : %d\nTablonun doluluk orani (load factor) : %.3f\n\n",M,hashTable[0].loadFactor);
	
	//kullanicidan arama veya ekleme islemi icin istegi alinir
	printf("Hash Tablosunda arama yapmak icin 1'e, tabloya kelime eklemek icin 2'ye basiniz.\n");
	scanf("%d",&control);
	
	if(control == 1) {
		searchHash(hashTable);	//arama fonksiyonu cagirlir
		
	}
	else if(control == 2) {
		printf("Input dosyasinin adini .txt uzantili olacak sekilde giriniz : ");
		scanf("%s",file_name);
		printf("\n");
		kontrol = readInputFile(file_name,&total_word_count,hashTable);	//okuma fonksiyonu cagirilir
		if(kontrol == 1) {
			printf("Input dosyasi okunamadi!\n");
			return 1; //dosya okunamazsa mainden cikilir
		}
//		printf("\n\n\nMAINDEYIZ -- total word: %d,load factor : %f\n\n\n",total_word_count,hashTable[0].loadFactor);
//		system("pause");		
	}
	

	//hash tablosunun oldugu dosya acilir
    if ( (table_file = fopen(HASH_TXT, "wb")) == NULL ) {
        printf("Error opening file\n");
        return 1;   
    }
    //hash tablosu dosyaya yazilir
	else {
		    fwrite(hashTable, sizeof(struct node) * M, 1, table_file); //tablo dosyaya yazilir
    		fclose(table_file);	//dosya kapatilir
    		printf("\nTablo Dosyaya Basari ile Yazildi!\nTablonun doluluk orani (load factor) : %.3f\n",hashTable[0].loadFactor);
	}
	

	free(file_name); //free islemi
	return 0;	//end of main
}





