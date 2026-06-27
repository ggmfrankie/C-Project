package com.Rendering.Light;

import com.Rendering.Textures.Texture;
import org.joml.Vector4f;

import static org.lwjgl.opengl.GL11.glAreTexturesResident;

public class Material {
    Vector4f ambientColor;
    Vector4f diffuseColor;
    Vector4f specularColor;
    Vector4f emissionColor;
    float specularExponent;
    float opticalDensity;
    float dissolve;
    int illum;

    String name;

    Texture texture;

    public Material(String name,
                    Vector4f ambientColor,
                    Vector4f diffuseColor,
                    Vector4f specularColor,
                    Vector4f emissiveColor,
                    float specularExponent,
                    float opticalDensity,
                    float dissolve,
                    int illum,
                    Texture texture) {

        this.ambientColor = ambientColor;
        this.diffuseColor = diffuseColor;
        this.specularColor = specularColor;
        this.emissionColor = emissiveColor;

        this.specularExponent = specularExponent;

        this.opticalDensity = opticalDensity;
        this.dissolve = dissolve;

        this.illum = illum;

        this.name = name;
        this.texture = texture;
    }

    public Material(String name, Texture texture){
        setDefaultValues();
        this.name = name;
        this.texture = texture;
    }

    public Material(String name){
        this.name = name;
        setDefaultValues();
    }

    private void setDefaultValues(){
        this.ambientColor = new Vector4f(0.5f);
        this.diffuseColor = new Vector4f(0.5f);
        this.specularColor = new Vector4f(0.5f);
        this.emissionColor = new Vector4f(0.5f);

        this.specularExponent = 0.3f;

        this.opticalDensity = 1.0f;
        this.dissolve = 1.0f;

        this.illum = 2;
    }

    public Vector4f getEmissionColor() {
        return emissionColor;
    }

    public float getOpticalDensity() {
        return opticalDensity;
    }

    public float getDissolve() {
        return dissolve;
    }

    public int getIllum() {
        return illum;
    }

    public Vector4f getAmbientColor() {
        return ambientColor;
    }

    public Vector4f getDiffuseColor() {
        return diffuseColor;
    }

    public Vector4f getSpecularColor() {
        return specularColor;
    }


    public float getSpecularExponent() {
        return specularExponent;
    }

    public void setTexture(Texture texture){
        this.texture =texture;
    }

    public Texture getTexture() {
        return texture;
    }

    public boolean hasTexture() {
        return texture != null;
    }

    public String getName(){
        return name;
    }


    public void initTexture(){
        if(texture == null) return;
        texture.init();
    }
}
