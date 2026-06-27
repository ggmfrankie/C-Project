package gui;

import gui.panels.SubPanel;

import java.awt.Frame;

import javax.swing.BoxLayout;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SpringLayout;

import sortAlgorithms.AlgorithmEnum;
import tools.SpringUtilities;

/**
 * Options dialog (modal!) for runtime measurement. Sets itself invisible when
 * clicking OK or otherwise closing the dialog. Then, the settings can be
 * queried by the get... methods. Note: The user canceled the dialog iff
 * OKPressed() returns false!
 * 
 * @author Franz-Josef
 */
public class RTMAllAlgosOptionsDialog extends OptionsDialog {

	private static final long serialVersionUID = 1L;

	private JPanel algPanel = null;
	private JComboBox algorithmComboBox = null;
	private JPanel dataPanel = null;
	private JLabel dataTypeLabel = null;
	private JComboBox dataTypeComboBox = null;

	/**
	 * This is the default constructor
	 */
	public RTMAllAlgosOptionsDialog(Frame owner) {
		super(owner);
		initialize();
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		JPanel jContentPane = getJContentPane();
		jContentPane.setLayout(new BoxLayout(jContentPane, BoxLayout.Y_AXIS));
		jContentPane.add(getAlgPanel());
		jContentPane.add(getDataPanel());
		jContentPane.add(getJOKButton());
		this.pack();
	}

	/**
	 * This method initializes algPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getAlgPanel() {
		if (algPanel == null) {
			algPanel = new SubPanel("Algorithmus");
			algPanel.setLayout(new BoxLayout(algPanel, BoxLayout.X_AXIS));
			algPanel.add(getAlgorithmComboBox(), null);
		}
		return algPanel;
	}

	/**
	 * This method initializes algorithmComboBox
	 * 
	 * @return javax.swing.JComboBox
	 */
	private JComboBox getAlgorithmComboBox() {
		if (algorithmComboBox == null) {
			algorithmComboBox = new JComboBox(AlgorithmEnum.values());
			// algorithmComboBox.addActionListener(this);
		}
		return algorithmComboBox;
	}

	/**
	 * This method initializes dataTypeComboBox
	 * 
	 * @return javax.swing.JComboBox
	 */
	private JComboBox getDataTypeComboBox() {
		if (dataTypeComboBox == null) {
			dataTypeComboBox = new JComboBox(ArrayCreationEnum
					.valuesWithoutUserdef());
		}
		return dataTypeComboBox;
	}

	/**
	 * This method initializes dataPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getDataPanel() {
		if (dataPanel == null) {
			dataTypeLabel = new JLabel();
			dataTypeLabel.setText("Art der Elemente:");
			dataPanel = new SubPanel("Feld");
			dataPanel.setLayout(new SpringLayout());
			dataPanel.add(dataTypeLabel, null);
			dataPanel.add(getDataTypeComboBox(), null);
			SpringUtilities.makeCompactGrid(dataPanel, 1, 2, 5, 5, 5, 5);
		}
		return dataPanel;
	}

	/**
	 * Returns selected algorithm.
	 * 
	 * @return
	 */
	public AlgorithmEnum getAlgorithm() {
		return ((AlgorithmEnum) algorithmComboBox.getSelectedItem());
	}

	/**
	 * Returns selected type for array creation.
	 * 
	 * @return
	 */
	public ArrayCreationEnum getArrayCreationType() {
		return ((ArrayCreationEnum) dataTypeComboBox.getSelectedItem());
	}
}
