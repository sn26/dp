package AA;

import java.util.*;


class Nodes{
	
	public Integer sup;
	public Integer prod; 
	public Integer pasos;
	
	public Nodes(Integer sup, Integer producto, Integer pasos) {
		this.sup = sup; 
		this.prod = producto; 
		this.pasos = pasos; 
	}
}


class Triplet{
	
	public Integer value; 
	public Nodes node; 
	public ArrayList<Integer>pos; 
	
	public Triplet(Integer value, Nodes node, ArrayList<Integer> pos) {
		this.value = value; 
		this.node = node; 
		this.pos = pos; 
	}
	
}


class Checker implements Comparator<Triplet>{

	@Override
	public int compare(Triplet arg0, Triplet arg1) {
		if(arg0.value<arg1.value) return 1;
		return -1; 
		
	}
	
}


public class P101 {
	
	//Nos definimos una cola de prioridad de la que vamos a ir sacando nodos para expandir ( ES UNA COLA COMPUESTA DE STRUCTS DE FILAS Y COLUMNAS )
	PriorityQueue<Triplet> pQueue = new PriorityQueue<Triplet>(100, new Checker()); 
	Integer[] supers_values;
	Integer[][]supers_products_values;
	Integer[][] vectorNodos; 
	ArrayList<Integer> result1 ;
	Integer[] camino_result; 
	Integer best; 
	
	//Funcion para sacar la cota optimista
	private Integer cota_optimista(int n ) {
		Integer min_value = 10000;
		Integer total = 0; 
		for(int i = n; i<supers_products_values[0].length; i++ ) {
			for(int j = 0; j<supers_values.length ; j++ ) {
				
				if(supers_products_values[j][i]<min_value  ) {
					
					min_value = supers_products_values[j][i]; 
					//System.out.println("El nuevo valor min es " + min_value); 
				}
				if(j+1 == supers_values.length) {
					total = total + min_value; 
				}
			}
		}
		return total;
	}
	
	//Funcion para sacar la cota pesimista (Algoritmo voraz donde cogemos todos los del primer super)
	private Integer cota_pesimista(int n) {
		Integer res = supers_values[0]; 
		vectorNodos[0][0] = res; 
		for(int i = n; i<supers_products_values[0].length ; i++) {
			res += supers_products_values[0][i]; 
			vectorNodos[0][i] = res; 
			camino_result[i] = 0; //Metemos para cada producto, el mismo super
		}
		return res; 
	}
	
	
	//Funcion para calcular el valor actual del camino del nodo actual
	private int calcula_valor( ArrayList<Integer>camino  , int super_last ) {
		ArrayList<Integer> aSumar = new ArrayList<Integer>(); 
		int res = 0; 
		//Miraremos si hay que sumar el desplazamiento
		for(int i = 0; i< camino.size(); i++ ) { //Para cada uno de los productos, tenemos su super
			boolean encontrado = false; 
			for(int j = 0; j<aSumar.size() ; j++ ) {
				if(aSumar.get(j)== camino.get(i) ) {
					encontrado = true; 
				}
				
			}
			if(encontrado == false ) aSumar.add(supers_values[camino.get(i)]); 
			res = res + supers_products_values[camino.get(i)][i]; 
		}
		boolean last_encontrado = false; 
		for(int i = 0; i< aSumar.size() ; i++ ) {
			
			res = res + aSumar.get(i);
			
		}
		for(int i= 0; i<camino.size(); i++) {
			if(camino.get(i)== super_last ) {
				last_encontrado = true; 
			}
		}
		if(!last_encontrado ) {
			res = res + supers_values[super_last];
		}
		
		return res; 
	}
	
	
	
	private void minVal(Integer fila_act,  ArrayList<Integer> camino , Integer actual) {
		Integer min = 10000; 
		
		for(int i = 0; i<supers_values.length; i ++ ) { //Miramos cada uno de los supermercados con el camino actual
			Integer actual_value= calcula_valor(camino , i ) + supers_products_values[i][fila_act]; //Cogemos el valor
			System.out.println("El best es "+ best);
			//System.out.println("Estamos entrando con el prod " + fila_act + " Con un valor actual de " + actual_value+ "Y con un camino de " + Arrays.toString(camino.toArray()) );
			if(  actual_value+ cota_optimista(fila_act) < best ) { 
				
				if(fila_act == supers_products_values[0].length -1   ) { //Llegamos al final
					System.out.println("Estamos entrando");
					for(int j = 0; j<camino.size(); j++ ) {
						camino_result[i] = camino.get(i);
					}
					
					vectorNodos[i][fila_act] = actual_value;
					best = actual_value;  //Modificamos la cota
					
				
				}
				else { 
					
					
					if(vectorNodos[i][fila_act]>= actual_value  ){
						 //Le metemos el super en el que estamos mirando el producto
						
						
						vectorNodos[i][fila_act] = actual_value; 
						//Metemos en la cola como nodo a explorar
						Nodes node_exp1 = new Nodes(i , fila_act+1 , actual_value);
						//Metemos en la cola el que acabamos de explorar
						ArrayList<Integer> camino2 = new ArrayList<Integer>(camino);
						camino2.add(i);
						
						pQueue.add(new Triplet(cota_optimista(fila_act), node_exp1, camino2 ));
						
					}
					
				}
				
			
			}
		}
		
	}
	
	
	///*********METODO ITERATIVO PARA CALCULAR EL RESULTADO( RAMIFICACION Y PODA)******************
	private void mazeRP( ) {
		best = cota_pesimista(0);
		
		//System.out.println("La cota opt es " + cota_optimista(0));
		Nodes node_exp1 = new Nodes(0,0, 0); //Empezamos en el primer super con el primer producto
		
		ArrayList<Integer> camino = new ArrayList<Integer>(); 
 		//Metemos la primera posicion en la cola de prioridad
		pQueue.add(new Triplet(cota_optimista(0), node_exp1, new ArrayList<Integer>()));
		Integer actuales = 0; 
		Integer productAct = 0;
		
		while(!pQueue.isEmpty()) {
			minVal(productAct,  camino, actuales);
			Triplet new_node = pQueue.poll();
			camino= new_node.pos; //Cogemos el camino que tenía el nodo
			
			actuales = new_node.node.pasos;
			productAct =new_node.node.prod; //Le pasamos el producto 
			System.out.println("El best es "+ best);
			System.out.println("Estamos entrando con el prod " + productAct + " Con un valor actual de " + actuales+ " Y con un camino de " + Arrays.toString(camino.toArray()) );
			 
			
			
		}
		
	}
	
	public ArrayList<Integer> bestSolution(String[] data){
		/*String cad = ""; 
		for(int i = 0; i<data.length ; i ++ ) {
			cad = cad +  data[i] + " , "; 
		}
		if(!cad.equals("10 4 4 99 4 4 , 11 5 5 5 5 5 , ")) {
			
			Integer.parseInt(cad);
		}*/
		supers_values = new Integer[data.length];  
		String[] parts = data[0].split("\\p{Space}+");
		camino_result = new Integer[parts.length - 1 ]; 
		supers_products_values = new Integer[data.length][parts.length-1 ]; //[Super][Producto]
		result1 = new ArrayList<Integer>(); //Donde alamacenaremos el resultado de los productos que se cogen en los diferentes supers 
		vectorNodos = new Integer[data.length][parts.length -1 ]; 
		
		for(int i =0 ; i<data.length ; i ++ ) {
			setData(data[i] , i); 
			for(int j = 0; j<parts.length-1 ; j++) {
				vectorNodos[i][j] = 100000; 
			}
		}
		mazeRP();
		//Parsing the result to an array list
		for(int i = 0; i < camino_result.length ; i++ ) {
			result1.add(camino_result[i] + 1 ); 
		}
		return result1; 
	}
	
	//Function to set the initial data
	private void setData(String data , Integer j) {
		String[] parts = data.split("\\p{Space}+");
		supers_values[j]= Integer.parseInt(parts[0]); //Cogemos el valor de pasar al super
        for(int i = 1; i<parts.length;i++) {
    		//System.out.println(Integer.parseInt(data.substring(i, j)));
        	supers_products_values[j][i-1] = Integer.parseInt(parts[i]); 
        }  
       
        	
	}
	
}
