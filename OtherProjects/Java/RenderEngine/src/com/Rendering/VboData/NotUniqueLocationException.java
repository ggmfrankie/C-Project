package com.Rendering.VboData;

public class NotUniqueLocationException extends RuntimeException {
    public NotUniqueLocationException(String message) {
        super("Your Vbo locations are not unique. "+message);
    }
}
