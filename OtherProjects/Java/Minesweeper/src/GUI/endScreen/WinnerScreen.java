package GUI.endScreen;

import game.Speicher;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class WinnerScreen {
    public JPanel main;
    private JTextField youWonTextField;
    private JButton continueButton;
    private JButton continueButton1;





    public WinnerScreen() {
    continueButton.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            Speicher.getFrameForGame().dispose();
            Speicher.startFrame();
            Speicher.getFrameForWin().dispose();
        }
    });
        continueButton1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Speicher.getFrameForGame().dispose();
                int column = Speicher.game().getMaxColumn()+1;
                int row = Speicher.game().getMaxRow()+1;
                int bombCount = Speicher.game().getBombCount();
                Speicher.init(column, row);
                Speicher.game().placeBombs(bombCount);
                Speicher.game().CountBombs();
                Speicher.getFrameForWin().dispose();
            }
        });
    }
}
