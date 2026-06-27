package main.game.Figuren;

import main.game.Figur;

import java.awt.*;

public class Läufer extends Figur {
    public Läufer(boolean isWhite, int x,int y) {
        super(isWhite, 3,x,y);
    }
    @Override
    public void markPossibleMoves(){
        Spielfeld[this.x][this.y].setBackground(Color.yellow);
        markDiagonal();
    }
}
