
/**
 * Beschreiben Sie hier die Klasse Baum.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Baum
{
    Baumelement Wurzel;
    public Baum()
    {
        Wurzel = new Ende();
    }

    public void einfuegen(Root root){
        Wurzel = Wurzel.einfuegen(root);
    }
    
}
