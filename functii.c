//Stoica ALexandru-Patru  315_CB

#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int sumacifre(unsigned long long x)
{	
	unsigned long long n=x;
	int s=0;
	
	while(n)
	{
		s=s+n%10;
		n/=10;
	}
	
	return s;
}

TListaCard AlocCelulaCard(unsigned long long card_number,char pin[5],
	char exp_date[6],int cvv,char status[9],int balance,
	int nr_greseli,TListaHistory history)         
{ 
	TListaCard aux = (TListaCard)calloc(1,sizeof(TCard));  
	  
	if (aux)                         
	{ 
		aux->card_number = card_number;   
	  	strcpy(aux->pin, pin);
	  	strcpy(aux->pin_init, pin);
	    strcpy(aux->exp_date,exp_date);
	    aux->cvv = cvv;
	    strcpy(aux->status,status);
	    aux->nr_greseli=nr_greseli;
	    aux->balance=balance;
	    aux->next_card = NULL;
	    aux->history=history;
	}

  	return aux;           
}

TListaHistory AlocHistory(char a[800])
{
	TListaHistory aux=(TListaHistory)calloc(1,sizeof(THistory));

	if(aux)
	{
		aux->next_history=NULL;
		strcpy(aux->info_history,a);
	}

	return aux;
}

int InserareHistoryLaInc(TListaHistory *L,char a[800])
{
	TListaHistory aux;
	aux=AlocHistory(a);
	
	if(!aux)
		return 0;
	
	aux->next_history=*L;
	*L=aux;
	
	return 1;
}

int InserareCardLaInc(TListaCard *L,unsigned long long card_number,char pin[5],
	char exp_date[6], int cvv,char status[9],int balance,int nr_greseli,
	TListaHistory history)
{	

	TListaCard aux;
	
	aux=AlocCelulaCard(card_number,pin,exp_date,cvv,status,balance,
		nr_greseli,history);
	
	if(!aux)	
		return 0;
	
	aux->next_card=*L;
	*L=aux;

	return 1;
}
	
TLista Aloca()
{
	TLista aux=(TLista)calloc(1,sizeof(TCelula));
	
	if(aux)
	{	
		aux->card=NULL;
		aux->next_LSC=NULL;
	}

	return aux;
}


int InserareInc(TLista *L)
{
	TLista aux;
	aux=Aloca();

	if(!aux)
		return 0;

	aux->next_LSC=*L;
	*L=aux;

	return 1;
}

int Eliminahistory(TListaHistory *L, char x[800])
{
	TListaHistory ant,p;

	for(p=*L,ant=NULL;p!=NULL;ant=p,p=p->next_history)
		if(!strcmp(p->info_history,x))
			break;

	if(!p)
		return 0;
	
	if(!ant)
		*L=p->next_history;
	else 
		ant->next_history=p->next_history;

	free(p);

	return 1;
}
int Elimina(TListaCard *L, unsigned long long x)
{
	TListaCard ant,p;

	for( p = *L , ant = NULL; p != NULL; ant = p, p = p->next_card)
		if(p->card_number==x)
			break;

	if(!p)
		return 0;
	
	if(!ant)
		*L=p->next_card;
	else 
		ant->next_card=p->next_card;

	free(p);

	return 1;
}

int check_pin(char pin[10],char m[10])
{
	return strcmp(pin,m);
}

int find(TListaCard L,unsigned long long card_number)
{	
	int k=0;

	while(L)
	{
		if(L->card_number==card_number)
			k++;
		
		L=L->next_card;
	}
	if(k!=0)
		return 1;
	else 
		return 0;
}

void Distrugere_History(AListaHistory aL)
{
	TListaHistory aux;

	while(*aL)
	{
		aux=*aL;
		*aL=(*aL)->next_history;
		free(aux);
	}
}

void Distrugere_LSC(AListaCard aL)
{
	TListaCard aux;

	while(*aL)
	{
		aux=*aL;
		*aL=(*aL)->next_card;
		free(aux);
	}
}

void Distrugere_LC(ALista aL)
{
	TLista aux;

	while(*aL)
	{
		aux=*aL;
		*aL=(*aL)->next_LSC;
		free(aux);
	}
}

int add(TLista L,int poz,unsigned long long card_number,char pin[9],
	char exp_date[6],int cvv,char status[9],int balance,FILE *g,
	int nr_greseli,TListaHistory history)
{
	int k=0,p=0;
	TLista P=L;

	while(P)
	{
		if(k==poz)
			break;
			
		k++;
		P=P->next_LSC;
	}		

 	TListaCard A=P->card;
 			
 	while(A)
 	{
 		if(card_number==A->card_number)
 		{
 			p=1;
 			break;
 		}

 		A=A->next_card;
 	}	
 			
 	if(p==1)
 	{
 		fprintf(g,"The card already exists\n");
 		return 1;
 	}
 	else
 	{
		InserareCardLaInc(&(P->card),card_number,pin,exp_date,cvv,status,
			balance,nr_greseli,history);
		return 0;
	}
					
}

void show(FILE *g,TLista L,unsigned long long card_number,int nr_max_carduri)
{	
	TLista aux=L;
	int s=sumacifre(card_number);
	int k=0,m=1;
	
	while(aux&&m)
	{	
		if(s%nr_max_carduri==k)
			break;
		
		k++;
		m++;
		aux=aux->next_LSC;
	}
	
	TListaCard aux2=aux->card;
	
	while(aux2)
	{
		if(card_number==aux2->card_number)
			break;

		m++;
		aux2=aux2->next_card;
	}

	fprintf(g,"(card number: %016llu, ",aux2->card_number );
	fprintf(g,"PIN: %s, ",aux2->pin );
	fprintf(g,"expiry date: %s, ",aux2->exp_date );
	fprintf(g,"CVV: %03d, ",aux2->cvv);
	fprintf(g,"balance: %llu, ",aux2->balance );
	fprintf(g,"status: %s, ",aux2->status );
	
	TListaHistory aux1=aux2->history;
	
	if(!aux1)		
		fprintf(g,"history: [])");
	else
	{
		fprintf(g,"history: [");
		
		while(aux1)
		{	
			if(!aux1->next_history)
				fprintf(g,"(%s",aux1->info_history);
			else
				fprintf(g,"(%s), ",aux1->info_history);

			aux1=aux1->next_history;
		}
		
		fprintf(g,")])");
	}
	
	fprintf(g,"\n");
}

void showall(FILE *g,TLista L,int nr_max_carduri,int i,int max)
{	
	TLista aux=L;
	int k=0;
	
	max=max+1;
	
	if(max==0)
		return;

	while(aux&&max)
	{	
		if(!aux)
			break;
		
		TListaCard aux1=aux->card;
		
		if(!aux1)
			fprintf(g,"pos%d: []\n",i );
		else
		{
			fprintf(g,"pos%d: [\n",i );

			while(aux1)
			{
				fprintf(g,"(card number: %016llu, ",aux1->card_number );
				fprintf(g,"PIN: %s, ",aux1->pin );
				fprintf(g,"expiry date: %s, ",aux1->exp_date );
				fprintf(g,"CVV: %03d, ",aux1->cvv);
				fprintf(g,"balance: %llu, ", aux1->balance);
				fprintf(g,"status: %s, ",aux1->status );
			
				TListaHistory aux5=aux1->history;
			
				if(!aux5)
				{
					fprintf(g,"history: [])");
				}
				else
				{
				fprintf(g,"history: [");
				
				while(aux5)
				{	
					if(!aux5->next_history)
						fprintf(g,"(%s",aux5->info_history);
					else
						fprintf(g,"(%s), ",aux5->info_history);

					aux5=aux5->next_history;
				}
				
				fprintf(g,")])");
				}
				
				fprintf(g,"\n");
				
				if(!aux1->next_card)
					break;
				
				aux1=aux1->next_card;
			}

		fprintf(g, "]");
		fprintf(g,"\n");

		}
		if(!aux->next_LSC)
			break;

		aux=aux->next_LSC;
		i++;
		max--;
	}
}
	