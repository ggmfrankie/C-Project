package com.Basics.ReadAndWrite;

import com.Basics.Interface.ClickAction;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import static com.Basics.Utils.gameEngine;

public class JsonWriter {

    File file;
    List<String> content;
    FileWriter writer;
    String path;

    public JsonWriter(String path) {
        path = System.getProperty("user.dir") +path;
        file = new File(path);
        this.path = path;

        content = new ArrayList<>();
        content.add("{");
    }

    public void printToFile(){
        try {
            int lastIndex = content.size()-1;
            content.set(lastIndex, content.get(lastIndex).replace(",", ""));
            content.add("}");
            System.out.println(path);
            writer = new FileWriter(path);
            for(String s : content){
                writer.append(s).append(System.lineSeparator());
            }
            writer.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public String getName(ClickAction clickAction){
        return gameEngine.getObjectLoader().getEvents().getName(clickAction);
    }

    class JsonObject{

        List<String> content;

        public JsonObject(String name) {
            content = new ArrayList<>();
            content.add(format(name) + ": {");
        }

        public void addData(String key, String value){
            content.add(format(key)  + ": " + format(value) +",");
        }

        public void addData(String key, int value){
            content.add(format(key)  + ": " + Integer.toString(value) +",");
        }

        public void addData(String key, float value){
            content.add(format(key)  + ": " + Float.toString(value) +",");
        }

        public void addData(String key, boolean value){
            content.add(format(key)  + ": " + Boolean.toString(value) +",");
        }

        public void addData(GUIJsonWriter.JsonObject object){
            content.addAll(object.getList());
        }

        private String format(String s){
            return ('"'+s+'"');
        }

        public List<String> getList(){
            int lastIndex = content.size()-1;
            content.add("},");
            for (int i = 1 ; i < content.size() -1 ; i++){
                content.set(i, "  " +content.get(i));
                //content.set(i, content.get(i).replace("\\", "\\\\"));
            }
            content.set(lastIndex, content.get(lastIndex).replace(",", ""));
            return content;
        }
    }
}
