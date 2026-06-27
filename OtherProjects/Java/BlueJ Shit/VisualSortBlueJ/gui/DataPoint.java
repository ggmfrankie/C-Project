package gui;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;

/**
 * Represents a data point. Used in GraphPanel. Extends the java.awt.Point
 * class.
 * 
 * @author Franz-Josef
 * 
 */
public class DataPoint extends Point {
	private static final long serialVersionUID = 1L;
	private static final int radius = 4;

	public enum Shape {
		CROSS {
			@Override
			public void draw(Graphics2D g, Point2D p) {
				g.draw(new Line2D.Double(p.getX() - radius, p.getY() - radius,
						p.getX() + radius, p.getY() + radius));
				g.draw(new Line2D.Double(p.getX() + radius, p.getY() - radius,
						p.getX() - radius, p.getY() + radius));
			}
		},
		CIRCLE {
			@Override
			public void draw(Graphics2D g, Point2D p) {
				g.draw(new Ellipse2D.Double(p.getX() - radius, p.getY()
						- radius, 2 * radius, 2 * radius));
			}
		},
		RECTANGLE {
			@Override
			public void draw(Graphics2D g, Point2D p) {
				g.draw(new Rectangle2D.Double(p.getX() - radius, p.getY()
						- radius, 2 * radius, 2 * radius));
			}
		},
		ELLIPTIC_CROSS {
			@Override
			public void draw(Graphics2D g, Point2D p) {
				g.draw(new Ellipse2D.Double(p.getX() - 2, p.getY() - radius,
						2 * 2, 2 * radius));
				g.draw(new Ellipse2D.Double(p.getX() - radius, p.getY() - 2,
						2 * radius, 2 * 2));
			}
		},
		ROUND_RECTANGLE {
			@Override
			public void draw(Graphics2D g, Point2D p) {
				g.draw(new RoundRectangle2D.Double(p.getX() - radius, p.getY()
						- radius, 2 * radius, 2 * radius, 2, 2));
			}
		};

		public abstract void draw(Graphics2D g, Point2D p);
	};

	public Shape shape;
	public Color color;

	public DataPoint(int x, int y, Color color, Shape shape) {
		super(x, y);
		this.color = color;
		this.shape = shape;
	}

	public DataPoint(DataPoint p) {
		super(p);
		this.color = p.color;
		this.shape = p.shape;
	}

	public void draw(Graphics2D g) {
		shape.draw(g, this);
	}
}
