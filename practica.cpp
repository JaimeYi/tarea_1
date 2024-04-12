#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Pieza{ // este struct definira el tipo y la posicion de la pieza en cuestion
    char c; // aqui se almacenara el tipo de ficha
    int x,y; // aqui se almacenara la fila y la columna en la que se encuentra la ficha
};

struct Tablero { // este Struct definira la cantidad de piezas en el tablero y tendra un puntero a un arreglo dinamico creado en el codigo
    int cantidad_piezas; // aqui se guardaran cuantas fichas tiene el tablero
    Pieza *piezas_tablero; // este es el puntero que ira dirigido al arreglo dinamico
};



const int tamano_tablero = 64;  //variable constante del largo del tablero 8x8

char tablero_completo[tamano_tablero];  //arreglo de tipo char para guardar el tablero completo

void tab_completo(char* tablero_completo, int tamano_tablero){  //variable tipo void para guardar el tablero completo en un arreglo
    fstream archivo;    
    string cantidad_fichas; //string que contiene la primera linea, la cual aca no vamos a ocupar
    archivo.open("tablero.txt");    //se abre el archivo
    getline(archivo, cantidad_fichas);  //se separa la primera linea
    for (int i = 0; i < tamano_tablero; i++){
        archivo >> tablero_completo[i]; //se empieza a leer el tablero completo saltando la linea con la cantidad de fichas

    }
    archivo.close();    //se cierra el archivo
}

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
void movimiento(char* tablero, int x, int y){
    //variable auxiliar para el peon, guarda el valor de la coordenada x, supongo que leyendo abajo vas a entender que hace
    int aux_p = 0;
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
            if (tablero[coordenadas_a_indice(x,y)] == '.'){
                mostrar_movimiento(tablero, coordenadas_a_indice(x, y), '*');  
                }         
            if (tablero[coordenadas_a_indice(aux_p, y)] == '.'|| tablero[coordenadas_a_indice(aux_p, y)] == '*'){     
                if (aux_p != 0){
                    mostrar_movimiento(tablero, coordenadas_a_indice(aux_p, y), '*');
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
            }else{
                break;
            }           
        }
        while (aux_y > 1){
            aux_y--;
            int indice_nuevo = coordenadas_a_indice(x,aux_y);
            if (tablero[indice_nuevo] == '.' || tablero[indice_nuevo] == '*'){
                mostrar_movimiento(tablero, indice_nuevo, '*');
            }else{
                break;
            } 
        }
        while (x < 8){
            x++;
            int indice_nuevo = coordenadas_a_indice(x, aux2_y);
            if (tablero[indice_nuevo] == '.' || tablero[indice_nuevo] == '*'){
                mostrar_movimiento(tablero, indice_nuevo, '*');
            }else{
                break;
            }            
        }
        while (aux_x > 1){
            aux_x--;
            int indice_nuevo = coordenadas_a_indice(aux_x, aux2_y);
            if (tablero[indice_nuevo] == '.' || tablero[indice_nuevo] == '*'){
                mostrar_movimiento(tablero, indice_nuevo, '*');
            }else{
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
                }
            }
        }
        break;

    //Alfil listo
    case 'A':
        bool diag1 = true;
        int diag_x = x, diag_y = y;
        while (diag1){
            --diag_x;
            ++diag_y;
            if ((tablero[coordenadas_a_indice(diag_x,diag_y)] == '.' || tablero[coordenadas_a_indice(diag_x,diag_y)] == '*') && (diag_x < 8 && diag_x > 0) && (diag_y < 8 && diag_y > 0)){
                mostrar_movimiento(tablero, coordenadas_a_indice(diag_x,diag_y), '*');
            } else if(tablero[coordenadas_a_indice(diag_x,diag_y)] == 'K' || 'C' || 'T' || 'R' || 'X' || 'P'){ // esto es para que no marque más casillas si se cruza un wn
                diag1 = false;
            }else{
                diag1 = false;
            }
        }

        bool diag2 = true;
        diag_x = x;
        diag_y = y;
        while (diag2){
            ++diag_x;
            ++diag_y;
            if ((tablero[coordenadas_a_indice(diag_x,diag_y)] == '.' || tablero[coordenadas_a_indice(diag_x,diag_y)] == '*') && (diag_x < 8 && diag_x > 0) && (diag_y < 8 && diag_y > 0)){
                mostrar_movimiento(tablero, coordenadas_a_indice(diag_x,diag_y), '*');
            } else if(tablero[coordenadas_a_indice(diag_x,diag_y)] == 'K' || 'C' || 'T' || 'R' || 'X' || 'P'){
                diag2 = false;
            }else{
                diag2 = false;
            }
        }

        bool diag3 = true;
        diag_x = x;
        diag_y = y;
        while (diag3){
            --diag_x;
            --diag_y;
            if ((tablero[coordenadas_a_indice(diag_x,diag_y)] == '.' || tablero[coordenadas_a_indice(diag_x,diag_y)] == '*') && (diag_x < 8 && diag_x > 0) && (diag_y < 8 && diag_y > 0)){
                mostrar_movimiento(tablero, coordenadas_a_indice(diag_x,diag_y), '*');
            } else if(tablero[coordenadas_a_indice(diag_x,diag_y)] == 'K' || 'C' || 'T' || 'R' || 'X' || 'P'){
                diag3 = false;
            }else{
                diag3 = false;
            }
        }

        bool diag4 = true;
        diag_x = x;
        diag_y = y;
        while (diag4){
            ++diag_x;
            --diag_y;
            if ((tablero[coordenadas_a_indice(diag_x,diag_y)] == '.' || tablero[coordenadas_a_indice(diag_x,diag_y)] == '*') && (diag_x < 8 && diag_x > 0) && (diag_y < 8 && diag_y > 0)){
                mostrar_movimiento(tablero, coordenadas_a_indice(diag_x,diag_y), '*');
            } else if(tablero[coordenadas_a_indice(diag_x,diag_y)] == 'K' || 'C' || 'T' || 'R' || 'X' || 'P'){
                diag4 = false;
            }else{
                diag4 = false;
            }
        }
        
        break;
    // Este ctm esta en desarrollo pero más o menos la idea esta
    /*
    case 'R':
        int eje_y = y, eje_x = x;
        int eje_neg_y = y, eje_neg_x = x;
        while (eje_x < 8) {
            ++eje_x;
            if (tablero[coordenadas_a_indice(eje_x, y)] == 'A'){
                eje_x = 8;
            } else {
                if (eje_x < 8 && (tablero[coordenadas_a_indice(eje_x, y)] == '.' || tablero[coordenadas_a_indice(eje_x, y)] == '*')){
                    mostrar_movimiento(tablero, coordenadas_a_indice(eje_x, y), '*');
                }
            }
        }
        while (eje_y < 8){
            ++eje_y;
            if (tablero[coordenadas_a_indice(x, eje_y)] == 'A'){
                eje_y = 8;
            } else {
                if (eje_y < 8 && (tablero[coordenadas_a_indice(x, eje_y)] == '.' || tablero[coordenadas_a_indice(x, eje_y)] == '*')){
                    mostrar_movimiento(tablero, coordenadas_a_indice(x, eje_y), '*');
                }
            }
        }
        while (eje_neg_x > 0){
            --eje_neg_x;
            if (tablero[coordenadas_a_indice(eje_neg_x, y)] == 'A'){
                eje_neg_x = 0;
            } else {
                if (eje_neg_x > 0 && (tablero[coordenadas_a_indice(eje_neg_x,y)] == '.' || tablero[coordenadas_a_indice(eje_neg_x, y)] == '*')){
                    mostrar_movimiento(tablero, coordenadas_a_indice(eje_neg_x, y), '*');
                }
            }
        }
        while (eje_neg_y > 0){
            --eje_neg_y;
            if (tablero[coordenadas_a_indice(x, eje_neg_y)] == 'A'){
                eje_neg_y = 0;
            } else {
                if (eje_neg_y > 0 && (tablero[coordenadas_a_indice(x,eje_neg_y)] == '.' || tablero[coordenadas_a_indice(x, eje_neg_y)] == '*')){
                    mostrar_movimiento(tablero, coordenadas_a_indice(x, eje_neg_y), '*');
                }
            }
        }
        break;
        */
    }
}


//faltaria esta funcion que es la importante al final
//la forma en que lo pense fue que con la funcion de arriba marcara todas las casillas donde las piezas se pueden mover menos la X
//despues en la funcion que viene aca abajo revisar todas las casillas alrededor de X y si alguna no es un * es que no esta en jaquemate
//ya que se puede mover hacia álla, en cambio si todas sus casillas alrededor son un * si se encuentra en jaquemate
//segun yo esa wea deberia funcionar, pero no se, no caxo mucho de ajedrez
//la funcion de abajo no deberia ser dificil, segun yo deberia ser algo parecido a lo que hice para marcar los posibles movimientos del rey
//pero revisando las casillas alrededor de la X, retornando true si esta en jaque y false si no

/*bool tableroEnJaqueMate(Tablero &tablero){
    
}*/

int main(){
    fstream tablero; // aqui se declara "tablero" como la variable en la que se almacenara el archivo de texto
    int count = 0; // este es un indice que se usara para el arreglo dinamico que esta más adelante
    Tablero tab; // aqui creamos una instancia a partir del Struct Tablero

    tablero.open("tablero.txt"); // aqui se abre el archivo de texto
    
    tablero >> tab.cantidad_piezas; // en esta linea se almacena en la variable cant_fichas el numero de fichas que se indica en la primera linea del archivo de texto
    
    tab.piezas_tablero = new Pieza[tab.cantidad_piezas]; // aqui se crea un arreglo dinamico, para que de esta forma, el arreglo basado en el struct "pieza" tenga el largo unicamente necesario para cada caso
    
    for (int fila = 8; fila > 0; --fila){ // este ciclo "for" se usara para iterar entre cada fila del archivo
        
        string fila_tablero; // este string guardara una fila por cada iteracion del ciclo
        tablero >> fila_tablero; // aqui se guarda la fila en el string
        
        for (int columna = 0; columna < 8; ++columna){ // este ciclo iterara en cada columna de la fila en la que estamos tabajando
            
            if (fila_tablero[columna] != '.'){ // este condicional if filtra las casillas vacias, marcadas en el documento de texto con un .
                tab.piezas_tablero[count].c = fila_tablero[columna]; // aqui se guarda el tipo de ficha en el struct pieza
                tab.piezas_tablero[count].x = columna + 1; // aqui se guarda la fila donde se encuentra la ficha
                tab.piezas_tablero[count].y = fila ; // aqui se guarda la columna donde se encuentra la ficha
                count++; // aqui se aumenta la cuenta para que se avance en el indice del arreglo
            }
        }
    }
    tablero.close(); // aqui cerramos el archivo del tablero luego de terminar de utilizarlo
    /*
    for (int i = 0; i < tab.cantidad_piezas; ++i){ // este ciclo se usara para iterar en los distintos elementos presentes en el arreglo
        cout << "Coordenadas " << tab.piezas_tablero[i].c << " : (" << tab.piezas_tablero[i].x << "," << tab.piezas_tablero[i].y << ")" << endl; // aqui se imprime el tipo de ficha y la coordenada donde esta está ubicada
    }

    */
    tab_completo(tablero_completo, tamano_tablero);
    for (int m = 0; m < tab.cantidad_piezas; ++m){
    movimiento(tablero_completo, tab.piezas_tablero[m].x, tab.piezas_tablero[m].y);
    }

   //esto de aca tambien despues se puede borrar, lo use para probar las fichas



    //el codigo de cada abajo es para probar, imprime el tablero completo, despues se puede borrar
    for (int i = 0; i < tamano_tablero; ++i) {
        cout << tablero_completo[i];
        if ((i + 1) % 8 == 0) {
            cout << endl;
        }
    }

    delete[] tab.piezas_tablero; // aqui borramos el espacio de memoria que le habiamos asignado al arreglo fichas
    return 0;
}

/*g++ archivo.cpp -o output -Wall*/
/*./output*/