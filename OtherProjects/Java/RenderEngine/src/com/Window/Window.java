package com.Window;


import org.joml.Vector2f;
import org.joml.Vector3f;

import org.lwjgl.glfw.GLFW;
import org.lwjgl.glfw.GLFWVidMode;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.*;

import java.nio.*;


import static com.Basics.Utils.debugPrint;
import static java.sql.Types.NULL;
import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL11C.GL_COLOR_BUFFER_BIT;
import static org.lwjgl.opengl.GL11C.GL_DEPTH_BUFFER_BIT;
import static org.lwjgl.opengl.GL11C.glClear;
import static org.lwjgl.system.MemoryStack.*;

public class Window {

    private long window;
    int width;
    int height;
    String windowName;

    Vector3f cameraInc;

    public Window(String windowName, int width, int height){



        this.width = width;
        this.height = height;
        this.windowName = windowName;


        cameraInc = new Vector3f();



        debugPrint("Creating com.Basics.Window");

    }


    public void createWindow(){
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        System.out.println(windowName+" "+width+" "+height);
        window = glfwCreateWindow(width,height, windowName, NULL, NULL);
        if (window == NULL)
            throw new RuntimeException("Failed to create the GLFW window");
    }



    public void initWindow(){
        IntBuffer pWidth;
        IntBuffer pHeight;
        try (MemoryStack stack = stackPush()) {
            pWidth = stack.mallocInt(1);
            pHeight = stack.mallocInt(1);

            // Get the window size passed to glfwCreateWindow
            glfwGetWindowSize(window, pWidth, pHeight);

            // Get the resolution of the primary monitor
            GLFWVidMode vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            // Center the window
            glfwSetWindowPos(
                    window,
                    (vidmode.width() - pWidth.get(0)) / 2,
                    (vidmode.height() - pHeight.get(0)) / 2
            );
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        glfwSetFramebufferSizeCallback(window, (win, width, height) -> {
            glViewport(0, 0, width, height);
            this.width = width;
            this.height = height;
        });


        glfwMakeContextCurrent(window);
        GL.createCapabilities();
        glfwSwapInterval(1);

        glfwShowWindow(window);
    }

    public long getWindowId(){
        return this.window;
    }

    public boolean update() {
        if (glfwWindowShouldClose(window)) {
            destroy();
            return false;
        }

        debugPrint("update com.Basics.Window");

        glfwSwapBuffers(window);
        return true;
    }

    public void destroy() {

        glfwFreeCallbacks(window);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    public Vector2f getMousePosition(){
        double[] xPos = new double[1];
        double[] yPos = new double[1];
        glfwGetCursorPos(window, xPos, yPos);
        return new Vector2f((float) xPos[0], (float) yPos[0]);
    }

    public boolean isKeyPressed(int key){
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    private boolean wasPressed = false;

    public boolean isKeyPressedSingle(int key){

        boolean isPressed = isKeyPressed(key);
        boolean isActive;
        isActive = isPressed && !wasPressed;

        wasPressed = isPressed;
        return isActive;
    }

    public void clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }
}
