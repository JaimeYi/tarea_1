#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int maximo_de_posiciones_interrumpidas = 64; //usaremos el 64 para que no pueda ocurrir ninguna falta de espacios en el arreglo
const int tamano_tablero = 64;  //variable constante del largo del tablero 8x8
char tablero_completo[tamano_tablero];  //arreglo de tipo char para guardar el tablero completo
int interrupciones[maximo_de_posiciones_interrumpidas]; // arreglo del tipo int para guardar el indice de las posiciones en las cuales se superpone una ficha en la trayectoria de otra ficha

struct Pieza{
    char c; 
    int x,y; 
};

struct Tablero { 
    int cantidad_piezas; 
    Pieza *piezas_tablero; 
};

/*****
 * void tab_completo
 *****
 * Esta función toma un arreglo que tiene un largo igual a la cantidad total de casillas del tablero y guarda cada casilla del tablero en un espacio del arreglo
 *****
 * Input:
 * char* tablero_completo : este parametro es un puntero al arreglo donde se guardan las casillas
 * int tamano_tablero : este parametro nos indica el largo del arreglo
 *****
 * Returns:
 * void, no retorna nada
*****/

void tab_completo(char* tablero_completo, int tamano_tablero){
    fstream archivo;    
    string cantidad_fichas;
    archivo.open("tablero.txt");
    getline(archivo, cantidad_fichas); 
    for (int i = 0; i < tamano_tablero; i++){
        archivo >> tablero_completo[i]; 

    }
    archivo.close(); 
}

/*****
 * int coordenadas_a_indice
 *****
 * Esta función toma las cordenadas (x,y) de una ficha y las convierte en un indice para asi guardarla en la lista de la función tab_completo
 *****
 * Input:
 * int x: coordenada x de la ficha
 * int y: coordenada y de la ficha, notar que ambas coordenadas son con respecto al eje x, eje y tradicional (x : hacia la derecha, y : hacia arriba)
 *****
 * Returns:
 * int, retorna el indice donde ira la ficha en el arreglo de la función tab_completo
*****/

int coordenadas_a_indice(int x, int y) {    //funcion para cambiar las coordenadas (x,y) a el indice del arreglo que contiene el tablero completo
    return (8 - y) * 8 + (x - 1);   // dar vuelta la coordenada y luego calcular el indice
}

//esta funcion la ocupo pa cambiar el . de una casilla vacia en un * para marcar donde se puede mover cada ficha
void mostrar_movimiento(char* tablero, int indice, char mov){
    tablero[indice] = mov;
}

// esta funcion guarda los posibles movimientos de cada ficha usando un switch
//tiene que recibir el tablero completo, no el que tiene solo las fichas, sino el que tiene tambien los espacios vacios
//desde el struct que tiene solo las fichas debe recibir las coordenadas, como lo hago en la parte final del main que ocupe para probar las weas
void movimiento(char* tablero, int x, int y, int* interrupciones, int& indice_interrupciones){
    //variable auxiliar para el peon, guarda el valor de la coordenada x, supongo que leyendo abajo vas a entender que hace
    int aux_p = 0;
    int diag_x = x, diag_y = y;
    //cree estos 2 arreglos con todos los movimientos posibles del rey, si esta en coordenadas (x,y)
    //en cada indice es lo q aumenta o diminuye (x,y). Ej el indice 0 es (0,1), si le sumas eso a (x,y) te queda (x+0, y+1) que es uno de los 8 movimientos posibles
    //y asi con el resto de indices
    int posiciones_k_x[8] = {0,-1,-1,-1,0,1,1,1}; 
    int posiciones_k_y[8] = {1,1,0,-1,-1,-1,0,1};


    //estos 2 arreglos de abajo tienen el mismo funcionamiento que los de arriba pero para el caballo
    //si la wea no te queda clara preguntame no ma, tengo un dibujo culiao q hice mientras programaba esto XDD
    int posiciones_c_x[8] = {2,1,-1,-2,-2,-1,1,2};
    int posiciones_c_y[8] = {1,2,2,1,-1,-2,-2,-1};
    switch (tablero[coordenadas_a_indice(x, y)]){
    case 'P':
        if (y < 8){
            y++;
            if (x < 8 && x != 1){
                aux_p = x - 1;
                x++;
            }else if (x == 1){
                x++;
            }else{
                aux_p = x - 1; 
            }
            int indice = coordenadas_a_indice(x,y);
            if (tablero[indice] == '.'){
                mostrar_movimiento(tablero, indice, '*');  
            } else if ((tablero[indice] == 'X')||(tablero[indice] == 'A')||(tablero[indice] == 'C')||(tablero[indice] == 'T')||(tablero[indice] == 'K')||(tablero[indice] == 'R')||(tablero[indice] == 'P')){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
            }
            int indice_aux = coordenadas_a_indice(aux_p, y);          
            if (tablero[indice_aux] == '.'|| tablero[indice_aux] == '*'){     
                if (aux_p != 0){
                    mostrar_movimiento(tablero, indice_aux, '*');
                } else if ((tablero[indice_aux] == 'X')||(tablero[indice_aux] == 'A')||(tablero[indice_aux] == 'C')||(tablero[indice_aux] == 'T')||(tablero[indice_aux] == 'K')||(tablero[indice_aux] == 'R')||(tablero[indice] == 'P')){
                    interrupciones[indice_interrupciones] = indice_aux;
                    indice_interrupciones++;
                }        
            }
        }
        break;

    case 'K':
        for (int i = 0; i < 8; i++){
            int x_nueva = x + posiciones_k_x[i];
            int y_nueva = y + posiciones_k_y[i];
            if (x_nueva >= 1 && x_nueva <= 8 && y_nueva >= 1 && y_nueva <= 8){
                int indice_nuevo = coordenadas_a_indice(x_nueva,y_nueva);
                if (tablero[indice_nuevo] == '.'|| tablero[indice_nuevo] == '*'){
                    mostrar_movimiento(tablero, indice_nuevo, '*');
                } else if ((tablero[indice_nuevo] == 'X')||(tablero[indice_nuevo] == 'A')||(tablero[indice_nuevo] == 'C')||(tablero[indice_nuevo] == 'T')||(tablero[indice_nuevo] == 'P')||(tablero[indice_nuevo] == 'R')){
                    interrupciones[indice_interrupciones] = indice_nuevo;
                    indice_interrupciones++;
                }
            }
        }
        break;
        
    case 'T':
        int aux_x, aux_y;
        int aux2_y;
        aux2_y = y;
        aux_x = x;
        aux_y = y;
        while (y < 8){
            y++;
            int indice_nuevo = coordenadas_a_indice(x, y);
            if (tablero[indice_nuevo] == '.' || tablero[indice_nuevo] == '*'){
                mostrar_movimiento(tablero, indice_nuevo, '*');
            } else if (tablero[indice_nuevo] == 'X'){
                interrupciones[indice_interrupciones] = indice_nuevo;
                indice_interrupciones++;
            } else if ((tablero[indice_nuevo] == 'A')||(tablero[indice_nuevo] == 'C')||(tablero[indice_nuevo] == 'P')||(tablero[indice_nuevo] == 'K')||(tablero[indice_nuevo] == 'R')||(tablero[indice_nuevo] == 'T')){
                interrupciones[indice_interrupciones] = indice_nuevo;
                indice_interrupciones++;
                break;
            } else {
                break;
            }           
        }
        while (aux_y > 1){
            aux_y--;
            int indice_nuevo = coordenadas_a_indice(x,aux_y);
            if (tablero[indice_nuevo] == '.' || tablero[indice_nuevo] == '*'){
                mostrar_movimiento(tablero, indice_nuevo, '*');
            } else if (tablero[indice_nuevo] == 'X'){
                interrupciones[indice_interrupciones] = indice_nuevo;
                indice_interrupciones++;
            } else if ((tablero[indice_nuevo] == 'A')||(tablero[indice_nuevo] == 'C')||(tablero[indice_nuevo] == 'P')||(tablero[indice_nuevo] == 'K')||(tablero[indice_nuevo] == 'R')||(tablero[indice_nuevo] == 'T')){
                interrupciones[indice_interrupciones] = indice_nuevo;
                indice_interrupciones++;
                break;
            } else {
                break;
            } 
        }
        while (x < 8){
            x++;
            int indice_nuevo = coordenadas_a_indice(x, aux2_y);
            if (tablero[indice_nuevo] == '.' || tablero[indice_nuevo] == '*'){
                mostrar_movimiento(tablero, indice_nuevo, '*');
            } else if (tablero[indice_nuevo] == 'X'){
                interrupciones[indice_interrupciones] = indice_nuevo;
                indice_interrupciones++;
            } else if ((tablero[indice_nuevo] == 'A')||(tablero[indice_nuevo] == 'C')||(tablero[indice_nuevo] == 'P')||(tablero[indice_nuevo] == 'K')||(tablero[indice_nuevo] == 'R')||(tablero[indice_nuevo] == 'T')){
                interrupciones[indice_interrupciones] = indice_nuevo;
                indice_interrupciones++;
                break;
            } else {
                break;
            }            
        }
        while (aux_x > 1){
            aux_x--;
            int indice_nuevo = coordenadas_a_indice(aux_x, aux2_y);
            if (tablero[indice_nuevo] == '.' || tablero[indice_nuevo] == '*'){
                mostrar_movimiento(tablero, indice_nuevo, '*');
            } else if (tablero[indice_nuevo] == 'X'){
                interrupciones[indice_interrupciones] = indice_nuevo;
                indice_interrupciones++;
            } else if ((tablero[indice_nuevo] == 'A')||(tablero[indice_nuevo] == 'C')||(tablero[indice_nuevo] == 'P')||(tablero[indice_nuevo] == 'K')||(tablero[indice_nuevo] == 'R')||(tablero[indice_nuevo] == 'T')){
                interrupciones[indice_interrupciones] = indice_nuevo;
                indice_interrupciones++;
                break;
            } else {
                break;
            }            
        }
        break;
        
    case 'C':
        for (int i = 0; i < 8; i++){
            int x_nueva = x + posiciones_c_x[i];
            int y_nueva = y + posiciones_c_y[i];
            if (x_nueva >= 1 && x_nueva <= 8 && y_nueva >= 1 && y_nueva <= 8){
                int indice_nuevo = coordenadas_a_indice(x_nueva, y_nueva);
                if (tablero[indice_nuevo] == '.'|| tablero[indice_nuevo] == '*'){
                    mostrar_movimiento(tablero, indice_nuevo, '*');
                } else if ((tablero[indice_nuevo] == 'X')||(tablero[indice_nuevo] == 'A')||(tablero[indice_nuevo] == 'T')||(tablero[indice_nuevo] == 'P')||(tablero[indice_nuevo] == 'K')||(tablero[indice_nuevo] == 'R')||(tablero[indice_nuevo] == 'C')){
                    interrupciones[indice_interrupciones] = indice_nuevo;
                    indice_interrupciones++;
                }
            }
        }
        break;

    case 'A':
        while (diag_x > 1 && diag_y < 8){
            --diag_x;
            ++diag_y;
            int indice_mov_diag = coordenadas_a_indice(diag_x, diag_y);
            if ((tablero[indice_mov_diag] == '.' || tablero[indice_mov_diag] == '*')){
                mostrar_movimiento(tablero, indice_mov_diag, '*');
            } else if (tablero[indice_mov_diag] ==  'X'){
                interrupciones[indice_interrupciones] = indice_mov_diag;
                indice_interrupciones++;
            } else if ((tablero[indice_mov_diag] == 'T') || (tablero[indice_mov_diag] == 'C') || (tablero[indice_mov_diag] == 'P') || (tablero[indice_mov_diag] == 'K') || (tablero[indice_mov_diag] == 'R')||(tablero[indice_mov_diag] == 'A')){
                interrupciones[indice_interrupciones] = indice_mov_diag;
                indice_interrupciones++;
                break;
            }else{
                break;
            }
        }

        diag_x = x;
        diag_y = y;
        while (diag_x < 8 && diag_y < 8){
            ++diag_x;
            ++diag_y;
            int indice_mov_diag = coordenadas_a_indice(diag_x, diag_y);
            if ((tablero[indice_mov_diag] == '.' || tablero[indice_mov_diag] == '*')){
                mostrar_movimiento(tablero, indice_mov_diag, '*');
            } else if (tablero[indice_mov_diag] ==  'X'){
                interrupciones[indice_interrupciones] = indice_mov_diag;
                indice_interrupciones++;
            } else if ((tablero[indice_mov_diag] == 'T') || (tablero[indice_mov_diag] == 'C') || (tablero[indice_mov_diag] == 'P') || (tablero[indice_mov_diag] == 'K') || (tablero[indice_mov_diag] == 'R')||(tablero[indice_mov_diag] == 'A')){
                interrupciones[indice_interrupciones] = indice_mov_diag;
                indice_interrupciones++;
                break;
            }else{
                break;
            }
        }

        diag_x = x;
        diag_y = y;
        while (diag_x > 1 && diag_y > 1){
            --diag_x;
            --diag_y;
            int indice_mov_diag = coordenadas_a_indice(diag_x, diag_y);
            if ((tablero[indice_mov_diag] == '.' || tablero[indice_mov_diag] == '*')){
                mostrar_movimiento(tablero, indice_mov_diag, '*');
            } else if (tablero[indice_mov_diag] ==  'X'){
                interrupciones[indice_interrupciones] = indice_mov_diag;
                indice_interrupciones++;
            } else if ((tablero[indice_mov_diag] == 'T') || (tablero[indice_mov_diag] == 'C') || (tablero[indice_mov_diag] == 'P') || (tablero[indice_mov_diag] == 'K') || (tablero[indice_mov_diag] == 'R')||(tablero[indice_mov_diag] == 'A')){
                interrupciones[indice_interrupciones] = indice_mov_diag;
                indice_interrupciones++;
                break;
            }else{
                break;
            }
        }

        diag_x = x;
        diag_y = y;
        while (diag_x < 8 && diag_y > 1){
            ++diag_x;
            --diag_y;
            int indice_mov_diag = coordenadas_a_indice(diag_x, diag_y);
            if ((tablero[indice_mov_diag] == '.' || tablero[indice_mov_diag] == '*')){
                mostrar_movimiento(tablero, indice_mov_diag, '*');
            } else if (tablero[indice_mov_diag] ==  'X'){
                interrupciones[indice_interrupciones] = indice_mov_diag;
                indice_interrupciones++;
            } else if ((tablero[indice_mov_diag] == 'T') || (tablero[indice_mov_diag] == 'C') || (tablero[indice_mov_diag] == 'P') || (tablero[indice_mov_diag] == 'K') || (tablero[indice_mov_diag] == 'R')||(tablero[indice_mov_diag] == 'A')){
                interrupciones[indice_interrupciones] = indice_mov_diag;
                indice_interrupciones++;
                break;
            }else{
                break;
            }
        }
        
        break;

    case 'R': 

        int eje_y, eje_x, eje_neg_y, eje_neg_x;
        eje_y = y, eje_neg_y = y;
        eje_x = x, eje_neg_x = x;
        
        while (eje_x < 8){ // movimiento de la reina hacia la derecha
            eje_x++;
            int indice = coordenadas_a_indice(eje_x, y);
            if (tablero[indice] == '.' || tablero[indice] == '*'){
                mostrar_movimiento(tablero, indice, '*');
            } else if (tablero[indice] == 'X'){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
            } else if ((tablero[indice] == 'A')||(tablero[indice] =='C')||(tablero[indice] =='P')||(tablero[indice] =='K')||(tablero[indice] =='T')||(tablero[indice] == 'R')){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
                break;
            } else {
                break;
            }
        }

        while (eje_neg_x > 1){ // movimiento de la reina hacia la izquierda
            eje_neg_x--;
            int indice = coordenadas_a_indice(eje_neg_x, y);
            if (tablero[indice] == '.' || tablero[indice] == '*'){
                mostrar_movimiento(tablero, indice, '*');
            } else if (tablero[indice] == 'X'){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
            } else if ((tablero[indice] == 'A')||(tablero[indice] =='C')||(tablero[indice] =='P')||(tablero[indice] =='K')||(tablero[indice] =='T')||(tablero[indice] == 'R')){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
                break;
            } else {
                break;
            }
        }

        while (eje_y < 8){ // movimiento de la reina hacia arriba
            eje_y++;
            int indice = coordenadas_a_indice(x, eje_y);
            if (tablero[indice] == '.' || tablero[indice] == '*'){
                mostrar_movimiento(tablero, indice, '*');
            } else if (tablero[indice] == 'X'){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
            } else if ((tablero[indice] == 'A')||(tablero[indice] =='C')||(tablero[indice] =='P')||(tablero[indice] =='K')||(tablero[indice] =='T')||(tablero[indice] == 'R')){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
                break;
            } else {
                break;
            }
        }

        while (eje_neg_y > 1){ // movimiento de la reina hacia abajo
            eje_neg_y--;
            int indice = coordenadas_a_indice(x, eje_neg_y);
            if (tablero[indice] == '.' || tablero[indice] == '*'){
                mostrar_movimiento(tablero, indice, '*');
            } else if (tablero[indice] == 'X'){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
            } else if ((tablero[indice] == 'A')||(tablero[indice] =='C')||(tablero[indice] =='P')||(tablero[indice] =='K')||(tablero[indice] =='T')||(tablero[indice] == 'R')){
                interrupciones[indice_interrupciones] = indice;
                indice_interrupciones++;
                break;
            } else {
                break;
            } 
        }

    // estos son los movimientos diagonales de la reina 
        int var_aux_x, var_aux_y;
        var_aux_x = x;
        var_aux_y = y;
        while (var_aux_x < 8 && var_aux_y < 8){ // movimiento diagonal arriba-derecha
            var_aux_x++;
            var_aux_y++;
            int indice_diag = coordenadas_a_indice(var_aux_x, var_aux_y);
            if (tablero[indice_diag] == '*' || tablero[indice_diag] == '.'){
                mostrar_movimiento(tablero, indice_diag, '*');
            } else if (tablero[indice_diag] ==  'X'){
                interrupciones[indice_interrupciones] = indice_diag;
                indice_interrupciones++;
            } else if ((tablero[indice_diag] == 'T')||(tablero[indice_diag] =='C')||(tablero[indice_diag] =='P')||(tablero[indice_diag] =='K')||(tablero[indice_diag] =='A')||(tablero[indice_diag] =='R')){
                interrupciones[indice_interrupciones] = indice_diag;
                indice_interrupciones++;
                break;
            } else {
                break;
            }
        }

        var_aux_x = x;
        var_aux_y = y;
        while (var_aux_x > 1 && var_aux_y < 8){ // movimiento diagonal arriba-izquierda
            var_aux_x--;
            var_aux_y++;
            int indice_diag = coordenadas_a_indice(var_aux_x, var_aux_y);
            if (tablero[indice_diag] == '*' || tablero[indice_diag] == '.'){
                mostrar_movimiento(tablero, indice_diag, '*');
            } else if (tablero[indice_diag] ==  'X'){
                interrupciones[indice_interrupciones] = indice_diag;
                indice_interrupciones++;
            } else if ((tablero[indice_diag] == 'T')||(tablero[indice_diag] =='C')||(tablero[indice_diag] =='P')||(tablero[indice_diag] =='K')||(tablero[indice_diag] =='A')||(tablero[indice_diag] =='R')){
                interrupciones[indice_interrupciones] = indice_diag;
                indice_interrupciones++;
                break;
            } else {
                break;
            }
        }

        var_aux_x = x;
        var_aux_y = y;
        while (var_aux_x < 8 && var_aux_y > 1){ // movimiento diagonal abajo-derecha
            var_aux_x++;
            var_aux_y--;
            int indice_diag = coordenadas_a_indice(var_aux_x, var_aux_y);
            if (tablero[indice_diag] == '*' || tablero[indice_diag] == '.'){
                mostrar_movimiento(tablero, indice_diag, '*');
            } else if (tablero[indice_diag] ==  'X'){
                interrupciones[indice_interrupciones] = indice_diag;
                indice_interrupciones++;
            } else if ((tablero[indice_diag] == 'T')||(tablero[indice_diag] =='C')||(tablero[indice_diag] =='P')||(tablero[indice_diag] =='K')||(tablero[indice_diag] =='A')||(tablero[indice_diag] =='R')){
                interrupciones[indice_interrupciones] = indice_diag;
                indice_interrupciones++;
                break;
            } else {
                break;
            }
        }

        var_aux_x = x;
        var_aux_y = y;
        while (var_aux_x > 1 && var_aux_y > 1){ // movimiento diagonal abajo-izquierda
            var_aux_x--;
            var_aux_y--;
            int indice_diag = coordenadas_a_indice(var_aux_x, var_aux_y);
            if (tablero[indice_diag] == '*' || tablero[indice_diag] == '.'){
                mostrar_movimiento(tablero, indice_diag, '*');
            } else if (tablero[indice_diag] ==  'X'){
                interrupciones[indice_interrupciones] = indice_diag;
                indice_interrupciones++;
            } else if ((tablero[indice_diag] == 'T')||(tablero[indice_diag] =='C')||(tablero[indice_diag] =='P')||(tablero[indice_diag] =='K')||(tablero[indice_diag] =='A')||(tablero[indice_diag] =='R')){
                interrupciones[indice_interrupciones] = indice_diag;
                indice_interrupciones++;
                break;
            } else {
                break;
            }
        }
        break;
    }
    cout << indice_interrupciones;
    cout << '(';
    for (int h = 0; h < tamano_tablero; h++){
        if (interrupciones[h] != 0){
            cout << interrupciones[h] << ',';
        }
    }
    cout << endl;
}

/*****
 * void esJaque
 *****
 * Esta función busca a la ficha 'X' y si esta se encuentra en la trayectoria de alguna otra ficha de Sebastián se cambia la 'X' por una 'J' para indicar que esta esta en jaque
 * y si alguna ficha está en alguna casilla contigua al 'X' evalua si se encuentra en la trayectoria de otra ficha, de ser asi, la ficha en cuestión se cambia por una 'V' (letra
 * elegida al azar) para asi saber que si la 'X' se come a esa ficha, seguira estando en jaque, por lo que, comerse esa ficha no es una posibilidad para salir del jaque
 *****
 * Input:
 * char* tablero: aqui se usa un puntero al arreglo que contiene el tablero, para asi, iterar sobre los elementos del tablero
 * int* interrupciones: este es un puntero hacia el arreglo 'interrupciones', arreglo en el cual se almacenan los indices de las fichas que se encuentran en la trayectoria de 
 * otras fichas
 *****
 * Returns:
 * void, no retorna nada
*****/

void esJaque(char* tablero, int* interrupciones){

    for (int i = 0; i < tamano_tablero; i++){
        if (tablero[interrupciones[i]] == 'X') {
            tablero[interrupciones[i]] = 'J';
        }
    }
}

void casillasContiguas(char* tablero, int* interrupciones){
    for (int j = 0; j < tamano_tablero; j++){
        if ((tablero[interrupciones[j]] == 'A')||(tablero[interrupciones[j]] == 'T')||(tablero[interrupciones[j]] == 'R')||(tablero[interrupciones[j]] == 'C')||(tablero[interrupciones[j]] == 'P')){
            tablero[interrupciones[j]] = 'V';
        }
    }
}

/*****
 * bool tableroEnJaqueMate
 *****
 * Esta función revisa si el tablero esta en jaque(revisando si el rey es una j o no), de ser asi revisa las casillas contiguas al rey, si no hay ninguna casilla alrededor distinta
 * de '*' o 'V' entonces estara en jaque mate el tablero.
 *****
 * Input:
 * Tablero &tablero : Este argumento es un arreglo de tipo 'Tablero', es usado un argumento de este tipo para poder operar con las posiciones (x,y) del Rey
 *****
 * Returns:
 * bool, retorna si es verdadero que el tablero esta en jaque mate o si es falso
*****/

bool tableroEnJaqueMate(Tablero &tablero){

    int posiciones_ton_x[8] = {0,-1,-1,-1,0,1,1,1}; 
    int posiciones_ton_y[8] = {1,1,0,-1,-1,-1,0,1};
    int aux_x, aux_y;

    for (int i = 0; i < tamano_tablero; i++){
        if (tablero.piezas_tablero[i].c == 'J'){
            for (int j = 0; j < 8; j++){
                aux_x = tablero.piezas_tablero[i].x + posiciones_ton_x[j];
                aux_y = tablero.piezas_tablero[i].y + posiciones_ton_y[j];
                int indice = coordenadas_a_indice(aux_x, aux_y);

                if (aux_x >=1 && aux_x <= 8 && aux_y >= 1 && aux_y <=8 ){
                    if (tablero.piezas_tablero[indice].c == '.'){
                    return false;
                    } else if (tablero.piezas_tablero[indice].c == 'A' || tablero.piezas_tablero[indice].c == 'P' || tablero.piezas_tablero[indice].c == 'T' || tablero.piezas_tablero[indice].c == 'R'|| tablero.piezas_tablero[indice].c == 'C') {
                    return false;
                    }
                }
            }
            return true;
        }
    }
    return false;
}


int main(){
    fstream tablero; // aqui se declara "tablero" como la variable en la que se almacenara el archivo de texto
    int count = 0; // este es un indice que se usara para el arreglo dinamico que esta más adelante
    Tablero tab; // aqui creamos una instancia a partir del Struct Tablero
    Tablero tab_aux;

    tablero.open("tablero.txt"); // aqui se abre el archivo de texto
    
    tablero >> tab.cantidad_piezas; // en esta linea se almacena en la variable cant_fichas el numero de fichas que se indica en la primera linea del archivo de texto
    
    tab.piezas_tablero = new Pieza[tab.cantidad_piezas]; // aqui se crea un arreglo dinamico, para que de esta forma, el arreglo basado en el struct "pieza" tenga el largo unicamente necesario para cada caso
    tab_aux.piezas_tablero = new Pieza[tamano_tablero];
    
    for (int fila = 8; fila > 0; --fila){ // este ciclo "for" se usara para iterar entre cada fila del archivo
        
        string fila_tablero; // este string guardara una fila por cada iteracion del ciclo
        tablero >> fila_tablero; // aqui se guarda la fila en el string
        
        for (int columna = 0; columna < 8; ++columna){ // este ciclo iterara en cada columna de la fila en la que estamos tabajando
            if (fila_tablero[columna] != '.'){ // este condicional if filtra las casillas vacias, marcadas en el documento de texto con un .
                tab.piezas_tablero[count].c = fila_tablero[columna]; // aqui se guarda el tipo de ficha en el struct pieza
                tab.piezas_tablero[count].x = columna + 1; // aqui se guarda la fila donde se encuentra la ficha
                tab.piezas_tablero[count].y = fila; // aqui se guarda la columna donde se encuentra la ficha
                count++; // aqui se aumenta la cuenta para que se avance en el indice del arreglo
            }
        }
    }
    int cuenta_interrupciones = 0;
    tab_completo(tablero_completo, tamano_tablero);
    for (int m = 0; m < tab.cantidad_piezas; ++m){
    movimiento(tablero_completo, tab.piezas_tablero[m].x, tab.piezas_tablero[m].y, interrupciones, cuenta_interrupciones);
    }

    esJaque(tablero_completo, interrupciones);


    int m = 0;
    for (int cuenta = 0; cuenta < tamano_tablero; cuenta++){
        tab_aux.piezas_tablero[cuenta].c = tablero_completo[cuenta];
        if ((tablero_completo[cuenta] == 'A') || (tablero_completo[cuenta] == 'J') || (tablero_completo[cuenta] == 'T') || (tablero_completo[cuenta] == 'R') || (tablero_completo[cuenta] == 'C') || (tablero_completo[cuenta] == 'P') || (tablero_completo[cuenta] == 'K')){
            tab_aux.piezas_tablero[cuenta].x = tab.piezas_tablero[m].x;
            tab_aux.piezas_tablero[cuenta].y = tab.piezas_tablero[m].y;
            m++;
        } else {
            tab_aux.piezas_tablero[cuenta].x = 0;
            tab_aux.piezas_tablero[cuenta].y = 0;
        }
    }

    bool JaqueMate = tableroEnJaqueMate(tab_aux);

    //el codigo de cada abajo es para probar, imprime el tablero completo, despues se puede borrar
    
    for (int i = 0; i < tamano_tablero; ++i) { //wena klo, oye que esta parte muerta nomas
        cout << tab_aux.piezas_tablero[i].c;
        if ((i + 1) % 8 == 0) {
            cout << endl;
        }
    }// hasta aca

    cout << (JaqueMate?"Si":"No") << endl;

    tablero.close(); // aqui cerramos el archivo del tablero luego de terminar de utilizarlo
    delete[] tab.piezas_tablero; // aqui borramos el espacio de memoria que le habiamos asignado al arreglo fichas
    return 0;
}

/*g++ archivo.cpp -o output -Wall*/
/*./output*/