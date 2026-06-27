package sortAlgorithms;

/**
 * This class enumerates the sorting algorithms.
 * 
 * @author Franz-Josef
 */
public enum AlgorithmEnum {
	BOZO_SORT(BozoSort.class), SELECTION_SORT(SelectionSort.class), BUBBLE_SORT(
			BubbleSort.class), INSERTION_SORT(InsertionSort.class), MERGE_SORT(
			MergeSort.class);

	private Class<? extends SortAlgorithm> sortAlgClass;

	private AlgorithmEnum(Class<? extends SortAlgorithm> sortAlgClass) {
		this.sortAlgClass = sortAlgClass;
	}

	/**
	 * Create an instance of this sorting algorithm.
	 * 
	 * @return
	 */
	public SortAlgorithm newAlgorithmInstance() {
		try {
			return sortAlgClass.getConstructor().newInstance();
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	/*
	 * @Override public String toString() { return sortAlgClass.getName(); }
	 */
}
