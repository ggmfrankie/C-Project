package com.Rendering.Light;

import org.joml.Vector3f;

public class PointLight {
    Vector3f color;
    Vector3f position;
    float intensity;
    Attenuation att;

    public PointLight(Vector3f color, Vector3f position, float intensity, Attenuation att) {
        this.color = color;
        this.position = position;
        this.intensity = intensity;
        this.att = att;
    }

    public PointLight(PointLight pointLight){
        this.color = pointLight.getColor();
        this.position = pointLight.getPosition();
        this.intensity = pointLight.getIntensity();
        this.att = pointLight.getAttenuation();
    }

    public Attenuation getAttenuation() {
        return att;
    }

    public float getIntensity() {
        return intensity;
    }

    public Vector3f getPosition() {
        return position;
    }

    public Vector3f getColor() {
        return color;
    }

    public void setPosition(Vector3f position){
        this.position = position;
    }




    public static class Attenuation{
        float constant;
        float linear;
        float exponent;

        public Attenuation(float constant, float linear, float exponent) {
            this.constant = constant;
            this.linear = linear;
            this.exponent = exponent;
        }

        public float getConstant() {
            return constant;
        }

        public float getLinear() {
            return linear;
        }

        public float getExponent() {
            return exponent;
        }
    }
}

