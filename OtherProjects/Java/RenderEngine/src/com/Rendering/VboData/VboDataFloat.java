package com.Rendering.VboData;

import java.nio.FloatBuffer;
import java.util.Arrays;

import static org.lwjgl.opengl.GL15C.*;
import static org.lwjgl.opengl.GL20.glEnableVertexAttribArray;
import static org.lwjgl.opengl.GL20.glVertexAttribPointer;
import static org.lwjgl.system.MemoryUtil.memAllocFloat;
import static org.lwjgl.system.MemoryUtil.memFree;

public class VboDataFloat extends VboData{

    FloatBuffer content;

    public VboDataFloat(String name, int location, int lengthData, float[] floatArray) {
        super(name, location, lengthData);

        //System.out.println(Arrays.toString(floatArray));

        this.content = memAllocFloat(floatArray.length);
        putInBuffer(floatArray);
    }

    private void putInBuffer(float[] floatArray){
        content.put(floatArray).flip();
    }

    @Override
    public void bind(){
        this.vboId = glGenBuffers();

        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, content, GL_STATIC_DRAW);
        glVertexAttribPointer(location, lengthData, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(location);
        super.bind();
    }

    @Override
    public void freeMemory(){
        memFree(content);
    }

    @Override
    public FloatBuffer getContentF(){
        return this.content;
    }
}

