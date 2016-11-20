
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

/*
 * properties del proyecto - build - C compiler - aditional options, añadir: -lncurses
 */
int main(int argc, char** argv) {
    srand(time(NULL));
    initscr();
   //holaMundo();
    mueveHolaAleatorio();
    refresh();
    getch();
    endwin(); //sin esto no sale del terminal, siempre lo último
    return (EXIT_SUCCESS);
}
/* funciones HAY QUE INICIALIZARLAS!! como en mueve Hola
 raw(); = en cuanto alguien pulse una tecla me la pasas y ya veré qué hago, son de inicialización
 keypad(stdscr,TRUE); = habilita las teclas "raras" el ESC, los cursores, las F1, F2...
 halfdelay(x);pide caracteres de manera asíncronas
 la x son los décimas de segundo que espera que metas la tecla (scanf se queda infinito hasta que le das la tecla...
 si le pongo un 0 se queda hasta el infinito
 */

void mueveHolaAleatorio(){
//hasta que metamos la z
    // while((c=getch())!='z'){} == for(c=getch();c!='z';c=getch()){} mejor con WHILE
    raw();
    halfdelay(1);
    keypad(stdscr,TRUE);
    char c; 
    int x,y;
    while((c=getch())!='z'){
        x=aleatorio(1,45);
        y=aleatorio(1,45);
        move(y,x);
        printw ("hola");
     }
}

void holaMundo(){
    move(8,5); //los números son por caracteres, 8 chares a la derecha, 5 abajo, y,x
    printw ("hola");
}

int aleatorio (int inicio, int fin){
    int x;
    x= inicio +(int)((((fin-inicio)*1.0)+1)*(rand()/(RAND_MAX+1.0)));
    return x;
}