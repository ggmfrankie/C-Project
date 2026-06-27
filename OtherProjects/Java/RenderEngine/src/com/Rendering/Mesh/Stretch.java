package com.Rendering.Mesh;

public class Stretch{

    public float left;
    public float right;
    public float top;
    public float bottom;

    public Stretch(float left, float right, float top, float bottom) {
        this.left = left;
        this.right = right;
        this.top = top;
        this.bottom = bottom;
    }

    public Stretch(){
        left = 0.0f;
        right = 0.0f;
        top = 0.0f;
        bottom = 0.0f;
    }
}
