package com.Rendering.Light;

import org.joml.Vector3f;

public class SpotLight extends PointLight{
    Vector3f direction;
    float angle;

    public SpotLight(Vector3f color, Vector3f position, float intensity, Attenuation att, Vector3f direction, float angle) {
        super(color, position, intensity, att);
        this.angle = angle;
        this.direction = direction;
    }

    public SpotLight(SpotLight spotLight){
        super(spotLight);
        this.direction = spotLight.direction;
        this.angle = spotLight.angle;
    }

    public Vector3f getDirection() {
        return direction;
    }

    public float getAngle() {
        return angle;
    }

    public void setDirection(Vector3f direction) {
        this.direction = direction;
    }

    public void setAngle(float angle) {
        this.angle = angle;
    }
}
