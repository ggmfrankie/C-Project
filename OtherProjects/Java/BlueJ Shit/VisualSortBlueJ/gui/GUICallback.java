package gui;

/**
 * Interface for callback mechanism to notify the GUI about events that
 * occur in the sorting thread.
 * @author Franz-Josef
 */
public interface GUICallback {
	
	public enum State {
		FINISHED, PAUSED;
	}
	
	public void callback(State state);
}
