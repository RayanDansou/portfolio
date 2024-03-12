package com.example.Vue;

import com.example.Modele.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;



/*
 * Classe se chargeant de l'affichage des statistiques
 */
public class StatDisplayer extends JFrame implements ActionListener{

    JButton b1;
    public StatDisplayer(Stat s){
        setSize(300, 300);
        setBackground(Color.white);
        setLayout(new GridLayout(6,0));

        JLabel j1= new JLabel("Joueur: "+s.getName());
        j1.setHorizontalAlignment(JLabel.CENTER);
        JLabel j2= new JLabel("Voici vos statistiques : ");
        j2.setHorizontalAlignment(JLabel.CENTER);
        JLabel j3= new JLabel("Vitesse d'ecriture: "+s.getSpeed()+"/Min");
        j3.setHorizontalAlignment(JLabel.CENTER);
        JLabel j4= new JLabel("Précision: "+s.getAccuracy()+"%");
        j4.setHorizontalAlignment(JLabel.CENTER);
        JLabel j5= new JLabel("Régularité: "+s.getRegularity());
        j5.setHorizontalAlignment(JLabel.CENTER);
        
        JButton b1=new JButton("REJOUER");
        b1.setBounds(100,100,50,50);
        b1.addActionListener(this);
       
        add(j1);
        add(j2);
        add(j3);
        add(j4);
        add(j5);
        add(b1);

        setVisible(true);
    
    
    }

    
    
    /** 
     * @param e
     */
    @Override 
	public void actionPerformed(ActionEvent e) {
            dispose();
            new Start();
    }
  
}
