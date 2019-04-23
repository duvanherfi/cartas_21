#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <stdbool.h>

typedef struct   // Se define la estructura carta, compuesta por el valor, el palo, la imagen y puntero siguiente
{
    char* valor;
    char* palo;
    BITMAP* imagen;
    int valor_numero;
    struct Cartas *sig;
} Carta;

Carta *ultima=NULL;//puntero que crea pila

Carta *cab=NULL;//se crea para crear lista con cartas ordenadas
Carta *movible=NULL;//puntero adicional para añadir elementos en la lista

Carta *cab_maquina=NULL;//Lista de la baraja de la maquina.
Carta *mov_maquina=NULL;//Puntero que apunta al ultimo nodo para añadir elementos en la lista maquina.
Carta *cab_jugador=NULL;//Lista de la baraja del jugador.
Carta *mov_jugador=NULL;//Puntero que apunta al ultimo nodo para añadir elementos en la lista jugador.
Carta *pre_mov_jugador=NULL;//Puntero que apunta al penultimo nodo en la lista jugador.
Carta *pre_mov_maquina=NULL;//Puntero que apunta al penultimo nodo en la lista maquina.

Carta *i;//puntero auxiiar

BITMAP *raton;
BITMAP *buffer;
BITMAP *carta_volteada;
BITMAP *fondo_menu;
BITMAP *fondo_jugar;
BITMAP *fondo_mazo_jugador;
BITMAP *fondo_mazo_maquina;
BITMAP *fondo_instrucciones;
BITMAP *fondo_acerca_de;

int cont_cartas_pedidas = 0; //Para realizar solo la animacion de su carta.
int cont_cartas_maq_pedidas = 0; //Para realizar solo la animacion de su carta.
int pos_x_cartas_pedidas; //Posición de las cartas pedidas.
int pos_x_maq_cartas_pedidas; //Posición de las cartas pedidas.
int suma_cartas_maquina = 0, suma_cartas_maquina_opcional = 0;
int suma_cartas_jugador = 0, suma_cartas_jugador_opcional = 0;

bool primera_A_jugador = false, primera_A_maquina = false;
bool menu=true, jugar=false, instrucciones=false, acerca_de=false;
bool rest_juego = true;
bool plantar=false;

Carta *add_lista(Carta *carta, Carta *cabecera, Carta *p)   // Método que permite añadir una carta a una lista, recibe la carta que se va añadir,la cabecera de la lista y un puntero auxiliar
{
    if(cabecera==NULL)   // si la lista esta vacia
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

Carta* retirar_de_la_lista(int r)  // Método que retira una carta de una lista, recibiendo como parametro la posicion de la carta
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

void push(Carta *carta)  //Método que agrega una carta en una pila
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

Carta* pop()  //Metodo que elimina la ultima carta de una pila
{
    if(ultima==NULL)
    {
        return NULL;
    }
    Carta *carta_retorno = ultima;
    ultima= carta_retorno->sig;
    return carta_retorno;
}

Carta* crear_carta(char* _valor, char* _palo, int* _valor_numero)  //  Crear una carta
{
    Carta *primera_carta = malloc(sizeof(Carta));
    char* d[80];
    sprintf(d, "Cartas\\%s\\%s.bmp", _palo, _valor);  // Guarda la ruta en d(imagenes de las cartas)
    const char* ca = d;

    primera_carta -> palo=_palo;
    primera_carta -> valor=_valor;
    primera_carta -> imagen= load_bitmap(ca, NULL);
    primera_carta -> valor_numero = _valor_numero;
    primera_carta ->sig=NULL;
    return primera_carta;
}

void ciclo(char* palo)  //Este metodo crea las 13 cartas de determinado palo mandado por parametro y las añade a una lista SE
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
            movible=add_lista(crear_carta("A", palo, i),cab,movible);
            if(cab==NULL)
            {
                cab=movible;
            }
            break;
        case 11:
            movible=add_lista(crear_carta("J", palo, i),cab,movible);
            break;
        case 12:
            movible=add_lista(crear_carta("Q", palo, i),cab,movible);
            break;
        case 13:
            movible=add_lista(crear_carta("K",palo, i),cab,movible);
            break;
        default:
            sprintf(cadena,"%i",i);
            movible=add_lista(crear_carta(cadena, palo, i),cab,movible);
            break;
        }
        cadena=NULL;
    }
}

void crear_baraja_ordenada()  //Mediante el metodo ciclo y la variable palo crea la baraja completa ordenada
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

void crear_baraja()  //Crea el mazo mezclado
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

void sumar(Carta *carta, char *participante) // Metodo que suma la ultima carta a añadir a una lista de un jugador o la maquina, teniendo cuenta
{
    if(participante == "jugador")
    {
        if(carta->valor_numero > 10)
        {
            suma_cartas_jugador = suma_cartas_jugador + 10;
            suma_cartas_jugador_opcional = suma_cartas_jugador_opcional + 10;
        }
        else if(carta->valor_numero == 1)
        {
            suma_cartas_jugador = suma_cartas_jugador + 1;
            if (primera_A_jugador == false)
            {
                suma_cartas_jugador_opcional = suma_cartas_jugador_opcional + 11;
                primera_A_jugador = true;
            }
            else suma_cartas_jugador_opcional = suma_cartas_jugador_opcional + 1;
        }
        else
        {
            suma_cartas_jugador = suma_cartas_jugador + carta->valor_numero;
            suma_cartas_jugador_opcional = suma_cartas_jugador_opcional +carta->valor_numero;
        }
    }
    else if(participante == "maquina")
    {
        if(carta->valor_numero > 10)
        {
            suma_cartas_maquina = suma_cartas_maquina + 10;
            suma_cartas_maquina_opcional = suma_cartas_maquina_opcional + 10;
        }
        else if(carta->valor_numero == 1)
        {
            suma_cartas_maquina = suma_cartas_maquina + 1;
            if (primera_A_maquina == false)
            {
                suma_cartas_maquina_opcional = suma_cartas_maquina_opcional + 11;
                primera_A_maquina = true;
            }
            else suma_cartas_maquina_opcional = suma_cartas_maquina_opcional + 1;
        }
        else
        {
            suma_cartas_maquina = suma_cartas_maquina + carta->valor_numero;
            suma_cartas_maquina_opcional = suma_cartas_maquina_opcional + carta->valor_numero;
        }
    }
}

void crear_baraja_inicial()  //  Crea el mazo(pila) inicial del jugador y la maquina
{
    mov_jugador=add_lista(pop(), cab_jugador, mov_jugador);
    cab_jugador=mov_jugador;
    pre_mov_jugador=mov_jugador;
    mov_jugador=add_lista(pop(), cab_jugador, mov_jugador);
    mov_maquina=add_lista(pop(), cab_maquina, mov_maquina);
    cab_maquina=mov_maquina;
    mov_maquina=add_lista(pop(), cab_maquina, mov_maquina);

    sumar(cab_jugador, "jugador");
    sumar(mov_jugador, "jugador");
    sumar(cab_maquina, "maquina");
    sumar(mov_maquina, "maquina");
}

void ordenar_mazo_jugador() // Ordena de manera ascendente el mazo del jugador
{
    if(pre_mov_jugador->valor_numero > mov_jugador->valor_numero)
    {
        if(cab_jugador->valor_numero > mov_jugador->valor_numero) //La ultima carta agregada se coloca antes de la primera carta.
        {
            pre_mov_jugador->sig = NULL;
            mov_jugador->sig = cab_jugador;
            cab_jugador = mov_jugador;
            mov_jugador = pre_mov_jugador;
        }
        else
        {
            Carta *actual = cab_jugador, *actual_sig = cab_jugador->sig;
            while(mov_jugador->valor_numero > actual->valor_numero && mov_jugador->valor_numero > actual_sig->valor_numero) //La ultima carta agregada se coloca en el intermedio de la lista de forma ordenada.
            {
                actual = actual->sig;
                actual_sig = actual_sig->sig;
            }
            pre_mov_jugador->sig = NULL;
            mov_jugador->sig = actual->sig;
            actual->sig = mov_jugador;
            mov_jugador = pre_mov_jugador;
        }
        blit(fondo_mazo_jugador, buffer, 0, 0, 339, 234, 461, 120);//Pintar fondo del mazo del jugador en el buffer.
        rest_animacion(false);
        i=cab_jugador;
        draw_sprite(buffer, i->imagen, 339, 234);//Muestra la imagen de la primera carta del jugador.
        i = i->sig;
        draw_sprite(buffer, i->imagen, 376, 234); //Muestra la imagen de la segunda carta del jugador.
    }
    rest_animacion(false);
}

void rest_animacion(bool aux)//Realiza una pequeña animacion de entrega de carta al iniciar el juego.
{
    if (rest_juego == true)//Animación de entrega de carta.
    {
        blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.
        rest(500);
        rest_juego = aux;
    }
}

void iniciarAllegro(int ancho, int alto)
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,ancho,alto, 0,0);
}

void dibujar_mouse()
{
    draw_sprite(buffer, raton, mouse_x, mouse_y);
}

void reiniciar()
{
    ultima=NULL;
    cab=NULL;
    movible=NULL;
    cab_maquina=NULL;
    mov_maquina=NULL;
    cab_jugador=NULL;
    mov_jugador=NULL;
    pre_mov_jugador=NULL;
    i=NULL;

    cont_cartas_pedidas = 0;
    cont_cartas_maq_pedidas = 0;
    pos_x_cartas_pedidas=0;
    pos_x_maq_cartas_pedidas=0;
    suma_cartas_maquina = 0;
    suma_cartas_maquina_opcional = 0;
    suma_cartas_jugador = 0;
    suma_cartas_jugador_opcional = 0;
    primera_A_jugador = false;
    primera_A_maquina = false;
    plantar=false;

}

int main()
{
    srand(time(NULL));
    iniciarAllegro(800,500);//LE DECIMOS A ALLEGRO QUE INICIALICE UNA VENTANA DE 800 DE ANCHO POR 500 DE ALTO

    buffer = create_bitmap(800,500);  // Se crea el buffer donde vamos a pintar para luego imprimir en pantalla, teniendo en cuenta si la A vale 1 o 11
    raton = load_bitmap("img\\cursor1.bmp",NULL);
    carta_volteada = load_bitmap("Cartas\\corazon\\Carta Volteada.bmp",NULL);
    fondo_menu = load_bitmap("img\\fondo_menu.bmp",NULL);
    fondo_jugar = load_bitmap("img\\fondo_jugar.bmp",NULL);
    fondo_mazo_jugador = load_bitmap("img\\fondo_mazo_jugador.bmp",NULL);
    fondo_instrucciones = load_bitmap("img\\fondo_instrucciones.bmp",NULL);
    fondo_acerca_de = load_bitmap("img\\fondo_acerca_de.bmp",NULL);

    //Crear fuente tipo casino.
    DATAFILE *archivo,*archivo2;
    FONT *mifont_30,*mifont_18;
    archivo = load_datafile("fuentes\\FuenteCasino.dat");
    archivo2 = load_datafile("fuentes\\FuenteCasino_18.dat");
    mifont_30 = (FONT *) archivo[0].dat;
    mifont_18 = (FONT *) archivo2[0].dat;

    while (true)
    {
        dibujar_mouse();
        blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.

        if(menu == true)//Ventana menu abierta
        {
            blit(fondo_menu, buffer, 0, 0, 0, 0, 800, 500);//Pintar menu en el buffer.

            //Imprimir texto de cada opción del menu.
            textout_centre_ex(buffer, mifont_30, "Jugar", 400, 100, 0x000000, text_mode(-1));
            textout_centre_ex(buffer, mifont_30, "Instrucciones", 400, 180, 0x000000, text_mode(-1));
            textout_centre_ex(buffer, mifont_30, "Acerca de", 400, 265, 0x000000, text_mode(-1));
            textout_centre_ex(buffer, mifont_30, "Salir", 400, 355, 0x000000, text_mode(-1));

            //Caracteristicas de las opciones de menu.
            if (mouse_x >= 305 && mouse_x <= 490 && mouse_y >= 80 && mouse_y <= 140) //Opción "Jugar".
            {

                textout_centre_ex(buffer, mifont_30, "Jugar", 400, 100, 0xFFFFFF, text_mode(-1));
                if(mouse_b & 1)//Entrar a la opción jugar.
                {

                    reiniciar();
                    crear_baraja_ordenada();
                    crear_baraja();
                    cab=NULL;
                    crear_baraja_inicial();

                    i=cab_jugador;
                    printf("\nBaraja Jugador\n");
                    while(i != NULL)  //Imprime por pantalla el mazo del jugador
                    {
                        printf("%s, %s, %d \n",i->valor,i->palo,i->valor_numero);
                        i=i->sig;
                    }
                    printf("Suma jugador: %d\n", suma_cartas_jugador);
                    printf("Suma jugador opcional: %d\n", suma_cartas_jugador_opcional);

                    i=cab_maquina;
                    printf("\nBaraja Maquina\n");
                    while(i != NULL) //Imprime por pantalla el mazo de la maquina
                    {
                        printf("%s, %s, %d \n",i->valor,i->palo,i->valor_numero);
                        i=i->sig;
                    }

                    printf("Suma maquina: %d", suma_cartas_maquina);
                    printf("\nSuma maquina opcional: %d", suma_cartas_maquina_opcional);
                    jugar = true;
                    menu = false;
                    rest(350);
                }
            }
            if (mouse_x >= 280 && mouse_x <= 515 && mouse_y >= 160 && mouse_y <= 220) //Opción "Instrucciones".
            {
                textout_centre_ex(buffer, mifont_30, "Instrucciones", 400, 180, 0xFFFFFF, text_mode(-1));
                if(mouse_b & 1)//Entrar a la opción instrucciones.
                {
                    menu = false;
                    instrucciones = true;
                }
            }
            if (mouse_x >= 295 && mouse_x <= 505 && mouse_y >= 245 && mouse_y <= 305) //Opción "Acerca de".
            {
                textout_centre_ex(buffer, mifont_30, "Acerca de", 400, 265, 0xFFFFFF, text_mode(-1));
                if(mouse_b & 1)//Entrar a la opción instrucciones.
                {
                    menu = false;
                    acerca_de = true;
                }
            }
            if (mouse_x >= 305 && mouse_x <= 490 && mouse_y >= 340 && mouse_y <= 395) //Opción "Salir".
            {
                textout_centre_ex(buffer, mifont_30, "Salir", 400, 355, 0xFFFFFF, text_mode(-1));

                if(mouse_b & 1)
                {
                    free(ultima);//liberar punteros
                    free(cab);
                    free(movible);
                    free(cab_maquina);
                    free(mov_maquina);
                    free(cab_jugador);
                    free(mov_jugador);
                    free(pre_mov_jugador);
                    free(i);

                    destroy_bitmap(buffer);
                    destroy_bitmap(raton);
                    destroy_bitmap(carta_volteada);
                    destroy_bitmap(fondo_menu);
                    destroy_bitmap(fondo_jugar);
                    destroy_bitmap(fondo_mazo_jugador);
                    return 0;
                }
            }
        }

        if(jugar == true)//Inicio del juego blackjack.
        {
            blit(fondo_jugar, buffer, 0, 0, 0, 0, 800, 500);//Pintar fondo mesa en el buffer.

            //Imprimir texto de cada opción al jugar.
            textout_centre_ex(buffer, mifont_18, "Salir", 87, 425, 0xFFFFFF, text_mode(-1));
            textout_centre_ex(buffer, mifont_18, "Menu", 230, 425, 0xFFFFFF, text_mode(-1));
            textout_centre_ex(buffer, mifont_18, "Pedir", 577, 425, 0xFFFFFF, text_mode(-1));
            textout_centre_ex(buffer, mifont_18, "Plantar", 713, 425, 0xFFFFFF, text_mode(-1));

            rest_animacion(true);
            draw_sprite(buffer, cab_jugador->imagen, 339, 234); //Muestra la imagen de la primera carta del jugador.
            rest_animacion(true);
            draw_sprite(buffer, carta_volteada, 339, 31);//Muestra la imagen de la primera carta de la maquina.
            rest_animacion(true);
            i = cab_jugador->sig;
            draw_sprite(buffer, i->imagen, 376, 234); //Muestra la imagen de la segunda carta del jugador.
            rest_animacion(true);
            i=cab_maquina->sig;
            draw_sprite(buffer, i->imagen, 376, 31);//Muestra la imagen de la segunda carta volteada de la maquina.
            rest_animacion(true);

            //Caracteristicas de las opciones al jugar.
            if (mouse_x >= 25 && mouse_x <= 150 && mouse_y >= 375 && mouse_y <= 493)  //Opción "Salir".
            {
                textout_centre_ex(buffer, mifont_18, "Salir", 87, 425, 0x000000, text_mode(-1));
                if(mouse_b & 1)
                {
                    free(ultima);//liberar punteros
                    free(cab);
                    free(movible);
                    free(cab_maquina);
                    free(mov_maquina);
                    free(cab_jugador);
                    free(mov_jugador);
                    free(pre_mov_jugador);
                    free(i);

                    destroy_bitmap(buffer);
                    destroy_bitmap(raton);
                    destroy_bitmap(carta_volteada);
                    destroy_bitmap(fondo_menu);
                    destroy_bitmap(fondo_jugar);
                    destroy_bitmap(fondo_mazo_jugador);
                    return 0;
                }
            }
            if (mouse_x >= 170 && mouse_x <= 290 && mouse_y >= 375 && mouse_y <= 493) //Opción "Menu".
            {
                textout_centre_ex(buffer, mifont_18, "Menu", 230, 425, 0x000000, text_mode(-1));
                if(mouse_b & 1)
                {
                    jugar=false;
                    menu=true;
                }
            }
            if(!plantar)//mientras el jugador no de click en plantar
            {
                if (mouse_x >= 518 && mouse_x <= 636 && mouse_y >= 375 && mouse_y <= 493) //Opción "Pedir".
                {
                    textout_centre_ex(buffer, mifont_18, "Pedir", 577, 425, 0x000000, text_mode(-1));
                    if(mouse_b & 1)
                    {
                        pre_mov_jugador=mov_jugador;
                        mov_jugador=add_lista(pop(), cab_jugador, mov_jugador);
                        sumar(mov_jugador, "jugador");
                        printf("\n\nSuma jugador: %d", suma_cartas_jugador);
                        printf("\nSuma jugador opcional: %d", suma_cartas_jugador_opcional);
                        cont_cartas_pedidas = cont_cartas_pedidas + 1;
                        rest_juego = true;
                        rest(500);
                    }
                }

                if (mouse_x >= 653 && mouse_x <= 775 && mouse_y >= 375 && mouse_y <= 493) //Opción "Plantar".
                {
                    textout_centre_ex(buffer, mifont_18, "Plantar", 713, 425, 0x000000, text_mode(-1));
                    if(mouse_b & 1)
                    {
                        plantar=true;
                    }
                }

                int q;

                pos_x_cartas_pedidas = 413;
                i=cab_jugador->sig;
                for (q=1; q<=cont_cartas_pedidas; q++)//Mostrar las cartas pedidas;
                {
                    i = i->sig;
                    draw_sprite(buffer, i->imagen, pos_x_cartas_pedidas, 234);
                    pos_x_cartas_pedidas = pos_x_cartas_pedidas + 37;
                    if (cont_cartas_pedidas == q) rest_animacion(true);
                }



                ordenar_mazo_jugador();//Ordena el mazo del jugador.
                if(suma_cartas_jugador>21)  //evaluar
                {
                    textout_centre_ex(buffer, mifont_30, "HAS PERDIDO", 398,182, 0xFFFFFF, text_mode(-1));
                    blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.
                    rest(2000);
                    jugar=false;
                    menu=true;

                }

            }
            else //si el jugador se planta sigue el turno de la maquina
            {
                int pedir_maq=1;
                if((suma_cartas_maquina>=16 && suma_cartas_maquina<=20) || (suma_cartas_maquina_opcional>=16 && suma_cartas_maquina_opcional<21)){
                    pedir_maq=(rand()%2)+1;
                    printf("imprimir aleatorio %d",pedir_maq);
                }else if (suma_cartas_maquina>21){
                    textout_centre_ex(buffer, mifont_30, "HAS GANADO", 398,182, 0xFFFFFF, text_mode(-1));
                        blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.
                        rest(2000);
                        jugar=false;
                        menu=true;
                        pedir_maq=0;
                }else if(suma_cartas_maquina==21){
                    pedir_maq=2;
                }


                if(pedir_maq==1)
                {

                    printf("Entra");
                    pre_mov_maquina=mov_maquina;
                    mov_maquina=add_lista(pop(), cab_maquina, mov_maquina);
                    sumar(mov_maquina, "maquina");
                    printf("\n\nSuma maquina: %d", suma_cartas_maquina);
                    printf("\nSuma maquina opcional: %d", suma_cartas_maquina_opcional);
                    cont_cartas_maq_pedidas = cont_cartas_maq_pedidas + 1;
                    rest_juego = true;
                    rest(500);

                    int q;
                    i=cab_maquina->sig;
                    pos_x_maq_cartas_pedidas = 413;
                    for (q=1; q<=cont_cartas_maq_pedidas; q++)//Mostrar las cartas pedidas;
                    {
                        i = i->sig;
                        draw_sprite(buffer, i->imagen, pos_x_maq_cartas_pedidas, 31);
                        pos_x_maq_cartas_pedidas = pos_x_maq_cartas_pedidas + 37;
                        if (cont_cartas_maq_pedidas == q) rest_animacion(true);
                    }

                }
                else if(pedir_maq==2)
                {

                    if(suma_cartas_maquina==suma_cartas_jugador ||
                            suma_cartas_maquina==suma_cartas_jugador_opcional ||
                            suma_cartas_maquina_opcional==suma_cartas_jugador_opcional ||
                            suma_cartas_maquina_opcional==suma_cartas_jugador)
                    {
                        textout_centre_ex(buffer, mifont_30, "HAS EMPATADO CON LA MAQUINA", 398,182, 0xFFFFFF, text_mode(-1));
                        blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.
                        rest(2000);
                        jugar=false;
                        menu=true;

                    }

                    else if(suma_cartas_maquina==21 || suma_cartas_maquina_opcional == 21)
                    {
                        textout_centre_ex(buffer, mifont_30, "HAS PERDIDO", 398,182, 0xFFFFFF, text_mode(-1));
                        blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.
                        rest(2000);
                        jugar=false;
                        menu=true;

                    }

                    else if(suma_cartas_jugador==21 || suma_cartas_jugador_opcional ==21)  //evaluar
                    {
                        textout_centre_ex(buffer, mifont_30, "HAS GANADO", 398,182, 0xFFFFFF, text_mode(-1));
                        blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.
                        rest(2000);
                        jugar=false;
                        menu=true;

                    }
                    else if(suma_cartas_maquina>suma_cartas_jugador){
                        textout_centre_ex(buffer, mifont_30, "HAS PERDIDO", 398,182, 0xFFFFFF, text_mode(-1));
                        blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.
                        rest(2000);
                        jugar=false;
                        menu=true;
                    }
                    else if(suma_cartas_jugador>suma_cartas_maquina)  //evaluar
                    {
                        textout_centre_ex(buffer, mifont_30, "HAS GANADO", 398,182, 0xFFFFFF, text_mode(-1));
                        blit(buffer, screen, 0, 0, 0, 0, 800, 500);//Pintar buffer en la pantalla.
                        rest(2000);
                        jugar=false;
                        menu=true;

                    }
                }


            }
        }

        if (instrucciones == true) //Ventana instrucciones abierta
        {
            blit(fondo_instrucciones, buffer, 0, 0, 0, 0, 800, 500);//Pintar fondo instrucciones en el buffer.
            if (mouse_x >= 170 && mouse_x <= 290 && mouse_y >= 375 && mouse_y <= 493) //Opción "Menu".
            {
                textout_centre_ex(buffer, mifont_18, "Menu", 230, 425, 0x000000, text_mode(-1));
                if(mouse_b & 1)
                {
                    instrucciones = false;
                    menu = true;
                }
            }

            if (mouse_x >= 25 && mouse_x <= 150 && mouse_y >= 375 && mouse_y <= 493)  //Opción "Salir".
            {
                textout_centre_ex(buffer, mifont_18, "Salir", 87, 425, 0x000000, text_mode(-1));
                if(mouse_b & 1)
                {
                    free(ultima);//liberar punteros
                    free(cab);
                    free(movible);
                    free(cab_maquina);
                    free(mov_maquina);
                    free(cab_jugador);
                    free(mov_jugador);
                    free(pre_mov_jugador);
                    free(i);

                    destroy_bitmap(buffer);
                    destroy_bitmap(raton);
                    destroy_bitmap(carta_volteada);
                    destroy_bitmap(fondo_menu);
                    destroy_bitmap(fondo_jugar);
                    destroy_bitmap(fondo_mazo_jugador);
                    return 0;
                }
            }
        }

        if (acerca_de == true) //Ventana "Acerca de" abierta
        {
            blit(fondo_acerca_de, buffer, 0, 0, 0, 0, 800, 500);//Pintar fondo instrucciones en el buffer.
            textout_centre_ex(buffer, mifont_30, "Volver", 600, 370, 0x000000, text_mode(-1));
            if (mouse_x >= 510 && mouse_x <= 695 && mouse_y >= 355 && mouse_y <= 510) //Opción "Menu".
            {
                textout_centre_ex(buffer, mifont_30, "Volver", 600, 370, 0xFFFFFF, text_mode(-1));
                if(mouse_b & 1)
                {
                    acerca_de = false;
                    menu = true;
                }
            }
        }
    }
}
END_OF_MAIN ()
