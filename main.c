//Stoica ALexandru-Patru  315_CB

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

int main()
{	
	char str[15]=" ",exp_date[6]=" ",pin[9]=" ",a[100]=" ",status[9]=" ",
		new_pin[5]=" ",numar[14]=" ";
	unsigned long long card_number=0,card_number_source=0,
		card_number_dest=0,v[199],balance=0,sum=0;
	int cvv=0,nr_max_carduri=0,poz=0,x=0,q=0,max=0,ok=0,q1=0,
		new_card_nr=0,s=0,m=0,i=0;

	FILE *f=fopen("input.in","r");
	fscanf(f,"%d",&nr_max_carduri);
	TLista L=NULL,T=NULL;

	strcpy(status,"NEW");

	for(i=0;i<nr_max_carduri;i++)
		InserareInc(&L);
	
	FILE *g=fopen("output.out","w");

	while(fscanf(f,"%s",str)!=EOF)
	{			
		if(!strcmp(str,"add_card"))
		{				
			T=NULL;
			TLista aux4=L;
			int cn=0,k=0,f1=0,yes=0;
				
			fscanf(f,"%llu",&card_number);
			fscanf(f,"%s",pin);
			fscanf(f,"%s",exp_date);
			fscanf(f,"%d",&cvv);
			poz=sumacifre(card_number)%nr_max_carduri;
			ok=1,s=0;

			while(aux4)
			{			
				if(poz==s)
				{
					f1=find(aux4->card,card_number);
					break;
				}
				
				s++;
				aux4=aux4->next_LSC;
			}
 
			if(f1==0)
				new_card_nr++;

			printf("%d %d\n",new_card_nr,nr_max_carduri );


			if((new_card_nr>nr_max_carduri)&&(f1==0))
			{
				nr_max_carduri=nr_max_carduri*2;
				
				for(i=0;i<nr_max_carduri/2;i++)
					InserareInc(&L);

				for(i=0;i<nr_max_carduri;i++)
					InserareInc(&T);

				while(cn<nr_max_carduri)
				{	
					cn++;
					TListaCard A1=L->card;
				
					while(A1)
					{
						poz=sumacifre(A1->card_number)%nr_max_carduri;
						add(T,poz,A1->card_number,A1->pin,A1->exp_date,A1->cvv,A1->status,A1->balance,g,A1->nr_greseli,A1->history);
						
						if(poz>max)
							max=poz;
						
						A1=A1->next_card;
					}
					
					L=L->next_LSC; 
				}

				L=T;
			}
		
			poz=sumacifre(card_number)%nr_max_carduri;

			add(L,poz,card_number,pin,exp_date,cvv,status,0,g,0,NULL);

			if(poz>max)
				max=poz;

		} 

		if(!strcmp(str,"delete_card"))
		{
			fscanf(f,"%llu",&card_number);	
			new_card_nr--;

			TLista P=L;
			int k=0;
			poz=sumacifre(card_number)%nr_max_carduri;

			while(P)
			{
				if(k==poz)
					break;
				
				k++;
				P=P->next_LSC;
			}		

			Elimina(&(P->card),card_number);
			q=0;
		}
	
		if(!strcmp(str,"reverse_transaction"))
		{
			fscanf(f,"%llu",&card_number_source);
			fscanf(f,"%llu",&card_number_dest);
			fscanf(f,"%llu",&sum);
			TLista P=L,A=L;
			int k=0,poz1,hs;
			
			poz=sumacifre(card_number_source)%nr_max_carduri;
			poz1=sumacifre(card_number_dest)%nr_max_carduri;

			while(P)
			{
				if(k==poz)
					break;
				
				k++;
				P=P->next_LSC;
			}		
			
			TListaCard aux=P->card;
			
			while(aux)
			{
				if(aux->card_number==card_number_source)
					break;
			
				aux=aux->next_card;
			}

			k=0;
			
			while(A)
			{
				if(k==poz1)
					break;
				
				k++;
				A=A->next_LSC;
			}
		
			TListaCard aux1=A->card;
			
			while(aux1)
			{
				if(aux1->card_number==card_number_dest)
					break;
				
				aux1=aux1->next_card;
			}
			
			char bum[100],buf[100],buf1[100],x[100]=" ";
			sprintf(buf,"%016llu",card_number_source);
			sprintf(buf1,"%016llu",card_number_dest);

			sprintf(bum,"%llu",sum);

			if(sum%10)
			{
				fprintf(g, "The transferred amount must be multiple of 10\n");
				
				strcpy(x,"FAIL, reverse_transaction ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,buf1);
				strcat(x," ");
				strcat(x,bum);
				
				InserareHistoryLaInc(&(aux->history),x);
				continue;
			}	

			if(sum<=aux1->balance)
			{		
				strcpy(x,"SUCCESS, reverse_transaction ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,buf1);
				strcat(x," ");
				strcat(x,bum);
					
				char bbb[90];
				
				strcpy(bbb,"SUCCESS, transfer_funds ");
				strcat(bbb,buf);
				strcat(bbb," ");
				strcat(bbb,buf1);
				strcat(bbb," ");
				strcat(bbb,bum);

				Eliminahistory(&(aux1->history),bbb);

				InserareHistoryLaInc(&(aux->history),x);
				
				aux->balance=aux->balance+sum;
				aux1->balance=aux1->balance-sum;
				continue;
			}
		
			if(sum>aux1->balance)
			{	
				fprintf(g, "The transaction cannot be reversed\n");
				
				strcpy(x,"FAIL, reverse_transaction ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,buf1);
				strcat(x," ");
				strcat(x,bum);
				continue;
			}	
		}
		
		if(!strcmp(str,"unblock_card"))
		{
			fscanf(f,"%llu",&card_number);
			TLista P=L;
			int k=0;
			
			poz=sumacifre(card_number)%nr_max_carduri;

			while(P)
			{
				if(k==poz)
					break;
				
				k++;
				P=P->next_LSC;
			}		
			
			TListaCard aux=P->card;
			
			while(aux)
			{
				if(aux->card_number==card_number)
					break;
				
				aux=aux->next_card;
			}

			aux->nr_greseli=0;

			strcpy(aux->status,"ACTIVE");
		}

		if(!strcmp(str,"show"))
		{	
			if(ok==1)
			{
				int m=0;
				TLista A=L;
				
				if(fscanf(f,"%llu",&card_number)==1)
					show(g,L,card_number,nr_max_carduri);
				
				else
					showall(g,L,nr_max_carduri,m,max);
			}
		}

		if(!strcmp(str,"insert_card"))
		{
			fscanf(f,"%llu",&card_number);
			fscanf(f,"%s",pin);
			
			TLista P=L;
			int k=0;

			poz=sumacifre(card_number)%nr_max_carduri;

			while(P)
			{
				if(k==poz)
					break;
				
				k++;
				P=P->next_LSC;
			}		
			
			TListaCard aux=P->card;
			
			while(aux)
			{
				if(aux->card_number==card_number)
					break;
				
				aux=aux->next_card;
			}
			
			char x[800]=" ",s[100],m[10],buf[18]=" ";
			sprintf(buf,"%016llu",card_number);

			if(!strcmp(aux->status, "LOCKED"))
			{
				fprintf(g,"The card is blocked. Please contact the administrator.\n");
				
				strcpy(x,"FAIL, insert_card ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,pin);

				InserareHistoryLaInc((&(aux->history)),x);
					continue;
			}

				if(check_pin(pin,aux->pin))
				{
					fprintf(g, "Invalid PIN\n");
					aux->nr_greseli++;
					
					if(aux->nr_greseli==3)
					{	
							fprintf(g,"The card is blocked. Please contact the administrator.\n");
							strcpy(aux->status,"LOCKED");
					}
					
					strcpy(x,"FAIL, insert_card ");
					strcat(x,buf);
					strcat(x," ");
					strcat(x,pin);

					InserareHistoryLaInc((&(aux->history)),x);
				}
				
				else
				{
					if(!strcmp(aux->pin_init,aux->pin))
						if(!strcmp(aux->status,"NEW"))						
							fprintf(g, "You must change your PIN.\n");
						
					strcpy(x,"SUCCESS, insert_card ");
					strcat(x,buf);
					strcat(x," ");
					strcat(x,pin);
						
					aux->nr_greseli=0;

					InserareHistoryLaInc((&(aux->history)),x);
				}
		}

		if(!strcmp(str,"recharge"))
		{
			fscanf(f,"%llu",&card_number);
			fscanf(f,"%llu",&sum);TLista P=L;
			
			int k=0;
			char bum[20];
			
			sprintf(bum,"%llu",sum);
			poz=sumacifre(card_number)%nr_max_carduri;
			
			while(P)
			{
				if(k==poz)
					break;
				
				k++;
				P=P->next_LSC;
			}		
			
			TListaCard aux=P->card;
			
			while(aux)
			{
				if(aux->card_number==card_number)
					break;
			
				aux=aux->next_card;
			}

			if(sum%10)
			{
				fprintf(g, "The added amount must be multiple of 10\n");
				
				char buf[100],x[100]=" ";
				
				sprintf(buf,"%016llu",card_number);
				strcpy(x,"FAIL, recharge ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,bum);
				
				InserareHistoryLaInc(&(aux->history),x);
			}
			
			else
			{
				aux->balance=aux->balance+sum;
				fprintf(g, "%llu\n",aux->balance );
	
				char buf[100],x[100]=" ";
	
				sprintf(buf,"%016llu",card_number);
				strcpy(x,"SUCCESS, recharge ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,bum);
				
				InserareHistoryLaInc(&(aux->history),x);
			}
		}

		if(!strcmp(str,"cash_withdrawal"))
		{
			fscanf(f,"%llu",&card_number);
			fscanf(f,"%llu",&sum);	
	
			TLista P=L;
			int k=0;
	
			poz=sumacifre(card_number)%nr_max_carduri;

			while(P)
			{
				if(k==poz)
					break;
	
				k++;
				P=P->next_LSC;
			}		
	
			TListaCard aux=P->card;
	
			while(aux)
			{
				if(aux->card_number==card_number)
					break;
	
				aux=aux->next_card;
			}

			char bum[100],buf[100],x[100]=" ";
	
			sprintf(buf,"%016llu",card_number);
			sprintf(bum,"%llu",sum);

			if(sum%10)
			{
				fprintf(g, "The requested amount must be multiple of 10\n");
	
				sprintf(buf,"%016llu",card_number);
				strcpy(x,"FAIL, cash_withdrawal ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,bum);
	
				InserareHistoryLaInc(&(aux->history),x);
			
				continue;
			}	
			
			if(sum>aux->balance)
			{
				fprintf(g, "Insufficient funds\n");
			
				sprintf(buf,"%016llu",card_number);
				strcpy(x,"FAIL, cash_withdrawal ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,bum);
			
				InserareHistoryLaInc(&(aux->history),x);
				
				continue;
			}	
			
			if(sum<=aux->balance)
			{
			
				sprintf(buf,"%016llu",card_number);
				strcpy(x,"SUCCESS, cash_withdrawal ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,bum);
			
				InserareHistoryLaInc(&(aux->history),x);
			
				aux->balance=aux->balance-sum;

				fprintf(g, "%llu\n",aux->balance );
				continue;
			}	
		}
		
		if(!strcmp(str,"balance_inquiry"))
		{
			fscanf(f,"%llu",&card_number);
		
			TLista P=L;
			int k=0;
		
			poz=sumacifre(card_number)%nr_max_carduri;

			while(P)
			{
				if(k==poz)
					break;
		
				k++;
				P=P->next_LSC;
			}		
		
			TListaCard aux=P->card;
		
			while(aux)
			{
				if(aux->card_number==card_number)
					break;
		
				aux=aux->next_card;
			}
		
			fprintf(g, "%llu\n",aux->balance );
			aux->balance=balance+aux->balance;
		
			char buf[100],x[100]=" ";
		
			sprintf(buf,"%016llu",card_number);
			strcpy(x,"SUCCESS, balance_inquiry ");
			strcat(x,buf);

			InserareHistoryLaInc(&(aux->history),x);
		}

		if(!strcmp(str,"transfer_funds"))
		{
			fscanf(f,"%llu",&card_number_source);
			fscanf(f,"%llu",&card_number_dest);
			fscanf(f,"%llu",&sum);
		
			TLista P=L,A=L;
			int k=0,poz1;
		
			poz=sumacifre(card_number_source)%nr_max_carduri;
			poz1=sumacifre(card_number_dest)%nr_max_carduri;
		
			while(P)
			{
				if(k==poz)
					break;
		
				k++;
				P=P->next_LSC;
			}		
		
			TListaCard aux=P->card;
		
			while(aux)
			{
				if(aux->card_number==card_number_source)
					break;
		
				aux=aux->next_card;
			}

			k=0;
			
			while(A)
			{
				if(k==poz1)
					break;
			
				k++;
				A=A->next_LSC;
			}
			
			TListaCard aux1=A->card;
			
			while(aux1)
			{
				if(aux1->card_number==card_number_dest)
					break;
			
				aux1=aux1->next_card;
			}
			
			char bum[100],buf[100],buf1[100];
			
			sprintf(buf,"%016llu",card_number_source);
			sprintf(buf1,"%016llu",card_number_dest);
			sprintf(bum,"%llu",sum);

			if(sum%10)
			{
				fprintf(g, "The transferred amount must be multiple of 10\n");
			
				char buf[100],x[100]=" ";
			
				strcpy(x,"FAIL, transfer_funds ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,buf1);
				strcat(x," ");
				strcat(x,bum);
			
				InserareHistoryLaInc(&(aux->history),x);
			}	
		
			else if(sum>aux->balance)
			{
				fprintf(g, "Insufficient funds\n");
		
				char buf[100],x[100]=" ";
		
				strcpy(x,"FAIL, transfer_funds ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,buf1);
				strcat(x," ");
				strcat(x,bum);
		
				InserareHistoryLaInc(&(aux->history),x);
			}	
			
			else
			{
				char buf[100],x[100]=" ";
			
				strcpy(x,"SUCCESS, transfer_funds ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,buf1);
				strcat(x," ");
				strcat(x,bum);
			
				InserareHistoryLaInc(&(aux->history),x);
			
				strcpy(x,"SUCCESS, transfer_funds ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,buf1);
				strcat(x," ");
				strcat(x,bum);
			
				InserareHistoryLaInc(&(aux1->history),x);
			
				aux->balance=aux->balance-sum;
				aux1->balance=aux1->balance+sum;

				fprintf(g, "%llu\n",aux->balance );
			}	
		}
		

		if(!strcmp(str,"pin_change"))
		{
			fscanf(f,"%llu",&card_number);
			fscanf(f,"%s",new_pin);					
		
			TLista P=L;
			int k=0;
		
			poz=sumacifre(card_number)%nr_max_carduri;

			while(P)
			{
				if(k==poz)
					break;
		
				k++;
				P=P->next_LSC;
			}		
		
			TListaCard aux=P->card;
		
			while(aux)
			{
				if(aux->card_number==card_number)
					break;
		
				aux=aux->next_card;
			}

			char buf[20],x[100]=" ";
			int ok=0;

		
			sprintf(buf,"%016llu",card_number);

			for(i=0;i<5;i++)
				if((new_pin[i]>='0'&&new_pin[i]<='9'))
					ok++;

			if(strlen(new_pin)!=4)
			{	
				fprintf(g, "Invalid PIN\n");

				strcpy(x,"FAIL, pin_change ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,new_pin);

				InserareHistoryLaInc(&(aux->history),x);
			}
			
			else if(ok==4)
			{ 
				strcpy(aux->pin,new_pin);
				strcpy(x,"SUCCESS, pin_change ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,new_pin);
			
				InserareHistoryLaInc(&(aux->history),x);

				strcpy(aux->status,"ACTIVE");
			
				q1=1;
			}
				
			else
			{	
				fprintf(g, "Invalid PIN\n");
		
				strcpy(x,"FAIL, pin_change ");
				strcat(x,buf);
				strcat(x," ");
				strcat(x,new_pin);
	
				InserareHistoryLaInc(&(aux->history),x);
			}
		}

		if(!strcmp(str,"cancel"))
		{
			fscanf(f,"%llu",&card_number);
		
			TLista P=L;
			int k=0;
		
			poz=sumacifre(card_number)%nr_max_carduri;

			while(P)
			{
				if(k==poz)
					break;
		
				k++;
				P=P->next_LSC;
			}		
		
			TListaCard aux=P->card;
		
			while(aux)
			{
				if(aux->card_number==card_number)
					break;
		
				aux=aux->next_card;
			}
			TListaHistory aux3;
			char buf[100]=" ",x[100]=" ";
		
			AListaHistory aux12=&(aux->history);

			sprintf(buf,"%016llu",card_number);
			strcpy(x,"SUCCESS, cancel ");
			strcat(x,buf);
		
			InserareHistoryLaInc((aux12),x);
		}
	}

	TLista A=L;

	while(A)
	{
		TListaCard aux21=A->card;

		while(aux21)
		{
			TListaHistory aux22=aux21->history;
			Distrugere_History(&aux22);
			aux21=aux21->next_card;
		}

		A=A->next_LSC;
	}

	TLista B=L;

	while(B)
	{
		TListaCard aux23=B->card;
		Distrugere_LSC(&aux23);
		B=B->next_LSC;
	}

	TLista C=L;
	Distrugere_LC(&L);
	fclose(f);
	fclose(g);
return 0;
}

