package com.Rendering.VboData;

import java.nio.IntBuffer;

import static org.lwjgl.opengl.GL15C.*;
import static org.lwjgl.opengl.GL15C.GL_ARRAY_BUFFER;
import static org.lwjgl.opengl.GL20.glEnableVertexAttribArray;
import static org.lwjgl.opengl.GL20.glVertexAttribPointer;
import static org.lwjgl.opengl.GL30C.glVertexAttribIPointer;
import static org.lwjgl.system.MemoryUtil.memAllocInt;
import static org.lwjgl.system.MemoryUtil.memFree;

public class VboDataInt extends VboData{

    IntBuffer content;

    public VboDataInt(String name, int location, int lengthData, int[] intArray) {
        super(name, location, lengthData);
        this.content = memAllocInt(intArray.length);
        putInBuffer(intArray);
    }


    private void putInBuffer(int[] intArray){
        content.put(intArray).flip();
    }

    @Override
    public void bind() {
        this.vboId = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, content, GL_STATIC_DRAW);
        glVertexAttribIPointer(location, lengthData, GL_INT,0, 0);
        glEnableVertexAttribArray(location);
        super.bind();
    }

    @Override
    public void freeMemory() {
        memFree(content);
    }
}
