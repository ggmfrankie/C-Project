#include "glad/gl.h"
#include "Render.h"
#include "../GuiElement/GuiElement.h"
#include "RenderTypes.h"
#include "DataStructures/CArrayList.h"
#include "GLFW/glfw3.h"
#include "GUI/Engine.h"
#include "Makros/Makros.h"
#include "Shader/Shader.h"
//
// Created by ertls on 04.03.2026.
//

static struct {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint elementSSBO;
    GLuint meshSSBO;
} graphicsData;

#define MAX_GUI_INSTANCES 81920

static void initBuffers() {
    glGenVertexArrays(1, &graphicsData.VAO);
    glBindVertexArray(graphicsData.VAO);

    glGenBuffers(1, &graphicsData.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 MAX_GUI_VERTICES * sizeof(GuiVertex),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    glGenBuffers(1, &graphicsData.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsData.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 MAX_GUI_INDICES * sizeof(uint32_t),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    // pos (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, pos));
    // uv (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, uv));
    // bufferBinding (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT,
                           sizeof(GuiVertex), (void*)offsetof(GuiVertex, bufferBinding));
    // ID (location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_INT,
                           sizeof(GuiVertex), (void*)offsetof(GuiVertex, ID));

    glBindVertexArray(0);

    glGenBuffers(1, &graphicsData.elementSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.elementSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ElementInstanceData) * MAX_GUI_INSTANCES, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, graphicsData.elementSSBO);

    glGenBuffers(1, &graphicsData.meshSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.meshSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(MeshInstanceData) * MAX_GUI_INSTANCES, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, graphicsData.meshSSBO);
}

void Render_init(GuiState *guiState) {
    //ComputeShader_createUniform(&renderer->computeShader, ("dataSize"));
    //ComputeShader_createUniform(&renderer->computeShader, ("thickness"));
    //ComputeShader_update(&renderer->computeShader, graphingFunction);

    initBuffers();
    Shader_createUniform(&guiState->guiShader, "screenWidth");
    Shader_createUniform(&guiState->guiShader, "screenHeight");

    Shader_createUniform(&guiState->guiShader, "meshDataOffset");
    Shader_createUniform(&guiState->guiShader, "elementDataOffset");
}

static void uploadVertices(const GuiVertex *aVertices, const int *aIndices, int vertexOffset, int indexOffset) {
#if GUI_DEBUG && GUI_DEBUG_TRACK_VERTICES
    only_every_do(100,
        printf("Number of vertices: %llu\n", arrLen(aVertices))
    );
#endif

    glBindVertexArray(graphicsData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);
    glBufferSubData(GL_ARRAY_BUFFER,
        vertexOffset * sizeof(GuiVertex),
        sizeof(GuiVertex) * arrLen(aVertices),
        aVertices
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsData.EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
        indexOffset * sizeof(int),
        sizeof(int) * arrLen(aIndices),
        aIndices
    );
}

static void uploadElementData(const ElementInstanceData* aElementData, int offset) {
    if (arrIsEmpty(aElementData)) return;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.elementSSBO);

    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
        offset * sizeof(ElementInstanceData),
        arrLen(aElementData) * sizeof(ElementInstanceData),
        aElementData
    );
}

static void uploadMeshData(const MeshInstanceData* aMeshData, int offset) {
    if (arrIsEmpty(aMeshData)) return;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.meshSSBO);

    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
        offset * sizeof(MeshInstanceData),
        arrLen(aMeshData) * sizeof(MeshInstanceData),
        aMeshData
    );
}

static void beginScissor(Vec2f pos, Vec2f dims) {
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    const GLfloat glY = (viewport[3] - (pos.y + dims.y));
    glScissor((GLint)pos.x, (GLint)glY, (GLsizei)dims.x, (GLsizei)dims.y);
}

static void drawBatches(const Shader* shader, const BatchAccumulator *accumulator) {
    glEnable(GL_SCISSOR_TEST);
    for_eachRevArr(const batch, accumulator->aDone, {
        beginScissor(batch->clip.pos, batch->clip.dims);

        Shader_setUniform(shader, "elementDataOffset", batch->offsets.elementData);
        Shader_setUniform(shader, "meshDataOffset", batch->offsets.meshData);

        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            arrLen(batch->aIndices),
            GL_UNSIGNED_INT,
            (void*) (batch->offsets.index * sizeof(int)),
            batch->offsets.vertex
        );
    });
    glDisable(GL_SCISSOR_TEST);
}

static void uploadBatches(BatchAccumulator *accumulator) {
    int vertexOffset = 0;
    int indexOffset = 0;

    int elementDataOffset = 0;
    int meshDataOffset = 0;

    for_eachArr(const batch, accumulator->aDone, {
        beginScissor(batch->clip.pos, batch->clip.dims);

        uploadVertices(batch->aVertices, batch->aIndices, vertexOffset, indexOffset);
        uploadElementData(batch->aElementData, elementDataOffset);
        uploadMeshData(batch->aMeshData, meshDataOffset);

        batch->offsets.vertex = vertexOffset;
        batch->offsets.index = indexOffset;
        batch->offsets.elementData = elementDataOffset;
        batch->offsets.meshData = meshDataOffset;

        vertexOffset += arrLen(batch->aVertices);
        indexOffset += arrLen(batch->aIndices);
        elementDataOffset += arrLen(batch->aElementData);
        meshDataOffset += arrLen(batch->aMeshData);
    });
}

static ssize_t addElementData(const Element* element, Batch* batch) {
    ElementInstanceData out = {};
    const ssize_t id = arrLen(batch->aElementData);
    const float brightness = (element->state >= UI_STATE_HOVER && element->flags.canBeHovered) ? element->visuals.brightness - 0.2 : element->visuals.brightness;
    out.worldPos = element->dims.worldPos;
    out.color = (Vec4f){
        .x = element->visuals.color.x * brightness,
        .y = element->visuals.color.y * brightness,
        .z = element->visuals.color.z * brightness,
        .w = 1.0f - element->visuals.transparency
    };
    out.atlasID = 0;
    arrPush(batch->aElementData, out);
    return id;
}

static void pushBatch(BatchAccumulator* accumulator, const Element* clipElement) {
    const Batch newBatch = {
        .clip.pos = (Vec2f){
            clipElement->dims.worldPos.x + clipElement->padding.left,
            clipElement->dims.worldPos.y + clipElement->padding.up
        },
        .clip.dims = (Vec2f){
            clipElement->dims.worldWidth  - clipElement->padding.right,
            clipElement->dims.worldHeight - clipElement->padding.down
        },
    };
    arrPush(accumulator->aUnfinished, newBatch);
}

static void popBatch(BatchAccumulator* accumulator) {
    arrPush(accumulator->aDone, arrPop(accumulator->aUnfinished));
}

static void accumulateMeshes(const ElementHandle elementHandle, BatchAccumulator *accumulator) {
    Element* self = Element_get(elementHandle);
    if (self == nullptr || !self->flags.isActive) return;

    Batch* curr = arrGetLast(accumulator->aUnfinished);

    const ssize_t id = addElementData(self, curr);

    if (self->generateMesh) {
        self->generateMesh(self, &curr->aVertices, &curr->aIndices, id);
#if GUI_DEBUG && GUI_DEBUG_ACCUMULATE_MESHES
        if (self->dims.worldWidth <= 0 || self->dims.worldHeight <= 0) {
            printf("WARNING: Element '%s' has invalid dimensions: %dx%d\n",
                   self->name ? self->name : "unnamed",
                   self->dims.worldWidth, self->dims.worldHeight);
        }
#endif
    }

    Text_accumulateTextQuads(self, curr, id);

    if (self->callbacks.drawCustom) {
        self->callbacks.drawCustom(self, &curr->aVertices, &curr->aIndices, &curr->aMeshData, id);
    }

    for_eachArr(const flowElementHandle, self->aFlowElements, {
        const Element* flowElement = Element_get(*flowElementHandle);

        if (flowElement->flags.useClipping) pushBatch(accumulator, flowElement);
        accumulateMeshes(*flowElementHandle, accumulator);
        if (flowElement->flags.useClipping) popBatch(accumulator);
    });

    for_eachArr(const staticElementHandle, self->aStaticElements, {
        const Element* staticElement = Element_get(*staticElementHandle);

        if (staticElement->flags.useClipping) pushBatch(accumulator, staticElement);
        accumulateMeshes(*staticElementHandle, accumulator);
        if (staticElement->flags.useClipping) popBatch(accumulator);
    });
}

void Render_drawGui(const GuiState *guiState) {
    static BatchAccumulator accumulator = {};

    for_eachArr(const batch, accumulator.aDone, {
        arrClear(batch->aVertices);
        arrClear(batch->aIndices);
        arrClear(batch->aMeshData);
        arrClear(batch->aElementData);
    });
    arrClear(accumulator.aDone);

    for_eachArr(const batch, accumulator.aUnfinished, {
        arrClear(batch->aVertices);
        arrClear(batch->aIndices);
        arrClear(batch->aMeshData);
        arrClear(batch->aElementData);
    });
    arrClear(accumulator.aUnfinished);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if GUI_DEBUG && GUI_DEBUG_RENDER
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    Shader_bindProgram(&guiState->guiShader);
    glEnable(GL_MULTISAMPLE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, guiState->texAtlas.ID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, guiState->font.fontAtlas.ID);

    Shader_setUniform(&guiState->guiShader, "screenWidth", (float) guiState->screenWidth);
    Shader_setUniform(&guiState->guiShader, "screenHeight", (float) guiState->screenHeight);

    pushBatch(&accumulator, Element_get(guiState->guiRoot));
    accumulateMeshes(guiState->guiRoot, &accumulator);
    popBatch(&accumulator);

    uploadBatches(&accumulator);
    drawBatches(&guiState->guiShader, &accumulator);

    glBindVertexArray(0);
    glDisable(GL_MULTISAMPLE);
    Shader_unbindProgram();
}

GLFWwindow* Render_initWindow(const int width, const int height, const char* name) {
    if (!glfwInit()) ERROR_("Failed to initialize glfw");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        ERROR_("Creating the Window failed");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        ERROR_("Initializing Glad failed");
    }
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return window;
}