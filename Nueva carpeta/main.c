#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
typedef struct
{
    char *valor;
    char *palo;
    struct Cartas *sig;
} Carta;

Carta *ultima=NULL;//puntero que crea pila

Carta *cab=NULL;//se crea para crear lista con cartas ordenadas
Carta *movible=NULL;//puntero adicional para añadir elementos en la lista

void add_lista(Carta *carta)
{
    if(cab==NULL)
    {
        cab=malloc(sizeof(Carta));
        cab=carta;
        movible=cab;
    }
    else
    {
        movible->sig=carta;
        movible=carta;
    }
}

Carta* retirar_de_la_lista(int r)
{
    Carta *i=cab;
    int contador=1;
    while(i!=r)
    {
        contador=1;
        i=i->sig;
    }
    return i;
}

void push(Carta *carta)
{
    carta ->sig=NULL;

    if(ultima==NULL)
    {
        ultima=carta;
    }
    else
    {
        carta->sig = ultima;
        ultima=carta;
    }
}

Carta* crear_carta(char* _valor, char* _palo)
{
    Carta *primera_carta = malloc(sizeof(Carta));

    primera_carta -> palo=_palo;
    primera_carta -> valor=_valor;
    return primera_carta;
}


int main()
{
    srand(time(NULL));
    Carta* carta=NULL;
    int palo=0;
    int i;
    char *cadena=NULL;


    for(i=1; i<=13; i++)
    {
        cadena=malloc(sizeof(char));
        switch(i)
        {
        case 1:
            push(crear_carta("A", "diamante"));
            break;
        case 11:
            push(crear_carta("J", "diamante"));
            break;
        case 12:
            push(crear_carta("Q", "diamante"));
            break;
        case 13:
            push(crear_carta("K", "diamante"));
            break;
        default:
            sprintf(cadena,"%i",i);
            push(crear_carta(cadena, "diamante"));
            break;
        }
        cadena=NULL;
    }

    Carta *j=ultima;
    while(j != NULL)
    {
        printf("%s, %s \n",j->valor,j->palo);

        j=j->sig;
    }
    free(ultima);
    free(cadena);

    return 0;
}
