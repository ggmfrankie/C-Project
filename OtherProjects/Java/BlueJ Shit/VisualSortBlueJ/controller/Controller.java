package controller;

import gui.ArrayCreationEnum;
import sortAlgorithms.AlgorithmEnum;
import sortAlgorithms.SortAlgorithm;

/**
 * Controller class for VisualSort
 * @author Franz-Josef
 *
 */
public class Controller {

	private int[] elems;
	private SortAlgorithm sortAlg;

	public Controller() {
		// elems = IntArrayTools.createPermutation(5);
		// sortAlg = AlgorithmEnum.values()[0].newAlgorithmInstance();
	}

	/** 
	 * Start execution of sort algorithm.
	 */
	public void play() {
		sortAlg.setSingleStepMode(false);
		resume();
	}

	/**
	 * Make one execution step.
	 */
	public void step() {
		sortAlg.setSingleStepMode(true);
		resume();
	}

	private void resume() {
		synchronized (sortAlg) {
			if (sortAlg.getState() == Thread.State.NEW) {
				sortAlg.start();
			} else if (sortAlg.getState() == Thread.State.WAITING) {
				sortAlg.notify();
			}
		}
	}

	/**
	 * Pause execution.
	 */
	public void pause() {
		sortAlg.pause();
	}

	/**
	 * Instantiate the given sort algorithm.
	 * @param algorithm
	 * @return
	 */
	public SortAlgorithm loadSortAlgorithm(AlgorithmEnum algorithm) {
		sortAlg = algorithm.newAlgorithmInstance();
		sortAlg.setElems(elems);
		return sortAlg;
	}

	public SortAlgorithm getSortAlgorithm() {
		return sortAlg;
	}

	/**
	 * Creates array with given array creation method.
	 * Returns null if operation was cancelled by user.
	 * 
	 * @param arrayType
	 * @param size
	 * @return
	 */
	public int[] createArray(ArrayCreationEnum arrayType, int size) {
		int[] elems = arrayType.create(size);
		if (elems != null) {
			this.elems = elems;
			sortAlg.setElems(elems);
		}
		return elems;
	}

}
