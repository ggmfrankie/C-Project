package com.Engine.GameObjects;

import com.Basics.ReadAndWrite.GUIJsonReader;
import com.Engine.Events;
import com.Rendering.GUI.Elements.BaseGuiComponent;
import com.Rendering.GUI.Elements.Button;
import com.Rendering.Light.DirectionalLight;
import com.Rendering.Light.PointLight;
import org.joml.Vector2f;
import org.joml.Vector3f;

import java.util.HashSet;

import static com.Basics.Utils.gameEngine;

public class ObjectLoader {
    Events events;
    GUIJsonReader guiJsonReader;

    public ObjectLoader(){
        events = new Events();
        guiJsonReader = new GUIJsonReader("GuiLayout.json");
    }

    public void loadObjects(){
        addGameItemsManual();
        addGuiElementsFromFile();
        addGameItemsManual();
    }

    private void addGuiElementsFromFile() {
        HashSet<BaseGuiComponent> guiComponents = guiJsonReader.getGuiComponents();
        for(BaseGuiComponent component : guiComponents){
            gameEngine.addGUIComponent(component);
        }
    }

    private void addGameItemsManual(){
        Vector3f color = new Vector3f(0.5f, 0.0f, 0.0f);
        Vector3f color2 = new Vector3f(1.0f, 1.0f, 1.0f);
        Vector3f position = new Vector3f(-1.0f, 1.0f, -1.5f);
        Vector3f direction = new Vector3f(0.0f, 0.0f, 0.0f);

        float intensity = 0.5f;

        PointLight.Attenuation att = new PointLight.Attenuation(0.8f, 0.0005f, 0.00001f);
        gameEngine.addGameItem(
                new Vector3f(0.0f, 0.0f, 0.0f),
                new Vector3f(90.0f, 0.0f, 0.0f),
                "grass_block.obj",
                events.getUpdateAction("spin")
        );

        gameEngine.addGameItem(
                new Vector3f(5.0f, 1.0f, 0.0f),
                new Vector3f(0.0f, 0.0f, 0.0f),
                "Sting-Sword-lowpoly.obj"
        );

        gameEngine.addGameItem(
                new Vector3f(0.0f, -10.0f, 5.0f),
                new Vector3f(0.0f, 0.0f, 0.0f),
                "Marlow66.obj"
        );

        gameEngine.addDirectionalLight(new DirectionalLight(color2, direction, 0.0f));
        //gameEngine.addSpotLight(new SpotLight(color2, new Vector3f(4.0f, 0.0f, -2.0f), 0.5f, att, direction, Math.cos(Math.toRadians(45))));

        gameEngine.addPointLight(color2, new Vector3f(26.0f, 0.0f, 0.0f), 1.0f, att);
    }

    private void addGuiElementsManual() {

    }

    public Events getEvents(){
        return events;
    }
}
