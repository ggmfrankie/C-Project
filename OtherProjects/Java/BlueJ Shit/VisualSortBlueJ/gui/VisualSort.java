package gui;

import gui.panels.SubPanel;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import sortAlgorithms.AlgorithmEnum;
import sortAlgorithms.SortAlgorithm;
import sortAlgorithms.stepHandlers.InteractiveStepHandler;
import controller.Controller;

/**
 * This is the main window for VisualSort. Run the main method of this class to
 * start the application.
 * 
 * @author Franz-Josef
 */
public class VisualSort extends JFrame implements ActionListener, GUICallback {

	private static final long serialVersionUID = 1L;
	private JSplitPane jContentPane = null;
	private JButton stepButton = null;
	private SortPanel sortPanel = null;
	private JPanel yPanel = null;
	private JButton playButton = null;
	private JButton restartButton = null;
	private JPanel buttonPanel = null;
	private SourceCodeListing jSourceCodeList = null;
	private JButton pauseButton = null;

	private VisualSortOptionsDialog optionsDialog;

	private Controller controller;

	/**
	 * This is the default constructor
	 */
	public VisualSort(Controller controller) {
		super();
		this.controller = controller;
		initialize();
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(800, 600);
		this.setContentPane(getJContentPane());
		this.setTitle("Visual Sort");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private Container getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
					getYPanel(), getJSourceCodeList());
			jContentPane.setDividerLocation(0.6d);
			jContentPane.setResizeWeight(0.6d);
		}
		return jContentPane;
	}

	/**
	 * This method initializes stepButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getStepButton() {
		if (stepButton == null) {
			stepButton = new JButton("Einzelschritt");
			stepButton.addActionListener(this);
			stepButton.setEnabled(false);
		}
		return stepButton;
	}

	/**
	 * This method initializes sortPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private SortPanel getSortPanel() {
		if (sortPanel == null) {
			sortPanel = new SortPanel();
			// sortPanel.setSize(640, 480);
			// sortPanel.setLayout(new GridBagLayout());
		}
		return sortPanel;
	}

	/**
	 * This method initializes yPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getYPanel() {
		if (yPanel == null) {
			yPanel = new JPanel();
			yPanel.setLayout(new BoxLayout(yPanel, BoxLayout.Y_AXIS));
			yPanel.add(getSortPanel(), null);
			yPanel.add(getButtonPanel(), null);
		}
		return yPanel;
	}

	/**
	 * This method initializes playButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getPlayButton() {
		if (playButton == null) {
			playButton = new JButton("bis Ende ausführen");
			playButton.addActionListener(this);
			playButton.setEnabled(false);
		}
		return playButton;
	}

	/**
	 * This method initializes restartButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getRestartButton() {
		if (restartButton == null) {
			restartButton = new JButton("Neustart");
			restartButton.addActionListener(this);
		}
		return restartButton;
	}

	@Override
	public void actionPerformed(ActionEvent event) {
		Object source = event.getSource();
		if (source == restartButton) {
			getOptionsDialog().setVisible(true);
			if (optionsDialog.OKPressed()) {
				AlgorithmEnum algo = optionsDialog.getAlgorithm();
				this.setTitle("Visual Sort - " + algo);
				SortAlgorithm sortAlg = controller.loadSortAlgorithm(algo);
				sortAlg.setAnimationHandler(sortPanel);
				sortAlg.setSourceCodeHandler(jSourceCodeList);
				sortAlg.setSetStepHandler(new InteractiveStepHandler(sortAlg));
				sortAlg.addCallback(this);

				jSourceCodeList.loadSourceCode(sortAlg.getSourceCode());

				int size = optionsDialog.getProblemSize();
				if(size<=0) {
					JOptionPane.showMessageDialog(null,
							"Kein gültiger Wert für Feldgröße angegeben.", "VisualSort",
							JOptionPane.ERROR_MESSAGE);
					return;
				}
				int[] elems = controller.createArray(optionsDialog
						.getArrayCreationType(), size);

				if (elems != null) {
					// operation was not user-cancelled.
					sortPanel.setElems(elems);
					playButton.setEnabled(true);
					stepButton.setEnabled(true);
				}
			}
		} else if (source == playButton) {
			controller.play();
			playButton.setEnabled(false);
			stepButton.setEnabled(false);
			restartButton.setEnabled(false);
			pauseButton.setEnabled(true);
		} else if (source == stepButton) {
			controller.step();
		} else if (source == pauseButton) {
			controller.pause();
			pauseButton.setEnabled(false);
		}
	}

	/**
	 * Callback from sortAlg. May be executed in sortAlg thread context.
	 */
	@Override
	public void callback(final GUICallback.State state) {
		/* Callback occurs in sorting thread.
		 * Invoke the following code on the event dispatching thread to
		 * prevent concurrent GUI modification:
		 */
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				boolean isPaused = (state == GUICallback.State.PAUSED);

				restartButton.setEnabled(true);
				playButton.setEnabled(isPaused);
				stepButton.setEnabled(isPaused);
				pauseButton.setEnabled(false);

				if (!isPaused) {
					JOptionPane.showMessageDialog(VisualSort.this,
							"Algorithmus beendet.", "Visual Sort",
							JOptionPane.OK_OPTION);
				}
			}
		});
	}

	/**
	 * This method initializes buttonPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getButtonPanel() {
		if (buttonPanel == null) {
			buttonPanel = new SubPanel("Algorithmus - Playback");
			buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
			buttonPanel.add(getRestartButton(), null);
			buttonPanel.add(getStepButton(), null);
			buttonPanel.add(getPlayButton(), null);
			buttonPanel.add(getPauseButton(), null);
			buttonPanel.add(Box.createHorizontalGlue());
		}
		return buttonPanel;
	}

	private VisualSortOptionsDialog getOptionsDialog() {
		if (optionsDialog == null) {
			optionsDialog = new VisualSortOptionsDialog(this);
		}
		return optionsDialog;
	}

	/**
	 * This method initializes jSourceCodeList
	 * 
	 * @return javax.swing.JList
	 */
	private JList getJSourceCodeList() {
		if (jSourceCodeList == null) {
			jSourceCodeList = new SourceCodeListing();
			jSourceCodeList.setPreferredSize(new Dimension(250, getHeight()));
		}
		return jSourceCodeList;
	}

	/**
	 * This method initializes jPauseButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getPauseButton() {
		if (pauseButton == null) {
			pauseButton = new JButton("anhalten");
			pauseButton.addActionListener(this);
			pauseButton.setEnabled(false);
		}
		return pauseButton;
	}

	/**
	 * This method runs the VisualSort application.
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception exc) {
			System.err.println("No such LookAndFeel.");
		}
		new VisualSort(new Controller()).setVisible(true);
	}
}
