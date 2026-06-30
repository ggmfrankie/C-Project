package GUI.endScreen;

import game.Speicher;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class LoserScreen {
    public JPanel main;
    private JButton NewGame;
    private JButton againButton1;


    public LoserScreen() {
    NewGame.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            // Dispose of the frames
            Speicher.getFrameForLoose().dispose();
            Speicher.getFrameForGame().dispose();

            // Start a new frame after disposing of the previous ones
            SwingUtilities.invokeLater(Speicher::startFrame);
        }
    });
        againButton1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {


                Speicher.getFrameForGame().dispose();
                Speicher.game().gameIs(false);
                int column = Speicher.game().getMaxColumn()+1;
                int row = Speicher.game().getMaxRow()+1;
                int bombCount = Speicher.game().getBombCount();
                Speicher.init(column, row);
                Speicher.game().placeBombs(bombCount);
                Speicher.game().CountBombs();
                Speicher.getFrameForLoose().dispose();
            }
        });
    }
}
