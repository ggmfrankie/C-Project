package main.game;

import main.Main;
import main.game.Figuren.*;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.util.Objects;

public class Feld extends JButton {
    int x, y;
    Figur figur = null;
    boolean isBlack, isMarked;
    public Feld(String text, int x, int y,boolean isBlack) {
        super(text);
        this.x = x;
        this.y = y;
        this.isBlack = isBlack;
    }
    public void addFigur(Figur figur){
        this.figur =figur;
    }
    public Figur getFigur(){
        return this.figur;
    }
    public void setMarked(boolean isMarked){
        this.isMarked = isMarked;
        if(isMarked){
            this.setBackground(Color.ORANGE);
        }
    }
    public boolean getMarked(){
        return this.isMarked;
    }
    public int getx(){
        return this.x;
    }
    public int gety(){
        return this.y;
    }
    public void updateApperarance(){
        if(figur != null){
            if(figur.isWhite()){
                if(figur instanceof Bauer){
                    this.setIcon(createImageIcon("/w_pawn_1x_ns.png"));
                }
                if(figur instanceof Springer){
                    this.setIcon(createImageIcon("/w_knight_1x_ns.png"));
                }
                if(figur instanceof Läufer){
                    this.setIcon(createImageIcon("/w_bishop_1x_ns.png"));
                }
                if(figur instanceof Turm){
                    this.setIcon(createImageIcon("/w_rook_1x_ns.png"));
                }
                if(figur instanceof Dame){
                    this.setIcon(createImageIcon("/w_queen_1x_ns.png"));
                }
                if(figur instanceof König){
                    this.setIcon(createImageIcon("/w_king_1x_ns.png"));
                }
            }
            else{
                if(figur instanceof Bauer){
                    this.setIcon(createImageIcon("/b_pawn_1x_ns.png"));
                }
                if(figur instanceof Springer){
                    this.setIcon(createImageIcon("/b_knight_1x_ns.png"));
                }
                if(figur instanceof Läufer){
                    this.setIcon(createImageIcon("/b_bishop_1x_ns.png"));
                }
                if(figur instanceof Turm){
                    this.setIcon(createImageIcon("/b_rook_1x_ns.png"));
                }
                if(figur instanceof Dame){
                    this.setIcon(createImageIcon("/b_queen_1x_ns.png"));
                }
                if(figur instanceof König){
                    this.setIcon(createImageIcon("/b_king_1x_ns.png"));
                }
            }
        }
        else{
            this.setIcon(null);
        }
        if(isBlack){
            this.setBackground(Color.darkGray);
        }
        else {
            this.setBackground(Color.lightGray);
        }
    }
    private ImageIcon createImageIcon(String path) {
        java.net.URL imgURL = Main.class.getResource(path);
        if (imgURL != null) {
            try {
                Image img = ImageIO.read(imgURL);
                img = img.getScaledInstance(this.getWidth(), this.getHeight(), Image.SCALE_SMOOTH);
                return new ImageIcon(img);
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            System.err.println("Couldn't find file: " + path);
        }
        return null;
    }
}
