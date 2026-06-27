package gui;

import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.KeyStroke;

/**
 * Options dialog (modal!) for VisualSort. Sets itself invisible when clicking
 * OK or otherwise closing the dialog. Then, the settings can be queried by the
 * get... methods (to be implemented in subclasses). Note: The user canceled the
 * dialog iff OKPressed() returns false!
 * 
 * Usage: Make a subclass; retrieve content pane by calling getJContentPane()
 * and add components. Don't forget to add the OK button. Retrieve it with
 * getJOKButton().
 * 
 * @author Franz-Josef
 */
public class OptionsDialog extends JDialog implements ActionListener {

	private static final long serialVersionUID = 1L;

	private JPanel jContentPane = null;
	private JButton jOKButton = null;
	private boolean okPressed;

	/**
	 * This is the default constructor
	 */
	public OptionsDialog(Frame owner) {
		super(owner, true);
		initialize();
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(300, 200);
		this.setTitle("Optionen");
		this.setContentPane(getJContentPane());
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	protected JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new JPanel();

			InputMap inputMap = jContentPane
					.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
			inputMap.put(KeyStroke.getKeyStroke("ESCAPE"), "ESCAPE");

			Action escAction = new AbstractAction() {
				private static final long serialVersionUID = 1L;

				@Override
				public void actionPerformed(ActionEvent e) {
					okPressed = false;
					OptionsDialog.this.dispose();
				}
			};

			jContentPane.getActionMap().put("ESCAPE", escAction);
		}
		return jContentPane;
	}

	/**
	 * This method initializes jOKButton
	 * 
	 * @return javax.swing.JButton
	 */
	protected JButton getJOKButton() {
		if (jOKButton == null) {
			jOKButton = new JButton("OK");
			jOKButton.addActionListener(this);
			jOKButton.setAlignmentX(0.5f);
		}
		return jOKButton;
	}

	@Override
	public void setVisible(boolean b) {
		okPressed = false;
		getRootPane().setDefaultButton(getJOKButton());
		setLocationRelativeTo(super.getOwner());
		super.setVisible(b);
	}

	@Override
	public void actionPerformed(ActionEvent event) {
		Object source = event.getSource();
		if (source == jOKButton) {
			okPressed = true;
			this.dispose();
		}
	}

	/**
	 * Returns whether the user has closed the dialog by clicking "OK" or
	 * canceling somehow (i. e. closing the window by clicking on "x")
	 * 
	 * @return
	 */
	public boolean OKPressed() {
		return okPressed;
	}
}
