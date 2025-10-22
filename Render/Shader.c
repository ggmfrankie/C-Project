//
// Created by Stefan on 10.10.2025.
//


#include "Shader.h"

#include <string.h>


#include "glad/gl.h"



String readFile(const String *fileName);
int createShader(const GLchar** shaderSource, int shaderType, int programId);
int createVertexShader(const String *fileName, int programId);
int createFragmentShader(const String *fileName, int programId);

Shader newShader() {

    const int programId = glCreateProgram();
    int success;
    char infoLog[512];
    const String vertexShader = newString("GuiVertexShader.vert");
    const String fragmentShader = newString("GuiFragmentShader.frag");

    const int vertexId = createVertexShader(&vertexShader, programId);
    const int fragmentId = createFragmentShader(&fragmentShader, programId);


    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
        printf("Vertex Shader Compile Error:\n%s\n", infoLog);
    }

    // --- DEBUG: Check fragment shader compile status ---
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
        printf("Fragment Shader Compile Error:\n%s\n", infoLog);
    }

    glLinkProgram(programId);

    // --- DEBUG: Check program link status ---
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        printf("Shader Program Link Error:\n%s\n", infoLog);
    } else {
        printf("Shader Program linked successfully! ID: %d\n", programId);
    }

    return (Shader){
        .programId = programId,
        .vertexId = vertexId,
        .fragmentId = fragmentId,
        .bind = Shader_bindProgram,
        .unbind = Shader_unbindProgram,
        .createUniform = Shader_createUniform,
        .uniforms = newMap_Uniforms(16, str_equals)
    };
}

void Shader_createUniform(Shader *shader, const String name) {
    const int uniformLocation = glGetUniformLocation(shader->programId, name.content);
    if(uniformLocation < 0){
        printf("Error creating Uniform");
    }
    shader->uniforms.put(&shader->uniforms, name, uniformLocation);
}

int createVertexShader(const String *fileName, const int programId) {
    //const String shaderCode = readFile(fileName);
    const GLchar* shaderSrc[] ={
        "#version 460\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec3 vertexNormal;\n"
        "layout(location = 2) in vec2 texCoord;\n"
        "out vec3 outPosition;\n"
        "void main() {\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "    outPosition = position;\n"
        "}\n"};
    return createShader(shaderSrc, GL_VERTEX_SHADER, programId);
}

int createFragmentShader(const String *fileName, const int programId) {

    const GLchar* shaderSrc[] ={
        "#version 460\n"
        "in vec3 outPosition;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    fragColor = vec4(1.0, 0.5, 1.0, 1.0);\n"
        "}\n"};
    String shaderCode = readFile(fileName);
    shaderCode.println(&shaderCode);
    return createShader(shaderSrc, GL_FRAGMENT_SHADER, programId);
}

int createShader(const GLchar** shaderSource, const int shaderType, const int programId) {
    const int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, shaderSource, NULL);
    glCompileShader(shaderId);
    glAttachShader(programId, shaderId);
    return shaderId;
}

void Shader_bindProgram(const Shader *shader) {
    glUseProgram(shader->programId);
}

void Shader_unbindProgram() {
    glUseProgram(0);
}

String readFile(const String *fileName) {
    const String defaultShaderPath = newString("../Shader/");
    const String completePath = str_combine(&defaultShaderPath, fileName);

    FILE *file = fopen(completePath.content, "rb"); // binary mode to detect BOM
    if (!file) {
        printf("Failed to open file: %s\n", completePath.content);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        exit(1);
    }

    fread(buffer, 1, size, file);
    fclose(file);

    buffer[size] = '\0'; // null-terminate

    // Strip UTF-8 BOM if present
    if ((unsigned char)buffer[0] == 0xEF &&
        (unsigned char)buffer[1] == 0xBB &&
        (unsigned char)buffer[2] == 0xBF) {
        memmove(buffer, buffer + 3, size - 2);
        }

    return newString(buffer);
}


