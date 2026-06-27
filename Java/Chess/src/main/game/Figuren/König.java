package main.game.Figuren;

import main.game.Figur;
import main.game.Speicher;

import java.awt.*;

public class König extends Figur {
    public boolean isInCheck, isRochade;
    public König (boolean isWhite, int x,int y){
        super(isWhite,900,x,y);
        this.isInCheck = false;
    }
    @Override
    public void markPossibleMoves(){
        Spielfeld[this.x][this.y].setBackground(Color.yellow);
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                System.out.println("marking moves");
                this.markCheck(i+this.x,j+this.y);
            }
        }
        if (this.isWhite){
            if(checkForRochade(7,0)){
                isRochade = 0==markCheck(7,2);
            }
            if(checkForRochade(7,7)){
                isRochade = 0==markCheck(7,6);
            }
        }
        else{
            if(checkForRochade(0,0)){
                isRochade = 0==markCheck(0,2);
            }
            if(checkForRochade(0,7)){
                isRochade = 0==markCheck(0,6);
            }
        }

    }
    //@Override
    //public int markCheck(int x,int y){
        //if(!this.isInCheck(x,y)){
            //return super.markCheck(x,y);
        //}
        //return 1;
    //}
    @Override
    public boolean isInCheck(int x, int y){
        if(checkDiagonal(x, y) || checkHorizontal(x, y) ||checkPawn(x,y) || checkKnight(x, y)||checkKing(x,y)){
            return true;
        }
        return false;
    }
    public boolean checkKing(int x,int y){
        int Enemy=0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                Enemy += isEnemyKing(getFigure(x+i,y+j));
            }
        }
        return Enemy >0;
    }
    public int isEnemyKing(Figur fig){
        if(fig != null){
            if(fig instanceof König && fig.isWhite() != this.isWhite){
                return 1;
            }
        }
        return 0;
    }
    public boolean checkPawn(int x, int y){
        int isEnemy = 0;
        if(this.isWhite){

            isEnemy += isEnemyPawn(getFigure(x-1, y+1));
            isEnemy += isEnemyPawn(getFigure(x-1, y-1));
        }
        else{

            isEnemy += isEnemyPawn(getFigure(x+1, y+1));
            isEnemy += isEnemyPawn(getFigure(x+1, y-1));
        }
        return isEnemy !=0;
    }
    public int isEnemyPawn(Figur fig){
        if(fig != null){
            if(fig instanceof Bauer && fig.isWhite() != this.isWhite){
                return 1;
            }
        }
        return 0;
    }
    public boolean checkKnight(int x, int y){
        int isEnemy = 0;
        isEnemy += isEnemyKnight(getFigure(x+2,y+1));
        isEnemy += isEnemyKnight(getFigure(x+2,y-1));
        isEnemy += isEnemyKnight(getFigure(x-2,y+1));
        isEnemy += isEnemyKnight(getFigure(x-2,y-1));
        isEnemy += isEnemyKnight(getFigure(x+1,y+2));
        isEnemy += isEnemyKnight(getFigure(x-1,y+2));
        isEnemy += isEnemyKnight(getFigure(x+1,y-2));
        isEnemy += isEnemyKnight(getFigure(x-1,y-2));
        return isEnemy!=0;
    }
    public int isEnemyKnight(Figur fig){
        if(fig != null){
            if(fig instanceof Springer && fig.isWhite() != this.isWhite){
                return 1;
            }
        }
        return 0;
    }
    public boolean checkHorizontal(int x, int y){
        Figur figur = null;
        System.out.println("checking Horizontal");
        int enemy =0, enemy1=0,enemy2=0,enemy3=0;
        for(int i = 1; i<=7; i++){

            if(enemy == 0){
                enemy += isFigureHorizontal(getFigure(x+i, y));
            }
            if(enemy > 0){
                return true;
            }
            if(enemy1 == 0){
                enemy1 += isFigureHorizontal(getFigure(x-i, y));
            }
            if(enemy1 > 0){
                return true;
            }
            if(enemy2 == 0){
                enemy2 += isFigureHorizontal(getFigure(x, y-i));
            }
            if(enemy2 > 0){
                return true;
            }
            if(enemy3 == 0){
                enemy3 += isFigureHorizontal(getFigure(x, y+i));
            }
            if(enemy3 > 0){
                return true;
            }
        }

        return false;
    }
    public int isFigureHorizontal(Figur fig){
        if(fig != null){
            if(fig instanceof König) return 0;
            if(isFigureEnemyHorizontal(fig)){
                return 1;
            }
            return -1;
        }
        return 0;
    }
    public int isFigureDiagonal(Figur fig){
        if(fig != null){
            if(fig instanceof König) return 0;
            if(isFigureEnemyDiagonal(fig)){
                return 1;
            }
            return -1;
        }
        return 0;
    }
    public boolean isRochade(){
        return this.isRochade;
    }
    public boolean isFigureEnemyDiagonal(Figur figur){
        if(figur != null){
            return (figur instanceof Dame || figur instanceof Läufer) && figur.isWhite() != this.isWhite;
        }
        return false;
    }
    public boolean isFigureEnemyHorizontal(Figur figur){
        if(figur != null){
            return (figur instanceof Turm || figur instanceof Dame) && figur.isWhite() != this.isWhite;
        }
        return false;
    }
    public boolean checkDiagonal(int x, int y){
        System.out.println("checking Diagonal");
        Figur figur = null;
        int enemy =0, enemy1=0,enemy2=0,enemy3=0;
        for(int i = 1; i<=7; i++){

            if(enemy == 0){
                enemy += isFigureDiagonal(getFigure(x+i, y+i));
            }
            if(enemy > 0){
                return true;
            }
            if(enemy1 == 0){
                enemy1 += isFigureDiagonal(getFigure(x-i, y+i));
            }
            if(enemy1 > 0){
                return true;
            }
            if(enemy2 == 0){
                enemy2 += isFigureDiagonal(getFigure(x-i, y-i));
            }
            if(enemy2 > 0){
                return true;
            }
            if(enemy3 == 0){
                enemy3 += isFigureDiagonal(getFigure(x+i, y-i));
            }
            if(enemy3 > 0){
                return true;
            }
        }
        return false;
    }
    public Figur getFigure(int x, int y){
        if(x < 0 || y < 0||x>7||y>7) {
            return null;
        }
        return Spielfeld[x][y].getFigur();
    }
    public boolean checkForRochade(int x, int y){
        if(this.moves>0|| !(Spielfeld[x][y].getFigur() instanceof Turm) || Spielfeld[x][y].getFigur().getMoves()>0) return false;
        if( y==0){
            if(getFigure(x,y+1) != null ||getFigure(x,y+2) != null|| getFigure(x,y+3) != null)return false;
        }
        else if (y ==7){
            if(Spielfeld[x][y-1].getFigur() != null || Spielfeld[x][y-2].getFigur() != null)return false;
        }
        return true;
    }
}

/* if(selectedFigur instanceof König){
                    if(selectedFigur.isWhite()){
                        if(source.getx() == 7 && source.gety() == 2&& spielfeld[0][0].getFigur()instanceof Turm){
                            if(spielfeld[7][3].getFigur()==null ){
                                Figur Turm = spielfeld[0][0].getFigur();
                                spielfeld[7][3].addFigur(Turm);
                                Turm.setCords(7,3);
                            }
                        }
                    }
                    else{

                    }
                }
                 **/

