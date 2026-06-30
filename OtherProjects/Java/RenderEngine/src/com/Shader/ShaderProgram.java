package com.Shader;

import com.Rendering.Light.DirectionalLight;
import com.Rendering.Light.Material;
import com.Rendering.Light.PointLight;
import com.Rendering.Light.SpotLight;
import com.Rendering.Mesh.Stretch;
import com.Rendering.Textures.PatchTexture;
import org.joml.Matrix4f;
import org.joml.Vector2f;
import org.joml.Vector3f;
import org.joml.Vector4f;
import org.lwjgl.system.MemoryStack;

import java.nio.FloatBuffer;
import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.opengl.GL20.*;

public class ShaderProgram {
    private final int programId;
    private int vertexShaderId;
    private int fragmentShaderId;
    private int timeUniformLocation;
    private final Map<String, Integer> uniforms;

    public ShaderProgram() throws Exception{
        programId = glCreateProgram();
        uniforms = new HashMap<>();
        if(programId == 0){
            throw new Exception("No com.Basics.Shader Program Created");
        }
    }

    public void createVertexShader(String shaderCode) throws Exception{
        vertexShaderId = createShader(shaderCode, GL_VERTEX_SHADER);
    }

    public void createFragmentShader(String shaderCode) throws Exception{
        fragmentShaderId = createShader(shaderCode, GL_FRAGMENT_SHADER);
    }

    public int createShader(String shaderCode, int shaderType) throws Exception{
        int shaderId = glCreateShader(shaderType);
        if(shaderId == 0){
            throw new Exception("Error creating shader. Type: " + shaderType);
        }

        glShaderSource(shaderId, shaderCode);
        glCompileShader(shaderId);

        if (glGetShaderi(shaderId, GL_COMPILE_STATUS) == 0) {
            throw new Exception("Error compiling com.Basics.Shader code: " + glGetShaderInfoLog(shaderId, 1024));
        }

        glAttachShader(programId,shaderId);
        return shaderId;
    }
    public void link() throws Exception {
        glLinkProgram(programId);
        if (glGetProgrami(programId, GL_LINK_STATUS) == 0) {
            throw new Exception("Error linking com.Basics.Shader code: " + glGetProgramInfoLog(programId, 1024));
        }

        if (vertexShaderId != 0) {
            glDetachShader(programId, vertexShaderId);
        }
        if (fragmentShaderId != 0) {
            glDetachShader(programId, fragmentShaderId);
        }

        glValidateProgram(programId);

        if (glGetProgrami(programId, GL_VALIDATE_STATUS) == 0) {
            System.err.println("Warning validating com.Basics.Shader code: " + glGetProgramInfoLog(programId, 1024));
        }

    }

    public void createUniforms(String uniformName) throws Exception{
        int uniformLocation = glGetUniformLocation(this.programId, uniformName);
        if(uniformLocation < 0){
            throw new Exception("Could not find uniform: " +
                    uniformName);
        }
        uniforms.put(uniformName, uniformLocation);
    }

    public void setUniform(String uniformName, Matrix4f value){
        try(MemoryStack stack =MemoryStack.stackPush()){
            FloatBuffer fb = stack.mallocFloat(16);
            value.get(fb);
            glUniformMatrix4fv(uniforms.get(uniformName), false, fb);
        }
    }
    public void setUniform(String uniformName, double value){
        glUniform1f(uniforms.get(uniformName), (float) value);
    }

    public void setUniform(String uniformName, int value){
        glUniform1i(uniforms.get(uniformName), value);
    }

    public void setUniform(String uniformName, Vector4f value){
        glUniform4f(uniforms.get(uniformName), value.x, value.y, value.z, value.w);
    }
    public void setUniform(String uniformName, Vector3f value){
        glUniform3f(uniforms.get(uniformName),  value.x, value.y, value.z);
    }
    public void setUniform(String uniformName, Vector2f value){
        glUniform2f(uniforms.get(uniformName),  value.x, value.y);
    }
    public void bind(){
        glUseProgram(programId);
    }
    public void unbind(){
        glUseProgram(0);
    }
    public void cleanup(){
        unbind();
        if (programId != 0){
            glDeleteProgram(programId);
        }
    }

    public void createPointLightUniform(String uniformName) throws Exception {
        createUniforms(uniformName + ".color");
        createUniforms(uniformName + ".position");
        createUniforms(uniformName + ".intensity");
        createUniforms(uniformName + ".att.constant");
        createUniforms(uniformName + ".att.linear");
        createUniforms(uniformName + ".att.exponent");

    }

    public void createSpotLightUniform(String uniformName) throws Exception{
        createUniforms(uniformName + ".color");
        createUniforms(uniformName + ".position");
        createUniforms(uniformName + ".intensity");
        createUniforms(uniformName + ".att.constant");
        createUniforms(uniformName + ".att.linear");
        createUniforms(uniformName + ".att.exponent");
        createUniforms(uniformName + ".direction");
        createUniforms(uniformName + ".angle");
    }

    public void createDirectionalLightUniform(String uniformName) throws Exception {
        createUniforms(uniformName + ".color");
        createUniforms(uniformName + ".direction");
        createUniforms(uniformName + ".intensity");
    }

    public void createMaterialUniform(String uniformName) throws Exception {
        createUniforms(uniformName + ".ambient");
        createUniforms(uniformName + ".diffuse");
        createUniforms(uniformName + ".specular");
        createUniforms(uniformName + ".emission");
        createUniforms(uniformName + ".specularExponent");
        createUniforms(uniformName + ".opticalDensity");
        createUniforms(uniformName + ".dissolve");
        createUniforms(uniformName + ".hasTexture");

    }

    public void createNinePatchUniform(String uniformName) throws Exception {
        createUniforms(uniformName + ".stretchStartX");
        createUniforms(uniformName + ".stretchStartY");
        createUniforms(uniformName + ".stretchEndX");
        createUniforms(uniformName + ".stretchEndY");
    }

    public void createScaleUniform(String uniformName) throws Exception{
        createUniforms(uniformName + ".left");
        createUniforms(uniformName + ".right");
        createUniforms(uniformName + ".top");
        createUniforms(uniformName + ".bottom");
    }

    public void setUniform(String uniformName, PointLight pointLight){
        setUniform(uniformName + ".color", pointLight.getColor() );
        setUniform(uniformName + ".position", pointLight.getPosition());
        setUniform(uniformName + ".intensity", pointLight.getIntensity());
        PointLight.Attenuation att = pointLight.getAttenuation();
        setUniform(uniformName + ".att.constant", att.getConstant());
        setUniform(uniformName + ".att.linear", att.getLinear());
        setUniform(uniformName + ".att.exponent", att.getExponent());
    }

    public void setUniform(String uniformName, SpotLight spotLight){
        setUniform(uniformName + ".color", spotLight.getColor() );
        setUniform(uniformName + ".position", spotLight.getPosition());
        setUniform(uniformName + ".intensity", spotLight.getIntensity());
        PointLight.Attenuation att = spotLight.getAttenuation();
        setUniform(uniformName + ".att.constant", att.getConstant());
        setUniform(uniformName + ".att.linear", att.getLinear());
        setUniform(uniformName + ".att.exponent", att.getExponent());
    }

    public void setUniform(String uniformName, Material material) {
        setUniform(uniformName + ".ambient", material.getAmbientColor());
        setUniform(uniformName + ".diffuse", material.getDiffuseColor());
        setUniform(uniformName + ".specular", material.getSpecularColor());
        setUniform(uniformName + ".emission", material.getEmissionColor());
        setUniform(uniformName + ".specularExponent", material.getSpecularExponent());
        setUniform(uniformName + ".opticalDensity", material.getOpticalDensity());
        setUniform(uniformName + ".dissolve", material.getDissolve());
        setUniform(uniformName + ".hasTexture", material.hasTexture() ? 1 : 0);
    }

    public void setUniform(String uniformName, DirectionalLight dirLight) {
        setUniform(uniformName + ".color", dirLight.getColor() );
        setUniform(uniformName + ".direction", dirLight.getDirection());
        setUniform(uniformName + ".intensity", dirLight.getIntensity());
    }

    public void setUniform(String uniformName, PatchTexture texture){
        setUniform(uniformName + ".stretchStartX", texture.getStretchStartX());
        setUniform(uniformName + ".stretchStartY", texture.getStretchStartY());
        setUniform(uniformName + ".stretchEndX", texture.getStretchEndX());
        setUniform(uniformName + ".stretchEndY", texture.getStretchEndY());
    }

    public void setUniform(String uniformName, Stretch stretch){
        setUniform(uniformName + ".left", stretch.left);
        setUniform(uniformName + ".right", stretch.right);
        setUniform(uniformName + ".top", stretch.top);
        setUniform(uniformName + ".bottom", stretch.bottom);
    }

    public long getFragmentShaderId(){
        return this.fragmentShaderId;
    }

    public int getTimeUniformLocation(){
        return this.timeUniformLocation;
    }
}
