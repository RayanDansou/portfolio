package com.example.Modele;


/*
 * Cette Classe représente le niveau du joueur ainsi que la vitesse correspindante
 */
public class Level {
    private int difficulty;
    private double speed;

    Level(){
        difficulty=0;
        speed= 12000.00;
    }

    public void levelUp(){
        difficulty++;
        speed*=0.95;
    }

    
    /** 
     * @return int renvoie le niveau actuel du joueur
     */
    public int getLvl(){
        return difficulty;
    }

    
    /** 
     * @return double renvoie la vitess correspondant au niveau
     */
    public double getSpeed(){
        return speed;
    }
    
}
