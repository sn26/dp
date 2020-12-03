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
	
	public Integer value; //Cota opt (Para poder ordenarlos en la cola) 
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
		/*if(arg0.pos.size()>arg1.pos.size() ) {
			if(arg0.node.pasos<arg1.node.pasos) return 1;
			return 1; 
		}
		if(arg0.pos.size() == arg1.pos.size()) {
			if(arg0.node.pasos<arg1.node.pasos) return 1;
			return -1;
		}*/
		return -1; 
		
	}
	
}


public class P101 {
	
	//Nos definimos una cola de prioridad de la que vamos a ir sacando nodos para expandir ( ES UNA COLA COMPUESTA DE STRUCTS DE FILAS Y COLUMNAS )
	PriorityQueue<Triplet> pQueue = new PriorityQueue<Triplet>(1000, new Checker()); 
	Integer[] supers_values;
	Integer[][]supers_products_values;
	Integer[] cota_optimista; 
	ArrayList<Integer> result1 ;
	Integer[] camino_result;
	Integer best; 
	
	//Funcion para sacar la cota optimista
	private Integer cota_optimista( int n ) {
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
					min_value = 10000; 
				}
				
			}
		}
		return total;
	}
	
	//Funcion para sacar la cota pesimista (Algoritmo voraz donde cogemos todos los del primer super)
	private Integer cota_pesimista(int n) {
		Integer res = supers_values[0]; 
		//vectorNodos[0][0] = res; 
		for(int i = n; i<supers_products_values[0].length ; i++) {
			res += supers_products_values[0][i]; 
			//camino_result[i] = 0; //Metemos para cada producto, el mismo super
			
		}
		return res; 
	}
	
	
	
	
	//Funcion para calcular el valor actual del camino del nodo actual
	private int calcula_valor( ArrayList<Integer>camino  , int super_last ) {
		for(int i = 0; i< camino.size(); i++ ) { //Para cada uno de los productos, tenemos su super
			if(camino.get(i)== super_last ) {
				return 0;  
			}
		}
		
		return supers_values[super_last];
	}
	

	
	
	private void minVal(Integer fila_act,  ArrayList<Integer> camino , Integer actual) {
		if(fila_act >= supers_products_values[0].length )return; 
		if(cota_optimista[fila_act] == null ) cota_optimista[fila_act] = cota_optimista(fila_act);  
		for(int i = 0; i<supers_values.length; i ++ ) { //Miramos cada uno de los supermercados con el camino actual
			if(  actual+ cota_optimista[fila_act] <best ) { //Podamos
				int actual_value= actual + supers_products_values[i][fila_act] +  calcula_valor(camino , i ); //Cogemos el valor 
				if(fila_act == supers_products_values[0].length -1    ) { //Llegamos al final
					if(actual_value < best ) {
						camino.add(i); 
						for(int j = 0; j<camino.size(); j++ ) {
							camino_result[j] = camino.get(j);
						}
						camino.remove(camino.size() - 1 ); 
						best = actual_value;  //Modificamos la cota
					}
				}
				else { 
						
						Nodes node_exp1 = new Nodes(i , fila_act+1, actual_value);
						ArrayList<Integer> camino2 = new ArrayList<Integer>(camino);
						camino2.add(i);
						pQueue.add(new Triplet(fila_act+1, node_exp1, camino2 ));	
				}
				
			
			}
		}
		
	}
	
	
	///*********METODO ITERATIVO PARA CALCULAR EL RESULTADO( RAMIFICACION Y PODA)******************
	private void mazeRP( ) {
		best = cota_pesimista(0);
		Nodes node_exp1 = new Nodes(0,0, 0); //Empezamos en el primer super con el primer producto
		ArrayList<Integer> camino = new ArrayList<Integer>(); 
 		//Metemos la primera posicion en la cola de priorid
		
		pQueue.add(new Triplet(0, node_exp1, new ArrayList<Integer>()));
		Integer actuales = 0; 
		Integer productAct = 0;
		
		while(!pQueue.isEmpty()) {
			minVal(productAct,  camino, actuales);
			Triplet new_node = pQueue.poll();
			camino= new_node.pos; 
			actuales = new_node.node.pasos;
			productAct =new_node.node.prod; //Le pasamos el producto 
		}
		
	}
	//private int counter = 0; 
	public ArrayList<Integer> bestSolution(String[] data){
		supers_values = new Integer[data.length];
		String[] parts = data[0].split("\\p{Space}+");
		camino_result = new Integer[parts.length - 1 ]; 
		cota_optimista = new Integer[parts.length -1]; 
		supers_products_values = new Integer[data.length][parts.length-1 ]; //[Super][Producto]
		result1 = new ArrayList<Integer>(); //Donde alamacenaremos el resultado de los productos que se cogen en los diferentes supers 
		for(int i =0 ; i<data.length ; i ++ ) {
			setData(data[i] , i); 
		}
		mazeRP();
		for(int i = 0;  i<camino_result.length ; i ++ ) {
			result1.add(camino_result[i] + 1 );
		}
		return result1; 
	}
	
	//Function to set the initial data
	private void setData(String data , Integer j) {
		String[] parts = data.split("\\p{Space}+");
		supers_values[j]= Integer.parseInt(parts[0]); //Cogemos el valor de pasar al super
        for(int i = 1; i<parts.length;i++) {
        	supers_products_values[j][i-1] = Integer.parseInt(parts[i]); 
        }  
       
        	
	}
	
}
