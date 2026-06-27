
/**
 * Beschreiben Sie hier die Klasse Root.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class Root implements Baumelement
{
    Inhalt inhalt;
    int key;
    Baumelement linkerNachfolger, rechterNachfolger;
    public Root(String s, int key)
    {
        this.inhalt = new Inhalt(s);
        linkerNachfolger = new Ende();
        rechterNachfolger = new Ende();
        this.key = key;
    }

    public int getKey(){
        return this.key;
    }
    public Baumelement einfuegen(Inhalt r){
        if(r.getKey() == this.key){
            System.out.println("Duplicate Key");
            return this;
        }
        if(r.getKey() < this.key){
            linkerNachfolger = linkerNachfolger.einfuegen(r);
        }
        else{
            rechterNachfolger = rechterNachfolger.einfuegen(r);
        }
        return this;
    }
}
