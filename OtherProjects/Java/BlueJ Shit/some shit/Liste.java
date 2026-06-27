
/**
 * Beschreiben Sie hier die Klasse Liste.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Liste
{
    // Instanzvariablen - ersetzen Sie das folgende Beispiel mit Ihren Variablen
    Listenelement Anfang;
    public Liste()
    {
        // Instanzvariable initialisieren
        Anfang = new Abschluss();
    }
    public void einfuegen(Knoten knoten){
        Anfang = Anfang.einfuegen(knoten);
    }
    public Knoten suchen(String s){
        return Anfang.suchen(s);
    }
}
