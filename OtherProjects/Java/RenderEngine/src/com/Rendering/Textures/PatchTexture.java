package com.Rendering.Textures;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.nio.ByteBuffer;

public class PatchTexture extends Texture{
    float stretchStartX;
    float stretchEndX;
    float stretchStartY;
    float stretchEndY;

    public PatchTexture() {
        super();
    }

    public PatchTexture(String name) {
        super(name);
    }

    @Override
    public ByteBuffer getRGBA(BufferedImage rgbImage) {

        int width = rgbImage.getWidth();
        int height = rgbImage.getHeight();
        BufferedImage cropped = rgbImage.getSubimage(1, 1, width - 2, height - 2);
        stretchStartX = -1;
        stretchEndX = -1;
        for(int x = 0; x < width; x++){
            int pixel = rgbImage.getRGB(x, 0);
            if((pixel & 0xFF000000) == 0xFF000000){
                if(stretchStartX == -1) stretchStartX = x -1;
                stretchEndX = x - 1;
            }
        }
        stretchStartY = -1;
        stretchEndY = -1;
        for(int y = 0; y < height; y++){
            int pixel = rgbImage.getRGB(0, y);
            if((pixel & 0xFF000000) == 0xFF000000){
                if(stretchStartY == -1) stretchStartY = y -1;
                stretchEndY = y - 1;
            }
        }

        widthImage = cropped.getWidth();
        heightImage = cropped.getHeight();

        stretchStartX = stretchStartX/widthImage;
        stretchEndX = stretchEndX/widthImage;
        stretchStartY = stretchStartY/heightImage;
        stretchEndY = stretchEndY/heightImage;

        return super.getRGBA(cropped);
    }

    public float getStretchStartX() {
        return stretchStartX;
    }

    public float getStretchEndX() {
        return stretchEndX;
    }

    public float getStretchStartY() {
        return stretchStartY;
    }

    public float getStretchEndY() {
        return stretchEndY;
    }
}
