import java.util.Random;

/**
 * Klasse Knoten.
 *  
 * @author 
 */
public class Knoten extends Baumelement{
    /*---------------Attribute-----*/
    Inhalt inhalt;
    int key;
    Baumelement rechterNachfolger, linkerNachfolger;
    Liste liste;
    /*---------------Konstruktor---*/
    public Knoten(Inhalt inhalt, int key) {
        this.inhalt = inhalt;
        liste = Speicher.getListe();
        this.key = key;
        rechterNachfolger = new Abschluss();
        linkerNachfolger = new Abschluss();
    }
    public Baumelement addElement(Knoten k){
        if(k.getKey() > this.key){
            rechterNachfolger = rechterNachfolger.addElement(k);
            return this;
        }
        linkerNachfolger = linkerNachfolger.addElement(k);
        return this;
    }
    public Inhalt getInhalt(){
        return null;
    }
    public int getHighestKey(int Key){
        //return rechterNachfolger.getHighestKey(this.Key);
        return 0;
    }
    public int getKey(){
        return this.key;
    }
    public Knoten searchKnoten(int Key){
        if(Key == this.key) return this;
        if(Key > this.key)  return rechterNachfolger.searchKnoten(Key);
        return linkerNachfolger.searchKnoten(Key);
    }
    public void setNachfolger(Baumelement rechterNachfolger, Baumelement linkerNachfolger){
        this.rechterNachfolger=rechterNachfolger;
        this.linkerNachfolger=linkerNachfolger;
    }
    public void sortieren(){
        liste.addElement(this);
        int key = this.key;
        searchKnoten(key+(key/2+key%key/2)).sortieren();
        searchKnoten(key-(key/2+key%key/2)).sortieren();
    }
    public Baumelement mitteFinder(Baumelement[] Baum,int key, int i){
        
        Baum[i]= searchKnoten(key+(key/2+key%key/2));
        i++;
        Baum[i]= searchKnoten(key-(key/2+key%key/2));
        Baum[i-1].mitteFinder(Baum, key, i++);
        return this;
    }
    
}
