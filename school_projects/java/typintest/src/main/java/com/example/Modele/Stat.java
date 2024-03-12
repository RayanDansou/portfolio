package com.example.Modele;

import java.text.DecimalFormat;


/*
 * Classe représentant des statistiques
 * 
 */

public class Stat {
    private String PName;
    private int words;
    private Double time;
    private int totaltyped;

     public Stat(String name,int w, Double t, int tt){
        PName=name;
        words=w;
        time=t;
        totaltyped=tt;
    
    }


    
    /** 
     * @return double renvoie la vitesse
     */
    public double getSpeed(){
        Double c=words /time;
        c =Double.parseDouble(new DecimalFormat("##.##").format(c/5.00));
        return c;
    }

    
    /** 
     * @return int revoie la précision
     */
    public int getAccuracy(){
        double c=(words*1.0/totaltyped);
        return (int)(c*100);
    }


    
    /** 
     * @return String renvoie le nom du joueur
     */
    public String getName() {
        return PName;
    }


    
    /** 
     * @return int regularitée
     */
    public int getRegularity() {
        return 0;
    }

}