package main;

import world.*;
import world.moon.*;
import java.util.Hashtable;

public class Main {
   public static void main(String[] args) {
      System.out.println("Main");
      HelloWorld.main(null);
      HelloMoon helloMoon = new HelloMoon();
      System.out.println(helloMoon.getHoleName());
      helloMoon.setHole("test_hole");
      System.out.println(helloMoon.getHoleName());

      Hashtable<Integer, String> ht = new Hashtable<>();
      ht.put(1, "one");
      ht.put(3, "three");
      System.out.println("Mappings of ht1 : " + ht);
   }
}
