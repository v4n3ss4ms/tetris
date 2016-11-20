/*
 * File:   main.c
 * Author: madrid
 *
 * Created on 22 de enero de 2010, 9:58
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
#define ANCHOTAB 13
#define ALTOTAB 23
#define ANCHOFICH 4
#define ALTOFICH 4

typedef struct {
    int y;
    int x;
    int tipo;
}PUNTO;
WINDOW *create_newwin(int height, int width, int starty, int startx,char titulo[]);
void destroy_win(WINDOW *local_win);

int main(int argc, char** argv) {
    int opcion;
    iniciar();
    iniciaColores();
    do{
        opcion=menu();
        switch (opcion)
        {
            case 0:
                break;
            case 1:
                erase();
                jugar();
                break;
            default:
                erase();
                move(3,0);
                printw("opcion no valida\n");
        }
    }while (opcion!=0);
    endwin();
    return (EXIT_SUCCESS);
}

void jugar(){
    noecho();
    int i,c,direccion,limD,limR,limU,limL,alto,ancho,starty1,startx1,puntuacion,fin,queFicha;
    alto=ALTOTAB+1;
    ancho=ANCHOTAB+1;
    fin=starty1=startx1=0;
    limD=alto-2;
    limR=ancho-2;
    limU=limL=1;
    queFicha=aleatorio(1,3);
    int tablero[ALTOTAB][ANCHOTAB];
    int ficha[ALTOFICH][ANCHOFICH];
    iniciaMatriz(tablero);
    //rellenaFicha(ficha,queFicha);
    rellenaFicha(ficha,1);
    puntuacion=0;
    c=KEY_DOWN;
    refresh();
    WINDOW *ventanaJugar;
    WINDOW *ventanaPuntos;
    WINDOW *ventanaSiguiente;
    ventanaJugar=create_newwin(alto,ancho,starty1,startx1,"TETRIS"); //(height, width, starty, startx,titulo)
    ventanaPuntos=create_newwin(5,15,3,50,"PUNTOS");
    ventanaSiguiente=create_newwin(8,15,11,50,"SIGUIENTE");
    halfdelay(3);
    while((c=getch())!='z'){
        attron(COLOR_PAIR(2));
        limpiaAnterior(laPieza);
        //refresh();
        if (c>0){ //he pulsado una tecla
        decideC(c,&direccion);
        }
        fin=decideDireccion(&direccion,&laPieza,limD,limR,limL,tablero,&puntuacion,ventanaSiguiente);
        if (fin==0){
        pintoMatriz(tablero);
        pintoPieza(laPieza);
        pintoPuntos(puntuacion);//hace que me salga una línea negra entre la pieza y los puntos
        refresh();
        }else{
            pintoFin(tablero);
        }
    }
    destroy_win(ventanaJugar);
    destroy_win(ventanaPuntos);
    refresh();
}
// decide la dirección cuando pulsas los cursores
int decideDireccion(int *pDir,PUNTO *miPieza,int down,int right,int left,int pMatriz[ALTOTAB][ANCHOTAB],int *pPuntuacion, WINDOW *miVentana){
    int puedo,muerto;
    puedo=muerto=0;
    switch (*pDir){
    case DOWN:
        puedo=compruebaAbajo(pMatriz,*miPieza);
        muerto=comprueboFin(pMatriz);
        if ((((*miPieza).y+1)<down)&&(puedo==1)){
            (*miPieza).y++;
        }else{
            if (muerto==0){
                switch((*miPieza).tipo){
                    case 1:
                        pMatriz[(*miPieza).y][(*miPieza).x]=1;
                        pMatriz[(*miPieza).y][((*miPieza).x+1)]=1;
                        pMatriz[((*miPieza).y+1)][(*miPieza).x]=1;
                        pMatriz[((*miPieza).y+1)][((*miPieza).x+1)]=1;
                        break;
                    case 2:
                        pMatriz[(*miPieza).y][(*miPieza).x]=1;
                        pMatriz[(*miPieza).y-1][((*miPieza).x)]=1;
                        pMatriz[((*miPieza).y-2)][(*miPieza).x]=1;
                        pMatriz[((*miPieza).y-3)][((*miPieza).x)]=1;
                        break;
                    case 3:
                        pMatriz[(*miPieza).y][(*miPieza).x]=1;
                        pMatriz[(*miPieza).y-1][((*miPieza).x)]=1;
                        pMatriz[((*miPieza).y-1)][(*miPieza).x-1]=1;
                        pMatriz[((*miPieza).y)][((*miPieza).x+1)]=1;
                        break;
                }
                (*miPieza).x=right/2;
                (*miPieza).y=1;
                (*miPieza).tipo=aleatorio(1,3);
                *pPuntuacion+=5;
                pintoSiguiente(*miPieza,miVentana);
            }
        }
        compruebaLineas(pMatriz,pPuntuacion);
        break;
    case LEFT:
        puedo=compruebaIzq(pMatriz,*miPieza);
        muerto=comprueboFin(pMatriz);
        if (muerto==0){
            if (((*miPieza).x>left)&&(puedo==1)){
                (*miPieza).x--;
            }
        }
        *pDir=DOWN;
        break;
    case RIGHT:
        puedo=compruebaDer(pMatriz,*miPieza);
        muerto=comprueboFin(pMatriz);
        if (muerto==0){
            if ((((*miPieza).x+1)<right)&&(puedo==1)){
                (*miPieza).x++;
            }
        }
        *pDir=DOWN;
        break;
    }
    return muerto;
}
void decideC(int c, int *pDir){
switch (c){
            case KEY_DOWN:
                *pDir=DOWN;
                break;
            case KEY_LEFT:
                *pDir=LEFT;
                break;
            case KEY_RIGHT:
                *pDir=RIGHT;
                break;
        }
}
int compruebaAbajo(int Matriz[ALTOTAB][ANCHOTAB],PUNTO miPieza){
    int muevo=0;
    switch (miPieza.tipo)
    {
        case 1:
            if (((Matriz[(miPieza).y+2][((miPieza).x+1)]==0))&&((Matriz[(miPieza).y+2][(miPieza).x]==0))){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
        case 2:
            if ((Matriz[(miPieza).y+1][((miPieza).x)]==0)){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
        case 3:
            if (((Matriz[(miPieza).y+1][((miPieza).x+1)]==0))&&((Matriz[(miPieza).y+1][(miPieza).x]==0))){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
    }
    return muevo;
}
int compruebaIzq(int Matriz[ALTOTAB][ANCHOTAB],PUNTO miPieza){
    int muevo=0;
    switch (miPieza.tipo)
    {
        case 1:
            if (((Matriz[(miPieza).y][(miPieza).x-1]==0))&&((Matriz[((miPieza).y+1)][(miPieza).x-1]==0))){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
        case 2:
            if (((Matriz[(miPieza).y][(miPieza).x-1]==0))&&((Matriz[((miPieza).y-1)][(miPieza).x-1]==0))&&((Matriz[((miPieza).y-2)][(miPieza).x-1]==0))&&((Matriz[((miPieza).y-3)][(miPieza).x-1]==0))){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
        case 3:
            if (((Matriz[(miPieza).y][(miPieza).x-1]==0))&&((Matriz[((miPieza).y+1)][(miPieza).x-1]==0))){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
    }
    return muevo;
}
int compruebaDer(int Matriz[ALTOTAB][ANCHOTAB],PUNTO miPieza){
    int muevo=0;
    switch (miPieza.tipo)
    {
        case 1:
            if (((Matriz[(miPieza).y][(miPieza).x+2]==0))&&((Matriz[((miPieza).y+1)][(miPieza).x+2]==0))){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
        case 2:
            if (((Matriz[(miPieza).y][(miPieza).x+1]==0))&&((Matriz[((miPieza).y-1)][(miPieza).x+1]==0))&&((Matriz[((miPieza).y-2)][(miPieza).x+1]==0))&&((Matriz[((miPieza).y-3)][(miPieza).x+1]==0))){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
        case 3:
            if (((Matriz[(miPieza).y][(miPieza).x+2]==0))&&((Matriz[((miPieza).y+1)][(miPieza).x+2]==0))){
                muevo=1;
            }else{
                muevo=0;
            }
            break;
    }
    return muevo;
}
int comprueboFin(int Matriz[ALTOTAB][ANCHOTAB]){
    int i,tope=0;
    for (i=0;i<ANCHOTAB;i++){
        if (Matriz[1][i] == 1){
            tope=1;
            break;
        }
    }
    return tope;
}
void pintoFin (int pMatriz[ALTOTAB][ANCHOTAB]){
    int i,j;
    for (i=1; i<ALTOTAB;i++){
        for (j=1; j<ANCHOTAB;j++){
            attron(COLOR_PAIR(1));//antes 1
            move(i,j);
            printw ("-");
        }
    }
    move(10,5);
    printw ("FIN");
    move(12,2);
    printw ("HAS MUERTO");
    move(14,2);
    printw ("(pulsa Z)");
}
void compruebaLineas (int Matriz[ALTOTAB][ANCHOTAB],int *pPuntitos){
//x=1-38 //y=1-22
int i,j,completa;
    completa=1;
    for (i=ALTOTAB-1; i>0;i--){
        completa=1;
        for (j=ANCHOTAB-1; j>0;j--){
            if (Matriz[i][j] == 0){
                completa=0;
                break;
            }
        }
        if(completa==1){
            borraLinea(Matriz,i);
            i++;
            *pPuntitos+=30;
        }
    }
}
void borraLinea (int Matriz[ALTOTAB][ANCHOTAB],int linea){
    int i,j;
    for (i=linea; i>0;i--){
        for (j=1; j<ANCHOTAB;j++){
            Matriz[i][j]=Matriz[i-1][j];
        }
    }
    for (j=1; j<ANCHOTAB;j++){
        Matriz[ANCHOTAB-1][j]=0;
    }
}
void pintoMatriz (int pMatriz[ALTOTAB][ANCHOTAB]){
    int i,j;
    for (i=1; i<ALTOTAB;i++){
        for (j=1; j<ANCHOTAB;j++){
            if (pMatriz[i][j]==1){
                attron(COLOR_PAIR(5));//antes 1
                move(i,j);
                printw ("@");
            }else{
                attron(COLOR_PAIR(2));
                move(i,j);
                printw (" ");
            }
        }
    }
}
void iniciaMatriz (int pMatriz[ALTOTAB][ANCHOTAB]){
    int i,j;
    for (i=0; i<ALTOTAB;i++){
        for (j=0; j<ANCHOTAB;j++){
            pMatriz[i][j]=0;
        }
    }
}
void rellenaFicha (int pFicha[ALTOFICH][ANCHOFICH], int tipoFicha){
    int i,j;
    for (i=0; i<ALTOFICH;i++){
        for (j=0; j<ANCHOFICH;j++){
            pFicha[i][j]=0;
        }
    }
    //i=y j=x
    switch (tipoFicha){
        case 1:
            pFicha[2][0]=1;
            pFicha[2][1]=1;
            pFicha[3][0]=1;
            pFicha[3][1]=1;
            break;
        case 2:
            pFicha[0][0]=1;
            pFicha[1][0]=1;
            pFicha[2][0]=1;
            pFicha[3][0]=1;
            break;
        case 3:
            pFicha[2][0]=1;
            pFicha[2][1]=1;
            pFicha[3][1]=1;
            pFicha[3][2]=1;
            break;
    }
}
void pintoPuntos(int score){
    move (5,52);
    attron(COLOR_PAIR(1));
    printw ("%d",score);
}
void pintoSiguiente(PUNTO miPieza, WINDOW *miVentana){//(height, width, starty, startx,titulo)ventanaSiguiente=create_newwin(6,15,11,50,"SIGUIENTE");
    borroSiguiente();
    destroy_win(miVentana);
    miVentana=create_newwin(8,15,11,50,"SIGUIENTE");
    switch (miPieza.tipo)
        {
            case 1:
                attron(COLOR_PAIR(5));
                move (14,57);
                printw ("@");
                move (15,57);
                printw ("@");
                move (14,58);
                printw ("@");
                move (15,58);
                printw ("@");
                break;
            case 2:
                attron(COLOR_PAIR(6));
                move (13,57);
                printw ("@");
                move (14,57);
                printw ("@");
                move (15,57);
                printw ("@");
                move (16,57);
                printw ("@");
                break;
            case 3:
                attron(COLOR_PAIR(7));
                move (14,56);
                printw ("@");
                move (14,57);
                printw ("@");
                move (15,57);
                printw ("@");
                move (15,58);
                printw ("@");
                break;
        }
}
void borroSiguiente(){
    attron(COLOR_PAIR(1));
    move (14,57);
    printw (" ");
    move (15,57);
    printw (" ");
    move (14,58);
    printw (" ");
    move (15,58);
    printw (" ");
    move (13,57);
    printw (" ");
    move (14,57);
    printw (" ");
    move (15,57);
    printw (" ");
    move (16,57);
    printw (" ");
    move (14,56);
    printw (" ");
    move (14,57);
    printw (" ");
    move (15,57);
    printw (" ");
    move (15,58);
    printw (" ");
}
void pintoPieza(PUNTO miPieza){
    switch (miPieza.tipo)
        {
            case 1:
                attron(COLOR_PAIR(5));
                move (miPieza.y,miPieza.x);
                printw ("@");
                move ((miPieza.y+1),miPieza.x);
                printw ("@");
                move (miPieza.y,(miPieza.x+1));
                printw ("@");
                move ((miPieza.y+1),(miPieza.x+1));
                printw ("@");
                break;
            case 2:
                attron(COLOR_PAIR(6));
                move (miPieza.y,miPieza.x);
                printw ("@");
                move ((miPieza.y-1),miPieza.x);
                printw ("@");
                move ((miPieza.y-2),miPieza.x);
                printw ("@");
                move ((miPieza.y-3),miPieza.x);
                printw ("@");
                break;
            case 3:
                attron(COLOR_PAIR(7));
                move (miPieza.y,miPieza.x);
                printw ("@");
                move ((miPieza.y-1),miPieza.x);
                printw ("@");
                move ((miPieza.y-1),(miPieza.x-1));
                printw ("@");
                move ((miPieza.y),(miPieza.x+1));
                printw ("@");
                break;
        }
}
void limpiaAnterior(PUNTO miPieza){
    move (miPieza.y,miPieza.x);
    printw (" ");
    move ((miPieza.y+1),miPieza.x);
    printw (" ");
    move (miPieza.y,(miPieza.x+1));
    printw (" ");
    move ((miPieza.y+1),(miPieza.x+1));
    printw (" ");
}
void iniciar(){
    srand(time(NULL));
    refresh();
    initscr();
    raw();
    cbreak();
    keypad(stdscr,TRUE);
    noecho();
    curs_set(0);//no mostrar el cursor
}
WINDOW *create_newwin(int height, int width, int starty, int startx,char titulo[]){
        WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	//establece el findo por defecto de la ventana
	wbkgd(local_win,COLOR_PAIR(2));
	//cambia el color a la hora de escribir
	wattron(local_win,COLOR_PAIR(4));
	waddstr(local_win,"Menu1");
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
        wprintw(local_win ,titulo);
        wrefresh(local_win);

	return local_win;
}
void destroy_win(WINDOW *local_win){
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}
void iniciaColores(){
   if (has_colors())
        start_color();
    init_pair(1, COLOR_WHITE,COLOR_BLACK);  //blanco sobre negro: menu
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); //pieza1  back de la ventana wbkgd(local_win,COLOR_PAIR(2));
    init_pair(3, COLOR_GREEN, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);//borde ventana wattron(local_win,COLOR_PAIR(4));
    init_pair(5, COLOR_BLACK, COLOR_CYAN);//pieza tipo 1
    init_pair(6, COLOR_BLACK, COLOR_RED);//pieza tipo 2
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA);//pieza tipo 3
    //attron(COLOR_PAIR(1));
}
int menu(){
    erase();
    attron(COLOR_PAIR(1));
    echo();
    curs_set(0);
    int menuNum;
    move(0,0);
    printw("pulsa (1) jugar al TETRIS\n");
    printw("pulsa (0) SALIR\n");
    printw("tu opcion: ");
    scanw("%d",&menuNum);
    return menuNum;
}
int aleatorio (int inicio, int fin){
    int x;
    x= inicio +(int)((((fin-inicio)*1.0)+1)*(rand()/(RAND_MAX+1.0)));
    return x;
}


 /* INFO DE PIEZAS
     * pieza tipo 1
     * ##
     * @#
     * pieza tipo2
     * #
     * #
     * #
     * @
     * pieza tipo 3
     * ##
     *   @#
     */