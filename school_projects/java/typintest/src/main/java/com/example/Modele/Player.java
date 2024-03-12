package com.example.Modele;

/*
 * 
 * Cette Classe modélise un Joueur  
 */
public class Player {
    private String name;
    private Life lives;
    private Level lvl;

    Player(String n){
        name=n;
        lives=new Life();
        lvl=new Level();
    }

    
    /** 
     * @return String renvoie le nom du joueur
     */
    public String getName(){
        return name;
    }

    public int getRemainingLife(){ //le nombre de vies Restantes
        return lives.remaining();
    }

    public void levelUp(){ // on augmente le niveau
        lvl.levelUp();
    }

    public void typeFoul(){ // Le joueur perds de la vie 
        lives.foul();
    }

    public int getLvl(){ // renvoie le niveau actuel du joueur 
        return lvl.getLvl();
    }

    public double getSpeed(){ // renvoie la vitesse de jeu correspondante au niveau du joueur
        return lvl.getSpeed();
    }


}
