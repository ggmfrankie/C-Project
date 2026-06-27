package gui.panels;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.border.TitledBorder;

/**
 * This class represents JPanels with a title and a rounded rectangle as a
 * border.
 * 
 * @author Franz-Josef
 * 
 */
public class SubPanel extends JPanel {
	private static final long serialVersionUID = 1L;

	public SubPanel(String title) {
		super();
		setBorder(BorderFactory.createTitledBorder(null, title,
				TitledBorder.DEFAULT_JUSTIFICATION,
				TitledBorder.DEFAULT_POSITION, null, null));
	}
}
