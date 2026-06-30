package com.Rendering.GUI.Elements;

import com.Rendering.Light.Material;
import com.Rendering.Mesh.Mesh;
import com.Rendering.Mesh.MeshData;
import com.Rendering.Mesh.NinePatchMesh;
import com.Rendering.Textures.PatchTexture;
import org.joml.Vector2f;
import org.joml.Vector3f;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static com.Basics.Utils.*;

public class MeshGenerator {

    public NinePatchMesh generateNinePatchMesh(Vector2f start, Vector2f end, PatchTexture texture){

        int[] patchIndices = new int[]{
                0, 1, -1, 0,
                2, 3, 4, 2,
                -2, 5, 6, -2,
                0, 1, -1, 0
        };

        float[] texX = new float[]{
                0.0f,
                texture.getStretchStartX(),
                texture.getStretchEndX(),
                1.0f
        };

        float[] texY = new float[]{
                0.0f,
                texture.getStretchStartY(),
                texture.getStretchEndY(),
                1.0f
        };
        List<Vector2f> texCoordQuad = new ArrayList<>();

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                texCoordQuad.add(new Vector2f(texX[j], texY[i]));
            }
        }

        NinePatchMesh mesh = new NinePatchMesh(generateQuads(4, 4, start, end), patchIndices);

        mesh.setTextCoords(flattenListVec2(texCoordQuad));
        mesh.getMaterial().setTexture(texture);
        return mesh;
    }

    public Mesh generateQuads(int numRows, int numCols, Vector2f start, Vector2f end){
        List<Vector3f> verticesQuad = new ArrayList<>();
        List<Vector3f> normalsQuad = new ArrayList<>();
        List<Vector2f> texCoordQuad = new ArrayList<>();

        List<Integer> indicesQuads = new ArrayList<>();

        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                float x = (numRows > 1) ? j * ((end.x-start.x)/(numRows-1)) + start.x : 0;
                float y = (numCols > 1) ? i * ((end.y-start.y)/(numCols-1)) + start.y : 0;

                verticesQuad.add(new Vector3f(x, y, 0.0f));
                normalsQuad.add(new Vector3f(0.0f, 0.0f, 1.0f));
                texCoordQuad.add(new Vector2f((float) j /((numRows > 1) ?  numRows-1 : 0), (float) i / ((numCols > 1) ? numCols-1 : 0)));

            }
        }

        for (int row = 0; row < numRows-1; row++) {
            for (int col = 0; col < numCols-1; col++) {
                int topLeft = row * 4 + col;
                int topRight = topLeft + 1;
                int bottomLeft = topLeft + 4;
                int bottomRight = bottomLeft + 1;

                // First triangle
                indicesQuads.add(topLeft);
                indicesQuads.add(bottomLeft);
                indicesQuads.add(topRight);

                // Second triangle
                indicesQuads.add(topRight);
                indicesQuads.add(bottomLeft);
                indicesQuads.add(bottomRight);
            }
        }

        float[] vertices = flattenListVec3(verticesQuad);

        float[] normals = flattenListVec3(normalsQuad);
        int[] indices = indicesQuads.stream().mapToInt(Integer::intValue).toArray();
        float[] texCoords = flattenListVec2(texCoordQuad);

        MeshData meshData = new MeshData(vertices, normals, texCoords, indices);

        return new Mesh(meshData, "NinePatch");
    }

    public Mesh generateBasicQuad(){
        float[] vertices = {
                +0.5f, +0.5f, 0,
                +0.5f, -0.5f, 0,
                -0.5f, +0.5f, 0,
                -0.5f, -0.5f, 0
        };
        float[] normals = {
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f
        };
        float[] textures = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f
        };

        int[] indices = {0, 1, 2, 2, 1, 3};

        MeshData meshData = new MeshData(vertices, normals, textures, indices, new Material("component"));
        return new Mesh(meshData, "Basic Quad");
    }
}
