package sortAlgorithms;

/**
 * This is the implementation of BubbleSort.
 * @author Franz-Josef
 *
 */
public class BubbleSort extends SortAlgorithm {

	private static final String[] SOURCE_CODE = new String[] {
			"boolean vertauscht;",
			"int i = elems.length - 1;",
			"do {",
			"  vertauscht = false;",
			"  for (int j = 0; j < i; j++) {",
			"    if (elems[j] > elems[j + 1]) {",
			"      swap(j, j+1);",
			"      vertauscht = true;",
			"    }",
			"  }",
			"  i = i - 1;",
			"} while (vertauscht && i >= 1);"};

	@Override
	public String[] getSourceCode() {
		return SOURCE_CODE;
	}

	@Override
	public void run() {
		setSourceCodeLine(0);
		boolean vertauscht;
		setSourceCodeLine(1);
		int i = elems.length - 1;
		aniHandler.setPointer("i", i);
		setSourceCodeLine(2);
		do {
			setSourceCodeLine(3);
			vertauscht = false;
			setSourceCodeLine(4);
			for (int j = 0; j < i; j++) {
				aniHandler.setPointer("j", j);
				aniHandler.highlight(j, j + 1);
				setSourceCodeLine(5);
				if (elems[j] > elems[j + 1]) {
					setSourceCodeLine(6);
					aniHandler.swapAnimation(j, j + 1);
					swap(j, j + 1);
					aniHandler.highlightOff();
					setSourceCodeLine(7);
					vertauscht = true;
				}
				setSourceCodeLine(4);
			}
			aniHandler.removePointer("j");
			aniHandler.highlightOff();
			aniHandler.highlightRange(i, elems.length);
			setSourceCodeLine(10);
			i = i - 1;
			aniHandler.setPointer("i", i);
			setSourceCodeLine(11);
		} while (vertauscht && i >= 1);
		//aniHandler.removePointer("i");
		aniHandler.highlightRange(0, elems.length);
		algorithmEnd();
	}

}
