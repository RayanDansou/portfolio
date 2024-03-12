package com.example.Modele;



/*
 * 
 * Cete Classe représente le vie du joueur (il part avec 13 vies)
 */
public class Life {
    private int count;

    Life(){
        count=13;
    }

    /*
     * retire 1 vie
     */
    public void foul(){
        if(count>0) count--;
    }

    /*
     * augmente la vie
     */
    public void lifeUp(){
        count++;
    }

    
    /** 
     * @return int renvoie le nombre de vies restantes
     */
    public int remaining(){
        return count;
    }
    
}
