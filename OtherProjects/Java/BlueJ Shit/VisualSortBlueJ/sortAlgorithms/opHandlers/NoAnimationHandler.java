package sortAlgorithms.opHandlers;

import gui.Range;

/**
 * Dummy animation handler. All methods are empty. 
 * @author Franz-Josef
 *
 */
public class NoAnimationHandler implements AnimationHandler {

	@Override
	public Range addRange(int from, int afterTo) {
		return null;
	}

	@Override
	public void createTempArray(int offset, int length, String name) {
	}

	@Override
	public void finished() {
	}

	@Override
	public void highlight(int i, int j) {
	}

	@Override
	public void highlightOff() {
	}

	@Override
	public void highlightRange(int from, int afterTo) {
	}

	@Override
	public void integrateTempArray() {
	}

	@Override
	public void loadItemFromTempArray(int tempArrayIndex, int arrayIndex) {
	}

	@Override
	public void loadItemIntoTempArray(int index) {
	}

	@Override
	public void moveAnimation(int from, int to) {
	}

	@Override
	public void removePointer(String string) {
	}

	@Override
	public void removeRange(Range leftRange) {
	}

	@Override
	public void removeTempPointer(String string) {
	}

	@Override
	public void setPointer(String string, int index) {
	}

	@Override
	public void setTempPointer(String string, int index) {
	}

	@Override
	public void shiftAnimation(int from, int to) {
	}

	@Override
	public void swapAnimation(int i, int j) {
	}

	@Override
	public void removeTempArray() {
	}
}
