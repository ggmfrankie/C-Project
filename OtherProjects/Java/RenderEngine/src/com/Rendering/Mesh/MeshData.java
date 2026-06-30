package com.Rendering.Mesh;

import com.Rendering.Light.Material;

public class MeshData {
    public float[] vertices;
    public int[] indices;
    float[] textCoords;
    float[] normals;
    Material material;

    public MeshData(float[] vertices,float[] normals, float[] textCoords, int[] indices, Material material) {
        this.vertices = vertices;
        this.indices = indices;
        this.textCoords = textCoords;
        this.normals = normals;
        this.material = material;
    }

    public MeshData(float[] vertices,float[] normals, float[] textCoords, int[] indices) {
        this.vertices = vertices;
        this.indices = indices;
        this.textCoords = textCoords;
        this.normals = normals;
        this.material = new Material("Default");
    }

    public float[] getVertices() {
        return this.vertices;
    }

    public int[] getIndices() {
        return this.indices;
    }

    public float[] getTextCoords() {
        return this.textCoords;
    }

    public float[] getNormals() {
        return this.normals;
    }

    public Material getMaterial() {
        return  this.material;
    }
}
