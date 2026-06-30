package GUI.endScreen;

import game.Speicher;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class invalid {
    public JPanel main;
    private JButton OKButton;



    public invalid() {
    OKButton.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            Speicher.invalidIs(false);
            Speicher.getFrameForInvalid().dispose();
        }
    });
}
}
