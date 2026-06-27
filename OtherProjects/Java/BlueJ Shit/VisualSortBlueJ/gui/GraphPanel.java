package gui;

import java.awt.Color;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.AffineTransform;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.util.ArrayList;

import javax.swing.JPanel;

/**
 * This class draws colored crosses marking points in a coordinate system. Note
 * that only non-negative coordinates will work correct. When adding points, the
 * units on both axis are automagically scaled so that the coordinate system can
 * display all points.
 * 
 * @author Franz-Josef
 */
public class GraphPanel extends JPanel {

	private static final long serialVersionUID = 1L;

	private ArrayList<DataPoint> points;

	private int minX, minY, maxX, maxY;

	public GraphPanel() {
		this.setOpaque(true);
		points = new ArrayList<DataPoint>();
		clearPoints();
	}

	/**
	 * Adds a colored point and repaints the window. Both axis might get scaled
	 * so that the new point can be displayed.
	 * 
	 * @param x
	 * @param y
	 * @param color
	 */
	public void addPoint(int x, int y, Color color, DataPoint.Shape shape) {
		synchronized (points) {
			minX = Math.min(x, minX);
			minY = Math.min(y, minY);
			maxX = Math.max(x, maxX);
			maxY = Math.max(y, maxY);
			points.add(new DataPoint(x, y, color, shape));
		}
		repaint();
	}

	public void clearPoints() {
		synchronized (points) {
			points.clear();
			minX = minY = Integer.MAX_VALUE;
			maxX = maxY = Integer.MIN_VALUE;
		}
		repaint();
	}

	/**
	 * Coords must be of length 4! Applies the given transformation to coords.
	 * (Therefore changes the coords array!) ... and draws the line from point
	 * (coords[0], coords[1]) to point (coords[2], coords[3].
	 */
	private final void drawLine(Graphics2D g, float[] coords,
			AffineTransform transform) {

		transform.transform(coords, 0, coords, 0, 2);
		g.draw(new Line2D.Float(coords[0], coords[1], coords[2], coords[3]));
	}

	/**
	 * Draws aligned text. The given x coordinate is at the start, end of center
	 * of the text, depending on requesting left, right or center alignment.
	 * 
	 * @param g
	 * @param s
	 * @param x
	 * @param y
	 * @param alignX
	 *            0.0f for top, 1.0f for bottom alignment
	 * @param alignY
	 *            0.5f for centered, 1.0f for right alignment
	 */
	private void drawAlignedString(Graphics2D g, String s, float x, float y,
			float alignX, float alignY) {
		FontMetrics fm = g.getFontMetrics();
		int width = fm.stringWidth(s);
		int ascent = fm.getAscent();
		g.drawString(s, x - alignX * width, y + (1.0f - alignY) * ascent);
	}

	private double calcUnit(final int X, float xWidth) {
		final float xunit = 30f * xWidth / X;
		final double pow10 = Math.pow(10, (int) Math.log10(xunit));
		double fac = xunit / pow10;
		assert fac > 1.0 && fac < 10.0;
		if (fac <= 2d) {
			fac = 2d;
		} else if (fac <= 5d) {
			fac = 5d;
		} else {
			fac = 10d;
		}
		return fac * pow10;
	}

	@Override
	public void paint(Graphics oldG) {
		Graphics2D g = (Graphics2D) oldG;
		g.setColor(Color.BLACK);
		g.fillRect(0, 0, getWidth(), getHeight());

		// distance of both coord axis to left/bottom of window.
		final int dist = 65;
		final int X = getWidth() - dist;
		final int Y = getHeight() - dist;
		if (X <= 0 || Y <= 0) {
			return;
		}

		final float xWidth = Math.max(X, maxX * 1.1f);
		final float yWidth = Math.max(Y, maxY * 1.1f);

		AffineTransform transform = new AffineTransform();
		transform.translate(dist, Y);
		transform.scale(X / xWidth, -Y / yWidth);

		g.setColor(Color.BLACK);
		g.fillRect(0, 0, getWidth(), getHeight());

		// draw coord system
		g.setColor(Color.WHITE);

		drawLine(g, new float[] { 0f, 0f, xWidth, 0f }, transform);

		final double xUnit = calcUnit(X, xWidth);
		final double yUnit = calcUnit(Y, yWidth);

		// draw x-axis
		for (int i = 0; i < xWidth; i += xUnit) {
			Point2D p = transform.transform(new Point(i, 0), null);

			g.draw(new Line2D.Double(p.getX(), p.getY() - 5d, p.getX(), p
					.getY() + 5d));

			AffineTransform oldTransform = g.getTransform();
			g.rotate(-0.5d * Math.PI, p.getX(), p.getY());
			drawAlignedString(g, Integer.toString(i), (float) p.getX() - 7,
					(float) p.getY(), 1.0f, 0.5f);
			g.setTransform(oldTransform);
		}
		drawAlignedString(g, "Feldgröße", getWidth() / 2, getHeight(), 0.5f,
				1.5f);

		// draw y-axis
		drawLine(g, new float[] { 0, 0, 0, yWidth }, transform);
		for (int i = 0; i < yWidth; i += yUnit) {
			Point2D p = transform.transform(new Point(0, i), null);

			g.draw(new Line2D.Double(p.getX() - 5d, p.getY(), p.getX() + 5d, p
					.getY()));
			drawAlignedString(g, Integer.toString(i), (float) p.getX() - 7f,
					(float) p.getY(), 1.0f, 0.5f);
		}

		AffineTransform oldTransform = g.getTransform();
		Point centerP = new Point(0, getHeight() / 2);
		g.rotate(-0.5d * Math.PI, centerP.getX(), centerP.getY());
		drawAlignedString(g, "Zeit in ms", (float) centerP.getX(),
				(float) centerP.getY(), 0.5f, -0.5f);
		g.setTransform(oldTransform);

		// draw points
		synchronized (points) {
			for (DataPoint p0 : points) {
				g.setColor(p0.color);
				DataPoint p = new DataPoint(p0);
				transform.transform(p, p);
				p.draw(g);
			}
		}
	}
}
