package com.Rendering.GUI;

import com.Basics.ReadAndWrite.FontLoader;
import com.Rendering.GUI.Elements.BaseGuiComponent;
import com.Rendering.GUI.Elements.Button;
import com.Rendering.Textures.PatchTexture;
import com.Shader.ShaderProgram;
import com.Window.Window;

import java.util.HashSet;

import static com.Basics.Utils.loadShaderFile;
import static org.lwjgl.opengl.GL11.GL_COLOR_BUFFER_BIT;
import static org.lwjgl.opengl.GL11.GL_DEPTH_BUFFER_BIT;
import static org.lwjgl.opengl.GL11C.*;

public class GUIRenderer {
    ShaderProgram shaderProgram;
    Transformations2D transformations;
    FontLoader fontLoader;
    HashSet<BaseGuiComponent> guiComponents;


    public GUIRenderer() {
        guiComponents = new HashSet<>();
        transformations = new Transformations2D();
        fontLoader = new FontLoader("MainFont2.fnt");
    }

    public void render(Window window){
        shaderProgram.bind();
        shaderProgram.setUniform("screenWidth", (float) window.getWidth());
        shaderProgram.setUniform("screenHeight", (float) window.getHeight());

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        for(BaseGuiComponent guiComponent : guiComponents){

            shaderProgram.setUniform("width", (float) guiComponent.getWidth());
            shaderProgram.setUniform("height", (float) guiComponent.getHeight());
            shaderProgram.setUniform("positionObject", guiComponent.getPosition());
            shaderProgram.setUniform("isActive", guiComponent.isActive() ? 1 : 0);
            if(guiComponent.isUsingNinePatch()) shaderProgram.setUniform("scale", transformations.recalculateStretch(guiComponent));
            shaderProgram.setUniform("hasTexture", guiComponent.getMesh().getMaterial().hasTexture() ? 1 : 0);
            if(guiComponent instanceof Button && guiComponent.getTexture() instanceof PatchTexture patchTexture) shaderProgram.setUniform("patchStretch", patchTexture);

            guiComponent.render();
        }
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        shaderProgram.unbind();
    }

    public void init() throws Exception {
        String fragmentShaderCode = loadShaderFile("\\GUI\\GuiFragment.glsl");
        String vertexShaderCode = loadShaderFile("\\GUI\\GuiVertex.glsl");
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shaderProgram = new ShaderProgram();
        shaderProgram.createVertexShader(vertexShaderCode);
        shaderProgram.createFragmentShader(fragmentShaderCode);
        shaderProgram.link();
        shaderProgram.createUniforms("isActive");
        shaderProgram.createUniforms("hasTexture");
        shaderProgram.createUniforms("screenHeight");
        shaderProgram.createUniforms("screenWidth");
        shaderProgram.createUniforms("positionObject");
        shaderProgram.createUniforms("width");
        shaderProgram.createUniforms("height");
        shaderProgram.createNinePatchUniform("patchStretch");
        shaderProgram.createScaleUniform("scale");
    }

    public void addGUIComponent(BaseGuiComponent guiComponent){
        guiComponent.getMesh().setShader(shaderProgram);
        guiComponents.add(guiComponent);
    }

    public void initGuiComponents(){
        for (BaseGuiComponent guiComponent : guiComponents) {
            guiComponent.init();
        }
    }

    public void cleanUp(){
        for (BaseGuiComponent guiComponent : guiComponents){
            guiComponent.getMesh().cleanUp();
        }
    }

    public void clear(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    public HashSet<BaseGuiComponent> getGuiComponents() {
        return guiComponents;
    }
}
