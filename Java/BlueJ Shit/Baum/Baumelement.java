
/**
 * Tragen Sie hier eine Beschreibung des Interface Baumelement ein.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */

public abstract class Baumelement
{
    public abstract Baumelement addElement(Knoten k);
    public abstract Inhalt getInhalt();
    public abstract Knoten searchKnoten(int Key);
    public abstract void sortieren();
    public abstract int getHighestKey(int Key);
    public abstract Baumelement mitteFinder(Baumelement[] Baum,int key, int i);
}
