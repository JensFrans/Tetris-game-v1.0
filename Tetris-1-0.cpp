#include <cstdlib>
#include <ctime>
#include "miniwin.h"
using namespace miniwin;
//constantes
const int TAM = 25; //constante
const int FILAS = 20; //filas de le tablero
const int COLUMNAS = 10; //colummas de el tablero
// "struct" de Coordenadas de las figuras
struct Coord{ int x,y; };
//sub division filas y columnas
typedef int Tablero[FILAS][COLUMNAS];

struct Pieza{
    Coord orig; // Posicion inicial fija
    Coord perif[3]; // Bloques perifericos (posicion relativa)
    int color;

    Coord posicion(int n) const; // un "N" entre 0  y 3 ( 0 = P. central, 1-3 Perif."perifericos")
};

Coord Pieza::posicion(int n) const {
    Coord ret = { orig.x, orig.y};
    if (n != 0) {
        ret.x +=  perif[n-1].x;
        ret.y +=  perif[n-1].y;
    }
    return ret;
}

void cuadrado(int x, int y){
    rectangulo_lleno(1 + x * TAM,
                     1 + y * TAM,
                     x * TAM + TAM,
                     y * TAM + TAM);

}
//Pintar una pieza
void pinta_pieza(const Pieza& P){
    color(P.color);
    for (int i = 0; i < 4; i++){ // P. Perifericos 1,2,3
        Coord c = P.posicion(i);
        cuadrado(c.x, c.y);
    }
}
//rotacion de la figura  a la derecha
Coord rota_derecha(const Coord& c){
    Coord ret = { -c.y, c.x};
    return ret;
}
//Rotacion de la figura a la izquierda
Coord rota_izquierda(const Coord& c){ //sin uso !
    Coord ret = { c.y, -c.x};
    return ret;
}
//rotacion de la figura  a la derecha
void rota_derecha(Pieza& P){
    for (int i = 0; i < 3; i++){
        P.perif[i] = rota_derecha(P.perif[i]);
    }
}
//Rotacion de la figura a la izquierda
void rota_izquierda(Pieza& P){  //sin uso !
    for (int i = 0; i < 3; i++){
        P.perif[i] = rota_izquierda(P.perif[i]);
    }
}
//Deteccion de si esta o no el tablero vacio
void tablero_vacia(Tablero& T){
    for (int i = 0; i < COLUMNAS; i++){
        for (int j = 0; j < FILAS; j++){
            T[i][j] = NEGRO; //casillas vacias de el tablero
        }
    }
}
//sentecnia para pintar el tablero
void tablero_pinta(const Tablero& T){
    for (int i = 0; i < COLUMNAS; i++){
        for (int j = 0; j < FILAS; j++){
             color(T[i][j]); //pinta las casillas de le tablero
             cuadrado(i, j);
        }
    }
}
//Setencia para incrustar la figura en el tablero
void tablero_incrusta_pieza(Tablero& T, const Pieza& P){
    for (int i = 0; i < 4; i++){
        Coord c = P.posicion(i);
        T[c.x][c.y] = P.color;
    }
}
//Deteccion de colision en el taablero y figuras
bool tablero_colision(const Tablero& T, const Pieza& P){
    for (int i = 0; i < 4 ; i++){
        Coord c = P.posicion(i);
        // Comprobar limetes -->
        if (c.x < 0 || c.x >= COLUMNAS) {
            return true;
        }
        if (c.y < 0 || c.y >= FILAS){
            return true;
            }
        //Comprobar restos en el tablero --->
        if (T[c.x][c.y] != NEGRO){
            return true;
        }
    }
    return false;
}
//piezas --->Figuras*6
const Coord perifs[6][3] = {
    { {1,0}, {0,1}, {1,1} },//cuadrado
    { {1,0}, {-1,1}, {0,1} },// S
    { {0,1}, {1,1}, {-1,0} },// 2
    { {0,-1}, {0,1}, {-1,1} },// l
    { {0,-1}, {0,1}, {-1,1} },//LR
    { {0,-1}, {0,1}, {0,2} },// Barra
};
//generar pieza nueva
void pieza_nueva(Pieza& P){
    P.orig.x = 5;
    P.orig.y = 3;
    P.color = 1 + rand() % 6; // 1 ++ --> RAND_MAX color
    //pieza random*1
    int r = rand()% 6;
    for (int i = 0; i < 3; i++){
        P.perif[i] = perifs[r][i];
    }
}

bool tablero_fila_llena(const Tablero& T, int fila){
    for (int i = 0; i < COLUMNAS; i++){
        if (T[i][fila] == NEGRO) return false;
    }
/

void tablero_colapsa(Tablero& T, int fila){
    for (int j = 0; j > 0; j--){
        for (int i = 0; i < COLUMNAS; i++){
            T[i][fila] = T[i][fila-1];
        }
    }
    //Vaciar hacia arriba--->
    for (int i = 0; i < COLUMNAS; i++){
        T[i][0] = NEGRO;
    }
}

void tablero_cuenta_lineas(Tablero& T){
    int fila = FILAS -1, cont = 0;
    while (fila >= 0){
    if (tablero_fila_llena(T, fila)){
    tablero_colapsa(T, fila);
    cont++;
    } else {
        fila--;
     }
    }
    return cont;
/

int main(){
    //dimensiones de la ventana
     vredimensiona(TAM * COLUMNAS, TAM * FILAS);
     srand(time(0));// Semilla default ---> Rand_Max

     Tablero T;
     tablero_vacia(T);
     tablero_pinta(T);

     Pieza c;
     pieza_nueva(c);
     pinta_pieza(c);

     refresca();
//Deteccion de las teclas de accion
    int t = tecla();

    while (t != ESCAPE){
            // obtener posicion actual
            Pieza copia = c;

            //mover en el tablero
        if (t == ABAJO){
        c.orig.y++;
        } else if (t == ARRIBA){
        rota_derecha(c);
        } else if (t == DERECHA){
        c.orig.x++;
        } else if (t == IZQUIERDA){
        c.orig.x--;
        }

        //comprobar colision
        if (tablero_colision(T, c)){
            c = copia;
        }

        if (t == ESPACIO){
            tablero_incrusta_pieza(T, c);
            int cont = ??? //Aqui sigue el codigo
            //int cont = tablero_cuenta_lineas(T);

            pieza_nueva(c);
        }

        //pintar
        if (t != NINGUNA){
        borra();
        tablero_pinta(T);
        pinta_pieza(c);
        refresca();
        }

    t = tecla();
    }
    vcierra();//cierre
    //Fin de el Programa ///JeanFranco Parra, 5|to informatica
    return 0;
}

