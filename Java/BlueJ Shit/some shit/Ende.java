
/**
 * Beschreiben Sie hier die Klasse Ende.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Ende implements Baumelement
{
    // Instanzvariablen - ersetzen Sie das folgende Beispiel mit Ihren Variablen
    

    /**
     * Konstruktor für Objekte der Klasse Ende
     */
    public Ende()
    {
        
    }
    public Baumelement einfuegen(Inhalt r){
        return new Root(r);
    }
    public int getKey(){
        return -1;
    }
}
