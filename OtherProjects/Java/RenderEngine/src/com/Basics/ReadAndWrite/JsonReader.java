package com.Basics.ReadAndWrite;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.HashMap;
import java.util.Map;

public class JsonReader {
    File file;
    String path;
    JsonObject content;

    public JsonReader(String path) {
        path = System.getProperty("user.dir") +path;
        file = new File(path);
        this.path = path;
    }

    public void readFile(){
        try {
            String fileContent = Files.readString(file.toPath()).substring(1);
            content = convertData(new JsonObject("main"), fileContent.trim());
            printObjects(content);

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }



    private JsonObject convertData(JsonObject object, String file){
        boolean inValue = false;
        boolean inKey = false;
        boolean isFirstChar = true;

        int quoteCount = 0;
        int dataType = -1;

        StringBuilder key = new StringBuilder();
        StringBuilder value = new StringBuilder();


        for (int i = 0; i < file.length(); i++) {
            char currChar = file.charAt(i);
            if (!inValue) {
                if (currChar == '"') {
                    if (quoteCount < 2) {
                        inKey = !inKey;
                        quoteCount++;
                    }
                    continue;
                }
                if (inKey) key.append(currChar);
                else if (currChar == ':') {
                    i++;
                    inValue = true;
                    isFirstChar = true;

                } else {
                    if (currChar == '}') return object;
                }
            } else {
                if (currChar == '{') {
                    String name = key.toString();
                    key.setLength(0);
                    value.setLength(0);
                    inValue = false;
                    quoteCount = 0;
                    dataType = 3;

                    String objectString = getObject(file.substring(i));
                    int objectLength = objectString.length();
                    object.addData(name, convertData(new JsonObject(name), objectString));
                    i += objectLength;
                    //System.out.println("object is: " +objectString);
                    isFirstChar = true;
                    inKey = false;
                    continue;
                }
                if (isFirstChar) {
                    //System.out.println("First char is: " + currChar);
                    if (currChar == '"') {
                        dataType = 1;
                    } else if (currChar == 't' || currChar == 'f') {
                        dataType = 2;
                    } else {
                        dataType = 0;
                    }
                    isFirstChar = false;
                } else if (currChar == ',' || currChar == '}') {
                    String name = key.toString().trim();
                    key.setLength(0);
                    String finalValue = value.toString().trim();
                    value.setLength(0);
                    isFirstChar = true;
                    inValue = false;
                    inKey = false;
                    quoteCount = 0;

                    switch (dataType) {
                        case 0 -> {
                            if (finalValue.contains(".")) object.addData(name, Float.parseFloat(finalValue));
                            else object.addData(name, Integer.parseInt(finalValue));
                        }
                        case 1 -> {
                            String finalString = finalValue.substring(1, finalValue.length()-1);
                            object.addData(name, finalString);
                        }
                        case 2 -> object.addData(name, Boolean.parseBoolean(finalValue));
                        case 3 ->{

                        }
                        default -> System.out.println("Error when parsing: " + value);
                    }
                    dataType = -1;
                    continue;
                }
                value.append(currChar);
            }
        }
        return object;
    }

    private String getObject(String s){
        //System.out.println("String in the length calculator is: " + s);
        int countBracks = 0;
        for(int i = 0; i < s.length(); i++){
            char currentChar = s.charAt(i);
            if(currentChar == '}') countBracks--;
            else if (currentChar == '{') countBracks++;
            if(countBracks == 0) return s.substring(0, i+1);
        }
        return "";
    }
    private void printObjects(JsonObject object){
        System.out.println(object.getObjectData().keySet());
    }

    static class JsonObject{
        String name;
        Map<String, Boolean> booleanData;
        Map<String, Integer> integerData;
        Map<String, Float> floatData;
        Map<String, String> stringData;
        Map<String, JsonObject> objectData;

        public JsonObject(String name){
            this.name = name;
            booleanData = new HashMap<>();
            integerData = new HashMap<>();
            floatData = new HashMap<>();
            stringData = new HashMap<>();
            objectData = new HashMap<>();
        }

        public void addData(String key, boolean value){
            booleanData.put(key, value);
        }

        public void addData(String key, int value){
            integerData.put(key, value);
        }

        public void addData(String key, float value){
            floatData.put(key, value);
        }

        public void addData(String key, String value){
            stringData.put(key, value);
        }

        public void addData(String key, JsonObject value){
            objectData.put(key, value);
        }

        public boolean getBool(String key){
            return booleanData.get(key);
        }

        public int getInt(String key){
            return integerData.get(key);
        }

        public float getFloat(String key){
            return floatData.get(key);
        }

        public String getString(String key){
            return stringData.get(key);
        }

        public JsonObject getObject(String key){
            return objectData.get(key);
        }

        public String getName() {
            return name;
        }

        public Map<String, Boolean> getBooleanData() {
            return booleanData;
        }

        public Map<String, Integer> getIntegerData() {
            return integerData;
        }

        public Map<String, Float> getFloatData() {
            return floatData;
        }

        public Map<String, String> getStringData() {
            return stringData;
        }

        public Map<String, JsonObject> getObjectData() {
            return objectData;
        }


    }
}
