package com.example.Modele;


/*
 * 
 * Classe représentant un mot(d'un Texte)
 */

public class Word {

    private String content;
    private int length;

    Word(String s){
        content=s;
        length=s.length();
    }

    
    /** 
     * @return String renvoie le mot
     */
    public String toString(){
        return content;
    }

    
    /** 
     * @return int renvoie la longueur du mot
     */
    public int getlength(){
        return length;
    }
    
}
