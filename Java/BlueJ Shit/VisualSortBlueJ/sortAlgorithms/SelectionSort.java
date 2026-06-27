package sortAlgorithms;

/**
 * This is the implementation of SelectionSort.
 * @author Franz-Josef
 *
 */
public class SelectionSort extends SortAlgorithm {

	private static final String[] SOURCE_CODE = new String[] {
			"for (int i = 0; i < elems.length - 1; i++) {",
			"  int minpos = minimumPosition(i);",
			"  swap(minpos, i);",
			"}" };

	@Override
	public String[] getSourceCode() {
		return SOURCE_CODE;
	}

	private int minimumPosition(int from) {
		int minpos = from;
		for (int i = from + 1; i < elems.length; i++) {
			if (elems[i] < elems[minpos]) {
				minpos = i;
			}
		}
		return minpos;
	}

	@Override
	public void run() {
		setSourceCodeLine(0);
		for (int i = 0; i < elems.length - 1; i++) {
			aniHandler.setPointer("i", i);

			setSourceCodeLine(1);
			int minpos = minimumPosition(i);
			aniHandler.setPointer("minpos", minpos);

			aniHandler.highlight(minpos, i);
			setSourceCodeLine(2);
			aniHandler.swapAnimation(minpos, i);
			swap(minpos, i);
			aniHandler.highlightOff();
			aniHandler.highlightRange(0, i + 1);

			aniHandler.removePointer("minpos");
			setSourceCodeLine(0);
		}
		aniHandler.removePointer("i");
		aniHandler.highlightRange(0, elems.length);
		algorithmEnd();
	}

}
