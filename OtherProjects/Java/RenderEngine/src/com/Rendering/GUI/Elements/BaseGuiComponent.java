package com.Rendering.GUI.Elements;

import com.Rendering.Mesh.Mesh;
import com.Rendering.Textures.Texture;
import com.Window.Window;
import org.joml.Vector2f;

public abstract class BaseGuiComponent {
    float width;
    float height;
    boolean isActive;
    boolean isVisible;

    Mesh mesh;
    String name;
    Vector2f position;

    static MeshGenerator meshGenerator = new MeshGenerator();

    public BaseGuiComponent(Vector2f position, float width, float height, String name) {
        this.position = position;
        this.width = width;
        this.height = height;
        this.name = name;
        this.isVisible = true;
    }

    public void setScale(float scale){
        this.width *= scale;
        this.height *= scale;
    }

    public boolean isMouseOver(Vector2f mousePos, Window window) {
        return Math.abs(position.x - mousePos.x) <= width / 2 &&
                Math.abs(position.y - mousePos.y) <= height / 2;
    }

    public void render() {
        mesh.render();
    }

    public void init() {
        mesh.init();
    }

    public void setPosition(Vector2f position) {
        this.position = position;
    }

    public void setTexture(Texture texture) {
        mesh.getMaterial().setTexture(texture);
    }

    public float getHeight(){
        return height;
    }

    public float getWidth() {
        return width;
    }

    public Texture getTexture() {
        return this.mesh.getMaterial().getTexture();
    }

    public String getName() {
        return this.name;
    }

    public Vector2f getPosition() {
        return this.position;
    }

    public Mesh getMesh() {
        return this.mesh;
    }

    public int getZIndex(){
        return 0;
    }

    public boolean isUsingNinePatch() {
        return false;
    }

    public boolean isActive() {
        return this.isActive;
    }

    public boolean isVisible(){
        return isVisible;
    }

    public void setVisible(boolean visible) {
        isVisible = visible;
    }

    public String getType(){
        return this.getClass().getSimpleName();
    }
}

