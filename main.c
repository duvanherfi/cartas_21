#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <stdbool.h>



typedef struct
{
    char valor[2];
    char palo[2];
    struct Cartas *sig;
} Carta;

Carta *ultima=NULL;


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

Carta* pop()
{
    if(ultima==NULL)
    {
        return NULL;
    }
    Carta *carta_retorno = ultima;
    ultima= carta_retorno->sig;
}

Carta* crear_carta(char _valor, char _palo)
{
    Carta *primera_carta = malloc(sizeof(Carta));

    primera_carta -> palo[0]=_palo;
    primera_carta -> palo[1]='\0';
    primera_carta -> valor[0]=_valor;
    primera_carta -> valor[1]='\0';
    return primera_carta;
}

bool comprobar_existencia_carta(char _valor, char _palo)
{
    Carta* i=ultima;
    while(i!=NULL)
    {
        if(i->valor[0]==_valor && i->palo[0]==_palo)
        {
            return true;
        }
        i=i->sig;
    }
    return false;
}

void crear_baraja()
{
    srand(time(NULL));
    Carta* carta=NULL;
    int contador=0;
    int valor=0;
    int palo=0;
    int i;
    for(i=1; contador<52; i++)
    {
        valor=(rand()%13)+1;
        srand(time(NULL));
        palo=(rand()%4)+1;

        switch(valor)
        {
        case 1:
                if(!comprobar_existencia_carta('A', (char) palo)){
                    push(crear_carta('A', (char) palo));
                    contador++;
                }
            break;
        case 11:
            if(!comprobar_existencia_carta('J', (char) palo))
            {
                push(crear_carta('J', (char) palo));
                contador++;
            }
            break;
        case 12:
            if(!comprobar_existencia_carta('Q', (char) palo))
            {
                push(crear_carta('Q', (char) palo));
                contador++;
            }
            break;
        case 13:
            if(!comprobar_existencia_carta('K', (char) palo))
            {
                push(crear_carta('K', (char) palo));
                contador++;
            }
            break;
        default:
            if(!comprobar_existencia_carta((char) valor, (char) palo))
            {
                push(crear_carta((char) valor, (char) palo));
                contador++;
            }
            break;

        }
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
    crear_baraja();

    Carta *i=ultima;
    while(i != NULL)
    {
        printf("%s, %s \n",i->valor,i->palo);
        i=i->sig;
    }


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
