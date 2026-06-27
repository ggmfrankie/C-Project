package game;
import GUI.ButtonArrayExample;

import javax.swing.*;
import java.awt.*;
import java.util.Random;
public class Game {
    Feld[][] mainFeld;
    ButtonArrayExample Spielfeld;
    int maxColumn, maxRow, bombCount, uncoveredLeft;
    boolean isGame;
    public Game(int columns, int rows){
        Spielfeld = new ButtonArrayExample(columns, rows);
        maxColumn = columns-1;
        maxRow = rows-1;
        uncoveredLeft = rows*columns;
        mainFeld = Spielfeld.getButtons();
        isGame = true;
        bombCount = 0;
        Speicher.invalidIs(false);
    }
    public int getMaxColumn(){
        return this.maxColumn;
    }
    public int getMaxRow(){
        return this.maxRow;
    }
    public int getBombCount(){
        return this.bombCount;
    }
    public void placeBombs(int l){
        bombCount = l;
        for(int i = 0; i < l; i++){
            bombCheck();
        }
    }
    public boolean isGame(){
        return this.isGame;
    }
    public void gameIs(boolean isGame){
        this.isGame = isGame;
    }
    public void bombCheck(){
        Random rand = new Random();
        int column = rand.nextInt(maxColumn+1);
        int row = rand.nextInt(maxRow+1);
        Feld feld = mainFeld[column][row];

        if (feld.isBomb){bombCheck();}
        else{feld.setBomb();}
    }
    public int getBombInt(int column, int row){
        return mainFeld[column][row].isBombInt();
    }
    public void CountBombs(){
        for(int i = 0; i<=maxColumn; i++){

            for(int j = 0; j <= maxRow; j++){
                mainFeld[i][j].setBombCount(bombCount(i, j));
            }
        }
    }
    public int bombCount(int column, int row){
        int count = 0;
        if(row > 0){
            if(column > 0){count += getBombInt(column-1,row-1);
                }
            if(column < maxColumn){count += getBombInt(column+1,row-1);
                }
            count += getBombInt(column,row-1);

        }
        if(row < maxRow){

            if(column > 0){count += getBombInt(column-1,row+1);

            }
            if(column < maxColumn){count += getBombInt(column+1,row+1);
                }
            count += getBombInt(column,row+1);

        }
        if(column > 0){count += getBombInt(column-1,row);
            }
        if(column < maxColumn){count += getBombInt(column+1,row);
            }
        return count;
    }
    /*public void uncoverBombs(int column, int row){
        if(mainFeld[column][row].getBombCount()==0){
            ;if(row > 0){
                if(column > 0){uncoverBombs(column-1,row-1);
                    System.out.println("oben links");}
                if(column < maxColumn){uncoverBombs(column+1,row-1);
                    System.out.println("oben rechts");}
                uncoverBombs(column,row-1);
                System.out.println("oben mitte");
            }
            if(row < maxRow){
                System.out.println(column-1+" "+row+1);
                if(column > 0){uncoverBombs(column-1,row+1);
                    System.out.println("unten links");
                }
                if(column < maxColumn){uncoverBombs(column+1,row+1);
                    System.out.println("unten rechts");}
                uncoverBombs(column,row+1);
                System.out.println("unten mitte");
            }
            if(column > 0){uncoverBombs(column-1,row);
                System.out.println("links mitte");}
            if(column < maxColumn){uncoverBombs(column+1,row);
                System.out.println("rechts mitte");}
        }
        uncoverBombAt(column,row);
    }
    public void uncoverBombAt(int column, int row){
        mainFeld[column][row].setUncovered();
    }
    public void isField finsihed(){

*/
    public int countMarked(int column, int row){
        int marked = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (column+i <= maxColumn && column+i >= 0 && row+j <= maxRow && row+j >= 0){
                        if (mainFeld[column+i][row+j].isMarked()){
                            marked++;
                        }
                    }
                }
            }
            return marked;
    }
    public void checkIfMarkedEqualsBombCount (int column, int row){
        if (countMarked(column, row) == mainFeld[column][row].getBombCount()){
            uncoverSingleBomb(column,row);
        }
    }
    public void uncoverSingleBomb(int column, int row){
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    uncoverBombs(column + i, row + j);
                }
            }
    }
    public void uncoverBombs(int column, int row) {
        if (column < 0 || row < 0 || column > maxColumn || row > maxRow || mainFeld[column][row].isUncovered() || mainFeld[column][row].isMarked) {
            return;
        }



        mainFeld[column][row].setUncovered();
        uncoveredLeft--;
        System.out.println(uncoveredLeft);
        System.out.println();
        System.out.println(bombCount);

        if (mainFeld[column][row].isBomb()&& isGame()) {
            gameIs(false);
            uncoverAll();
            SwingUtilities.invokeLater(Speicher::looseFrame);
            return;
        }
        if(uncoveredLeft <= bombCount && isGame()){
            gameIs(false);
            SwingUtilities.invokeLater(Speicher::winFrame);
            return;
        }

        if (mainFeld[column][row].getBombCount() == 0) {
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    uncoverBombs(column + i, row + j);
                }
            }
        }
    }
    public void uncoverAll() {
        for (int i = 0; i <= maxColumn; i++) {
            for (int j = 0; j <= maxRow; j++) {
                mainFeld[i][j].setUncovered();
                if(mainFeld[i][j].isBomb()){
                    mainFeld[i][j].setBackground(Color.red);
                }
            }
        }
    }
    /*public void uncoverBombs(int column, int row) {
        if(column<0||row<0||column > maxColumn|| row > maxRow||mainFeld[column][row].isUncovered()){return;}
        mainFeld[column][row].setUncovered();
        if(mainFeld[column][row].getBombCount()==0){
            uncoverBombs(column-1,row-1);
            uncoverBombs(column,row-1);
            uncoverBombs(column+1,row-1);

            uncoverBombs(column-1,row+1);
            uncoverBombs(column,row+1);
            uncoverBombs(column+1,row+1);

            uncoverBombs(column+1,row);
            uncoverBombs(column-1,row);

        }
    }
    */

}
