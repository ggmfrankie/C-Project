
/**
 * Beschreiben Sie hier die Klasse Ende.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Ende implements Listenelement
{
    // Instanzvariablen - ersetzen Sie das folgende Beispiel mit Ihren Variablen
    public Ende()
    {
        // Instanzvariable initialisieren
        
    }
    public Baumelement getInhalt(){
        return null;
    }
    public Listenelement addElement(Element e){
        e.addNachfolger(this);
        return e;
    }
    public Listenelement getNachfolger(){
        return null;
    }
    
}
