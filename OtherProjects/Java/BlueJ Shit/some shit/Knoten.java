
/**
 * Beschreiben Sie hier die Klasse Knoten.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Knoten implements Listenelement
{
    // Instanzvariablen - ersetzen Sie das folgende Beispiel mit Ihren Variablen
    Inhalt inhalt;
    Listenelement Nachfolger;
    public Knoten(String inhalt)
    {
        this.inhalt = new Inhalt(inhalt);
        
    }
    public String getInhalt(){
        return this.inhalt.getInhalt();
    }
    public Listenelement einfuegen(Knoten k){
        this.Nachfolger = Nachfolger.einfuegen(k);
        return this;
    }
    public void setNachfolger(Listenelement l){
        this.Nachfolger = l;
    }
    public Knoten suchen(String s){
        if(this.inhalt.getInhalt().equals(s)){
            return this;
        }
        return Nachfolger.suchen(s);
    }
}
