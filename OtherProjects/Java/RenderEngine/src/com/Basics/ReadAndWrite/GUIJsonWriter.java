package com.Basics.ReadAndWrite;

import com.Rendering.GUI.Elements.BaseGuiComponent;
import com.Rendering.GUI.Elements.IClickable;

public class GUIJsonWriter extends JsonWriter {


    public GUIJsonWriter(String name) {
        super("\\Resources\\Data\\GUI\\" +name);
    }

    public void addGuiComponent(BaseGuiComponent component){
        JsonObject object = new JsonObject(component.getName());
        object.addData("type", component.getType());

        JsonObject position = new JsonObject("position");
            position.addData("x", component.getPosition().x);
            position.addData("y", component.getPosition().y);
        object.addData(position);

        JsonObject size = new JsonObject("size");
            size.addData("width", component.getWidth());
            size.addData("height", component.getHeight());
        object.addData(size);

        object.addData("texture", component.getTexture().getTextureName());
        object.addData("isVisible", component.isVisible());
        object.addData("zIndex", component.getZIndex());

        if(component instanceof IClickable clickable){
            JsonObject events = new JsonObject("events");
            events.addData("click", getName(clickable.getClickAction()));
            object.addData(events);
        }
        content.addAll(object.getList());
    }
}
