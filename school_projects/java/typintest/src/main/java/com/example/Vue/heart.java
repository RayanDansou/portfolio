package com.example.Vue;

import java.awt.Color;
import java.awt.Graphics;
import javax.swing.JPanel;


/*
 * Classe représentant un coeur (Vie du Joueur dans le mode normal)
 * 
 */

public class heart extends JPanel {

  
  /** 
   * @param g 
   */
  public void paintComponent(Graphics g) {
    super.paintComponent(g);
    h(g,GameFrame.HeartsToPrint);
  }

  
  /** 
   * @param g
   * @param c : nombre de coeurs a imprimer
   */
  public void h(Graphics g,int c){
    int a=210;
    for(int i=0;i<c;i++){
        drawHeart(g,a,15, 18, 18);
        a += 25;
    }
  }

/** 
 * @param g
 * @param x coordonnée x
 * @param y coordonée y
 * @param width largeur du coeur
 * @param height Longueur du coeur
 */
public void drawHeart(Graphics g, int x, int y, int width, int height) {
    g.setColor(Color.RED);
    int[] triangleX = {
            x - 2*width/18,
            x + width + 2*width/18,
            (x - 2*width/18 + x + width + 2*width/18)/2};
    int[] triangleY = { 
            y + height - 2*height/3, 
            y + height - 2*height/3, 
            y + height };
    g.fillOval(
            x - width/12,
            y, 
            width/2 + width/6, 
            height/2); 
    g.fillOval(
            x + width/2 - width/12,
            y,
            width/2 + width/6,
            height/2);
    g.fillPolygon(triangleX, triangleY, triangleX.length);
}

}


