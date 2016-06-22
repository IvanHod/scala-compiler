import java.io.PrintStream; 
import java.util.Scanner; 

public class RuntimeLib{
	
	static PrintStream out = System.out; 
	static Scanner scanner = new Scanner(System.in);
 
	public static void main(String[] args) {
		printString("HelloWorld\n"); 
	}
	
	public static void printString(String str){
		int length = str.length(); 
		byte[] bytes = str.getBytes(); 
		
		for(int i = 0 ; i < bytes.length ; ++i) {
			char c = (char)bytes[i];
			if(c == '\\'){
				i++; 
				c = (char)bytes[i];
				if(c == 't'){
					c = '\t'; 
				}else if(c == 'n'){
					c = '\n'; 
				}else if(c == 'b'){
					c = '\b'; 
				}else if(c == 'f'){
					c = '\f'; 
				}else if(c == '\n'){
					c = '\n'; 
				}else if(c == 'r'){
					c = '\r'; 
				}else if(c == 'v'){
					c = 'v'; 
				}
			}
			out.print(c); 
		}
	}
	
	public static void printInteger(int value){
		out.print(value); 
	}
	
	public static void printFloat(float value){
		out.print(value); 
	}
	
	public static int scanInteger(){
		int value = scanner.nextInt(); 
		return value;
	}
	
	public static float scanFloat(){
		float value = scanner.nextFloat(); 
		return value; 
	}
	
	public static String scanString(){
		String str = scanner.nextLine(); 
		return str ; 
	} 
	
}
