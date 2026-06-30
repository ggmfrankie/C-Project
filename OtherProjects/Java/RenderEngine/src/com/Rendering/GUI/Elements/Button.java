package com.Rendering.GUI.Elements;

import com.Basics.Interface.ClickAction;
import com.Engine.Events;
import com.Rendering.Mesh.NinePatchMesh;
import com.Rendering.Textures.PatchTexture;
import com.Rendering.Textures.Texture;
import com.Window.Window;
import org.joml.Vector2f;

public class Button extends BaseGuiComponent implements IClickable {

    ClickAction clickAction;

    public Button(Vector2f position, float width, float height, String name, PatchTexture texture){
        super(position, width, height, name);
        mesh = generateMesh(texture);
    }

    public Button(Vector2f position, float width, float height, String name, PatchTexture texture, ClickAction clickAction){
        super(position, width, height, name);
        mesh = generateMesh(texture);
        this.clickAction = clickAction;
    }

    public Button(Vector2f position, float width, float height, String name, Texture texture){
        super(position, width, height, name);
        mesh = meshGenerator.generateBasicQuad();
    }

    private NinePatchMesh generateMesh(PatchTexture texture){
        Vector2f start = new Vector2f(-0.5f, -0.5f);
        Vector2f end = new Vector2f(0.5f, 0.5f);

        return meshGenerator.generateNinePatchMesh(start, end, texture);
    }

    @Override
    public boolean isMouseOver(Vector2f mousePos, Window window) {
        boolean isMouseOver = super.isMouseOver(mousePos, window);

        isActive = isMouseOver;
        return isMouseOver;
    }

    @Override
    public void click() {
        if(clickAction == null) return;
        clickAction.execute(this);
    }

    @Override
    public void setClickAction(ClickAction clickAction) {
        this.clickAction = clickAction;
    }

    @Override
    public ClickAction getClickAction() {
        return clickAction;
    }

    @Override
    public boolean isUsingNinePatch() {
        return mesh instanceof NinePatchMesh;
    }
}
