package sortAlgorithms;

import java.util.Random;

/**
 * This is the implementation of BozoSort,
 * which continuously swaps randomly chosen elements.
 * @author Franz-Josef
 *
 */
public class BozoSort extends SortAlgorithm {

	private static final String[] SOURCE_CODE = new String[] {
			"Random random = new Random();",
			"while (!isSorted()) {",
			"  int i = random.nextInt(elems.length - 1) + 1;",
			"  int j = random.nextInt(i);",
			"  swap(i, j);",
			"}" };

	@Override
	public String[] getSourceCode() {
		return SOURCE_CODE;
	}

	private boolean isSorted() {
		for (int i = 0; i < elems.length - 1; i++) {
			if (elems[i] > elems[i + 1]) {
				return false;
			}
		}
		return true;
	}

	@Override
	public void run() {
		setSourceCodeLine(0);
		Random random = new Random();

		setSourceCodeLine(1);
		while (!isSorted()) {
			setSourceCodeLine(2);
			int i = random.nextInt(elems.length - 1) + 1;
			aniHandler.setPointer("i", i);
			setSourceCodeLine(3);
			int j = random.nextInt(i);
			aniHandler.setPointer("j", j);
			aniHandler.highlight(i, j);
			setSourceCodeLine(4);
			aniHandler.swapAnimation(i, j);
			swap(i, j);
			aniHandler.highlightOff();
			aniHandler.removePointer("i");
			aniHandler.removePointer("j");
			setSourceCodeLine(1);
		}
		algorithmEnd();
	}
}
