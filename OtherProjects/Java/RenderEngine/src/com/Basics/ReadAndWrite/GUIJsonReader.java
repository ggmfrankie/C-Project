package com.Basics.ReadAndWrite;

import com.Engine.Events;
import com.Rendering.GUI.Elements.BaseGuiComponent;
import com.Rendering.GUI.Elements.Button;
import com.Rendering.Textures.PatchTexture;
import org.joml.Vector2f;

import java.util.HashSet;
import java.util.Map;

public class GUIJsonReader extends JsonReader{
    Events events;
    public GUIJsonReader(String name) {
        super("\\Resources\\Data\\GUI\\" +name);
        events = new Events();
        readFile();
        //printAllData();
    }

    public HashSet<BaseGuiComponent> getGuiComponents(){
        HashSet<BaseGuiComponent> guiComponents = new HashSet<>();
        Map<String, JsonObject> guiObjects = content.getObjectData();
        for(JsonObject object : guiObjects.values()){

            String name = object.getName();
            //System.out.println("name: " +name);
            JsonObject position = object.getObject("position");
            Vector2f pos = new Vector2f(position.getFloat("x"), position.getFloat("y"));

            JsonObject size = object.getObject("size");
            float width = size.getFloat("width");
            float height = size.getFloat("height");

            String texture = object.getString("texture");
            boolean isVisible = object.getBool("isVisible");
            int zIndex = object.getInt("zIndex");

            String type = object.getString("type");
            //System.out.println("type is: " +type);

            if(type.equals("Button")){
                //System.out.println("is a Button");
                JsonObject eventsObject = object.getObject("events");
                String eventName = eventsObject.getString("click");

                Button button = new Button(pos, width, height, name, new PatchTexture(texture));
                //System.out.println("searching for click action: "+eventName);
                button.setClickAction(events.getClickAction(eventName));
                guiComponents.add(button);
            }
        }
        return  guiComponents;
    }
}
