package com.example.Modele;


/*
 * Cette Classe représente un Jeu 
 */
public class Game {
    private Mode mode;
    private Text text;

    Game(String m){
        try {
            mode=new Mode(m);
            int i=mode.getMode();
            text=new Text(i);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    
    /** 
     * @return int : renvoie le mode de jeu
     */
    public int getMode(){
        return mode.getMode();
    }

    
    /** 
     * @return Text  renvoie le texte à copier
     */
    public Text getText(){
        return text;
    }
}