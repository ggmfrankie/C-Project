
/**
 * Beschreiben Sie hier die Klasse Element.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Element implements Listenelement
{
    // Instanzvariablen - ersetzen Sie das folgende Beispiel mit Ihren Variablen
    Baumelement inhalt;
    Listenelement nachfolger;
    public Element(Baumelement inhalt)
    {
        this.inhalt = inhalt;
        
    }
    public Listenelement addElement(Element e){
        nachfolger.addElement(e);
        return this;
    }
    public Baumelement getInhalt(){
        return this.inhalt;
    }
    public void addNachfolger(Listenelement l){
        this.nachfolger = l;
    }
    public Listenelement getNachfolger(){
        return this.nachfolger;
    }
}
