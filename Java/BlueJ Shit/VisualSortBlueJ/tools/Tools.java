package tools;

/**
 * Static helper methods.
 * @author Franz-Josef
 *
 */
public class Tools {
	
	/**
	 * Sleep the given time (in milliseconds).
	 * @param ms
	 */
	public static void sleep(long ms) {
		try {
			Thread.sleep(ms);
		} catch (InterruptedException e) {
		}
	}
}
