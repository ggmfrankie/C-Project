package com.Rendering;

import com.Engine.GameObjects.GameItem;
import com.Rendering.Light.DirectionalLight;
import com.Rendering.Light.PointLight;
import com.Rendering.Light.SpotLight;
import com.Rendering.MatrixTransformation.Camera;
import com.Rendering.MatrixTransformation.Transformation;
import com.Rendering.Mesh.Mesh;
import com.Shader.ShaderProgram;
import com.Window.Window;
import org.joml.Matrix4f;
import org.joml.Vector3f;

import java.util.HashSet;

import static com.Basics.Utils.*;

import static org.lwjgl.opengl.GL20.*;

public class Renderer {
    ShaderProgram shaderProgram;
    HashSet<GameItem> gameItems;
    Mesh currentMesh;
    Camera camera;
    private static final float FOV = (float) Math.toRadians(60.0f);

    private static final float Z_NEAR = 0.01f;

    private static final float Z_FAR = 1000.f;

    private final Transformation transformation;
    private PointLight pointLight;
    private DirectionalLight directionalLight;
    private SpotLight spotLight;


    public Renderer(){
        debugPrint("Creating Renderer Object");
        try {
            transformation = new Transformation();

            camera = new Camera();

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        gameItems = new HashSet<>();

    }
    public void addPointLight(Vector3f color, Vector3f position, float intensity, PointLight.Attenuation att){
        PointLight newPointLight = new PointLight(color, position, intensity, att);
        addPointLight(newPointLight);
    }

    public void addPointLight(PointLight pointLight){
        this.pointLight = pointLight;
    }

    public void addSpotLight(SpotLight spotLight){
        this.spotLight = spotLight;
    }

    public void addDirectionalLight(DirectionalLight directionalLight){
        this.directionalLight = directionalLight;
    }

    public void init() throws Exception{
        debugPrint("initializing Renderer");
        String fragmentShaderCode = loadShaderFile("ImprovedFragmentShader.glsl");
        String vertexShaderCode = loadShaderFile("VertexShader.glsl");

        glEnable(GL_DEPTH_TEST);

        shaderProgram = new ShaderProgram();
        shaderProgram.createVertexShader(vertexShaderCode);
        shaderProgram.createFragmentShader(fragmentShaderCode);
        shaderProgram.link();
        shaderProgram.createUniforms("projectionMatrix");
        shaderProgram.createUniforms("modelViewMatrix");
        shaderProgram.createUniforms("texture_sampler");
        shaderProgram.createMaterialUniform("material");
        shaderProgram.createPointLightUniform("pointLight");
        shaderProgram.createDirectionalLightUniform("directionalLight");
        shaderProgram.createSpotLightUniform("spotLight");

        setUniforms();
    }

    public void setUniforms(){
        shaderProgram.setUniform("texture_sampler", 0);
    }

    public void addGameItem(GameItem gameItem){
        debugPrint("Adding Mesh");
        for(Mesh mesh : gameItem.getMeshes()){
            mesh.setShader(shaderProgram);
        }
        gameItems.add(gameItem);
    }

    public void initGameItems(){
        for(GameItem gameItem : gameItems){
            gameItem.init();
        }
    }

    public void setCurrentMesh(Mesh currentMesh){
        this.currentMesh = currentMesh;
    }

    public void render(Window window) {
        debugPrint("Renderer Render function");
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Set clear color to white
        clear();
        shaderProgram.bind();

        Matrix4f viewMatrix = transformation.getViewMatrix(camera);
        Matrix4f projectionMatrix = transformation.getProjectionMatrix(FOV, window.getWidth(), window.getHeight(), Z_NEAR, Z_FAR);

        shaderProgram.setUniform("projectionMatrix", projectionMatrix);
        if(pointLight != null) shaderProgram.setUniform("pointLight", transformation.getPointLight(pointLight));
        if(directionalLight != null) shaderProgram.setUniform("directionalLight", transformation.getDirectionalLight(directionalLight));
        if(spotLight != null) shaderProgram.setUniform("spotLight", transformation.getSpotLight(spotLight));

        for (GameItem gameItem : gameItems) {

            Matrix4f modelViewMatrix = transformation.getModelViewMatrix(gameItem, viewMatrix);
            for (Mesh mesh : gameItem.getMeshes()){
                shaderProgram.setUniform("material", mesh.getMaterial());
            }
            shaderProgram.setUniform("modelViewMatrix", modelViewMatrix);
            gameItem.render();
        }
        shaderProgram.unbind();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    public void cleanUp(){
        for (GameItem gameItem : gameItems){
            gameItem.cleanUp();
        }
    }

    public void clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    public Camera getCamera() {
        return camera;
    }
    public DirectionalLight getDirectionalLight(){
        return directionalLight;
    }

    public HashSet<GameItem> getGameItems(){
        return gameItems;
    }
}
