package com.example.Modele;

import java.util.ArrayList;

public class GameMulti extends Game{
    private ArrayList<Player> players= new ArrayList<Player>();
    private int currentPlayer;

    GameMulti(String m,ArrayList<Player> p){
        super(m);
        players=p;
        currentPlayer=0;
    }

    
    /** 
     * @return List<Player> renvoie la liste des joueurs du jeu
     */
    public ArrayList<Player> getPlayers(){
        return players;
    }

    
    /** 
     * @return Player renvoiele joueur actuel
     */
    public Player getCurrentPlayer(){
        return players.get(currentPlayer);
    }

    
    /** 
     * @return int renvoie l;indice du joueur actuel
     */
    public int getCurrentPlayerIndex(){
        return (currentPlayer);
    }
}
