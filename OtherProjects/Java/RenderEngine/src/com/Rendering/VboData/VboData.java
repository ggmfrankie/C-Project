package com.Rendering.VboData;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.HashSet;

import static com.Basics.Utils.debugPrint;
import static org.lwjgl.opengl.GL15C.*;
import static org.lwjgl.opengl.GL20.glDisableVertexAttribArray;
import static org.lwjgl.opengl.GL20.glEnableVertexAttribArray;

public class VboData {

    public static HashSet<Integer> locationsCheck;

    int location;
    int lengthData;
    int vboId;

    String name;

    public VboData(String name, int location, int lengthData) {
        this.name = name;
        this.location = location;
        this.lengthData = lengthData;
        //only unique locations
        debugPrint("HashSet is: " +locationsCheck);
        if(!locationsCheck.add(location)) throw new NotUniqueLocationException("Location " +location+ " is not unique in set: " +locationsCheck);
    }

    public void bind(){
        freeMemory();
    }

    public void enableVertexAttribArray(){
        glEnableVertexAttribArray(this.location);
    }

    public void disableVertexAttribArray(){
        glDisableVertexAttribArray(this.location);
    }

    public void deleteBuffer(){
        glDeleteBuffers(this.vboId);
    }

    public void freeMemory(){

    }

    public int getLocation(){
        return location;
    }

    public FloatBuffer getContentF(){
        return null;
    }

    public IntBuffer getContentI(){
        return null;
    }
}
