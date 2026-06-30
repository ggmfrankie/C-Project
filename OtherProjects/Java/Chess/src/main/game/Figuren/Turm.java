package main.game.Figuren;

import main.game.Figur;

import java.awt.*;

public class Turm extends Figur {
    public Turm(boolean isWhite, int x,int y){
        super(isWhite, 5,x,y);
    }
    @Override
    public void markPossibleMoves(){
        Spielfeld[this.x][this.y].setBackground(Color.yellow);
        markHorizontal();
        markVertikal();
    }

}
