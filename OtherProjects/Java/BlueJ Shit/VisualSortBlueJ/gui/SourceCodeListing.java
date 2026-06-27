package gui;

import java.awt.Color;
import java.awt.Font;

import javax.swing.AbstractListModel;
import javax.swing.JList;

import sortAlgorithms.opHandlers.SourceCodeView;
import tools.RALColors;

/**
 * This class extends JList for displaying lines of source code.
 * One line at a time can be highlighted.
 * @author Franz-Josef
 *
 */
public class SourceCodeListing extends JList implements SourceCodeView {

	private static final long serialVersionUID = 1L;

	public SourceCodeListing() {
		super();
		super.setEnabled(false);
		super.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
		super.setForeground(Color.BLACK);
		super.setBackground(Color.WHITE);
		super.setSelectionBackground(RALColors.SCHWEFELGELB);
		super.setSelectionForeground(Color.BLACK);
		super.setPrototypeCellValue("AaBbCc");
	}

	/**
	 * Populates the object with the given lines of source code.
	 */
	@Override
	public void loadSourceCode(final String[] sourceCode) {
		super.setModel(new AbstractListModel() {
			private static final long serialVersionUID = 1L;

			public int getSize() {
				return sourceCode.length;
			}

			public Object getElementAt(int i) {
				return sourceCode[i];
			}
		});
	}

	/**
	 * Highlights the given line.
	 */
	@Override
	public void setSourceCodeLine(int line) {
		super.setSelectedIndex(line);
	}
}
