package com.example.Modele;


public class GameTest {

    private static final GameSolo game = new GameSolo("Solo","joueurTest");
    private static final Stat s= new Stat ("test",200, 45.23, 89); 
    private static final Stat s2= new Stat ("test2",400, 45.23, 79);

    static String reussi="Test reussi";
    static String echoue="Test echoue";

   static  void testOf() {
        // Vérifie que les différents modes de jeu sont bien créés
        if(game instanceof GameSolo) System.out.println(reussi);
        else System.out.println(echoue);
    }
    
    static void testPlayer() {
        // Vérifie que les différents modes de jeu sont bien créés
        Player p= game.getPlayer();
        p.typeFoul();
        if(p.getRemainingLife()==12) System.out.println(reussi);
        else System.out.println(echoue);

        p.levelUp();
        if(p.getLvl()==1) System.out.println(reussi);
        else System.out.println(echoue);
        
        if(p.getSpeed()==11400.00) System.out.println(reussi);
        else System.out.println(echoue);    }


    static void testGetPrecision() {
        if(s.getAccuracy()==224) System.out.println(reussi);
        else System.out.println(echoue); 

        if(s2.getAccuracy()==506) System.out.println(reussi);
        else System.out.println(echoue); 
    }

    static void testGetSpeed() {
        
        if(s.getSpeed()==0.88) System.out.println(reussi);
        else System.out.println(echoue);

        if(s2.getSpeed()==1.77) System.out.println(reussi);
        else System.out.println(echoue);
    }
    
    public static void main(String[] args) {
        testOf();
        testPlayer();
        testGetPrecision();
        testGetSpeed();
    }
    
}