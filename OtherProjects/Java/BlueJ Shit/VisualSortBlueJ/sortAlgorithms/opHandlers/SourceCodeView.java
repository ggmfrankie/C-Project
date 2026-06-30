package sortAlgorithms.opHandlers;

/**
 * This interface serves for the sorting algorithms to submit the source code to
 * the GUI -- and to update the source code line in the GUI.
 * 
 * @author Franz-Josef
 */
public interface SourceCodeView {

	/**
	 * Submits the source code of the algorithm as a String array.
	 */
	void loadSourceCode(String[] sourceCode);

	/**
	 * Highlights the given line.
	 */
	void setSourceCodeLine(int line);

}
