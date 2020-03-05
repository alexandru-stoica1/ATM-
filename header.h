//Stoica ALexandru-Patru  315_CB

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct history
{
	char info_history[800];
	struct history *next_history;
}THistory,*TListaHistory,**AListaHistory;

typedef struct LSC
{
	unsigned long long card_number,balance;
	int cvv,nr_greseli;
	char exp_date[6],status[9];
	char pin[5],pin_init[5];
	struct LSC *next_card;
	struct history *history;
}TCard, *TListaCard, **AListaCard;

typedef struct LC
{
	TListaCard card;
	struct LC *next_LSC;
}TCelula,*TLista,**ALista;

int sumacifre(unsigned long long x);
TListaCard AlocCelulaCard(unsigned long long card_number,char pin[5],
	char exp_date[6],int cvv,char status[9],int balance,
	int nr_greseli,TListaHistory history);

TListaHistory AlocHistory(char a[800]);
int InserareHistoryLaInc(TListaHistory *L,char a[800]);
int InserareCardLaInc(TListaCard *L,unsigned long long card_number,
	char pin[5],char exp_date[6], int cvv,char status[9],int balance,
	int nr_greseli,TListaHistory history);

TLista Aloca();
int InserareInc(TLista *L);
int Eliminahistory(TListaHistory *L, char x[800]);
int Elimina(TListaCard *L, unsigned long long x);
int check_pin(char pin[10],char m[10]);
int find(TListaCard L,unsigned long long card_number);
void Distrugere_History(AListaHistory aL);
void Distrugere_LSC(AListaCard aL);
void Distrugere_LC(ALista aL);
int add(TLista L,int poz,unsigned long long card_number,char pin[9],
	char exp_date[6],int cvv,char status[9],int balance,FILE *g,
	int nr_greseli,TListaHistory history);

void show(FILE *g,TLista L,unsigned long long card_number,int nr_max_carduri);
void showall(FILE *g,TLista L,int nr_max_carduri,int i,int max);


