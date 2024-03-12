package com.example.Modele;

/*
 * Cette Classe représente un Jeu avec un seul joueur(en mode solo ou normal)
 */

public class GameSolo extends Game {
    private Player player;

   public GameSolo(String m,String p){
        super(m);
        player=new Player(p);
    }

    
    /** 
     * @return Player renvoie le joueur du  jeu
     */
    public Player getPlayer(){
        return player;
    }


}
