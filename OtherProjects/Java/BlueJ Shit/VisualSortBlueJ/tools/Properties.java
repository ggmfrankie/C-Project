package tools;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class Properties extends java.util.Properties {

	private static final long serialVersionUID = 1L;

	private static final File HOME_PATH = new File(System
			.getProperty("user.home"));

	private static final File PROPERTIES_FILE = new File(HOME_PATH,
			"visualsort.properties");

	private static Properties INSTANCE;

	public static Properties getInstance() {
		if (INSTANCE == null) {
			INSTANCE = loadPropsFromFile();
		}
		return INSTANCE;
	}

	private Properties() {
	}

	private static Properties loadPropsFromFile() {
		Properties props = new Properties();
		props.defaults = getDefaults();

		try {
			props.load(new FileInputStream(PROPERTIES_FILE));
		} catch (IOException exc) {
			System.err.println("Properties file \"" + PROPERTIES_FILE
					+ "\" not found. Using default settings.");
		}
		return props;
	}

	private static java.util.Properties getDefaults() {
		java.util.Properties props = new java.util.Properties();
		props.setProperty("", "");
		return props;
	}
}
