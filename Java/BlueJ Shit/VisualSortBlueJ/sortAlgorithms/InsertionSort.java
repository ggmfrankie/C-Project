package sortAlgorithms;

/**
 * This is the implementation of InsertionSort.
 * @author Franz-Josef
 *
 */
public class InsertionSort extends SortAlgorithm {

	private static final String[] SOURCE_CODE = new String[] {
			"for (int j = 1; j < elems.length; j++) {",
			"  int key = elems[j];",
			"  int i = j - 1;",
			"  while (i >= 0 && elems[i] > key) {",
			"    elems[i + 1] = elems[i];",
			"    i = i - 1;", "  }",
			"  elems[i + 1] = key;",
			"}" };

	@Override
	public String[] getSourceCode() {
		return SOURCE_CODE;
	}

	@Override
	public void run() {
		aniHandler.highlightRange(0, 1);
		setSourceCodeLine(0);
		for (int j = 1; j < elems.length; j++) {
			aniHandler.setPointer("j", j);

			setSourceCodeLine(1);
			int key = elems[j];
			aniHandler.createTempArray(j, 1, "key");
			aniHandler.loadItemIntoTempArray(j);
			setSourceCodeLine(2);
			int i = j - 1;
			aniHandler.setPointer("i", i);

			setSourceCodeLine(3);
			while (i >= 0 && elems[i] > key) {
				setSourceCodeLine(4);
				aniHandler.moveAnimation(i, i+1);
				elems[i + 1] = elems[i];
				setSourceCodeLine(5);
				i = i - 1;
				aniHandler.setPointer("i", i);
				setSourceCodeLine(3);
			}
			setSourceCodeLine(7);
			elems[i + 1] = key;
			aniHandler.loadItemFromTempArray(0, i+1);

//			aniHandler.highlight(j, j);
//			aniHandler.shift(j, i + 1);
//			aniHandler.highlightOff();
			aniHandler.highlightRange(0, j + 1);

			aniHandler.removePointer("i");
			aniHandler.removeTempArray();
			setSourceCodeLine(0);
		}
		aniHandler.removePointer("j");
		algorithmEnd();
	}

}
