package GUI.endScreen;

import game.Speicher;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class MainMenu {


    public JPanel mainPanel;
    private JTextField Columns;
    private JTextField Rows;
    private JButton startButton;
    private JTextField BombCount;


    public MainMenu() {

    startButton.addActionListener(e -> {
        if (!Speicher.invalid()) {
            int columns = getNumber(Columns);
            int rows = getNumber(Rows);
            int bombCount = getNumber(BombCount);

            if (columns > 1 && rows > 1 && bombCount > 1 && bombCount<= columns*rows) {
                Speicher.init(columns, rows);
                Speicher.game().placeBombs(bombCount);
                Speicher.game().CountBombs();
                Speicher.getFrameForStart().dispose();

            } else {
                invalid();
            }
        }
    });

}
    public void invalid(){
        Speicher.invalidIs(true);
        Speicher.invalidFrame();
    }


    public int getNumber(JTextField a){
        try {
            int row = Integer.parseInt(a.getText());
            return Math.max(row, 1);

        } catch (NumberFormatException er) {
            return 1;
        }
    }
}
