#include "syscall.h"




void afficheTeste(){
	PutChar('\n');
	PutString("--<prgramme> n: pour lancer programme n foix maximum 20\n");
	PutChar('\n');
	PutString("--forkexec :lance deux processus qui vont crée un thread et écrire\n");
	PutString("--putchar :créé un processus qui va écrire\n");
	PutString("--getstring :créé un processus qui va lire une chaine de caractère et l'écrire\n");
	PutString("--getint :créé un processus qui lie et écrire 5 entier\n");
	PutString("--t n:éssaye de crée n threads maximume 20 ,qui afficherons ab\n");
	return;
}


void affichecommande(){

	PutChar('\n');
	PutString("--q :quitter le minishell\n");
	PutString("--i :afficher les programme disponible\n");
	PutString("--c :afficher les commandes\n");
	PutChar('\n');
}


void teste(void *a){

	int i=0;
	char c='i';
	while(c!='\0'){
		c=((char*)a)[i];
		PutChar(c);
		i++;
	}
	PutChar('\n');

}



int
main ()
{
    
    char prompt[3], buffer[60];
    int i,newProc[20],n,attenteprocess,arg,finarg;

    char a[3];
	a[0]='a';
	a[1]='b';
	a[2]='\0';

    prompt[0] = '-';
    prompt[1] = '-';
    prompt[2] = '\0';

    affichecommande();
    while (1)
      {
	  arg=0;
	  n=0;
          finarg=0;
	  PutString(prompt);

	  i = 0;
	  GetString(buffer, 60);
	  while (buffer[i] != '\n' && i<60) {

		if(buffer[i]==' '){
			arg=1;
			finarg=i;
		}
		else if(arg==1 && buffer[i]!='\n'){
			n=n*10+buffer[i]-'0';
		}
		i++;

	  }
	  if(n>19){
		n=19;
	  }
	  attenteprocess=n;
	  if(arg){
		buffer[finarg] = '\0';
	  }
	  else{
		buffer[i] = '\0';
	  }
	  if(buffer[1] == '\0'){//lacommande fait 1 caractère l'entrée est destiner aux minishelle et pas a 
		   //crée un processus
		if(buffer[0]=='q'){//fin du minishelle
			return 0;
		}
		else if(buffer[0]=='i'){
			afficheTeste();//indique les fichier de teste disponible
		}
		else if(buffer[0]=='c'){
			affichecommande();
		}
		else if(buffer[0]=='t'){
			while(n>=0){
				newProc[n]=UserThreadCreate(teste,(void *)a,(int)UserThreadExit);
				n--;
			}
			while(attenteprocess>=0){
				if(newProc[attenteprocess]!=-1){
					UserThreadJoin(newProc[attenteprocess]);
				}
				attenteprocess--;
			}
		}
	  }
	  else if (i > 0 && i<60){
		if(n>0){
			n--;
			attenteprocess--;
		}
		while(n>=0){
			newProc[n]=ForkExec(buffer);
			n--;
		}
		while(attenteprocess>=0){
			if(newProc[attenteprocess]!=-1){
				UserProcessJoin(newProc[attenteprocess]);
			}
			attenteprocess--;
		}
	  }
	  else if (i>=60){//si la chaine de caractère n'a pas pus être récuperer en une seule fois on
			//le buffer de synchonsole
		PutString("le nom de prgramme est trops long\n");
		while (i!=-1){
			i=0;
			GetString(buffer, 60);
			while(i<60 && i!=-1){
				if(buffer[i]=='\n')
					i=-1;
				else 
					i++;
			}
		}
          }
	}
}
