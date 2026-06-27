package tools;

import java.util.Random;

/**
 * This class has only static methods for manipulating int arrays.
 * 
 * @author Franz-Josef
 * 
 */
public class IntArrayTools {
	/**
	 * Swap two items.
	 * 
	 * @param elems
	 * @param i
	 * @param j
	 */
	public static void swap(int[] elems, int i, int j) {
		int temp = elems[i];
		elems[i] = elems[j];
		elems[j] = temp;
	}

	/**
	 * Shifts items. This means: Move item at position "from" to position "to"
	 * and move all items between these two positions one step further.
	 * 
	 * @param elems
	 * @param from
	 * @param to
	 */
	public static void shift(int[] elems, int from, int to) {
		int i = elems[from];
		if (from > to) {
			for (int j = from; j > to; j--) {
				elems[j] = elems[j - 1];
			}
		} else {
			for (int j = from; j < to; j++) {
				elems[j] = elems[j + 1];
			}
		}
		elems[to] = i;
	}

	/**
	 * Create an array "elems" of given size where for all i: elems[i] = i
	 * 
	 * @param size
	 * @return
	 */
	public static int[] createSortedArray(int size) {
		int[] elems = new int[size];
		for (int i = 0; i < size; i++) {
			elems[i] = i;
		}
		return elems;
	}

	public static int[] createReverseSortedArray(int size) {
		int[] elems = new int[size];
		for (int i = 0; i < size; i++) {
			elems[i] = size - 1 - i;
		}
		return elems;
	}

	/**
	 * Creates an array of given size which is a permutation of the numbers 0
	 * ... size-1
	 * 
	 * @param size
	 * @return
	 */
	public static int[] createPermutation(int size) {
		Random random = new Random(System.currentTimeMillis());
		int[] elems = createSortedArray(size);

		// shuffle
		for (int i = 0; i < size; i++) {
			int r = random.nextInt(i + 1);
			int swap = elems[r];
			elems[r] = elems[i];
			elems[i] = swap;
		}
		return elems;
	}

	/**
	 * Creates an array populated with random numbers out of the interval 0 ..
	 * (2*size)-1
	 */
	public static int[] createRandomArray(int size) {

		int[] elems = new int[size];
		Random random = new Random(System.currentTimeMillis());
		for (int i = 0; i < size; i++) {
			elems[i] = random.nextInt(2 * size);
		}
		return elems;
	}
}
