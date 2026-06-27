package tools;

import java.awt.Color;

/**
 * This class contains static Color constants and static methods concerning
 * Colors.
 * 
 * @author Franz-Josef
 * 
 */
public class RALColors {

	public static final Color LICHTBLAU = new Color(41, 115, 184);

	public static final Color REINORANGE = new Color(242, 59, 28);

	public static final Color GRASGRUEN = new Color(54, 105, 38);

	public static final Color VERKEHRSGRUEN = new Color(36, 145, 64);

	public static final Color SMARAGDGRUEN = new Color(38, 102, 41);

	public static final Color MOOSGRUEN = new Color(10, 56, 31);

	public static final Color FEUERROT = new Color(171, 31, 28);

	public static final Color PURPURROT = new Color(105, 15, 20);

	public static final Color SCHWEFELGELB = new Color(255, 245, 66);

	public static final Color VERKEHRSGELB = new Color(252, 184, 33);

	public static final Color MAUSGRAU = new Color(97, 105, 105);

	/**
	 * An array of some Colors.
	 */
	public static final Color[] COLORS = { Color.MAGENTA, Color.RED,
			Color.GREEN, Color.YELLOW, Color.BLUE, Color.ORANGE, Color.CYAN,
			Color.PINK };

	/**
	 * Instantiates the given Color with the given alpha value.
	 * 
	 * @param color
	 * @param alpha
	 * @return
	 */
	public static Color setAlpha(Color color, int alpha) {
		return new Color((color.getRGB() & 0xFFFFFF) | ((alpha & 0xFF) << 24),
				true);
	}

	/**
	 * Instantiates the given Color with the given alpha value.
	 * 
	 * @param color
	 * @param alpha
	 * @return
	 */
	public static Color setAlpha(Color color, float alpha) {
		return new Color((color.getRGB() & 0xFFFFFF)
				| (((int) (alpha * 255f)) << 24), true);
	}
}
