package com.Rendering.Mesh;

import com.Rendering.Textures.Texture;
import com.Rendering.VboData.VboData;
import com.Rendering.VboData.VboDataInt;

import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.HashSet;

public class NinePatchMesh extends Mesh {
    int[] patchIndices;

    public NinePatchMesh(MeshData meshData, Texture texture) {
        super(meshData, texture);
    }

    public NinePatchMesh(MeshData meshData) {
        super(meshData);
    }

    public NinePatchMesh(Mesh mesh, int[] patchIndices){
        super(mesh);
        this.patchIndices = patchIndices;

    }

    public void setPatchIndices(int[] patchIndices) {
        this.patchIndices = patchIndices;
    }

    public NinePatchMesh(MeshData meshData, int[] patchIndices) {
        super(meshData);
        this.patchIndices = patchIndices;
    }

    @Override
    public void init() {
        super.init();
    }

    @Override
    public void genBuffers(HashSet<VboData> vboDataSet) {
        vboDataSet.add(new VboDataInt("patchIndexBuffers", 3, 1, patchIndices));
        super.genBuffers(vboDataSet);
    }
}
