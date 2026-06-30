package GUI;

import game.Feld;
import game.Game;
import game.Speicher;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import static javax.swing.text.StyleConstants.setBackground;

public class ButtonArrayExample {
    Feld[][] buttons;
    public ButtonArrayExample(int columns, int rows) {
        // Create the main frame

        JFrame frame = Speicher.generateFrameForGame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(rows * 45, columns * 45);

        // Create a panel with a grid layout
        JPanel panel = new JPanel(new GridLayout(columns, rows));

        // Create an array of buttons
        buttons = new Feld[columns][rows];
        for (int i = 0; i < columns; i++) {
            for (int j = 0; j < rows; j++) {
                buttons[i][j] = new Feld("", i, j);
                buttons[i][j].addActionListener(new ButtonClickListener());
                buttons[i][j].addMouseListener(new RightClickListener());
                panel.add(buttons[i][j]);
            }
        }

        // Add the panel to the frame
        frame.add(panel);

        // Make the frame visible
        frame.setVisible(true);
    }

    public Feld[][] getButtons() {
        return buttons;
    }

    // ActionListener for the buttons
    static class ButtonClickListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            Feld source = (Feld) e.getSource();

            // Qualify isGame() with Speicher.game()
            if (Speicher.game().isGame() && !source.isMarked() && !source.isUncovered()) {

                Speicher.game().uncoverBombs(source.getRow(), source.getColumn());

            } else if (Speicher.game().isGame() && source.isUncovered()) {
                Speicher.game().checkIfMarkedEqualsBombCount(source.getRow(), source.getColumn());
            }
        }
    }

    // MouseAdapter for right-click events
    static class RightClickListener extends MouseAdapter {
        @Override
        public void mouseClicked(MouseEvent e) {
            Feld source = (Feld) e.getSource();
            if(isGame() && !source.isUncovered()){
                if (SwingUtilities.isRightMouseButton(e)) {


                    if (source.isMarked()){
                        source.setText("");
                        source.setMarked(false);
                        source.setBackground(null);
                    }
                    else{
                        source.setMarked(true);
                        source.setText("F");
                        source.setBackground(Color.darkGray);
                    }
                }
            }
        }
    }
    public static boolean isGame(){
        return Speicher.game().isGame();
    }
    public static void gameIs(boolean c){
        Speicher.game().gameIs(c);
    }
}