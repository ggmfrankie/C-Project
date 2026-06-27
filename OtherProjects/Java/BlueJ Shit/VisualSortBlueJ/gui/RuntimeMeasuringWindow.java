package gui;

import gui.panels.SubPanel;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.Point2D;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.ListCellRenderer;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import sortAlgorithms.AlgorithmEnum;
import sortAlgorithms.SortAlgorithm;
import tools.RALColors;
import tools.ThreadTimeMeasurement;

/**
 * This class is the main frame for runtime measuring the sort algorithms. Run
 * the main method of this class to start the application.
 * 
 * @author Franz-Josef
 * 
 */
public class RuntimeMeasuringWindow extends JFrame implements ActionListener {

	private static final long serialVersionUID = 1L;

	private Integer[] suggestedArraySizes = { new Integer(100),
			new Integer(1000), new Integer(2000), new Integer(5000),
			new Integer(10000), new Integer(20000) };
	private JPanel jContentPane = null;
	private JPanel jButtonPanel = null;
	private JButton jClearButton = null;
	private JButton jRunAlgButton = null;
	private GraphPanel graph = null;
	private JList jAlgoLegend = null;
	private JList jArrayTypeLegend = null;
	private JPanel jLegendPanel = null;
	private JButton jRunAllButton = null;

	private VisualSortOptionsDialog singleTestDialog;
	private RTMAllAlgosOptionsDialog allTestsDialog;

	/**
	 * Re-enables buttons after terminating a thread. To be called in
	 * event-dispatching thread.
	 * 
	 * @author Franz-Josef
	 * 
	 */
	private class ButtonEnabler implements Runnable {
		@Override
		public void run() {
			jRunAlgButton.setText("Einzeltest");
			jRunAlgButton.setEnabled(true);
			jRunAllButton.setText("Testreihe");
			jRunAllButton.setEnabled(true);
		}
	}

	/**
	 * This is the constructor
	 * 
	 * @param algorithmEnum
	 */
	public RuntimeMeasuringWindow() {
		super();
		initialize();

		singleTestDialog = new VisualSortOptionsDialog(this,
				suggestedArraySizes);
		allTestsDialog = new RTMAllAlgosOptionsDialog(this);
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(640, 480);
		this.setContentPane(getJContentPane());
		this.setTitle("Laufzeitmessung Sortieralgorithmen");
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
	}

	/**
	 * This method initializes jButtonPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJButtonPanel() {
		if (jButtonPanel == null) {
			jButtonPanel = new JPanel();
			jButtonPanel
					.setLayout(new BoxLayout(jButtonPanel, BoxLayout.X_AXIS));
			jButtonPanel.add(getJClearButton(), new GridBagConstraints());
			jButtonPanel.add(getJRunAlgButton(), new GridBagConstraints());
			jButtonPanel.add(getJRunAllButton(), null);
		}
		return jButtonPanel;
	}

	/**
	 * This method initializes jClearButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJClearButton() {
		if (jClearButton == null) {
			jClearButton = new JButton("löschen");
			jClearButton.addActionListener(this);
		}
		return jClearButton;
	}

	/**
	 * This method initializes jRunAlgButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJRunAlgButton() {
		if (jRunAlgButton == null) {
			jRunAlgButton = new JButton("testen");
			jRunAlgButton.addActionListener(this);
		}
		return jRunAlgButton;
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new JPanel();
			jContentPane.setLayout(new BorderLayout());
			jContentPane.add(getJButtonPanel(), BorderLayout.SOUTH);
			jContentPane.add(getGraph(), BorderLayout.CENTER);
			jContentPane.add(getJLegendPanel(), BorderLayout.EAST);
		}
		return jContentPane;
	}

	/**
	 * This method initializes graph
	 * 
	 * @return javax.swing.JPanel
	 */
	private GraphPanel getGraph() {
		if (graph == null) {
			graph = new GraphPanel();
			graph.setLayout(new GridBagLayout());
		}
		return graph;
	}

	/**
	 * Maps an algorithm to a color.
	 * 
	 * @param algo
	 * @return
	 */
	private Color algoToColor(AlgorithmEnum algo) {
		return RALColors.COLORS[algo.ordinal()];
	}

	/**
	 * Maps an ArrayCreationType to a shape.
	 * 
	 * @param arrayType
	 * @return
	 */
	private DataPoint.Shape arrayTypeToShape(ArrayCreationEnum arrayType) {
		return DataPoint.Shape.values()[arrayType.ordinal()];
	}

	@Override
	public void actionPerformed(ActionEvent event) {
		Object source = event.getSource();
		if (source == jClearButton) {
			graph.clearPoints();
		} else if (source == jRunAlgButton) {
			jRunAlgButton.setText("Einzeltest läuft...");
			jRunAlgButton.setEnabled(false);
			jRunAllButton.setEnabled(false);
			(new Thread() {
				@Override
				public void run() {
					singleTestDialog.setVisible(true);
					if (singleTestDialog.OKPressed()) {
						int size = singleTestDialog.getProblemSize();
						if (size > 0) {
							AlgorithmEnum algo = singleTestDialog
									.getAlgorithm();
							if (algo != AlgorithmEnum.BOZO_SORT) {
								SortAlgorithm sortAlg = algo
										.newAlgorithmInstance();
								ArrayCreationEnum arrayType = singleTestDialog
										.getArrayCreationType();
								int[] elems = arrayType.create(size);
								if (elems != null) {
									sortAlg.setElems(elems);

									long time = ThreadTimeMeasurement
											.measureExecutionTime(sortAlg);

									// This call is thread-safe since
									// Graph.addPoint
									// is
									// synched:
									graph.addPoint(size, (int) time,
											algoToColor(algo),
											arrayTypeToShape(arrayType));
								}
							} else {
								JOptionPane
										.showMessageDialog(
												RuntimeMeasuringWindow.this,
												"BozoSort ist nicht empfehlenswert - dieser Algorithmus beendet u. U. nie!",
												"VisualSort",
												JOptionPane.WARNING_MESSAGE);
							}
						} else {
							JOptionPane
									.showMessageDialog(
											null,
											"Kein gültiger Wert für Feldgröße angegeben.",
											"VisualSort",
											JOptionPane.ERROR_MESSAGE);
						}
					}

					// Not thread-safe: -> put in event-dispatching thread:
					SwingUtilities.invokeLater(new ButtonEnabler());
				}
			}).start();

		} else if (source == jRunAllButton) {
			jRunAllButton.setText("Testreihe läuft...");
			jRunAllButton.setEnabled(false);
			jRunAlgButton.setEnabled(false);
			(new Thread() {
				@Override
				public void run() {
					allTestsDialog.setVisible(true);
					if (allTestsDialog.OKPressed()) {
						ArrayCreationEnum arrayCreationType = allTestsDialog
								.getArrayCreationType();
						AlgorithmEnum algo = allTestsDialog.getAlgorithm();
						if (algo != AlgorithmEnum.BOZO_SORT) {
							for (int size = 0; size < 25000; size += 1000) {
								SortAlgorithm sortAlg = algo
										.newAlgorithmInstance();
								sortAlg
										.setElems(arrayCreationType
												.create(size));

								long time = ThreadTimeMeasurement
										.measureExecutionTime(sortAlg);

								// This call is thread-safe since Graph.addPoint
								// is
								// synched:
								graph.addPoint(size, (int) time,
										algoToColor(algo),
										arrayTypeToShape(arrayCreationType));
							}
						} else {
							JOptionPane
									.showMessageDialog(
											RuntimeMeasuringWindow.this,
											"BozoSort ist nicht empfehlenswert - dieser Algorithmus beendet u. U. nie!",
											"VisualSort",
											JOptionPane.WARNING_MESSAGE);

						}
					}
					// Not thread-safe: -> put in event-dispatching thread:
					SwingUtilities.invokeLater(new ButtonEnabler());
				}
			}).start();
		}
	}

	/**
	 * This method initializes jLegendPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJLegendPanel() {
		if (jLegendPanel == null) {
			jLegendPanel = new SubPanel("Legende");
			jLegendPanel
					.setLayout(new BoxLayout(jLegendPanel, BoxLayout.Y_AXIS));
			jLegendPanel.add(getJAlgoLegend());
			jLegendPanel.add(getJArrayTypeLegend());
		}
		return jLegendPanel;
	}

	/**
	 * This method initializes jAlgoLegend
	 * 
	 * @return javax.swing.JList
	 */
	private JList getJAlgoLegend() {
		if (jAlgoLegend == null) {
			jAlgoLegend = new JList();
			jAlgoLegend.setCellRenderer(new ListCellRenderer() {

				@Override
				public Component getListCellRendererComponent(JList list,
						Object value, int index, boolean isSelected,
						boolean cellHasFocus) {

					Component component = (Component) value;
					// component.setBackground(isSelected ? Color.black
					// : Color.white);
					// component.setForeground(isSelected ? Color.white
					// : Color.black);
					return component;
				}
			});

			AlgorithmEnum[] algos = AlgorithmEnum.values();
			JLabel[] listEntries = new JLabel[algos.length];

			for (int i = 0; i < algos.length; i++) {
				JLabel label = new JLabel(algos[i].name());
				label.setOpaque(true);
				label.setBackground(algoToColor(algos[i]));
				listEntries[i] = label;
			}

			jAlgoLegend.setListData(listEntries);
		}
		Dimension maxSize = jAlgoLegend.getMaximumSize();
		jAlgoLegend.setMaximumSize(new Dimension(1000, maxSize.width));
		return jAlgoLegend;
	}

	/**
	 * Class for representing DataPoint shape icons in jArrayTypeLegend
	 * 
	 * @author Franz-Josef
	 * 
	 */
	private static class DataPointIcon extends JPanel {

		private static final long serialVersionUID = 1L;

		private static Point2D MIDDLE_POINT = new Point2D.Float(7, 7);

		private DataPoint.Shape shape;

		public DataPointIcon(DataPoint.Shape shape) {
			this.shape = shape;
			Dimension size = new Dimension(15, 15);
			setMinimumSize(size);
			setMaximumSize(size);
			setPreferredSize(size);
		}

		@Override
		protected void paintComponent(Graphics g) {
			super.paintComponent(g);
			shape.draw((Graphics2D) g, MIDDLE_POINT);
		}
	}

	/**
	 * This method initializes jArrayTypeLegend
	 * 
	 * @return javax.swing.JList
	 */
	private JList getJArrayTypeLegend() {
		if (jArrayTypeLegend == null) {
			jArrayTypeLegend = new JList();
			jArrayTypeLegend.setCellRenderer(new ListCellRenderer() {

				@Override
				public Component getListCellRendererComponent(JList list,
						Object value, int index, boolean isSelected,
						boolean cellHasFocus) {

					Component component = (Component) value;
					// component.setBackground(isSelected ? Color.black
					// : Color.white);
					// component.setForeground(isSelected ? Color.white
					// : Color.black);
					return component;
				}
			});

			ArrayCreationEnum[] arrayTypes = ArrayCreationEnum
					.valuesWithoutUserdef();
			JPanel[] listEntries = new JPanel[arrayTypes.length];

			for (int i = 0; i < arrayTypes.length; i++) {
				JLabel label = new JLabel(arrayTypes[i].toString(), JLabel.LEFT);
				label.setOpaque(true);

				JPanel panel = new JPanel();
				panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));
				panel.add(new DataPointIcon(arrayTypeToShape(arrayTypes[i])));
				// panel.add(Box.createHorizontalStrut(10));
				panel.add(label);
				panel.add(Box.createHorizontalGlue());
				listEntries[i] = panel;
			}

			jArrayTypeLegend.setListData(listEntries);
		}
		return jArrayTypeLegend;
	}

	/**
	 * This method initializes jRunAllButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJRunAllButton() {
		if (jRunAllButton == null) {
			jRunAllButton = new JButton("Testreihe");
			jRunAllButton.addActionListener(this);
		}
		return jRunAllButton;
	}

	/**
	 * This method runs the Runtime Measuring application.
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception exc) {
			System.err.println("No such LookAndFeel.");
		}

		(new RuntimeMeasuringWindow()).setVisible(true);
	}
}
