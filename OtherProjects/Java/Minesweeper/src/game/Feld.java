package game;

import javax.swing.*;
import java.awt.*;

public class Feld extends JButton {
    int bombCount, row, column;
    boolean isBomb, isUncovered, isMarked;
    public Feld(String text, int row, int column) {
        super(text);
        this.bombCount = 0;
        this.isBomb = false;
        this.isUncovered = false;
        this.row = row;
        this.column = column;
    }
    public int getBombCount() {
        return this.bombCount;
    }
    public boolean isBomb() {
        return this.isBomb;
    }
    public int isBombInt(){
        int yes = 0;
        if(this.isBomb){yes = 1;}
        return yes;
    }
    public boolean isMarked() {
        return this.isMarked;
    }
    public void setMarked(boolean isMarked) {
        this.isMarked =isMarked;
    }
    public void setBombCount(int count) {
        this.bombCount = count;
    }
    public int getColumn() {
        return this.column;
    }
    public int getRow (){
        return this.row;
    }
    public void setBomb() {
        this.isBomb = true;
    }
    public boolean isUncovered() {
        return isUncovered;
    }

    public void setUncovered() {
        isUncovered = true;
        updateButtonAppearance(); // Update the button appearance when uncovered status changes
    }


    private void updateButtonAppearance() {
        if (isUncovered) {
            if (isBomb) {
                setText("B");
                setBackground(Color.red);
            } else {
                if(bombCount == 0) {
                    setText("");
                }else {
                    setText(Integer.toString(bombCount));
                }
            }
            setBackground(Color.LIGHT_GRAY);
        } else {
            setText(" ");
            setBackground(null);
        }
    }
}
