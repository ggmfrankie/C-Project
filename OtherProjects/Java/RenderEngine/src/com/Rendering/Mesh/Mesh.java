package com.Rendering.Mesh;

import com.Rendering.Light.Material;
import com.Rendering.Textures.Texture;
import com.Rendering.VboData.VboData;
import com.Rendering.VboData.VboDataFloat;
import com.Shader.ShaderProgram;
import org.joml.Vector3f;
import org.lwjgl.opengl.GL11;

import java.nio.IntBuffer;
import java.util.*;

import static com.Basics.Utils.debugPrint;

import static org.lwjgl.opengl.ARBVertexArrayObject.*;
import static org.lwjgl.opengl.GL15C.GL_ARRAY_BUFFER;
import static org.lwjgl.opengl.GL15C.glBindBuffer;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.system.MemoryUtil.*;

public class Mesh {

    float[] vertices;
    float[] textCoords;
    float[] normals;

    int[] indices;

    int vertexCount;
    int vaoId;
    int indexVboId;

    String name;

    Vector3f color;
    ShaderProgram shaderProgram;
    Material material;
    HashSet<VboData> vboDataSet;


    public Mesh(MeshData meshData, String name){
        this.name = name;
        debugPrint("Creating mesh");
        initVariables(meshData);
    }
    public Mesh(MeshData meshData){
        name = null;
        debugPrint("Creating mesh");
        initVariables(meshData);
    }

    public Mesh(MeshData meshData, Texture texture, String name){
        this.name = name;
        initVariables(meshData);
        meshData.getMaterial().setTexture(texture);
        vertexCount = vertices.length/3;
    }

    public Mesh(MeshData meshData, Texture texture){
        name = null;
        initVariables(meshData);
        meshData.getMaterial().setTexture(texture);
        vertexCount = vertices.length/3;
    }

    public Mesh(Mesh mesh){
        this.name = mesh.getName();
        this.color = mesh.getColor();
        this.vertices = mesh.getVertices();
        this.indices = mesh.getIndices();
        this.textCoords = mesh.getTextCoords();
        this.normals = mesh.getNormals();
        this.material = mesh.getMaterial();
        this.vertexCount = mesh.getVertexCount();
        this.vaoId = mesh.getVaoId();
        this.indexVboId = mesh.getIndexVboId();
        this.shaderProgram = mesh.getShaderProgram();
        vboDataSet = mesh.getVboDataSet();
        vertexCount = mesh.getVertexCount();
    }

    private void initVariables(MeshData meshData){
        this.color = new Vector3f(0.5f, 0.0f, 0.0f);
        this.vertices = meshData.getVertices();
        this.indices = meshData.getIndices();
        this.textCoords = meshData.getTextCoords();
        this.normals = meshData.getNormals();
        this.material = meshData.getMaterial();
        vboDataSet = new HashSet<>();
        vertexCount = vertices.length/3;
    }

    public Vector3f getColor(){
        return color;
    }

    public void setShader(ShaderProgram shaderProgram){
        this.shaderProgram = shaderProgram;
    }

    public void init(){
        material.initTexture();
        VboData.locationsCheck = new HashSet<>();
        vaoId = glGenVertexArrays();
        glBindVertexArray(vaoId);

        genBuffers(vboDataSet);

        for(VboData vboData : vboDataSet){
            vboData.bind();
        }

        IntBuffer indicesBuffer = memAllocInt(indices.length);

        indicesBuffer.put(indices).flip();
        indexVboId = glGenBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        debugPrint("initializing mesh");

        memFree(indicesBuffer);

        System.gc();
    }

    public void genBuffers(HashSet<VboData> vboDataList){
        vboDataList.add(new VboDataFloat("verticesBuffer", 0, 3, vertices));
        vboDataList.add(new VboDataFloat("textCoordsBuffer", 1, 2, textCoords));
        vboDataList.add(new VboDataFloat("vecNormalsBuffer", 2, 3, normals));
    }

    public void render(){
        debugPrint(Arrays.toString(vertices));
        debugPrint("Render Mesh");

        if (material.hasTexture()) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material.getTexture().getTextureId());
        }
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(vaoId);

        enableAllAttribArrays();

        glDrawElements(GL_TRIANGLES, indices.length, GL_UNSIGNED_INT, 0);

        disableAllAttribArrays();

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    private void enableAllAttribArrays(){
        for(VboData vboData : vboDataSet){
            vboData.enableVertexAttribArray();
        }
    }

    private void disableAllAttribArrays(){
        for(VboData vboData : vboDataSet){
            vboData.disableVertexAttribArray();
        }
    }

    public void cleanUp() {
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        for(VboData vboData : vboDataSet){
            vboData.deleteBuffer();
        }
        glBindVertexArray(0);
        glDeleteVertexArrays(vaoId);
    }


    public Material getMaterial(){
        return this.material;
    }

    public float[] getVertices() {
        return vertices;
    }

    public float[] getTextCoords() {
        return textCoords;
    }

    public float[] getNormals() {
        return normals;
    }

    public int[] getIndices() {
        return indices;
    }

    public int getVertexCount() {
        return vertexCount;
    }

    public ShaderProgram getShaderProgram() {
        return shaderProgram;
    }

    public int getVaoId(){
        return vaoId;
    }

    public int getIndexVboId(){
        return indexVboId;
    }

    public String getName(){
        return this.name;
    }

    public HashSet<VboData> getVboDataSet() {
        return vboDataSet;
    }

    public void setTextCoords(float[] textCoords) {
        this.textCoords = textCoords;
    }
}
