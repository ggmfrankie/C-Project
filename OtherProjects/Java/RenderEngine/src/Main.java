import com.Engine.GameEngine;
import com.Engine.Games.DummyGame;
import com.Extern.KotlinTest;
import com.Extern.Rechner;

import static com.Basics.Utils.*;
import static com.Extern.Rechner.*;

public class Main {

    public static void main(String[] args) {
        gameEngine = new GameEngine("Game", 1000, 1000, new DummyGame());
        gameEngine.start();
    }


}