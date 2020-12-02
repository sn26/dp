package AA;

import java.util.ArrayList;

public class main {
	public static void main(String[] args) {
		String[] prueba = {"22 9 39 16 5 47 2" , "38 46 19 35 5 20 26" , "15 11 29 2 19 1 43" , "91 17 50 38 47 9 13"  };
		P101 prueba1= new P101(); 
		ArrayList<Integer> result = prueba1.bestSolution(prueba);
		for(int i= 0; i <result.size(); i++ ) {
			System.out.println("SUPER: " + result.get(i));
		}
	}
}
