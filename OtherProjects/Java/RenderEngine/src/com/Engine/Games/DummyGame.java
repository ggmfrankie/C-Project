package com.Engine.Games;

import com.Engine.GameObjects.GameItem;
import com.Engine.MouseInput;
import com.Rendering.GUI.Elements.BaseGuiComponent;
import com.Rendering.GUI.Elements.IClickable;
import com.Rendering.Light.DirectionalLight;
import com.Rendering.MatrixTransformation.Camera;
import com.Window.Window;
import org.joml.Math;
import org.joml.Vector2f;
import org.joml.Vector3f;

import java.util.HashSet;

import static com.Basics.Utils.gameEngine;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.glfw.GLFW.GLFW_KEY_A;
import static org.lwjgl.glfw.GLFW.GLFW_KEY_D;
import static org.lwjgl.glfw.GLFW.GLFW_KEY_S;
import static org.lwjgl.glfw.GLFW.GLFW_KEY_W;
import static org.lwjgl.opengl.GL11.glAreTexturesResident;

public class DummyGame implements IGameLogic {
    private static final float CAMERA_POS_STEP = 0.0003f;
    private static final float MOUSE_SENSITIVITY = 0.0005f;
    float lightAngle;
    Vector3f cameraInc;
    Camera camera;
    DirectionalLight directionalLight;
    HashSet<BaseGuiComponent> guiComponents;
    HashSet<GameItem> gameItems;


    public DummyGame(){
        cameraInc = new Vector3f();
        lightAngle = 0.0f;
    }

    @Override
    public void init(){

    }

    @Override
    public void input(Window window, MouseInput mouseInput) {
        cameraInc.set(0, 0, 0);
        if(window.isKeyPressed(GLFW_KEY_W)){
            cameraInc.z = -1;
        } else if (window.isKeyPressed(GLFW_KEY_S)) {
            cameraInc.z = 1;
        }
        if(window.isKeyPressed(GLFW_KEY_A)){
            cameraInc.x = -1;
        } else if (window.isKeyPressed(GLFW_KEY_D)) {
            cameraInc.x = 1;
        }
        if(window.isKeyPressed(GLFW_KEY_LEFT_CONTROL)){
            cameraInc.y = -1;
        } else if (window.isKeyPressed(GLFW_KEY_SPACE)) {
            cameraInc.y = 1;
        }
        if(window.isKeyPressedSingle(GLFW_KEY_ESCAPE)){
            System.out.println("pressed Escape");
            gameEngine.saveAll();
        }

        for(BaseGuiComponent guiComponent : guiComponents){
            if(guiComponent.isMouseOver(window.getMousePosition(), window)){

                if(mouseInput.isLeftButtonPressed()){
                    if(guiComponent instanceof IClickable clickable) clickable.click();
                }
            }
        }
    }

    @Override
    public void update(float interval, MouseInput mouseInput) {
        updateCamera(mouseInput);
        if(directionalLight != null) updateDirectionalLight();
        updateObjects();
    }

    private void updateObjects(){
        for(GameItem gameItem : gameItems){
            gameItem.update();
        }
    }

    private void updateCamera(MouseInput mouseInput){
        camera.movePosition(cameraInc.x * CAMERA_POS_STEP,
                cameraInc.y * CAMERA_POS_STEP,
                cameraInc.z * CAMERA_POS_STEP);
        if (mouseInput.isRightButtonPressed()) {
            Vector2f rotVec = mouseInput.getDisplVec();
            camera.moveRotation(rotVec.x * MOUSE_SENSITIVITY, rotVec.y * MOUSE_SENSITIVITY, 0);
        }
    }

    private void updateDirectionalLight(){
        lightAngle += 0.0001f;
        if (lightAngle > 90) {
            directionalLight.setIntensity(0);
            if (lightAngle >= 360) {
                lightAngle = -90;
            }
        } else if (lightAngle <= -80 || lightAngle >= 80) {
            float factor = (1 - (float)(Math.abs(lightAngle) - 80)/ 10.0f);
            directionalLight.setIntensity(factor);
            directionalLight.getColor().y = Math.max(factor/2, 0.9f);
            directionalLight.getColor().z = Math.max(factor/2, 0.5f);
        } else {
            directionalLight.setIntensity(0.5f);
            directionalLight.getColor().x = 1;
            directionalLight.getColor().y = 1;
            directionalLight.getColor().z = 1;
        }
        double angRad = Math.toRadians(lightAngle);
        directionalLight.getDirection().x = (float) Math.sin(angRad);
        directionalLight.getDirection().y = (float) Math.cos(angRad);
    }

    @Override
    public void render() {

    }

    @Override
    public void setCamera(Camera camera) {
        this.camera = camera;
    }

    @Override
    public void setDirectionalLight(DirectionalLight directionalLight) {
        this.directionalLight = directionalLight;
    }

    @Override
    public void setGUIComponents(HashSet<BaseGuiComponent> guiComponents) {
        this.guiComponents = guiComponents;
    }

    @Override
    public void setGameItems(HashSet<GameItem> gameItems) {
        this.gameItems = gameItems;
    }
}
