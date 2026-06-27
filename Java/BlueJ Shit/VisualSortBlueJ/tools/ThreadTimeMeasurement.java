package tools;

import gui.GUICallback;
import sortAlgorithms.SortAlgorithm;

/**
 * Class for measuring execution time for sorting algorithms.
 * @author Franz-Josef
 *
 */
public class ThreadTimeMeasurement {

	private Object mutex = new Object();

	private long endTime;

	/**
	 * Measure execution time for given sorting algorithm.
	 * @param sortAlg
	 * @return Thread execution time in milliseconds
	 */
	public static long measureExecutionTime(SortAlgorithm sortAlg) {
		return (new ThreadTimeMeasurement())
				.measureExecutionTimeInternal(sortAlg);
	}

	private long measureExecutionTimeInternal(SortAlgorithm sortAlg) {
		sortAlg.addCallback(new GUICallback() {
			@Override
			public void callback(State state) {
				if (state == GUICallback.State.FINISHED) {
					endTime = System.currentTimeMillis();
					synchronized (mutex) {
						mutex.notify();
					}
				}
			}
		});

		long startTime;
		synchronized (mutex) {
			startTime = System.currentTimeMillis();
			sortAlg.start();
			try {
				mutex.wait();
			} catch (InterruptedException exc) {
			}
		}

		return endTime - startTime;
	}
}
