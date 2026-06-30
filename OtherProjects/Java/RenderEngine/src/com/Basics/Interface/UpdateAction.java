package com.Basics.Interface;

import com.Engine.GameObjects.GameItem;

@FunctionalInterface
public interface UpdateAction {
    void update(GameItem gameItem);
}
