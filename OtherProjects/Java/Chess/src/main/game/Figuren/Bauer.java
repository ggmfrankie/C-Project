package main.game.Figuren;

import main.game.Figur;

import java.awt.*;

public class Bauer extends Figur {
    public Bauer (boolean isWhite, int x,int y){
        super(isWhite,1,x,y);
    }
    @Override
    public void markPossibleMoves(){
        if(this.isWhite()){
            markCheckPawn(x-1,y);
            Spielfeld[x][y].setBackground(Color.yellow);
            if(x == 6){
                markCheckPawn(x-2,y);
            }
            isFigureAt(x-1,y+1);
            isFigureAt(x-1,y-1);

        }
        else{
            markCheckPawn(x+1,y);
            Spielfeld[x][y].setBackground(Color.yellow);
            if(x == 1){
                markCheckPawn(x+2,y);
            }
            isFigureAt(x+1,y+1);
            isFigureAt(x+1,y-1);
        }
    }
    public void isFigureAt(int x,int y){
        if(x<0||y<0||x>7||y>7){return;}
        if(Spielfeld[x][y].getFigur() ==null){return;}
        if(Spielfeld[x][y].getFigur().isWhite()!=this.isWhite()){
            Spielfeld[x][y].setMarked(true);
        }
    }

    public void markCheckPawn(int x, int y){
        if(x<0 || y<0 ||x>7||y>7){
            return;
        }
        if(Spielfeld[x][y].getFigur()!=null){
            return;
        }
        if(!checkIfAllowed(x,y)) return ;
        Spielfeld[x][y].setMarked(true);
    }
}
