package com.Engine.Games;

import com.Engine.GameObjects.GameItem;
import com.Engine.MouseInput;
import com.Rendering.GUI.Elements.BaseGuiComponent;
import com.Rendering.Light.DirectionalLight;
import com.Rendering.MatrixTransformation.Camera;
import com.Window.Window;

import java.util.HashSet;

public interface IGameLogic {
    void init();
    void input(Window window, MouseInput mouseInput);

    void update(float interval, MouseInput mouseInput);
    void render();

    void setCamera(Camera camera);
    void setDirectionalLight(DirectionalLight directionalLight);
    void setGUIComponents(HashSet<BaseGuiComponent> guiComponents);
    void setGameItems(HashSet<GameItem> gameItems);
}
