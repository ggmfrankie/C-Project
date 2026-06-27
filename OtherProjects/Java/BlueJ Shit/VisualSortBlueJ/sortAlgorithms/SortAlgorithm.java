package sortAlgorithms;

import gui.GUICallback;

import java.util.LinkedList;
import java.util.List;

import sortAlgorithms.opHandlers.AnimationHandler;
import sortAlgorithms.opHandlers.NoAnimationHandler;
import sortAlgorithms.opHandlers.NoSourceCodeView;
import sortAlgorithms.opHandlers.SourceCodeView;
import sortAlgorithms.stepHandlers.NoStepHandler;
import sortAlgorithms.stepHandlers.StepHandler;

/**
 * This is the superclass of all sorting algorithms. The subclasses should
 * implement their algorithm by overriding the run() method.
 * 
 * @author Franz-Josef
 * 
 */
public class SortAlgorithm extends Thread {
	private List<GUICallback> callbacks;
	protected int[] elems;
	protected AnimationHandler aniHandler;
	protected StepHandler stepHandler;
	private SourceCodeView sourceCodeHandler;

	/**
	 * Instantiates an sorting algorithm with
	 * no step handler, no source code view and no animation handler.  
	 */
	protected SortAlgorithm() {
		aniHandler = new NoAnimationHandler();
		sourceCodeHandler = new NoSourceCodeView();
		stepHandler = new NoStepHandler();
		callbacks = new LinkedList<GUICallback>();
	}

	public SortAlgorithm(int[] elems) {
		this();
		this.elems = elems;
	}

	protected void swap(int i, int j) {
		int temp = elems[i];
		elems[i] = elems[j];
		elems[j] = temp;
	}

	public int[] getElems() {
		return elems;
	}

	public void setElems(int[] elems) {
		this.elems = elems;
	}

	public void setAnimationHandler(AnimationHandler aniHandler) {
		this.aniHandler = aniHandler;
	}

	public void setSourceCodeHandler(SourceCodeView sourceCodeHandler) {
		this.sourceCodeHandler = sourceCodeHandler;
	}

	public void setSetStepHandler(StepHandler stepHandler) {
		this.stepHandler = stepHandler;
	}

	public void setSingleStepMode(boolean isSingleStep) {
		stepHandler.setSingleStepMode(isSingleStep);
	}

	public void pause() {
		stepHandler.pause();
	}

	/**
	 * Sets the new source code line and signals stepCompleted()
	 * to the step handler.
	 * @param line
	 */
	protected void setSourceCodeLine(int line) {
		sourceCodeHandler.setSourceCodeLine(line);
		stepHandler.stepCompleted();
	}

	/**
	 * Called by subclasses then the algorithm finishes.
	 */
	protected void algorithmEnd() {
		aniHandler.finished();
		callback(GUICallback.State.FINISHED);
	}

	/**
	 * Perform registered callbacks.
	 * This method is called when algorithm gets paused or finished. 
	 * @param state
	 */
	public void callback(GUICallback.State state) {
		for (GUICallback callback : callbacks) {
			callback.callback(state);
		}
	}

	/**
	 * Register a callback.
	 * @param callback
	 */
	public void addCallback(GUICallback callback) {
		callbacks.add(callback);
	}

	public String[] getSourceCode() {
		return new String[] {};
	}

}
