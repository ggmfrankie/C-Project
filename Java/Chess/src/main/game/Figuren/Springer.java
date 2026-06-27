package main.game.Figuren;

import main.game.Figur;

import java.awt.*;

public class Springer extends Figur {
    public Springer(boolean isWhite, int x,int y){
        super(isWhite, 3,x,y);
    }
    @Override
    public void markPossibleMoves(){
        Spielfeld[x][y].setBackground(Color.yellow);
        markCheck(x-2,y+1);
        markCheck(x-2,y-1);
        markCheck(x+2,y+1);
        markCheck(x+2,y-1);

        markCheck(x+1,y-2);
        markCheck(x-1,y-2);
        markCheck(x+1,y+2);
        markCheck(x-1,y+2);
    }
}
