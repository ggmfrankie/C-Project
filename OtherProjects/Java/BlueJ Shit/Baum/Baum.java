
/**
 * Abstrakte Klasse Baum 
 * 
 * @author 
 */
public class Baum {
    /*---------------Attribute-----*/
    Baumelement Wurzel;
    int key;
    Baumelement[] Baum;
    /*---------------Konstruktor---*/
    public Baum() {
        Wurzel = new Abschluss();
        key = 0;
        Speicher.generateListe();
    }
    public void addElement(String s){
        Wurzel = Wurzel.addElement(new Knoten(new Inhalt(s), key));
        key++;
    }
    public Knoten searchKnoten(int Key){
        return Wurzel.searchKnoten(Key);
    }
    public int getHighestKey(){
        Wurzel.getHighestKey(0);
        return 0;
    }
    public void sortieren(int key){
        Baum[0]= searchKnoten(getHighestKey()/2);
        Baum[0].mitteFinder(Baum, key,1);
    }
    
}
