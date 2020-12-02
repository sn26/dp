package AA;

import java.util.ArrayList;


public class P100 {
	
	ArrayList<Integer> data_init = new ArrayList<Integer>(); 
	Integer num_max_dias = 0;  //N
	Integer separacion = 0; //L
	
	ArrayList<Integer>[][] result1;// = new ArrayList<ArrayList<Integer>>(); 
	//Integer[][] maxCogArr; 
	Integer[][]maxArr;
	//ArrayList<Integer> result2 = new ArrayList<Integer>();
	private ArrayList<Integer> bestSolutionMemo(Integer n,Integer  m){
		if(data_init.size() <= m || n<=0 ) { //Si no nos quedan elementos para comprobar
			return new ArrayList<Integer> ();
		}
		//Miraremos si ya tenemos guardado el valor de la que estamos mirando
		if(result1[m][n]!=null) return result1[m][n];

		//Getting the first value
		//result1.add(data_init.get(m));
		//System.out.println("La m es " + m + " Y mi n es " + n);
		ArrayList<Integer>actualCogido= new ArrayList<Integer>();
		actualCogido.add(m+1);
		actualCogido.addAll(bestSolutionMemo(n-1, m+separacion ));
		Integer maxCog= 0;
		if(actualCogido.size()>1) {
			if(maxArr[m+separacion][n-1] != null) {
				maxCog = maxArr[m+separacion][n-1] + data_init.get(m);
				
			}else {
				for(int i = 0; i<actualCogido.size(); i++) maxCog= data_init.get(actualCogido.get(i)-1) + maxCog;
				maxArr[m+separacion][n-1] = maxCog - data_init.get(m);
			}
			
		}else {
			for(int i = 0; i<actualCogido.size(); i++) maxCog= data_init.get(actualCogido.get(i)-1) + maxCog;
		}
		//maxArr[m][n] = maxCog; 
		
		ArrayList<Integer> actualNoCogido= bestSolutionMemo(n, m+1);
		Integer maxNoCog= 0;
		if(actualNoCogido.size()>1) {
			if(maxArr[m+1][n]!= null ) {
				maxNoCog = maxArr[m+1][n];
			}else {
				for(int i= 0; i<actualNoCogido.size(); i++) maxNoCog= data_init.get(actualNoCogido.get(i)-1) + maxNoCog; 
				maxArr[m+1][n] = maxNoCog;
			}
			
		}else {
			for(int i= 0; i<actualNoCogido.size(); i++) maxNoCog= data_init.get(actualNoCogido.get(i)-1) + maxNoCog; 
		}
		//maxArr[m][n] = maxNoCog; 
			
		if(maxCog<maxNoCog ) {
			//actualCogido.add(0,m+1);
			//maxCogArr[m][n] = maxNoCog; 
			maxArr[m][n] = maxNoCog; 
			result1[m][n]=actualNoCogido; 
			return result1[m][n]; 
		}else {
			//maxCogArr[m][n] = maxCog; 
			maxArr[m][n] = maxCog; 
			result1[m][n]=actualCogido; 
			return result1[m][n]; 
		}
		
		
	}
	
	public ArrayList<Integer> bestSolution(String data){
		data_init.clear();
		this.setData(data);
		return this.bestSolutionMemo(num_max_dias , 0);
		
	}
	
	private void setData(String data) {
		String[] parts = data.split("\\p{Space}+");
		num_max_dias= Integer.parseInt(parts[0]);
		separacion= Integer.parseInt(parts[1]);
        for(int i = 2; i<parts.length;i++) {
    		//System.out.println(Integer.parseInt(data.substring(i, j)));
        	data_init.add(Integer.parseInt(parts[i])); 
        }
       
        maxArr = new Integer[data_init.size()+1][num_max_dias+1];
        //maxNoCogArr = new Integer[data_init.size()+1][num_max_dias+1];
        result1 =new ArrayList[data_init.size()+1][num_max_dias+1];  
       
        	
	}

}
