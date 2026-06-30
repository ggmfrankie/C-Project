package main.GUI;

import main.game.Feld;
import main.game.Figur;
import main.game.Figuren.König;
import main.game.Figuren.Turm;
import main.game.Speicher;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;

public class Board extends JFrame{
    public JPanel mainBoard;
    public Feld[][] buttons;

    public Board(String name) {
        super(name);
        JPanel mainBoard = new JPanel(new GridLayout(8, 8));
        buttons = new Feld[8][8];
        int x = 0;
        for (int i = 0; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                x++;
                if(i%2 == 0){
                    if (j%2 == 0){
                        buttons[i][j] = new Feld("", i, j,true);
                    }
                    else{
                        buttons[i][j] = new Feld("", i, j,false);
                    }
                }
                else {
                    if(j%2 == 0){
                        buttons[i][j] = new Feld("", i, j,false);
                    }
                    else {
                        buttons[i][j] = new Feld("", i, j,true);
                    }
                }


                buttons[i][j].addActionListener(new ButtonClickListener());
                mainBoard.add(buttons[i][j]);
            }
        }
        this.setContentPane(mainBoard);
    }
    public Feld[][] getfeld(){
        return buttons;
    }
    public void setfeld(Feld[][] feld){
        this.buttons = feld;
    }
    public void update(){
        for (int i = 0; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                buttons[i][j].updateApperarance();
            }
        }
    }
    public void unmarkAll(){
        for (int i = 0; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                buttons[i][j].setMarked(false);
            }
        }
    }
    static class ButtonClickListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            Feld source = (Feld) e.getSource();
            Figur wKönig = Speicher.getwKönig();
            Figur bKönig = Speicher.getbKönig();
            Figur figur = source.getFigur();

            Figur selectedFigur = Speicher.getFigur();
            Board board = Speicher.getBoard();
            Feld[][] spielfeld = Speicher.getSpielfeld();

            if (!source.getMarked()){

                if (figur!=null && figur.isWhite()== Speicher.IsWhiteTurn()){
                    board.unmarkAll();
                    Speicher.setFigur(figur);
                    board.update();

                    figur.markPossibleMoves();
                }
            }
            else{

                spielfeld[selectedFigur.getX()][selectedFigur.getY()].addFigur(null);
                source.addFigur(selectedFigur);
                selectedFigur.setCords(source.getx(), source.gety());
                selectedFigur.incrementMoves();
                Speicher.setFigur(null);
                board.unmarkAll();
                //if(Speicher.)
                board.update();
                Speicher.nextTurn();
            }


        }
    }

}
