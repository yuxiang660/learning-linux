import java.io.*;

public class Main {
   public static void main(String[] args) throws IOException{
      Parser parse = new Parser();
      parse.expr(); System.out.write('\n');
   }
}