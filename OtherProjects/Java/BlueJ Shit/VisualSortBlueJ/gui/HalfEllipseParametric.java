package gui;

import java.awt.Point;

/**
 * This class represents a function f:
 * 
 * t -> (centerX, centerY) + (a*cos(t*PI), b*sin(t*PI))
 * 
 * @author Franz-Josef
 */
public class HalfEllipseParametric {

	private float centerX;
	private float centerY;
	private float a;
	private int b;

	public HalfEllipseParametric(int startX, int endX, int y, int b) {
		centerX = (startX + endX) / 2f;
		centerY = y;
		a = (startX - endX) / 2f;
		this.b = b;
	}

	public Point evalAt(float t) {
		t *= Math.PI;
		double dX = a * Math.cos(t);
		double dY = b * Math.sin(t);
		return new Point((int) Math.round(centerX + dX), (int) Math
				.round(centerY + dY));
	}
}
