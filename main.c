#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <stdbool.h>

typedef struct     // Se define la estructura carta, compuesta por el valor, el palo, la imagen y puntero siguiente
{
    char* valor;
    char* palo;
    BITMAP* imagen;
    struct Cartas *sig;
} Carta;

Carta *ultima=NULL;//puntero que crea pila

Carta *cab=NULL;//se crea para crear lista con cartas ordenadas
Carta *movible=NULL;//puntero adicional para añadir elementos en la lista

Carta *cab_maquina=NULL;//Lista de la baraja de la maquina.
Carta *mov_maquina=NULL;//Puntero para añadir elementos en la lista maquina.
Carta *cab_jugador=NULL;//Lista de la baraja del jugador.
Carta *mov_jugador=NULL;//Puntero para añadir elementos en la lista jugador.

BITMAP *braton;
BITMAP *raton;
BITMAP *buffer;
BITMAP *buffer2;

Carta *add_lista(Carta *carta, Carta *cabecera, Carta *p)  // Método que permite añadir una carta a una lista, recibe la carta que se va añadir,la cabecera de la lista y un puntero auxiliar
{
    if(cabecera==NULL) // si la lista esta vacia
    {
        cabecera=malloc(sizeof(Carta));
        cabecera=carta;
        cabecera->sig=NULL;
        p=cabecera;
    }
    else
    {
        p->sig=carta;
        carta->sig=NULL;
        p=carta;
    }
    return p;
}

Carta* retirar_de_la_lista(int r) // Método que retira una carta de una lista, recibiendo como parametro la posicion de la carta
{
    Carta *i=cab;
    Carta *aux=cab;
    int contador=1;
    if(r==contador)
    {
        aux=aux->sig;
        cab=aux;
        return i;
    }

    while(contador<r)
    {
        aux=i;
        i=i->sig;
        contador++;
    }

    aux->sig=i->sig;
    return i;
}

void push(Carta *carta) //Método que agrega una carta en una pila
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

Carta* pop() //Metodo que elimina la ultima carta de una pila
{
    if(ultima==NULL)
    {
        return NULL;
    }
    Carta *carta_retorno = ultima;
    ultima= carta_retorno->sig;
    return carta_retorno;
}

Carta* crear_carta(char* _valor, char* _palo) //  Crear una carta
{
    Carta *primera_carta = malloc(sizeof(Carta));
    char* d[80];
    sprintf(d, "Cartas\\%s\\%s.bmp", _palo, _valor); // Guarda la ruta en d(imagenes de las cartas)
    const char* ca = d;

    primera_carta -> palo=_palo;
    primera_carta -> valor=_valor;
    primera_carta ->imagen= load_bitmap(ca, NULL);
    primera_carta ->sig=NULL;
    return primera_carta;
}

void ciclo(char* palo) //Este metodo crea las 13 cartas de determinado palo mandado por parametro y las añade a una lista SE
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
            movible=add_lista(crear_carta("A", palo),cab,movible);
            if(cab==NULL)
            {
                cab=movible;
            }
            break;
        case 11:
            movible=add_lista(crear_carta("J", palo),cab,movible);
            break;
        case 12:
            movible=add_lista(crear_carta("Q", palo),cab,movible);
            break;
        case 13:
            movible=add_lista(crear_carta("K",palo),cab,movible);
            break;
        default:
            sprintf(cadena,"%i",i);
            movible=add_lista(crear_carta(cadena, palo),cab,movible);
            break;
        }
        cadena=NULL;
    }
}

void crear_baraja_ordenada() //Mediante el metodo ciclo y la variable palo crea la baraja completa ordenada
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

void crear_baraja() //Crea el mazo mezclado
{
    srand(time(NULL));
    int i,n=52,c=52;
    printf("pila******* \n");
    for(i=1; i<=c; i++)
    {
        int random=(rand()%n)+1;

        push(retirar_de_la_lista(random));
        n--;
    }
}

void crear_baraja_inicial() //  Crea el mazo(pila) inicial del jugador y la maquina
{
    mov_jugador=add_lista(pop(), cab_jugador, mov_jugador);
    cab_jugador=mov_jugador;
    mov_jugador=add_lista(pop(), cab_jugador, mov_jugador);
    mov_maquina=add_lista(pop(), cab_maquina, mov_maquina);
    cab_maquina=mov_maquina;
    mov_maquina=add_lista(pop(), cab_maquina, mov_maquina);
}

void iniciarAllegro(int ancho, int alto)
{
    allegro_init();
    text_mode(-1);
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,ancho,alto, 0,0);
}

void dibujar_mouse()
{
    blit(raton, braton, 0,0,0,0, 13, 22);
    draw_sprite(screen, braton, mouse_x, mouse_y);
    draw_sprite(buffer, braton, mouse_x, mouse_y);
}

int main()
{
    iniciarAllegro(700,350);
    crear_baraja_ordenada();



    crear_baraja();


    Carta *i=ultima;
    while(i != NULL) //Imprime por pantalla el mazo completo
    {
        printf("%s, %s \n",i->valor,i->palo);
        i=i->sig;
    }

    crear_baraja_inicial();

    i=cab_jugador;
    printf("\nBaraja Jugador\n");
    while(i != NULL) //Imprime por pantalla el mazo del jugador
        printf("%s, %s \n",i->valor,i->palo);
        i=i->sig;
    }

    i=cab_maquina;
    printf("\nBaraja Maquina\n");
    while(i != NULL) //Imprime por pantalla el mazo de la maquina
    {
        printf("%s, %s \n",i->valor,i->palo);
        i=i->sig;
    }



    free(cab);
    free(ultima);
    free(i);
    free(movible);


    buffer = create_bitmap(700,350); // Se crea el buffer donde vamos a pintar para luego imprimir en pantalla
    buffer2 = create_bitmap(200,300);
    braton = create_bitmap(13,22);
    raton = load_bitmap("cursor1.bmp",NULL);

    //blit(cab_jugador->imagen, buffer, 0, 0, 0, 0, 200, 300);
    bool x = true;

    while (x == true)
    {
        i=cab_jugador;
        int y = 0;
        blit(buffer, screen, 0, 0, 0, 0, 700, 350); // imprimimos el buffer en la pantalla
        while(i != NULL)
        {
            draw_sprite(buffer, i->imagen, y, 50); //pintamos la carta en el buffer y aunmentamos x para mostrar la siguiente
            y = y + 100;
            i=i->sig;
        }
        i=cab_maquina;
        int z = 400;
        while(i != NULL)
        {
            draw_sprite(buffer, i->imagen, z, 50)//pintamos la carta en el buffer y aunmentamos x para mostrar la siguiente
            z = z + 100;
            i=i->sig;
        }
        textout_centre_ex(buffer, font, "Baraja Jugador", 150, 25, 0xFFFFFF, 0x000000);
        textout_centre_ex(buffer, font, "Baraja Maquina", 550, 25, 0xFFFFFF, 0x000000);
        //clear_to_color(buffer, 0x999999);
        //textout_centre_ex(buffer, font, "Mi Primer Ventana", 160, 25, 0xFFFFFF, 0x999999);
        //blit(buffer, screen, 0, 0, 0, 0, 640, 480);

        //dibujar_mouse();
        //if(mouse_b & 1)
        //{
        //  clear_to_color(buffer, 0x0000FF);
        //  textout_centre_ex(buffer, font, "Mi Primer Ventana", 160, 25, 0xFFFFFF, 0x999999);
        //  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
        //  rest(80);
        //  x = false;
        //  printf("Hola");

        //}


    }


    readkey();
    destroy_bitmap(buffer);
    return 0;
}
END_OF_MAIN ()
