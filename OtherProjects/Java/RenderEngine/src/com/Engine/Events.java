package com.Engine;

import com.Basics.Interface.ClickAction;
import com.Basics.Interface.UpdateAction;
import com.Window.Window;
import org.joml.Vector2f;
import org.joml.Vector3f;

import java.util.HashMap;
import java.util.Map;

import static com.Basics.Utils.gameEngine;

public class Events {
    Map<String, ClickAction> clickActions;
    Map<String, UpdateAction> updateActions;

    private static Vector2f dragOffset;

    public Events(){
        clickActions = new HashMap<>();
        updateActions = new HashMap<>();
        init();
    }

    private void init(){
        clickActions.put(
                "dragging",
                (component) -> {
                    Window window = gameEngine.getWindow();
                    Vector2f mousePos = window.getMousePosition();
                    component.setPosition(mousePos);
        });

        updateActions.put(
                "spin",
                (gameItem) -> {
                    Vector3f rotation = gameItem.getRotation();
                    Vector3f rotationNew = new Vector3f(
                            rotation.x + 0.0005f,
                            rotation.y + 0.0005f,
                            rotation.z +0.0005f
                            );
                    gameItem.setRotation(rotationNew);
        });
    }

    public static void drag(){

    }

    public ClickAction getClickAction(String key){
        return clickActions.get(key);
    }

    public UpdateAction getUpdateAction(String key) {
        return updateActions.get(key);
    }

    public String getName(ClickAction clickAction){
        for (Map.Entry<String, ClickAction> entry : clickActions.entrySet()) {
            if (entry.getValue().equals(clickAction)) {
                return entry.getKey();
            }
        }
        return null;
    }

    public void addEvent(String key, ClickAction clickAction){
        clickActions.put(key, clickAction);
    }

    public Map<String, ClickAction> getEventMap(){
        return clickActions;
    }








}
