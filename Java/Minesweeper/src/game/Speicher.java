package game;

import GUI.endScreen.LoserScreen;
import GUI.endScreen.MainMenu;
import GUI.endScreen.WinnerScreen;
import GUI.endScreen.invalid;

import javax.swing.*;

public class Speicher {
    static Game game;
    static JFrame frameForInvalid, frameForStart, frameForLoose, frameForWin, frameForGame;
    static boolean invalid;
    public static boolean invalid() {
        return invalid;
    }
    public static void invalidIs(boolean invalid) {
        Speicher.invalid = invalid;
    }
    public static JFrame generateFrameForGame(){
        return frameForGame = new JFrame("Game");
    }
    public static JFrame getFrameForGame(){
        return frameForGame;
    }
    public static void init(int columns, int rows){
        game = new Game(columns,rows);
    }
    public static void startFrame(){
        frameForStart = new JFrame("MainMenu");
        frameForStart.setContentPane(new MainMenu().mainPanel);
        frameForStart.setLocationRelativeTo(null);
        frameForStart.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frameForStart.pack();
        frameForStart.setVisible(true);
    }
    public static void invalidFrame(){
        frameForInvalid = new JFrame("InvalidFrame");
        frameForInvalid.setContentPane(new invalid().main);
        frameForInvalid.setLocationRelativeTo(null);
        frameForInvalid.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frameForInvalid.pack();
        frameForInvalid.setVisible(true);
    }
    public static void looseFrame(){
        frameForLoose = new JFrame("LoserScreen");
        frameForLoose.setContentPane(new LoserScreen().main);
        frameForLoose.setLocationRelativeTo(null);
        frameForLoose.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frameForLoose.pack();
        frameForLoose.setVisible(true);
    }
    public static void winFrame(){
        frameForWin = new JFrame("WinnerScreen");
        frameForWin.setContentPane(new WinnerScreen().main);
        frameForWin.setLocationRelativeTo(null);
        frameForWin.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frameForWin.pack();
        frameForWin.setVisible(true);
    }


    public static JFrame getFrameForInvalid(){
        return frameForInvalid;
    }
    public static JFrame getFrameForStart(){return frameForStart;}
    public static JFrame getFrameForLoose(){return frameForLoose;}
    public static JFrame getFrameForWin(){return frameForWin;}
    public static Game game(){
        return game;
    }

}

