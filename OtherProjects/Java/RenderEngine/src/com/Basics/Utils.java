package com.Basics;

import com.Engine.GameEngine;
import org.joml.Vector2f;
import org.joml.Vector3f;
import org.joml.Vector4f;

import java.awt.image.BufferedImage;
import java.io.*;
import java.util.List;


public class Utils {
    static final boolean isDebug = false;
    public static GameEngine gameEngine;

    public static String loadShaderFile(String name){
        debugPrint("loading File...");

        return String.join("\n", readFile("\\Resources\\Shader\\" + name));
    }

    public static void debugPrint(String in){
        if(isDebug) System.out.println(in);
    }

    public static List<String> readFile(String path){
        List<String> fileContents = null;
        try{
            BufferedReader reader = new BufferedReader(
                    new FileReader(
                            System.getProperty("user.dir") + path));
            fileContents = reader.lines().toList();


        } catch (FileNotFoundException e){
            System.out.println("File not Found at "+ System.getProperty("user.dir") + path);
        }
        return fileContents;
    }

    public static float[] flattenToArray(Vector3f[] vec3s){
        float[] converted = new float[vec3s.length*3];
        for(int i = 0; i < vec3s.length; i++){
            Vector3f vec3 = vec3s[i];
            converted[i*3] = vec3.x;
            converted[i*3+1] = vec3.y;
            converted[i*3+2] = vec3.z;
        }
        return converted;
    }

    public static float[] flattenToArray(Vector2f[] vec2s){
        float[] converted = new float[vec2s.length*2];
        for(int i = 0; i < vec2s.length; i++){
            Vector2f vec2 = vec2s[i];
            converted[i*2] = vec2.x;
            converted[i*2+1] = vec2.y;
        }
        return converted;
    }

    public static float[] flattenListVec3(List<Vector3f> vec3s){
        return flattenToArray(vec3s.toArray(new Vector3f[0]));
    }

    public static float[] flattenListVec2(List<Vector2f> vec2s){
        return flattenToArray(vec2s.toArray(new Vector2f[0]));
    }
}
