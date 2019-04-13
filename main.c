#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <stdbool.h>



typedef struct
{
    char* valor;
    char* palo;
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
    Carta *aux=cab;
    int contador=1;
    if(r==contador){
        aux=aux->sig;
        cab=aux;
        return i;
    }

    while(contador<r){
        aux=i;
        i=i->sig;
        contador++;
    }

    aux->sig=i->sig;
    return i;
}

void push(Carta *carta)
{


    if(ultima==NULL)
    {
        ultima=carta;
        ultima->sig=NULL;
    }
    else
    {
        carta->sig = ultima;
        ultima=carta;
    }
}

Carta* pop()
{
    if(ultima==NULL)
    {
        return NULL;
    }
    Carta *carta_retorno = ultima;
    ultima= carta_retorno->sig;
    return carta_retorno;
}

Carta* crear_carta(char* _valor, char* _palo)
{
    Carta *primera_carta = malloc(sizeof(Carta));

    primera_carta -> palo=_palo;
    primera_carta -> valor=_valor;
    primera_carta ->sig=NULL;
    return primera_carta;
}
void ciclo(char* palo)
{
    srand(time(NULL));
    int i;
    char *cadena=NULL;


    for(i=1; i<=13; i++)
    {
        cadena=malloc(sizeof(char));
        switch(i)
        {
        case 1:
            add_lista(crear_carta("A", palo));
            break;
        case 11:
            add_lista(crear_carta("J", palo));
            break;
        case 12:
            add_lista(crear_carta("Q", palo));
            break;
        case 13:
            add_lista(crear_carta("K",palo));
            break;
        default:
            sprintf(cadena,"%i",i);
            add_lista(crear_carta(cadena, palo));
            break;
        }
        cadena=NULL;
    }
}
void crear_baraja_ordenada()
{
    char *palo="corazon";
    ciclo(palo);

    palo="diamante";
    ciclo(palo);

    palo="picas";
    ciclo(palo);

    palo="trebol";
    ciclo(palo);
}

void crear_baraja()
{
    srand(time(NULL));
    int i,n=52,c=52;
    printf("pila******* \n");
    for(i=1;i<=c;i++){
        int random=(rand()%n)+1;

        push(retirar_de_la_lista(random));
        n--;
    }
}


void iniciarAllegro(int ancho, int alto)
{
    allegro_init();
    text_mode(-1);
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,ancho,alto, 0,0);

}

int main()
{
    crear_baraja_ordenada();

    Carta *i=cab;
    while(i != NULL)
    {
        printf("%s, %s \n",i->valor,i->palo);
        i=i->sig;
    }

    crear_baraja();



    i=ultima;
    while(i != NULL)
    {
        printf("%s, %s \n",i->valor,i->palo);
        i=i->sig;
    }

    free(cab);
    free(ultima);
    free(i);
    free(movible);


//    iniciarAllegro(320,320);
//    BITMAP *buffer = create_bitmap(320,320);
//    clear_to_color(buffer, 0x999999);
//
//    textout_centre_ex(buffer, font, "Mi Primer Ventana", 160, 25, 0xFFFFFF, 0x999999);
//    blit(buffer, screen, 0, 0, 0, 0, 640, 480);
//    readkey();
//
//    destroy_bitmap(buffer);
    return 0;
}
END_OF_MAIN ()
