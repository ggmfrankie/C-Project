package sortAlgorithms.stepHandlers;

import gui.GUICallback;
import sortAlgorithms.SortAlgorithm;
import tools.Tools;

/**
 * Step handler for sorting algorithms.
 * Supports pausing and stepping.
 * @author Franz-Josef
 */
public class InteractiveStepHandler implements StepHandler {
	private SortAlgorithm sortAlg;

	private boolean pauseRequested;

	private boolean singleStepMode;

	public InteractiveStepHandler(SortAlgorithm sortAlg) {
		this.sortAlg = sortAlg;
		pauseRequested = false;
		singleStepMode = false;
	}

	@Override
	public void setSingleStepMode(boolean isSingleStep) {
		singleStepMode = isSingleStep;
	}

	@Override
	public void pause() {
		pauseRequested = true;
		singleStepMode = true;
	}

	@Override
	public void stepCompleted() {
		if (singleStepMode) {
			try {
				synchronized (sortAlg) {
					if (pauseRequested) {
						sortAlg.callback(GUICallback.State.PAUSED);
						pauseRequested = false;
					}
					sortAlg.wait();
				}
			} catch (InterruptedException e) {
			}
		} else {
			Tools.sleep(500);
		}
	}
}
