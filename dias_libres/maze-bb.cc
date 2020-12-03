//Sirvent Navarro, Álvaro 74531046Y
#include<iostream> 
#include<cstdlib> 
#include<fstream> 
#include<stdio.h> 
#include <cstring>
#include<sstream>
#include<math.h>
#include<vector> 
#include<ctime> 
#include<queue>
#include<tuple>
using namespace std;


int length= 0; //Mínimo de pasos dados
struct posicion{ //Nos creamos un struct que utilizaremos para la recursión
    int fila;
    int columna;
    int pasos; 
};
typedef struct posicion pos;
string camino="";
bool arrived = false; 
bool pasaPor= true;

int minimo= 10000000;  

vector<pos> noFactibles; 
vector<pos> explorados; 
bool opP= false; 

struct nodos{
    pos nodo; 
    int pasos;
};

typedef struct nodos miStructDeNodos; 
typedef tuple<int , miStructDeNodos , vector<pos>> tuplaDeNodos; 

struct LessThanByOpt{
    bool operator() ( const tuplaDeNodos& t1 , const tuplaDeNodos& t2 ){
        return get<0>(t1) < get<0>(t2);
    }
};

//Nos definimos la cola de prioridad de la que vamos a ir sacando nodos para expandir ( ES UNA COLA COMPUESTA DE STRUCTS DE FILAS Y COLUMNAS )
priority_queue<tuplaDeNodos, vector<tuplaDeNodos> , LessThanByOpt> miCola; 
vector<pos> resultado; 
//int pasos= 0;  
int best; //Mejor camino encontrado hasta el momento ( En un principio, contendrá el valor de la cota pesimista)

//*********************************************MÉTODO VORAZ PARA CALCULAR LA COTA PESIMISTA*************************************************************
int vorazGreedy( int n , int m , int **matriz , pos& exp , string cam ,int **copia){
    
    if(matriz[0][0]== 0){
        
        pasaPor = false;
        return length+ 0; 
    }
    copia[exp.fila][exp.columna]=-1;
    if(exp.fila == n-1 && exp.columna == m-1  ){
            
        arrived= true;
        stringstream ss;
        ss<<exp.columna;
        string col= ss.str();
        stringstream sp; 
        sp<<exp.fila;
        string fil = sp.str();
        cam = cam + '(' + fil + ',' + col + ')';
        camino= cam;
        length++; 
        exp.fila = 1000000 ; 
        exp.columna= 10000 ;
        return 1;
    }
    stringstream ss;
    ss<<exp.columna;
    string col= ss.str();
    stringstream sp; 
    sp<<exp.fila;
    string fil = sp.str();
    cam = cam + '(' + fil + ',' + col + ')';
    camino= cam;
    
    if(n==1 && m>1){
       
        if(exp.fila<n &&exp.columna+1<m){
            if (matriz[exp.fila][exp.columna+1]==1){
                if(exp.fila==n-1 &&exp.columna+1 ==m-1) arrived= true;
                pos exp2; 
                exp2.fila = exp.fila; 
                exp2.columna = exp.columna + 1; 
                return 1 + vorazGreedy(n , m , matriz , exp2 , cam , copia);
            }
        }else{
           if(exp.fila<n &&exp.columna+1==m){
                arrived=true;
                return 1;
            }else{
                return 0;
            }
        }
    }else if (n>1 &&m==1){
       
        if(exp.fila+1<n &&exp.columna<m){
            
            if(matriz[exp.fila+1][exp.columna]==1){
                if(exp.fila==n-1 &&exp.columna+1 ==m-1) arrived= true;
                
                pos exp2; 
                exp2.fila = exp.fila+ 1; 
                exp2.columna = exp.columna; 
                return 1 + vorazGreedy(n , m , matriz , exp2 , cam , copia);
            }
        }else {
            if(exp.fila+1==n &&exp.columna<m){
                arrived=true;
                return 1;
            }else{
                return 0;
            }
        }
    }
    
    
    if(exp.fila < n && exp.columna<m){ //Miramos a la derecha
       
       
        if(matriz[exp.fila][exp.columna]==1){
            length++;
            pos exp2; 
            exp2.fila = exp.fila+1; 
            exp2.columna = exp.columna + 1; 
            if(exp2.fila<n &&exp2.columna<m){
                if(matriz[exp2.fila][exp2.columna]==1){
                    return 1+ vorazGreedy(n , m , matriz , exp2 , cam, copia);
                }else if(matriz[exp.fila][exp2.columna]==1){
                    
                    exp2.fila= exp.fila; 
                    return 1 + vorazGreedy(n , m , matriz , exp2 , cam, copia); //Llamamos pasándole la posición de la derecha
                
                }else if(matriz[exp2.fila][exp.columna]==1){
                    
                    exp2.columna= exp.columna;
                    return 1 + vorazGreedy( n , m , matriz , exp2,cam , copia); // Llamamos al que mira hacia abajo
                
                }
            
            }else if (exp2.fila<n){
                if(matriz[exp2.fila][exp.columna]==1){
                    exp2.columna= exp.columna;
                    return 1 + vorazGreedy( n , m , matriz , exp2,cam , copia); // Llamamos al que mira hacia abajo
                }
            }else if (exp2.columna<m){
                if(matriz[exp.fila][exp2.columna]==1){
                    
                    exp2.fila= exp.fila; 
                    return 1 + vorazGreedy(n , m , matriz , exp2 , cam,copia); //Llamamos pasándole la posición de la derecha
                }
            }
                
        }
    }else {
        return 1;
    }
   
    
        return 0; 
    

}

//MÉTODO PARA CALCULAR LA COTA OPTIMISTA***************************************************
int cotaOpt(int n , int m){
    return sqrt( (n)*(n) + (m)*(m)); // Devolvemos el cálculo de la diagonal del rectángulo como cota optimista
}

int distanciaEuclidea( int a1 , int a2 , int b1 , int b2){ //Método que me sirve para sacar la distancia existente entre un nodo y el final, y así saber si se tiene que meter en la cola o no
    return sqrt( pow(a1 - b1,2) + pow(a2 - b2,2)); 
}

int distanciaCheb(int a1 , int a2, int b1 , int b2){
    return max(abs(a1-b1) , abs(a2-b2));
}

bool llegado = false;
int added = 0 ;
int explorando=0;  
int nodosExpandidos = 0; 
int noF=0;
int noPrometedores= 0 ; 
int nodosFull= 0; 
int noMejor= 0; 
int siProm = 0; 
 
void miraPosiciones(int n , int m , int **matriz , int n2 , int m2 , int **vectorNodos , int actuales , vector<pos> posiciones){
    explorando++;
    if( n2<n && n2>=0 && m2 <m && m2>=0 &&minimo>actuales && actuales+ distanciaCheb(n2, m2 , n-1 ,m-1) <best && cotaOpt(n2,m2)<=cotaOpt(n-1,m-1) ){
        if( matriz[n2][m2]==1){
            /*pos exp; 
    		exp.fila = n2; 
    		exp.columna = m2;
    		posiciones.push_back( exp); *///Metemos la posiciones actual
            actuales++; //Sumamos +1 al actual 
            if( n2== n-1 && m2 == m-1) { //CUANDO LLEGA AL FINAL
            	pos exp; 
            	exp.fila = n2; 
            	exp.columna = m2; 
            	vector<pos> cam; 
            	cam = posiciones; 
                cam.push_back(exp); 
                vectorNodos[n2][m2] = actuales; 
                minimo = actuales; 
                resultado= cam; //Pasamos el camino
                best = actuales; //CAMBIAMOS EL MEJOR ENCONTRADO (*****COTA PESIMISTA)
                nodosFull++;

                
            }
            else if(vectorNodos[n2][m2] > actuales && minimo> actuales){
                vectorNodos[n2][m2] = actuales;
                added++;
                miStructDeNodos exp2; 
                exp2.nodo.fila = n2; 
                exp2.nodo.columna = m2;
                exp2.pasos = actuales; 
                int res = distanciaCheb(n2, m2 , n-1 ,m-1);
                vector<pos> cam; 
                pos exp3; 
                exp3.fila = n2 ;
                exp3.columna= m2; 
                cam = posiciones; 
                cam.push_back(exp3); 
                miCola.push(tuplaDeNodos( res,exp2, cam));
                pos exp; 
                exp.fila = n2; 
                exp.columna = m2; 
                exp.pasos = actuales; 
                explorados.push_back(exp);
                //posiciones.push_back(exp);
        
            }
        }
    }else{
    	if(actuales + distanciaCheb(n2,m2,n-1,m-1) >=best){
            noF++;
        }
        if( actuales + cotaOpt(n2,m2)>cotaOpt(n-1,m-1)){
            noPrometedores++;
        }
        if(n2 == n-1 && m2 == m-1){
        	noMejor++;
        }
        if( actuales + cotaOpt(n2,m2)<cotaOpt(n-1,m-1)){
        	siProm++;
        }


    }

    
}

void posicionesIda( int n , int m , int  n2 , int m2 , int **matriz , int **vectorNodos , vector<pos> posiciones , int actuales){ //Método para meter en la cola todos los nodos que sean viables de expandir ( mejores o iguales a la pesimista) y expandir la mejor de ellas
    //MIRAMOS TODOS LOS NODOS POSIBLES DESDE EL NODO EN EL QUE ESTAMOS

    miraPosiciones( n , m , matriz , n2+1 , m2+1 , vectorNodos , actuales , posiciones); //DIAGONAL
    miraPosiciones( n , m , matriz ,n2,m2+1, vectorNodos , actuales , posiciones);
    miraPosiciones( n , m , matriz ,n2+1,m2 , vectorNodos , actuales , posiciones);
    miraPosiciones( n , m , matriz ,n2+1,m2 -1 , vectorNodos , actuales , posiciones);
    miraPosiciones( n , m , matriz ,n2-1,m2 -1 , vectorNodos , actuales, posiciones);
    miraPosiciones( n , m , matriz , n2-1,m2 ,  vectorNodos , actuales ,posiciones);
    miraPosiciones( n , m , matriz , n2,m2-1, vectorNodos , actuales, posiciones);
    miraPosiciones( n , m , matriz ,n2-1,m2+1,  vectorNodos , actuales , posiciones);

}

///*********MÉTODO ITERATIVO PARA CALCULAR EL RESULTADO( RAMIFICACIÓN Y PODA)*********************************************
int mazeBB( int n , int m , int **matriz , int **vectorNodos , vector<pos> posiciones ) {
   //Lo primero que hacemos es ver la cota optimista
    //int optimo = cotaOpt( n- 1 , m-1); //SACAMOS LO QUE SERÍA LA COTA OPTIMISTA
    //Metemos en la cola de prioridad el primero que tenemos
    explorando++;
    if( n== 1 && m == 1 ){
        if ( matriz[0][0] == 1){
            added = 1; 
            minimo = 0; 
            return 1;
        }else{
            added = 0;
            
            return 0; 
        } 
    }

    
    int counter = -1; 
    miStructDeNodos exp1; 
    exp1.nodo.fila = 0 ; 
    exp1.nodo.columna = 0 ;
    exp1.pasos = 0;  
    int actuales = 0; 
    vector<pos> camino; 
    miCola.push(tuplaDeNodos(0 , exp1 , camino)); //Metemos la posición (0, 0) en la cola de prioridad, que será la primera en expandir
    int filaAct= 0;  //Filas y columnas actuales 
    int columAct =0 ; 
    while(  !miCola.empty()){//Hacemos un bucle que mirará por la fila y la columna actual del nodo que estamos expandiendo
        posicionesIda(n , m , filaAct , columAct , matriz , vectorNodos , camino , actuales );
        miStructDeNodos exp2; 
        tuplaDeNodos nd = miCola.top();//MIramos en el nodo que estamos expandiendo todas las posiciones a las que se puede ir, con el fin de saber si expandirlas o no.
        miCola.pop();
        exp2 = get<1>(nd);
        actuales = exp2.pasos; 
        filaAct = exp2.nodo.fila;
        columAct = exp2.nodo.columna;
        counter++; //Nodos explorados
    }
    return counter; //DEVOLVEMOS EL NÚMERO DE NODOS EXPLORADOS

}

void genMat(int n , int m , string nombre ){ //Generador de la matríz
    ifstream archivo; 
   
    archivo.open(nombre.c_str()); 
    string siguientes; 
    string valores;
    int filas= 0;
    bool primera = false ; 
    int **matriz = new int *[n];
    for(int i= 0 ; i<n ; i++) matriz[i] = new int [m]; 
    int **copia= new int *[n];
    for(int i= 0 ; i<n ; i++) copia[i] = new int [m]; 
    int **vectorNodos = new int *[n]; 
    for( int i = 0 ; i<n ; i++) vectorNodos[i] = new int [m];
    for( int i = 0 ; i<n ; i++) {
        for( int j = 0 ; j<m ; j++) {
            vectorNodos[i][j]= 1000000; 
        }
    }
    if ( archivo.is_open() ){ 
        while(! archivo.eof()){
            if (primera == false ){
                getline(archivo, valores); //BASURA
            
                primera = true; 
            }else{
                getline(archivo, siguientes);
                istringstream iss(siguientes);
                
                for(int i = 0 ; i<m && filas<n ; i++){
                    iss>>matriz[filas][i];
                   
                }
                filas++; 
                siguientes.clear();
                    
            }
        }
    }else{
        cout<<"ERROR";
        exit(1);
    }
    for(int i = 0 ; i<n; i++){
        for(int j =0 ; j<m ; j++){
            copia[i][j]=matriz[i][j];
        }
    }
   
    //int counter =0; 
    int min= minimo; 
    vector<pos> posiciones; 
    time_t s0= clock(); 
    //COGEMOS LA COTA PESIMISTA 
    string cam= ""; 
     pos exp2; 
    exp2.fila= 0; 
    exp2.columna= 0;  
    best = vorazGreedy(n , m , matriz , exp2 , cam,copia );
    bool sol = true; 
    //Miramos si ha conseguido llegar hasta el final 
    if(arrived == false &&pasaPor == false ){
        best = n*m; // Ponemos como cota pesimista como si la matriz fueran todo 1s
        sol = false; 
    }else if(arrived == false && pasaPor ==true){
        best =n*m;//Ponemos como cota pesimista como si la matríz fueran todo 1s
        sol= false;
    }
    //Una vez cogida la cota pesimista, nos pasamos a la llamada del algoritmo de poda
    int nodosExpandidos = mazeBB(n , m , matriz , vectorNodos , posiciones );
    //minimo = pasos;
    time_t st = clock(); 
    if( minimo!= min) {  
        cout<<"Shortest path length= "<< minimo+1<<endl;
        cout<<"Explored nodes= "<<added+noPrometedores+noF<<" (Added= "<<added<<"; nonpromising= "<<noPrometedores<<"; nonfactible= "<<noF<<")"<<endl;
        cout<<"Expanded nodes= "<<nodosExpandidos<<endl;
        cout<<"Completed nodes= "<<noMejor + nodosFull<<" (Best solution updated= " <<nodosFull<<")"<<endl;
        cout<<"Promising but dicarded nodes= "<<siProm<<endl;
        cout<<"Best solution updated from a pessimistic bound= "<<sol<<endl;
        cout<<"CPU elapsed time= " << (double (st - s0)/CLOCKS_PER_SEC)*1000 <<" ms."<<endl;
        if( opP==true) {
            cout<<"Path 2D:"<<endl;
            for( unsigned int i =0 ; i< resultado.size(); i++) {
                copia[resultado[i].fila][resultado[i].columna]= -1;
            }
            for( int i =0 ; i< n; i++) {
                for( int j =0 ; j < m ; j++) {
                    if( copia[i][j] == -1 ) cout<<"*"; 
                        else cout<<copia[i][j];
                }
                cout<<endl; 
            }
        }
        
    }else{
    	cout<<"NO EXIT"<<endl;
    	cout<<"Explored nodes= "<<added+noPrometedores+noF<<" (Added= "<<added<<"; nonpromising= "<<noPrometedores<<"; nonfactible= "<<noF<<")"<<endl;
        cout<<"Expanded nodes= "<<nodosExpandidos<<endl;
        cout<<"Completed nodes= "<<noMejor + nodosFull<<" (Best solution updated= " <<nodosFull<<")"<<endl;
        cout<<"Promising but dicarded nodes= "<<siProm<<endl;
        cout<<"Best solution updated from a pessimistic bound= "<<sol<<endl;
        cout<<"CPU elapsed time= " << (double (st - s0)/CLOCKS_PER_SEC)*1000 <<" ms."<<endl;
    } 
    
    
    
}

//Una vez que hemos leído del fichero, paamos a hacer los diferenetes métodos para resolver el ejercicio
bool lecturaFech (int argc , char * argv []) {
    bool encontrado = false; 
    int n; //filas
    int m; //columnas
    for(int i = 0 ; i < argc && encontrado == false ; i++){
        
        string cad = argv[i];
        if(  cad ==  "-f" ){
            ifstream archivo; 
            string nombre = argv[i+1];
            archivo.open(nombre.c_str()); 
            string siguientes; 
            string valores; 
            bool primera = false ; 
            if ( archivo.is_open() ){ 
                while(! archivo.eof()){
                    
                    if (primera == false ){
                        //archivo>>valores;
                        getline(archivo, valores); 
                        istringstream iss (valores);
                        for(int i = 0 ; i<2 ; i++){
                            if(i==0)
                               
                                iss>>n; //FIlas 
                            if(i==1)
                                
                                iss>>m;//COlumnas 
                        }
                        primera=true;
                   
                    }
                    getline(archivo, valores); //BASURA
                }
            genMat(n , m, nombre);
            encontrado= true;
            }else{
                encontrado = true; 
                cout<<"ERROR: can't open file: "<<nombre<<"."<<endl;
                cout<<"Usage: "<<endl;
                cout<<"maze [-p] [-t] [--ignore-recursive] -f file" <<endl; 
                exit(1); 
            }
            archivo.close(); // cerramos el archivo que hemos abierto para su lectura
        }
    }
   
    return encontrado; 
}

int main(int argc , char *argv [] ){
    if( argc < 1 ) 
        exit(1);
    bool encontrado= false; 
    
    
    for(int i = 1; i< argc ; i++){
        
        string cad = argv[i];
        if(cad=="-f"){
            i++; 
        }
        if( cad=="-p"){
            opP= true; 
        }
        if(cad!="-f" && cad!="-p"){
            cout<<"ERROR: unknown option "<<cad<<"."<<endl;
            cout<<"Usage: "<<endl;
            cout<<"maze-greedy -f file -p" <<endl; 
            exit(1);
        }
    }
    if(encontrado==false){
        // Hacemos la lectura del fichero para guardarme los datos de la matriz en un array
        if ( lecturaFech(argc, argv)==false ){
            cout<<"ERROR: can't find -f"<<endl; 
            cout<<"Usage: "<<endl;
            cout<<"maze-greedy -f file" <<endl; 
            exit(1);
        }
    }
   
    /*if(opP== true){
        cout<<endl<<"A possible path:"<<endl;
        if(length!= 0 ){
            //cout<<camino<<endl;
        }else{
            cout<<"BLOCKED"<<endl;
        }
    }*/
    
}