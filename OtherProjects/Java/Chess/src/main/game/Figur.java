package main.game;

import main.GUI.Board;
import main.game.Figuren.König;

import java.util.Arrays;

public class Figur {
    public boolean isWhite;
    public int value, x, y, moves = 0;
    public Feld[][] Spielfeld;
    public Figur(boolean isWhite, int value,int x,int y) {
        this.value = value;
        this.isWhite = isWhite;
        updateSpielfeld();
        this.x = x;
        this.y = y;
    }
    public int getValue() {
        return this.value;
    }

    public int getX() {
        return x;
    }
    public int getMoves(){
        return this.moves;
    }
    public void incrementMoves(){
        moves += moves;
    }
    public void updateSpielfeld(){
        this.Spielfeld = Speicher.getSpielfeld();
    }
    public void setSpielfeld(Feld[][] feld) {
        this.Spielfeld=feld;
    }
    public int getY() {
        return y;
    }
    public void setCords(int x,int y){
        this.x = x;
        this.y = y;
    }

    public boolean isWhite() {
        return this.isWhite;
    }
    public void markPossibleMoves(){
        Speicher.getBoard().unmarkAll();
    }
    public int markCheck(int x, int y){
        if(x<0 || y<0 ||x>7||y>7) return 1;
        if(Spielfeld[x][y].getFigur()!=null){
            if (Spielfeld[x][y].getFigur().isWhite() != this.isWhite()) {
                if(!checkIfAllowed(x,y)) return 1;
                Spielfeld[x][y].setMarked(true);
            }
            return 1;
        }
        if(!checkIfAllowed(x,y)) return 1;
        Spielfeld[x][y].setMarked(true);
        return 0;
    }
    public void markDiagonal(){
        int a=0,b=0,c=0,d =0;
        for(int i = 1; i<=7; i++){
            if(a==0){
                a+= markCheck(this.x-i,this.y+i);
            }
            if(b==0){
                b+= markCheck(this.x+i,this.y+i);
            }
            if(c==0){
                c+= markCheck(this.x-i,this.y-i);
            }
            if(d==0){
                d+= markCheck(this.x+i,this.y-i);
            }
        }
    }
    public boolean isInCheck(int x, int y){
        return false;
    }

    public void markHorizontal(){
        markLeft();
        markRight();
    }
    public void markVertikal(){
        markUp();
        markDown();
    }
    public void markLeft(){
        for(int i =this.x-1;i>=0;i--){
            if(Spielfeld[i][this.y].getFigur() !=null){
                if(Spielfeld[i][this.y].getFigur().isWhite()!=this.isWhite()){
                    markCheck(i,this.y);
                }
                return;
            }
            markCheck(i,this.y);
        }
    }
    public void markRight(){
        for(int i=this.x+1;i<=7;i++){
            if(Spielfeld[i][this.y].getFigur() !=null){
                if(Spielfeld[i][this.y].getFigur().isWhite()!=this.isWhite()){
                    markCheck(i,this.y);
                }
                return;
            }
            markCheck(i,this.y);
        }
    }

    public void markUp(){
        for(int i=this.y-1;i>=0;i--){
            if(Spielfeld[this.x][i].getFigur() !=null){
                if(Spielfeld[this.x][i].getFigur().isWhite()!=this.isWhite()){
                    markCheck(this.x,i);
                }
                return;
            }
            markCheck(this.x,i);
        }
    }
    public void markDown(){
        for(int i=this.y+1;i<=7;i++){
            if(Spielfeld[this.x][i].getFigur() !=null){
                if(Spielfeld[this.x][i].getFigur().isWhite()!=this.isWhite()){
                    markCheck(this.x,i);
                }
                return;
            }
            markCheck(this.x,i);
        }
    }
    public boolean checkIfAllowed(int x,int y){
        boolean inCheck = false;
        Figur figur = null;
        Feld[][] feld = Speicher.getSpielfeld();
        figur = feld[x][y].getFigur();
        feld[this.x][this.y].addFigur(null);
        feld[x][y].addFigur(this);

        if(this.isWhite){
            if(this instanceof König){
                inCheck = isCheck(Speicher.getwKönig(),x,y);
            }
            else{
                inCheck = isCheck(Speicher.getwKönig());
            }


        }
        else {
            if(this instanceof König){
                inCheck = isCheck(Speicher.getbKönig(),x,y);
            }
            else{
                inCheck = isCheck(Speicher.getbKönig());
            }
        }

        feld[x][y].addFigur(figur);
        feld[this.x][this.y].addFigur(this);
        return !inCheck;
    }
    public boolean isCheck(Figur k, int x, int y){
        return k.isInCheck(x,y);
    }
    public boolean isCheck(Figur k){
        return k.isInCheck(k.getX(),k.getY());
    }
}
