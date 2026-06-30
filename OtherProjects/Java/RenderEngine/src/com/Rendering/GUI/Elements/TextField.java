package com.Rendering.GUI.Elements;

import com.Rendering.Textures.Texture;
import org.joml.Vector2f;

public class TextField extends BaseGuiComponent{

    public TextField(Vector2f position, float width, float height, String text, Texture fontTexture, int numRows, int numCols) {
        super(position, width, height, text);
        this.mesh = meshGenerator.generateQuads(numRows, numCols, new Vector2f(0), new Vector2f(1,1));
        mesh.getMaterial().setTexture(fontTexture);
    }

    public TextField(Vector2f position, float width, float height, String text, Texture fontTexture) {
        super(position, width, height, text);
        System.out.println("Text Field initialized---------------------");
        this.mesh = meshGenerator.generateQuads(2, text.length(), new Vector2f(-0.5f, -0.5f), new Vector2f(0.5f, 0.5f));
        mesh.getMaterial().setTexture(fontTexture);
    }
}
