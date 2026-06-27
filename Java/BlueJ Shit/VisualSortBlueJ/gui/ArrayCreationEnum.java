package gui;

import javax.swing.JOptionPane;

import tools.IntArrayTools;

/**
 * Enumerates the methods for creating arrays as offered in
 * OptionsDialog.dataTypeComboBox.
 * 
 * @author Franz-Josef
 */
public enum ArrayCreationEnum {
	PERMUTATION_NUMBERS("zufällige Permutation von Zahlen") {
		@Override
		public int[] create(int size) {
			return IntArrayTools.createPermutation(size);
		}
	},
	RANDOM_NUMBERS("zufällige Zahlen") {
		@Override
		public int[] create(int size) {
			return IntArrayTools.createRandomArray(size);
		}
	},
	SORTED("sortierte Zahlen") {
		@Override
		public int[] create(int size) {
			return IntArrayTools.createSortedArray(size);
		}
	},
	REVERSE("umgekehrt sortierte Zahlen") {
		@Override
		public int[] create(int size) {
			return IntArrayTools.createReverseSortedArray(size);
		}
	},
	USERDEF("benutzerdefiniert") {
		@Override
		public int[] create(int size) {
			while (true) {
				String input = JOptionPane.showInputDialog(
						"Gib mehrere, durch Komma getrennte Zahlen ein:",
						"0, 1, 2, 3, 4, 5, 6, 7");
				if (input == null) {
					return null;
				}
				String[] values = input.split(",");
				int[] intValues = new int[values.length];
				try {
					for (int i = 0; i < values.length; i++) {
						intValues[i] = Integer.parseInt(values[i].trim());
						if (intValues[i] < 0) {
							throw new RuntimeException();
						}
					}
					return intValues;
				} catch (NumberFormatException exc) {
					JOptionPane.showMessageDialog(null, "Fehlerhafte Eingabe.",
							"VisualSort", JOptionPane.ERROR_MESSAGE);
				} catch (RuntimeException exc) {
					JOptionPane.showMessageDialog(null,
							"Keine negative Zahlen angeben.", "VisualSort",
							JOptionPane.ERROR_MESSAGE);
				}
			}
		}
	};

	private String name;

	private ArrayCreationEnum(String name) {
		this.name = name;
	}

	@Override
	public String toString() {
		return name;
	}

	/**
	 * Creates an array with the method given by this object.
	 * 
	 * @param size
	 *            size of array
	 * @return The array or null if operation was cancelled by user.
	 */
	public abstract int[] create(int size);

	public static ArrayCreationEnum[] valuesWithoutUserdef() {
		return new ArrayCreationEnum[] { PERMUTATION_NUMBERS, RANDOM_NUMBERS,
				SORTED, REVERSE };
	}
}
