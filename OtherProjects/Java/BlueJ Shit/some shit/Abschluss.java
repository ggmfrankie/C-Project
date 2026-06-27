
/**
 * Beschreiben Sie hier die Klasse Abschluss.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Abschluss implements Listenelement
{
    // Instanzvariablen - ersetzen Sie das folgende Beispiel mit Ihren Variablen
    
    
    
    public Abschluss()
    {
        
    }
    public String getInhalt(){
        return null;
    }
    public Listenelement einfuegen(Knoten k){
        k.setNachfolger(this);
        return k;
    }
    public Knoten suchen(String s){
        System.out.println("Knoten nicht gefunden");
        return null;
    }
}
