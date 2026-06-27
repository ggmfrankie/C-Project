
/**
 * Beschreiben Sie hier die Klasse Speicher.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Speicher
{
    static Liste liste;
    public static void generateListe(){
        liste = new Liste();
    }
    public static Liste getListe(){
        return liste;
    }
}
