// cudzi-hash.c -- Diana Lazárová, 22.10.2019 16:24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prevzata implementacia ineho hashovania patri do tohto suboru.
// Dolezite: uvedte zdroj odkial ste to prevzali -- musi existovat v case konzultacii!
typedef struct uzol
{
	int kluc; //integer
	//char *kluc; //string
} UZOL;

typedef struct hashtabulka
{
	int velkost, vsetky_uzly;
	UZOL **uzly;
} HASH_TABLE;

HASH_TABLE* hash_tabulka;

//void vlozDoTabulky(char *kluc); //string
void vlozDoTabulky(int kluc); //integer

// int hash(char *kluc) // string
// {
//     int i, len = strlen(kluc), vysledok = 0;
//     for(i = 0; i < len; i++)
//     {
//         vysledok = 29 * vysledok + kluc[i];
//     }
//     return vysledok;
// }

int hash(int kluc) // integer
{

	int vysledok = 0;
	vysledok = 29 * kluc;
	return vysledok;
}

// unsigned int hashKompresia(char *kluc) // string
// {
//     return ((unsigned int)hash(kluc)) % hash_tabulka->velkost;
// }

unsigned int hashKompresia(int kluc) // integer
{
	return ((unsigned int)hash(kluc)) % hash_tabulka->velkost;
}

void init(int velkost)
{
	int i;
	hash_tabulka = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
	hash_tabulka->velkost = velkost;
	hash_tabulka->vsetky_uzly = 0;
	hash_tabulka->uzly = (UZOL**)malloc(velkost * sizeof(UZOL*));
	for (i = 0; i < velkost; i++)
	{
		hash_tabulka->uzly[i] = NULL;
	}
}

void zmenVelkost(int nova_velkost)
{
	int i, aktualna_velkost = hash_tabulka->velkost;
	UZOL *aktualny_uzol;
	UZOL **aktualne_uzly = hash_tabulka->uzly;

	hash_tabulka->velkost = nova_velkost;
	hash_tabulka->vsetky_uzly = 0;

	hash_tabulka->uzly = (UZOL**)malloc(nova_velkost * sizeof(UZOL*));
	for (i = 0; i < nova_velkost; i++)
	{
		hash_tabulka->uzly[i] = NULL;
	}

	for (i = 0; i < aktualna_velkost; i++)
	{
		aktualny_uzol = aktualne_uzly[i];
		if (aktualny_uzol != NULL)
		{
			vlozDoTabulky(aktualny_uzol->kluc);
		}
	}
}

//void vlozDoTabulky(char* kluc) // string
void vlozDoTabulky(int kluc) // integer
{
	int dalsi_index, iteracia;
	// pridat resize, ak sa to uz moc plni
	if (hash_tabulka->velkost * 0.5 <= hash_tabulka->vsetky_uzly)
	{
		printf("resizing....\n");
		zmenVelkost(hash_tabulka->velkost * 2);
	}

	int index = hashKompresia(kluc);

	UZOL* novy_uzol = (UZOL*)malloc(sizeof(UZOL));
	// novy_uzol->kluc = (char*)malloc((strlen(kluc) + 1) * sizeof(char)); // string
	// strcpy(novy_uzol->kluc, kluc); // string
	novy_uzol->kluc = kluc; // integer

	if (hash_tabulka->uzly[index] == NULL)
	{
		hash_tabulka->uzly[index] = novy_uzol;
	}
	else // kolizia
	{
		dalsi_index = index;
		iteracia = 0;
		while (1)
		{
			if (hash_tabulka->uzly[dalsi_index] == NULL)
			{
				hash_tabulka->uzly[dalsi_index] = novy_uzol;
				break;
			}
			if (dalsi_index == index && iteracia != 0)
			{
				printf("tabulka je plna, neda sa pridat prvok, zlyhal aj resize\n");
				break;
			}
			dalsi_index++; iteracia++;
			if (dalsi_index >= hash_tabulka->velkost)
			{
				dalsi_index = 0;
			}
		}
	}
	hash_tabulka->vsetky_uzly++;
}

//char hladajVTabulke(char* kluc) // string
char hladajVTabulke(int kluc) // integer
{
	int index = hashKompresia(kluc);
	printf("index kluca %d je %d\n", kluc, index);
	int dalsi_index = index;
	int iteracia = 0;
	while (1)
	{
		if (hash_tabulka->uzly[dalsi_index] == NULL)
		{
			printf("Prvok nie je v tabulke\n");
			break;
		}
		//if (strcmp(kluc, hash_tabulka->uzly[dalsi_index]->kluc) == 0) // string
		if (kluc == hash_tabulka->uzly[dalsi_index]->kluc) // integer
			return 1;

		if ((iteracia == hash_tabulka->velkost) || hash_tabulka->uzly[dalsi_index] == NULL)
		{
			printf("Prvok nie je v tabulke\n");
			break;
		}
		dalsi_index++; iteracia++;
		if (dalsi_index >= hash_tabulka->velkost)
		{
			dalsi_index = 0;
		}
	}

	return 0;
}

void vypisTabulku()
{
	printf("hash tabulka ma velkost: %d, pocet vsetkych: %d\n", hash_tabulka->velkost, hash_tabulka->vsetky_uzly);
	int i;
	UZOL *aktualny;
	for (i = 0; i < hash_tabulka->velkost; i++)
	{
		printf("[%d] ", i);
		if (hash_tabulka->uzly[i] == NULL)
		{
			printf("-");
		}
		else
		{
			aktualny = hash_tabulka->uzly[i];
			//printf("->%s", aktualny->kluc); // string
			printf("->%d", aktualny->kluc); // integer
		}
		printf("\n");
	}
}

void main()
{
	init(10);
	vlozDoTabulky(10);
	vlozDoTabulky(20);
	vlozDoTabulky(30);
	vypisTabulku();
}