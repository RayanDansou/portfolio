package com.example.Vue;

import com.example.Modele.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.concurrent.TimeUnit;

public class GameFrame extends JFrame implements ActionListener, KeyListener{
    public static int HeartsToPrint;
	private String passage=""; //Passage écrire
	private String typedPass=""; //Passage  écrit par le joueur
    private Text t=new Text(0); // le texte à écrie (dans sa totalité)
    private int length=0; // Longueur du texte
    private GameSolo g; // le jeu courant
	private Player player;// le joueur courant
	 
	private int typed=0; //le nombre de mots ecrits par le joueur
	private int count=0; // le nombre de caractères ecrits par le joueur
	
	private boolean running; // Etat du jeu en cours
	private boolean error; // error d'ecriture
	private boolean ended; // Etat du jeu fini
    
    private int nberror; // nombre d'erreurs
    private int mode; // mode du jeu
	
	private JButton button; 
	private Timer timer;
	private JLabel a,label; 
    private JPanel f,Panel; 
	private int index; // index courant dans le mot
    private int total; // le total de caractères ecrits
    private double duree; 
    private long start;
    private int up;


	public GameFrame(GameSolo g){
        this.g=g;
		player=g.getPlayer();
        t=g.getText();
        length=g.getText().getlength();
        index=0;
        mode = g.getMode();
		this.setLayout(new BorderLayout());
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); 
		this.setSize(765,400);
		this.setVisible(true); 
		this.setLocationRelativeTo(null); 
		
		button=new JButton("Start");
		button.setFont(new Font("MV Boli",Font.BOLD,30));
		button.setForeground(Color.BLUE);
		button.setVisible(true);
		button.addActionListener(this);
		button.setFocusable(false);
		
        Panel=new JPanel();
        Panel.setBackground(Color.WHITE);
		label=new JLabel();
		if (mode==0)label.setText("Joueur: "+player.getName());
		else label.setText("Joueur : "+player.getName()+"                Lvl : "+player.getLvl());
		label.setFont(new Font("serif",Font.PLAIN,30));
		label.setVisible(true);
		label.setOpaque(true);
		label.setHorizontalAlignment(JLabel.CENTER); 
		label.setBackground(Color.lightGray);
        Panel.setLayout(new GridLayout(3,0));
        Panel.add(label);
        if(g.getMode()==0){
         a=new JLabel("erreurs: "+nberror);
         a.setFont(new Font("serif",Font.PLAIN,30));
         a.setHorizontalAlignment(JLabel.CENTER);
         Panel.add(a);
        }else{
            f= new heart();
            HeartsToPrint=player.getRemainingLife();
            f.setBackground(Color.WHITE);
            Panel.add(f);
        }
        JPanel j=new JPanel();
        j.setBackground(Color.WHITE);
		Panel.add(j);

		
		this.add(button, BorderLayout.SOUTH);
		this.add(Panel, BorderLayout.NORTH);
		this.getContentPane().setBackground(Color.WHITE);
		this.addKeyListener(this);
		this.setFocusable(true); 
		this.setResizable(false);
		this.setTitle("Typing Test");
		this.revalidate(); 
	}

	
	
	/** 
	 * @param g
	 */
	@Override
	public void paint(Graphics g)	
	{
		super.paint(g);
		draw(g); 
	}
	
	
	
	/** 
	 * @param g
	 */
	public void draw(Graphics g){
		g.setFont(new Font("serif", Font.PLAIN, 25));
		
		if(running){
			// AFFICHAGE DU TEXTE 
			if(passage.length()>0 && passage.length()<50) { 
				g.drawString(passage.substring(0,passage.length()), g.getFont().getSize()/2, g.getFont().getSize()*8);
			}
            else if(passage.length()>=50 && passage.length()<100) {
				g.drawString(passage.substring(0,50), g.getFont().getSize()/2, g.getFont().getSize()*8);
				g.drawString(passage.substring(50,passage.length()), g.getFont().getSize()/2, g.getFont().getSize()*10);
			}
            else if(passage.length()>=100 && passage.length()<150) {
				g.drawString(passage.substring(0,50), g.getFont().getSize()/2, g.getFont().getSize()*8);
				g.drawString(passage.substring(50,100), g.getFont().getSize()/2, g.getFont().getSize()*10);
				g.drawString(passage.substring(100,passage.length()), g.getFont().getSize()/2, g.getFont().getSize()*12);
			}
            
            // GESTION DES FAUTES DE FRAPPE (MOTS EN ROUGE)
	            if(error){
                g.setColor(Color.RED);	 // on met le couleur en eouge		
                if(typedPass.length()>0){
                    g.drawString(passage.substring(0,index), g.getFont().getSize()/2,g.getFont().getSize()*8); 
                }
                if(index>0) index-=1;
                try {
                    TimeUnit.MILLISECONDS.sleep(300); // le mot devient rouge 
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                 error=false;
            }else{
			// SI PAS DE FAUTES
			g.setColor(Color.GREEN);// on met la couleur en vert
            if(typedPass.length()>0){
                g.drawString(passage.substring(0,index), g.getFont().getSize()/2,g.getFont().getSize()*8); 
            }
        }
	} 
	//FIN DE JEU
         if(ended){
            new StatDisplayer(new Stat(this.player.getName(),count,duree,total)); //appel du Displayer
            dispose();
            timer.stop();
			ended=false; 
		}
	}
	
	
	/** 
	 * On uttilise un keyListener pour ecouter le clavier
	 */
	@Override
	public void keyTyped(KeyEvent e) 
	{
		 total++;
		if(passage.length()>-1) 
		{ // tant que le passage n'est pas vide
		if((count==length-1 && mode==0) || (mode==1 && player.getRemainingLife()==0)) 
		{ //si on a les conditions de fin (texte fini/plus de vie)
				ended=true;
				running=false;
				count++;
			}
			char[] pass=passage.toCharArray(); // conversion du texte en char[]
			if(typed<length-1)
			{
				running=true;
				if(e.getKeyChar()==pass[index]) 
				{ //Bonne Lettre ecrite
                    if(e.getKeyChar()==' '){ // si on tape espace
                        typedPass=typedPass+' '; 
                        typed++;
						count++;
                        up=0;
                        index=0;
                        t.actualize();//actalise le texte
                        passage=t.getCurrent();
                        repaint();
                         }else{ // si caractère normal
                        typedPass=typedPass+pass[index];  // on ajoute ce qui est ecrit dans typedpass
                        index++;
                        count++; 
                        }
                        error=false;// erreur
                    } else { // si faute de frappe
                    error=true; //erreur
                    index++;
                    nberror++;
					//change l'affichage
		            if(mode==0){
                        a.setText("erreurs:"+nberror);
                    }else{
                         player.typeFoul(); 
                         HeartsToPrint--;
                         f.repaint();
                         repaint();}
                }
			}
		}
	}

	

	
	/** 
	 * @param e
	 */
	@Override
	public void keyPressed(KeyEvent e){}

	
	
	/** 
	 * @param e
	 */
	@Override
	public void keyReleased(KeyEvent e){}

	
	
	/** 
	 * @param e
	 */
	@Override 
	public void actionPerformed(ActionEvent e) {
		//SI ON APPUIE SUR START 
		if(e.getSource()==button){
			passage=t.getCurrent(); //affiche le texte a ecrire
			//initialisation du jeu
			timer=new Timer(100,this);
			timer.start();
			running=true;
			ended=false;
            error=false;
            start=  System.currentTimeMillis();
			duree=(System.currentTimeMillis()-start)/1000.00;
			
			typedPass="";
			
			typed=0;
			count=0;
			button.setVisible(false);//efface le bouton
		}
		// ACTUALISATION DE LA DUREE
		duree=(System.currentTimeMillis()-start)/1000.00;


		//SI MODE NORMAL
        if(mode==1){
        if(typed>0 && typed%100==0 && up==0) { // augmente de niveau tous les 100 mots
        player.levelUp();
		label.setText("Joueur : "+player.getName()+"                Lvl : "+player.getLvl());
        up=1;
        }
         if(duree>0.0 &&((duree*1000.00)%player.getSpeed()<=100)) {// actualise selon la vitesse du jeu
            if(index!=0){player.typeFoul(); // enlève de la vie
			HeartsToPrint--;
			f.repaint();
			}
            t.actualize();
            passage=t.getCurrent();
            index=0;
        }
        }
		
		if(running)
			repaint();
		if(ended)
			repaint();
	}
    

}
