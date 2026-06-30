package com.Rendering.GUI;

import com.Rendering.GUI.Elements.BaseGuiComponent;
import com.Rendering.Light.Material;
import com.Rendering.Mesh.Stretch;
import com.Rendering.Textures.PatchTexture;


public class Transformations2D {

    public Transformations2D() {

    }

    public Stretch recalculateStretch(BaseGuiComponent component){
        Material material = component.getMesh().getMaterial();

        if(!(material.getTexture() instanceof PatchTexture patchTexture)) return null;

        Stretch stretch = new Stretch();

        float stretchStartX = patchTexture.getStretchStartX();
        float stretchEndX = patchTexture.getStretchEndX();
        float stretchStartY = patchTexture.getStretchStartY();
        float stretchEndY = patchTexture.getStretchEndY();

        float textureHeight = patchTexture.getHeightImage();
        float textureWidth = patchTexture.getWidthImage();

        float componentWidth = component.getWidth();
        float componentHeight = component.getHeight();

        float actualWidthCorner = 1/3f * componentWidth;
        float actualHeightCorner = 1/3f * componentHeight;

        float stretchHorizontalLeft = -(actualWidthCorner - textureWidth * stretchStartX);
        float stretchHorizontalRight = (actualWidthCorner - textureWidth * (1.0f - stretchEndX));
        float stretchVerticalTop = -(actualHeightCorner - textureHeight * stretchStartY);
        float stretchVerticalBottom = (actualHeightCorner - textureHeight * (1.0f - stretchEndY));


        stretch.left = stretchHorizontalLeft;
        //System.out.println("--------------------------------------");
        //System.out.println("stretchHorizontal");
        //System.out.println(stretchHorizontalLeft);
        stretch.right = stretchHorizontalRight;

        stretch.top = stretchVerticalTop;
        //System.out.println("--------------------------------------");
        //System.out.println("stretchVertical");
        //System.out.println(stretchVerticalTop);
        stretch.bottom = stretchVerticalBottom;

        return stretch;
    }
}
