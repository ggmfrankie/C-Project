package com.Rendering.GUI.Elements;

import com.Basics.Interface.ClickAction;

public interface IClickable {
    void click();
    void setClickAction(ClickAction cLickAction);
    ClickAction getClickAction();
}