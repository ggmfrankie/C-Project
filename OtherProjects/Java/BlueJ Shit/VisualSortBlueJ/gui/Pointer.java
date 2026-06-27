package gui;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.geom.Rectangle2D;

/**
 * This class represents graphical arrows as used by SortPanel.
 * The arrows are shown beneath the items. 
 * @author Franz-Josef
 */
public class Pointer implements Comparable<Pointer> {
	private static Font FONT;
	private static final Color ARROW_COLOR = Color.BLACK;
	private static final Color FONT_COLOR = Color.YELLOW;
	private static final int UNIT = 7;
	public static final int POINTER_HEIGHT = 4 * UNIT;

	public String name;
	public int i;

	public Pointer(String name, int i) {
		this.name = name;
		this.i = i;
	}

	public void draw(Graphics g, int x, int y) {
		if (FONT == null) {
			FONT = new Font(Font.DIALOG, Font.PLAIN, 12);
		}

		int[] xPoints = { x, x + 4 * UNIT, x + 2 * UNIT, x + 2 * UNIT,
				x - 2 * UNIT, x - 2 * UNIT, x - 4 * UNIT };
		int[] yPoints = { y, y + 2 * UNIT, y + 2 * UNIT, y + 4 * UNIT,
				y + 4 * UNIT, y + 2 * UNIT, y + 2 * UNIT };

		g.setColor(ARROW_COLOR);
		g.fillPolygon(xPoints, yPoints, xPoints.length);

		g.setColor(FONT_COLOR);
		g.setFont(FONT);
		Rectangle2D rect = g.getFontMetrics().getStringBounds(name, g);
		g.drawString(name, (int) (x - rect.getWidth() / 2), y + 3 * UNIT);
	}

	@Override
	public boolean equals(Object o) {
		return o instanceof Pointer && name.equals(((Pointer) o).name);
	}

	@Override
	public int hashCode() {
		return name.hashCode();
	}

	@Override
	public String toString() {
		return "Pointer(" + name + ", " + i + ")";
	}

	@Override
	public int compareTo(Pointer anotherPointer) {
		return this.name.compareTo(anotherPointer.name);
	}
}
