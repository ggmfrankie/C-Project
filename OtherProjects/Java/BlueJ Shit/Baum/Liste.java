
/**
 * Beschreiben Sie hier die Klasse Liste.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Liste
{
    // Instanzvariablen - ersetzen Sie das folgende Beispiel mit Ihren Variablen
    Listenelement anfang;
    public Liste()
    {
        anfang = new Ende();
        
    }
    
    public void addElement(Baumelement b){
        anfang.addElement(new Element(b));
    }
    
    public Baumelement pop(){
        Baumelement b = anfang.getInhalt();
        anfang = anfang.getNachfolger();
        return b;
    }
}
