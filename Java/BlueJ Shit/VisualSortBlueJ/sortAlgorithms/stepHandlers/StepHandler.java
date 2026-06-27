package sortAlgorithms.stepHandlers;

/**
 * Sorting algorithms use implementations of this interface to control their
 * execution: The algorithms can be paused, executed in steps or executed in one
 * run.
 * 
 * @author Franz-Josef
 */
public interface StepHandler {
	
	/**
	 * Request stepping through the algorithm.
	 * Switches to "execution in one run" if isSingleStep is false.
	 * @param isSingleStep
	 */
	public void setSingleStepMode(boolean isSingleStep);

	/**
	 * Pause algorithm. 
	 */
	public void pause();

	/**
	 * Called by sort algorithms to signal the end of one step.
	 * The sort algorithm will be halted by calling wait() on it.
	 */
	public void stepCompleted();
}
