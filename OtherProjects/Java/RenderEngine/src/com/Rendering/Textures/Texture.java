package com.Rendering.Textures;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL30C.glGenerateMipmap;

public class Texture {
    int textureId;
    String textureName;
    int widthImage;
    int heightImage;
    ByteBuffer byteBuffer;

    public Texture(String name){
        textureName = name.replace("[", "").replace("]", "");
        BufferedImage rgbImage;

        if(textureName.isEmpty()) return;

        try {
            rgbImage = decodeImage(
                    System.getProperty("user.dir") + "\\Resources\\Textures\\" +textureName
            );
        } catch (RuntimeException e){
            rgbImage = decodeImage(System.getProperty("user.dir") + "\\Resources\\Textures\\Default\\default.png");
        }

        byteBuffer = getRGBA(rgbImage);
    }

    public  Texture(){
        textureName = "";
    }

    public BufferedImage decodeImage(String path) {
        try{
            //System.out.println("Path is: " +path);
            BufferedImage image = ImageIO.read(new File(path));
            //System.out.println(path);
            BufferedImage rgbImage = new BufferedImage(image.getWidth(), image.getHeight(), BufferedImage.TYPE_INT_ARGB);
            rgbImage.getGraphics().drawImage(image,0 ,0 ,null);
            widthImage = rgbImage.getWidth();
            heightImage = rgbImage.getHeight();
            return  rgbImage;
        } catch (IOException e) {
            throw new RuntimeException("Path "+path+" is invalid");
        }
    }

    public ByteBuffer getRGBA(BufferedImage rgbImage){

        int width = rgbImage.getWidth();
        int height = rgbImage.getHeight();
        int[] pixels = new int[width * height];
        rgbImage.getRGB(0, 0, width, height, pixels, 0, width);

        // Convert to byte array (RGBA order)
        byte[] rgbaBytes = new byte[width * height * 4];
        for (int i = 0; i < pixels.length; i++) {
            int argb = pixels[i];
            rgbaBytes[i * 4]     = (byte) ((argb >> 16) & 0xFF); // Red
            rgbaBytes[i * 4 + 1] = (byte) ((argb >> 8) & 0xFF);  // Green
            rgbaBytes[i * 4 + 2] = (byte) (argb & 0xFF);         // Blue
            rgbaBytes[i * 4 + 3] = (byte) ((argb >> 24) & 0xFF); // Alpha
        }
        ByteBuffer rgbBuffer = ByteBuffer.allocateDirect(4 * rgbImage.getWidth() * rgbImage.getHeight());
        rgbBuffer.put(rgbaBytes);
        rgbBuffer.flip();
        return rgbBuffer;
    }

    public void init(){
        if(textureName.isEmpty()) return;

        textureId = glGenTextures();
        glBindTexture(GL_TEXTURE_2D, textureId);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, byteBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    public int getTextureId(){
        return this.textureId;
    }

    public int getWidthImage() {
        return widthImage;
    }

    public int getHeightImage() {
        return heightImage;
    }

    public String getTextureName() {
        return textureName;
    }
}

