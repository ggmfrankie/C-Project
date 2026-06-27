package gui;

import gui.Item.State;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Point;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.SortedSet;
import java.util.TreeSet;

import javax.swing.JPanel;

import sortAlgorithms.opHandlers.AnimationHandler;
import tools.RALColors;
import tools.Tools;

/**
 * This class represents a panel for graphically sorting items.
 * 
 * @author Franz-Josef
 * 
 */
public class SortPanel extends JPanel implements AnimationHandler {
	private static final long serialVersionUID = 1L;
	private static final Color FINISHED_RANGE_COLOR = RALColors.LICHTBLAU;
	private static final Color RANGE_COLOR = RALColors.setAlpha(
			RALColors.REINORANGE, 128);
	private static final Color TEMP_ARRAY_COLOR = RALColors.VERKEHRSGELB;
	private static Font FONT;

	/**
	 * All stuff that paint() has to draw but can be modified from outside is
	 * synchronized with this lock:
	 */
	private Object itemAccessMutex = new Object();

	private Item[] items; // access only via mutex

	private Item[] tempItems; // access only via mutex
	private int tempItemsOffset; // access only via mutex
	private int tempArrayIndex; // access only via mutex
	private Pointer tempArrayPointer; // access only via mutex
	private String tempArrayName; // access only via mutex

	private Item highlight0 = null; // access only via mutex
	private Item highlight1 = null; // access only via mutex

	private enum AnimationType {
		ANI_NONE, ANI_MOVE, ANI_SWAP, ANI_SHIFT, ANI_CREATE_TEMPARRAY, ANI_INTEGRATE_TEMPLIST, ANI_LOAD_ITEM_INTO_TEMPARRAY, ANI_LOAD_ITEM_FROM_TEMPARRAY;
	}

	private AnimationType animationType; // access only via mutex

	private SortedSet<Pointer> pointers;
	private List<Range> ranges;
	private Range highlightRange;

	private int animationI; // i<j // access only via mutex
	private int animationJ; // access only via mutex
	private float animationProgress; // access only via mutex

	public SortPanel() {
		this.setOpaque(true);

		items = new Item[0];
		animationType = AnimationType.ANI_NONE;
		animationI = -1;
		animationJ = -1;
		ranges = Collections.synchronizedList(new LinkedList<Range>());
		pointers = Collections.synchronizedSortedSet(new TreeSet<Pointer>());
	}

	/**
	 * Sets all items to new values.
	 * 
	 * @param elems
	 */
	public void setElems(int[] elems) {
		Item[] newItems = new Item[elems.length];
		for (int i = 0; i < elems.length; i++) {
			newItems[i] = new Item(elems[i]);
		}

		synchronized (itemAccessMutex) {
			items = newItems;
			tempItems = null;
			highlight0 = null;
			highlight1 = null;
			ranges.clear();
			pointers.clear();
			highlightRange = null;
		}
		repaint();
	}

	/**
	 * Highlights two items in RED.
	 */
	public void highlight(int i, int j) {
		synchronized (itemAccessMutex) {
			highlightOff();
			highlight0 = items[i];
			highlight1 = items[j];
			highlight0.setState(State.SELECTED);
			highlight1.setState(State.SELECTED);
		}
		repaint();
	}

	/**
	 * Switches highlighting off.
	 */
	public void highlightOff() {
		synchronized (itemAccessMutex) {
			if (highlight0 != null) {
				highlight0.setState(State.DEFAULT);
				highlight0 = null;
			}
			if (highlight1 != null) {
				highlight1.setState(State.DEFAULT);
				highlight1 = null;
			}
		}
		repaint();
	}

	/**
	 * Highlight a range of items in BLUE.
	 */
	public void highlightRange(int from, int afterTo) {
		synchronized (itemAccessMutex) {
			highlightRange = new Range(from, afterTo);
		}
		repaint();
	}

	/**
	 * Adds a range to highlight to an array of ranges. These ranges will be
	 * painted in a semi-transparent RED, so that overlapping ranges can be
	 * seen. This is suitable i. e. for recursion.
	 */
	public Range addRange(int from, int afterTo) {
		Range range = new Range(from, afterTo);
		synchronized (itemAccessMutex) {
			ranges.add(range);
		}
		repaint();
		return range;
	}

	/**
	 * Removes a range from the array of ranges.
	 */
	public void removeRange(Range range) {
		synchronized (itemAccessMutex) {
			ranges.remove(range);
		}
		repaint();
	}

	/**
	 * Swaps two items.
	 */
	@Override
	public void swapAnimation(int i, int j) {
		if (i == j) {
			repaint();
			Tools.sleep(1000);
			repaint();
			return;
		}

		synchronized (itemAccessMutex) {
			if (i < j) {
				playAnimation(AnimationType.ANI_SWAP, i, j);
			} else {
				playAnimation(AnimationType.ANI_SWAP, j, i);
			}

			Item swap = items[i];
			items[i] = items[j];
			items[j] = swap;
		}
	}

	/**
	 * Shifts items. This means: Move item at position "from" to position "to"
	 * and move all items between these two positions one step further. This
	 * method works only when from >= to !
	 * 
	 */
	@Override
	public void shiftAnimation(int from, int to) {
		if (from == to) {
			repaint();
			Tools.sleep(1000);
			repaint();
			return;
		}

		synchronized (itemAccessMutex) {
			playAnimation(AnimationType.ANI_SHIFT, from, to);

			Item fromItem = items[from];
			for (int i = from; i > to; i--) {
				items[i] = items[i - 1];
			}
			items[to] = fromItem;
		}
	}

	private void playAnimation(AnimationType newAnimationType, int from, int to) {
		synchronized (itemAccessMutex) {
			animationI = from;
			animationJ = to;
			playAnimation(newAnimationType);
		}
	}

	private void playAnimation(AnimationType newAnimationType) {
		synchronized (itemAccessMutex) {
			this.animationType = newAnimationType;
			final int maxSteps = 20;
			final int delayMs = 50;
			for (int i = 0; i <= maxSteps; i++) {
				animationProgress = (float) i / (float) maxSteps;
				repaint();
				try {
					itemAccessMutex.wait(); // wait & release sync lock
					// now painting takes place
					Thread.sleep(delayMs);
				} catch (InterruptedException exc) {
				}
			}
			animationType = AnimationType.ANI_NONE;
		}
	}

	private final int calcXpos(int itemDist, int index) {
		return 5 + index * itemDist;
	}

	public void paint(Graphics g) {
		if (FONT == null) {
			FONT = new Font(Font.DIALOG, Font.PLAIN, 12);
		}

		synchronized (itemAccessMutex) {
			if (items.length == 0) {
				// fjftemp TODO no thread notification here
				return;
			}

			final int itemDist = (getWidth() - 10) / items.length;
			Item.prepareDraw(itemDist - 5,
					(getHeight() - 5 - Pointer.POINTER_HEIGHT) / 2
							/ items.length);

			// DRAW COMPLETE BACKGROUND
			// super.paint(g);
			g.setColor(RALColors.MAUSGRAU);
			g.fillRect(0, 0, getWidth(), getHeight());

			// DRAW FINISHED RANGE (IN BACKGROUND)
			if (highlightRange != null) {
				// highlight a range
				int begin = calcXpos(itemDist, highlightRange.rangeBegin);
				int end = calcXpos(itemDist, highlightRange.rangeAfterEnd);
				g.setColor(FINISHED_RANGE_COLOR);
				g.fillRect(begin - 2, 0, end - begin, getHeight());

				// g.setColor(Color.BLACK);
				// g.setFont(FONT);
				// g.drawString("fertig sortiert", begin + 3, 13);

			}

			// DRAW RANGES (IN BACKGROUND)
			g.setColor(RANGE_COLOR);
			for (Range r : ranges) {
				int begin = calcXpos(itemDist, r.rangeBegin);
				int end = calcXpos(itemDist, r.rangeAfterEnd);
				g.fillRect(begin - 2, 0, end - begin, getHeight());
			}

			// DRAW TEMPARRAY BACKGROUND
			if (tempItems != null) {
				// draw temp array area:
				float alpha = (animationType == AnimationType.ANI_CREATE_TEMPARRAY) ? animationProgress
						: 1f;
				int y = (this.getHeight() - Pointer.POINTER_HEIGHT) / 2;
				int x = calcXpos(itemDist, tempItemsOffset);
				g.setColor(RALColors.setAlpha(TEMP_ARRAY_COLOR, alpha));
				g.fillRect(x - 2, 0, itemDist * tempItems.length, y);

				g.setColor(RALColors.setAlpha(Color.BLACK, alpha));
				g.setFont(FONT);
				g.drawString(tempArrayName, x + 3, 13);
			}

			// DRAW ARRAY (MAYBE WITH ANIMATIONS)
			switch (animationType) {
			case ANI_SWAP: {
				int iX = paintConsecutiveItems(g, 0, animationI - 1, itemDist,
						5);
				int offset = iX + itemDist;
				int jX = paintConsecutiveItems(g, animationI + 1,
						animationJ - 1, itemDist, offset);
				offset = jX + itemDist;
				paintConsecutiveItems(g, animationJ + 1, items.length - 1,
						itemDist, offset);

				HalfEllipseParametric ellipse = new HalfEllipseParametric(iX,
						jX, getHeight() - Pointer.POINTER_HEIGHT, 30);
				Point p = ellipse.evalAt(animationProgress);
				Point q = ellipse.evalAt(-1 + animationProgress);
				items[animationI].draw(g, p.x, p.y);
				items[animationJ].draw(g, q.x, q.y);
			}
				break;
			case ANI_SHIFT: {
				assert animationI > animationJ;
				int i = animationJ;
				int j = animationI;
				int iX = paintConsecutiveItems(g, 0, i - 1, itemDist, 5);
				int jX = iX + (j - i) * itemDist;
				paintConsecutiveItems(g, j + 1, items.length - 1, itemDist, jX
						+ itemDist);

				paintConsecutiveItems(g, i, j - 1, itemDist, iX
						+ (int) (animationProgress * itemDist));

				HalfEllipseParametric ellipse = new HalfEllipseParametric(jX,
						iX, getHeight() - Pointer.POINTER_HEIGHT, 30);
				Point p = ellipse.evalAt(animationProgress);
				items[j].draw(g, p.x, p.y);
			}
				break;
			default: // ANI_NONE, ANI_MOVE, ANI_CREATE_TEMPARRAY,
				// ANI_INTEGRATE_TEMPLIST,
				// ANI_LOAD_ITEM_INTO_TEMPARRAY, ANI_LOAD_ITEM_FROM_TEMPARRAY
				paintConsecutiveItems(g, 0, items.length - 1, itemDist, 5);
				break;
			}

			// DRAW TEMPARRAY
			if (tempItems != null
					&& animationType != AnimationType.ANI_CREATE_TEMPARRAY) {
				// This case: tempArray is active or is being destructed.
				// draw tempArray:
				float y = (this.getHeight() - Pointer.POINTER_HEIGHT) / 2f;
				int yOffset = 0;
				if (animationType == AnimationType.ANI_INTEGRATE_TEMPLIST) {
					yOffset = (int) (y * (1f + animationProgress));
				} else {
					yOffset = (int) y;
				}
				paintConsecutiveItems(g, tempItems, 0, tempItems.length - 1,
						itemDist, calcXpos(itemDist, tempItemsOffset), yOffset);
			}

			// DRAW SOME OTHER ANIS
			if (animationType == AnimationType.ANI_MOVE) {
				// linear animation:
				int x1 = calcXpos(itemDist, animationI);
				int x2 = calcXpos(itemDist, animationJ);
				int y = this.getHeight() - Pointer.POINTER_HEIGHT;
				items[animationI].draw(g, (int) (x1 + animationProgress
						* (x2 - x1)), y);

			} else if (animationType == AnimationType.ANI_LOAD_ITEM_INTO_TEMPARRAY) {
				// linear animation:
				int x1 = calcXpos(itemDist, animationI);
				int x2 = calcXpos(itemDist, tempItemsOffset + tempArrayIndex);
				int y1 = this.getHeight() - Pointer.POINTER_HEIGHT;
				int y2 = y1 / 2;
				items[animationI].draw(g, (int) (x1 + animationProgress
						* (x2 - x1)),
						(int) (y1 + animationProgress * (y2 - y1)));

			} else if (animationType == AnimationType.ANI_LOAD_ITEM_FROM_TEMPARRAY) {
				// linear animation:
				int x1 = calcXpos(itemDist, tempItemsOffset + animationI);
				int x2 = calcXpos(itemDist, animationJ);
				int y2 = this.getHeight() - Pointer.POINTER_HEIGHT;
				int y1 = y2 / 2;
				tempItems[animationI].draw(g, (int) (x1 + animationProgress
						* (x2 - x1)),
						(int) (y1 + animationProgress * (y2 - y1)));
			}

			// DRAW POINTERS
			// calc pointer positions
			Map<Integer, String> pointerNames = new HashMap<Integer, String>();
			for (Pointer pointer : pointers) {
				String name = pointerNames.get(pointer.i);
				if (name == null) {
					name = pointer.name;
				} else {
					name = name + ", " + pointer.name;
				}
				pointerNames.put(pointer.i, name);
			}

			int y = this.getHeight() - Pointer.POINTER_HEIGHT;
			for (Map.Entry<Integer, String> entry : pointerNames.entrySet()) {
				int i = entry.getKey();
				int x = calcXpos(itemDist, i) + (itemDist - 5) / 2;
				(new Pointer(entry.getValue(), i)).draw(g, x, y);
			}

			if (tempArrayPointer != null) {
				int x = calcXpos(itemDist, tempItemsOffset + tempArrayPointer.i)
						+ (itemDist - 5) / 2;
				tempArrayPointer.draw(g, x, y / 2);
			}

			// NOTIFY POTENTIAL WAITING THREAD: PAINTING IS FINISHED
			itemAccessMutex.notify();
		}
	}

	private int paintConsecutiveItems(Graphics g, int start, int end,
			int itemDist, int offsetX) {

		int offsetY = this.getHeight() - Pointer.POINTER_HEIGHT;
		return paintConsecutiveItems(g, items, start, end, itemDist, offsetX,
				offsetY);
	}

	/**
	 * Only call from synched block!
	 */
	private int paintConsecutiveItems(Graphics g, Item[] items, int start,
			int end, int itemDist, int offsetX, int offsetY) {

		for (int i = start; i <= end; i++) {
			items[i].draw(g, offsetX, offsetY);
			offsetX = offsetX + itemDist;
		}
		return offsetX;
	}

	/**
	 * Sets a pointer (an arrow). A negative value for i removes an existing
	 * pointer.
	 */
	@Override
	public void setPointer(String name, int i) {
		Pointer p = new Pointer(name, i);
		synchronized (itemAccessMutex) {
			pointers.remove(p);
			if (i >= 0) {
				pointers.add(p);
			}
		}
		repaint();
	}

	/**
	 * Removes a pointer.
	 */
	@Override
	public void removePointer(String name) {
		setPointer(name, -1);
	}

	/**
	 * Sets the pointer to the temporary array. A negative value for i removes
	 * an existing pointer.
	 */
	@Override
	public void setTempPointer(String name, int index) {
		synchronized (itemAccessMutex) {
			if (index >= 0) {
				tempArrayPointer = new Pointer(name, index);
			} else {
				tempArrayPointer = null;
			}
		}
		repaint();
	}

	/**
	 * Removes the pointer to the temporary array.
	 */
	@Override
	public void removeTempPointer(String name) {
		setTempPointer(name, -1);
	}

	/**
	 * Creates a temporary array with given name and length. The temp. array is
	 * initially empty and will be displayed above original array position
	 * "offset".
	 */
	@Override
	public void createTempArray(int offset, int length, String name) {
		synchronized (itemAccessMutex) {
			tempItems = new Item[length];
			final Item nullItem = Item.getNullItem();
			for (int i = 0; i < tempItems.length; i++) {
				tempItems[i] = nullItem;
			}
			tempItemsOffset = offset;
			tempArrayIndex = 0;
			tempArrayName = name;
		}
		playAnimation(AnimationType.ANI_CREATE_TEMPARRAY);
	}

	/**
	 * Copies one array item with given index into the temp. array. In internal
	 * temp. array pointer will be advanced afterwards so that consecutive
	 * method calls will place items into the temp. array at the positions 0, 1,
	 * 2, and so on.
	 */
	@Override
	public void loadItemIntoTempArray(int itemIndex) {
		playAnimation(AnimationType.ANI_LOAD_ITEM_INTO_TEMPARRAY, itemIndex, -1);
		synchronized (itemAccessMutex) {
			tempItems[tempArrayIndex] = items[itemIndex];
			tempArrayIndex++;
		}
	}

	/**
	 * Copies one item from the temp. array with given index into the original
	 * array with given index.
	 */
	@Override
	public void loadItemFromTempArray(int tempArrayIndex, int arrayIndex) {
		playAnimation(AnimationType.ANI_LOAD_ITEM_FROM_TEMPARRAY,
				tempArrayIndex, arrayIndex);
		synchronized (itemAccessMutex) {
			items[arrayIndex] = tempItems[tempArrayIndex];
		}
	}

	/**
	 * Copies the whole temp. array into the original array, starting at the
	 * offset given when createTempArray was called. Afterwards, the temp. array
	 * is removed.
	 */
	@Override
	public void integrateTempArray() {
		playAnimation(AnimationType.ANI_INTEGRATE_TEMPLIST);
		synchronized (itemAccessMutex) {
			System.arraycopy(tempItems, 0, items, tempItemsOffset,
					tempItems.length);
			tempItems = null;
			tempArrayPointer = null;
		}
	}

	/**
	 * Removes the temp. array.
	 */
	@Override
	public void removeTempArray() {
		synchronized (itemAccessMutex) {
			tempItems = null;
			tempArrayPointer = null;
		}
	}

	/**
	 * Copies one item to another position.
	 */
	@Override
	public void moveAnimation(int from, int to) {
		playAnimation(AnimationType.ANI_MOVE, from, to);
		synchronized (itemAccessMutex) {
			items[to] = items[from];
		}
		// Necessary: remove picture of old item @ position to
		repaint();
	}

	/**
	 * Causes a repaint. This is needed by some algorithms to display the
	 * correct resulting diagram when finished.
	 */
	@Override
	public void finished() {
		repaint();
	}
}
