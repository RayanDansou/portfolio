package com.example.Vue;

import com.example.Modele.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/*
 * Page d'accueil et de choix du mode 
 * 
 */

public class Start extends JFrame implements ActionListener
{
    JButton b1;
    JPanel newPanel,namePanel;
    JCheckBox checkBox1, checkBox2, checkBox3;
    JTextField jt;

    public Start(){
    b1=new JButton("Jouer");
    b1.setBounds(100,100,50,50);
    namePanel=new JPanel();
    JPanel j1=new JPanel(new BorderLayout());
    j1.add(new JLabel("Entrez votre nom: "),BorderLayout.CENTER);
    JPanel j2=new JPanel(new BorderLayout());
        jt= new JTextField("Entrez votre nom");
        j2.add(jt,BorderLayout.CENTER);
        namePanel.add(j1);
        namePanel.add(j2);
        newPanel = new JPanel(new BorderLayout());
        this.setLayout(new GridLayout(5,1));
        checkBox1=new JCheckBox("Solo");
        checkBox1.setBounds(100,100,50,50);
        checkBox2 = new JCheckBox("Normal",true);
        checkBox2.setBounds(100,150,50,50);
        checkBox3 = new JCheckBox("Multi",true);
        checkBox3.setBounds(100,150,50,50);
        b1.addActionListener(this);
        this.add(namePanel);  
        this.add(checkBox1);
        this.add(checkBox2);
        this.add(checkBox3);
        newPanel.add(b1,BorderLayout.CENTER);
        this.add(newPanel);
       
        this.setSize(300,300);
        this.setVisible(true);
    }

    class Error extends JFrame implements ActionListener{
        Error(String s) {
            setSize(250, 100);
            this.setLayout(new BorderLayout());
            add(new JLabel(s),BorderLayout.CENTER);
            JButton b=new JButton("Ok");
            b.addActionListener(this);
            add(b,BorderLayout.SOUTH);
            setVisible(true);
        }

        @Override
        public void actionPerformed(ActionEvent arg0) {
            dispose();
        }

    }

    public GameSolo newSolo(String mode,String playername){
        return new GameSolo(mode,playername);

    }

    public void actionPerformed(ActionEvent ae){
        if(ae.getSource()==b1){
            if(jt.getText().length()<1 || jt.getText().equals("Entrez votre nom")){
                    new Error("entrez un nom valide!!");
            }else{

            if(checkBox1.isSelected() && !checkBox2.isSelected() && !checkBox3.isSelected()){
            this.dispose();
             new GameFrame(newSolo("Solo",jt.getText()));
            }
            else  if(!checkBox1.isSelected() && checkBox2.isSelected() && !checkBox3.isSelected()){
                this.dispose();
                 new GameFrame(newSolo("Normal",jt.getText()));
                }
                else if(!checkBox1.isSelected() && !checkBox2.isSelected() && checkBox3.isSelected()){
                    new Error("Ce mode n'existe pas :(");
                    }
            else if(!checkBox1.isSelected() && !checkBox2.isSelected() && !checkBox3.isSelected()){
                new Error("veuillez choisir 1 mode!!");
                }
            else{
                new Error("1 mode maximum!!");
            }
        }
        }
    }   
}   
    