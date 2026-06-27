package gui;

/**
 * This class represents ranges of items (i. e. for highlighting them somehow)
 * as used in SortPanel.
 * @author Franz-Josef
 *
 */
public class Range {
	/**
	 * Start of the range, inclusive
	 */
	public int rangeBegin;
	
	/**
	 * End of the range, exclusive (!)
	 * Usage as String.substring(a, b);
	 */
	public int rangeAfterEnd;

	public Range(int rangeBegin, int rangeAfterEnd) {
		this.rangeBegin = rangeBegin;
		this.rangeAfterEnd = rangeAfterEnd;
	}
}