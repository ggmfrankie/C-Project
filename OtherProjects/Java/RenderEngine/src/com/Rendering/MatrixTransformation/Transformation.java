package com.Rendering.MatrixTransformation;

import com.Engine.GameObjects.GameItem;
import com.Rendering.Light.DirectionalLight;
import com.Rendering.Light.PointLight;
import com.Rendering.Light.SpotLight;
import org.joml.Math;
import org.joml.Matrix4f;
import org.joml.Vector3f;
import org.joml.Vector4f;

public class Transformation {
    private final Matrix4f projectionMatrix;
    private final Matrix4f worldMatrix;
    private final Matrix4f modelViewMatrix;
    private final Matrix4f viewMatrix;


    public Transformation(){
        worldMatrix = new Matrix4f();
        projectionMatrix = new Matrix4f();
        modelViewMatrix = new Matrix4f();
        viewMatrix = new Matrix4f();
    }

    /*public PointLight getPointLight(PointLight pointLight){
        PointLight currPointLight = new PointLight(pointLight);
        Vector3f lightPos = currPointLight.getPosition();
        Vector4f aux = new Vector4f(lightPos, 1);
        aux.mul(viewMatrix);
        lightPos.x = aux.x;
        lightPos.y = aux.y;
        lightPos.z = aux.z;
        return currPointLight;
    }

     */

    public PointLight getPointLight(PointLight pointLight){
        PointLight currPointLight = new PointLight(pointLight);
        Vector3f lightPos = new Vector3f(currPointLight.getPosition());
        Vector4f aux = new Vector4f(lightPos, 1);
        aux.mul(viewMatrix);
        currPointLight.setPosition(new Vector3f(aux.x, aux.y, aux.z));
        return currPointLight;
    }

    public SpotLight getSpotLight(SpotLight spotLight){
        SpotLight currSpotLight = new SpotLight(spotLight);
        Vector4f dir = new Vector4f(currSpotLight.getDirection(), 0);
        Vector3f lightPos = new Vector3f(currSpotLight.getPosition());
        Vector4f aux = new Vector4f(lightPos, 1);
        dir.mul(viewMatrix);
        aux.mul(viewMatrix);
        dir.normalize();
        currSpotLight.setDirection(new Vector3f(dir.x, dir.y, dir.z));
        currSpotLight.setPosition(new Vector3f(aux.x, aux.y, aux.z));
        return currSpotLight;
    }

    public DirectionalLight getDirectionalLight(DirectionalLight directionalLight){
        DirectionalLight currDirectionalLight = new DirectionalLight(directionalLight);
        Vector4f dir = new Vector4f(currDirectionalLight.getDirection(), 0);
        dir.mul(viewMatrix);
        dir.normalize();
        currDirectionalLight.setDirection(new Vector3f(dir.x, dir.y, dir.z));
        return currDirectionalLight;
    }

    public final Matrix4f getProjectionMatrix(float fov, float width, float height, float zNear, float zFar){
        float aspectRatio = width/height;
        projectionMatrix.identity();
        projectionMatrix.perspective(fov, aspectRatio, zNear, zFar);
        return projectionMatrix;
    }

    public Matrix4f getWorldMatrix(Vector3f offset, Vector3f rotation, float scale){
        worldMatrix.identity().translate(offset).
                rotateX((float) Math.toRadians(rotation.x)).
                rotateY((float) Math.toRadians(rotation.y)).
                rotateZ((float) Math.toRadians(rotation.z)).
                scale(scale);
        return worldMatrix;
    }
    public Matrix4f getModelViewMatrix(GameItem gameItem, Matrix4f viewMatrix) {
        Vector3f rotation = gameItem.getRotation();
        modelViewMatrix.identity().translate(gameItem.getPosition()).
                rotateX((float)Math.toRadians(-rotation.x)).
                rotateY((float)Math.toRadians(-rotation.y)).
                rotateZ((float)Math.toRadians(-rotation.z)).
                scale(gameItem.getScale());
        Matrix4f viewCurr = new Matrix4f(viewMatrix);
        return viewCurr.mul(modelViewMatrix);
    }

    public Matrix4f getViewMatrix(Camera camera){
        Vector3f cameraPos = camera.getPosition();
        Vector3f rotation = camera.getRotation();

        viewMatrix.identity();

        viewMatrix.rotate((float) Math.toRadians(rotation.x), new Vector3f(1,0,0))
                .rotate((float) Math.toRadians(rotation.y), new Vector3f(0,1,0));
        viewMatrix.translate(-cameraPos.x, -cameraPos.y, -cameraPos.z);
        return  viewMatrix;
    }

    public void setViewMatrix(Matrix4f viewMatrix) {
        this.viewMatrix.set(viewMatrix);
    }
}
