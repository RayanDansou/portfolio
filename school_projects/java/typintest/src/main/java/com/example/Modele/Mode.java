package com.example.Modele;


public class Mode {

    private int mode;/* mode 0 pour le mode Solo
                        mode 1 pour le mode Normal
                        mode 2 pour le mode MultiPlayer */

    Mode(String m) throws Exception{
        if (m.equals("Solo")) mode=0;
        else if (m.equals("Normal")) mode=1;
        else if (m.equals("Multi")) mode=2;
        else {
            throw new Exception("mode inconnu");
        }
    }

    
    
    /** 
     * @return int renvoie le mode
     */
    public int getMode(){
        return mode;
    }

    
    
    /** 
     * @return String renvoie le String correspondant
     */
    public String stringMode(){
        if (mode==0) return("Solo");
        if (mode==1) return("Normal");
        return("MultiPlayer");
    }
    
}
