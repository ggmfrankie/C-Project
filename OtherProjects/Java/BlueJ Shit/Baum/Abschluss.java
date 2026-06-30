
/**
 * Klasse Abschluss.
 *  
 * @author 
 */
public class Abschluss extends Baumelement{
    /*---------------Attribute-----*/


    /*---------------Konstruktor---*/
    public Abschluss() {
    }
    public Baumelement addElement(Knoten k){
        k.setNachfolger(new Abschluss(), new Abschluss());
        return k;
    }
    public Inhalt getInhalt(){
        return null;
    }
    public Knoten searchKnoten(int Key){
        System.out.println("no element found");
        return null;
    }
    public void sortieren(){
        
    }
    public int getHighestKey(int Key){
        return Key;
    }
    public Baumelement mitteFinder(Baumelement[] Baum,int key, int i){
        return null;
    }
}
