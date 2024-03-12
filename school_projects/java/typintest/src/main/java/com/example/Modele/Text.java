package com.example.Modele;
import java.util.*;

/**
 * 
 * Cette classe génère le texte a copier par le joueur
 */
public class Text {
    private String texte;

    private ArrayList<Word> words= new ArrayList<Word>();
    private ArrayList<Word> current= new ArrayList<Word>();
    private int length;

    public Text(int o){
        String toReturn=" ";
        if(o==1){
            toReturn = TextUtils.getArticle();
        }else {     
		 toReturn=TextUtils.getText();
        }
        texte=toReturn;
        String[] t=toReturn.split(" ");
        length= toReturn.length();
        ArrayList<Word> l= new ArrayList<Word>();
        for(String v:t){
            Word w=new Word(v);
            l.add(w);
        }
        if(l.size()<15){
            for(int i=0; i<l.size();i++){
                current.add(l.get(i));
            }
        }else{
        for(int i=0; i<15;i++){
            current.add(l.get(i));
        }
        for(int i=15; i<l.size();i++){
            words.add(l.get(i));
        }
    }
        
    }

    /*
     * affiche le texte courant
     */
    public void printText(){
        for(Word w: current) System.out.print( w.toString()+" ");
    }

    /*
     * affiche le texte en entier
     */
    public void printWholeText(){
       for(Word w: words) System.out.print( w.toString()+" ");
        System.out.println();
    }

    /*
     * actualise le texte a ecrire (15 mots)
     */
    public void actualize(){
        current.remove(0);
        if(words.size()>0){
        current.add(words.get(0));
        words.remove(0);
        }
    }

    
    /** 
     * @return String renvoie le passage actuel a ecrire
     */
    public String getPassage(){
        return texte;
    }

    
    /** 
     * @return String 
     */
    public String getCurrent(){
        String res="";
        for(Word w: current) res+=w.toString()+" ";
        return res;
    }

    
    /** 
     * @return int renvoie la longueur du texte 
     */
    public int getlength(){
        return length;
    }
    
}
    

