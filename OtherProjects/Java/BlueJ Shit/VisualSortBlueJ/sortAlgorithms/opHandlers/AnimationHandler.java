package sortAlgorithms.opHandlers;

import gui.Range;

/**
 * This interface serves for the sorting algorithms to delegate the
 * graphical representation of operations.
 * @author Franz-Josef
 *
 */
public interface AnimationHandler {

	/**
	 * Highlights two items.
	 */
	public void highlight(int i, int j);

	/**
	 * Switches highlighting off.
	 */
	public void highlightOff();

	/**
	 * Highlight a range of items (usually completed items).
	 */
	public void highlightRange(int from, int afterTo);

	/**
	 * Adds a range to highlight to an array of ranges. These ranges will be
	 * painted in a semi-transparent RED, so that overlapping ranges can be
	 * seen. This is suitable i. e. for recursion.
	 */
	public Range addRange(int from, int afterTo);

	/**
	 * Removes a range from the array of ranges.
	 */
	public void removeRange(Range leftRange);

	/**
	 * Swaps two items.
	 */
	public void swapAnimation(int i, int j);

	/**
	 * Shifts items. This means: Move item at position "from" to position "to"
	 * and move all items between these two positions one step further. This
	 * method works only when from >= to !
	 * 
	 */
	public void shiftAnimation(int from, int to);

	/**
	 * Copies one item to another position.
	 */
	public void moveAnimation(int from, int to);

	/**
	 * Sets a pointer (an arrow).
	 */
	public void setPointer(String string, int index);

	/**
	 * Removes a pointer.
	 */
	public void removePointer(String string);

	/**
	 * Sets the pointer to the temporary array. A negative value for i removes
	 * an existing pointer.
	 */
	public void setTempPointer(String string, int index);

	/**
	 * Removes the pointer to the temporary array.
	 */
	public void removeTempPointer(String string);

	/**
	 * Creates a temporary array with given name and length. The temp. array is
	 * initially empty and will be displayed above original array position
	 * "offset".
	 */
	public void createTempArray(int offset, int length, String name);

	/**
	 * Copies one array item with given index into the temp. array. In internal
	 * temp. array pointer will be advanced afterwards so that consecutive
	 * method calls will place items into the temp. array at the positions 0, 1,
	 * 2, and so on.
	 */
	public void loadItemIntoTempArray(int index);

	/**
	 * Copies one item from the temp. array with given index into the original
	 * array with given index.
	 */
	public void loadItemFromTempArray(int tempArrayIndex, int arrayIndex);

	/**
	 * Copies the whole temp. array into the original array, starting at the
	 * offset given when createTempArray was called. Afterwards, the temp. array
	 * is removed.
	 */
	public void integrateTempArray();

	/**
	 * Removes the temp. array.
	 */
	public void removeTempArray();

	/**
	 * Causes a repaint. This is needed by some algorithms to display the
	 * correct resulting diagram when finished.
	 */
	public void finished();
}
