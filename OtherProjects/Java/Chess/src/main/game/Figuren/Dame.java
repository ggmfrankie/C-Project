package main.game.Figuren;

import main.game.Figur;

import java.awt.*;

public class Dame extends Figur {
    public Dame (boolean isWhite, int x,int y){
        super(isWhite,9,x,y);
    }
    @Override
    public void markPossibleMoves(){
        super.markPossibleMoves();
        Spielfeld[this.x][this.y].setBackground(Color.yellow);
        markHorizontal();
        markVertikal();
        markDiagonal();
    }
}
