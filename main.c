#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>


typedef struct {
        char valor;
        char palo;
        struct Cartas *sig;
}Carta;

Carta *primera=NULL;
Carta *ultima=NULL;


void agregar_carta (Carta *carta){
    carta ->sig=NULL;

    if(primera==NULL){
        primera=carta;
        ultima=carta;
    }else{
        ultima->sig = carta;
        ultima=carta;
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
   Carta *primera_carta = malloc(sizeof(Carta));

    primera_carta -> palo='d';
    primera_carta -> valor='9';

    Carta *segunda_carta = malloc(sizeof(Carta));
    primera_carta -> palo='p';
    primera_carta -> valor='5';

    agregar_carta(primera_carta);
    agregar_carta(segunda_carta);

    Carta *i= primera;
    while(i != NULL){
        printf("%i, %i \n",i->valor,i->palo);
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
