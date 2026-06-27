package gui;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;

import tools.RALColors;

/**
 * This class represents graphical items (bars with rounded corners) to be
 * sorted as used by SortPanel.
 * 
 * @author Franz-Josef
 */
public class Item {

	private static final Item NULLITEM = new Item(-1);

	/**
	 * The State of an item,
	 * which means currently: its color when it is drawn. 
	 * @author Franz-Josef
	 */
	public enum State {
		DEFAULT(RALColors.VERKEHRSGRUEN, RALColors.MOOSGRUEN), SELECTED(
				RALColors.FEUERROT, RALColors.PURPURROT);

		private Color fillColor;
		private Color drawColor;

		private State(Color fillColor, Color drawColor) {
			this.fillColor = fillColor;
			this.drawColor = drawColor;
		}
	}

	private static int WIDTH = 20;
	private static int HEIGHT_UNIT = 5;
	private static Font FONT;

	private static final int ARC_DIAM = 3;

	private State state;

	private int size;

	public Item(int size) {
		this.size = size;
		state = State.DEFAULT;
	}

	/**
	 * Call this method from paint
	 * shortly before you call draw on the several objects of class Item.
	 * @param itemWidth			The width of the following items to be drawn
	 * @param itemHeightUnit	The height unit (which will be multiplied by this.size)
	 */
	public static void prepareDraw(int itemWidth, int itemHeightUnit) {
		WIDTH = itemWidth;
		HEIGHT_UNIT = itemHeightUnit;
		FONT = null;
	}

	public void setState(State state) {
		this.state = state;
	}

	/**
	 * Draws an item with height and width specified as in prepareDraw.
	 * @param g		The graphics context to be used
	 * @param x		The left position of the item
	 * @param y		The bottom position of the item
	 */
	public void draw(Graphics g, int x, int y) {
		if (size < 0) {
			return;
		}

		if (FONT == null) {
			int height = g.getFontMetrics().getHeight();
			FONT = new Font(Font.DIALOG, Font.PLAIN, g.getFont().getSize()
					* WIDTH / height);
		}

		y -= HEIGHT_UNIT * size;
		g.setColor(state.fillColor);
		g.fillRoundRect(x, y, WIDTH, HEIGHT_UNIT * size, ARC_DIAM, ARC_DIAM);
		g.setColor(state.drawColor);
		g.drawRoundRect(x, y, WIDTH, HEIGHT_UNIT * size, ARC_DIAM, ARC_DIAM);
		g.setColor(Color.WHITE);

		g.setFont(FONT);
		g.drawString(Integer.toString(size), x + 3, y + HEIGHT_UNIT * size - 3);
	}

	public String toString() {
		return "Item(" + size + ")";
	}

	/**
	 * Returns the nullItem, which is the "empty" or "non-existent" item.
	 * This has the following advantage over just using null:
	 * getNullItem().draw(...) will just do nothing whereas null.draw(...)
	 * will throw an exception.
	 * @return
	 */
	public static Item getNullItem() {
		return NULLITEM;
	}
}
