package com.Basics.Interface;

import com.Rendering.GUI.Elements.BaseGuiComponent;

@FunctionalInterface
public interface ClickAction {
    void execute(BaseGuiComponent component);
}
