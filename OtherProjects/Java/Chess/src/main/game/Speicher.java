package main.game;

import main.GUI.Board;
import main.game.Figuren.*;

import javax.swing.*;



public class Speicher {
    static Board frameForGame;
    static Feld[][] spielfeld;
    static Figur selectedFigur;
    static boolean isWhiteTurn = true, isBlackTurn = false;
    static Figur wKönig, bKönig, selectedTurm = null;
    public static void gameFrame(){
        frameForGame = new Board("Spielfeld");
        frameForGame.setLocationRelativeTo(null);
        frameForGame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frameForGame.pack();
        frameForGame.setSize(1000,1000);
        frameForGame.setVisible(true);
        spielfeld = frameForGame.getfeld();
        init();
    }
    public static void init(){

        bKönig = new König(false,0,4);
        spielfeld[0][0].addFigur(new Turm(false,0,0));
        spielfeld[0][1].addFigur(new Springer(false,0,1));
        spielfeld[0][2].addFigur(new Läufer(false,0,2));
        spielfeld[0][3].addFigur(new Dame(false,0,3));
        spielfeld[0][4].addFigur(bKönig);
        spielfeld[0][5].addFigur(new Läufer(false,0,5));
        spielfeld[0][6].addFigur(new Springer(false,0,6));
        spielfeld[0][7].addFigur(new Turm(false,0,7));
        for (int i = 0; i <=7;i++){
            spielfeld[1][i].addFigur(new Bauer(false,1,i));
        }
        wKönig = new König(true,7,4);
        spielfeld[7][0].addFigur(new Turm(true,7,0));
        spielfeld[7][1].addFigur(new Springer(true,7,1));
        spielfeld[7][2].addFigur(new Läufer(true,7,2));
        spielfeld[7][3].addFigur(new Dame(true,7,3));
        spielfeld[7][4].addFigur(wKönig);
        spielfeld[7][5].addFigur(new Läufer(true,7,5));
        spielfeld[7][6].addFigur(new Springer(true,7,6));
        spielfeld[7][7].addFigur(new Turm(true,7,7));
        for (int i = 0; i <=7;i++){
            spielfeld[6][i].addFigur(new Bauer(true,6,i));
        }
        frameForGame.update();
    }
    public static void setFigur(Figur figur){
        selectedFigur = figur;
    }
    public static void setSelectedTurm(Figur figur){selectedTurm = figur;}
    public static Figur getFigur(){ return selectedFigur; }
    public static boolean IsWhiteTurn(){
        return isWhiteTurn;
    }
    public static void setSpielfeld(Feld[][] feld) {
        frameForGame.setfeld(feld);
        spielfeld = frameForGame.getfeld();
        frameForGame.update();
    }
    public static boolean IsBlackTurn(){
        return isBlackTurn;
    }
    public static Feld[][] getSpielfeld(){
        return frameForGame.getfeld();
    }
    public static Board getBoard(){
        return frameForGame;
    }
    public static void nextTurn(){
        isWhiteTurn = !isWhiteTurn;
        isBlackTurn = !isBlackTurn;
    }
    public static Figur getSelectedTurm(){return selectedTurm;}
    public static Figur getwKönig(){
        return wKönig;
    }
    public static Figur getbKönig(){
        return bKönig;
    }
}
