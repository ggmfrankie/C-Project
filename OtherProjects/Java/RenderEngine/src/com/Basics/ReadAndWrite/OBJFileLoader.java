package com.Basics.ReadAndWrite;

import com.Basics.Parsing;
import com.Engine.GameObjects.OBJLoader;
import com.Rendering.Light.Material;
import com.Rendering.Mesh.Mesh;
import com.Rendering.Mesh.MeshData;
import com.Rendering.Textures.Texture;
import org.joml.Vector2f;
import org.joml.Vector3f;
import org.joml.Vector4f;

import java.util.*;
import java.util.stream.Collectors;

import static com.Basics.Parsing.*;
import static com.Basics.Utils.*;

public class OBJFileLoader {
    Map<String, Material> materials;
    final boolean DEBUG = true;
    Defaults defaults = new Defaults();

    List<String> vertexSubstrings;
    List<String> textureSubstrings;
    List<String> normalSubstrings;

    public List<Mesh> loadAllMeshes(String fileName){
        List<String> meshFile;

        if(fileName.isEmpty()) meshFile = readFile("\\Resources\\Objects\\grass_block.mtl");
        else meshFile = readFile("\\Resources\\Objects\\" + fileName);

        return loadMeshesFromFile(meshFile);
    }

    private List<Mesh> loadMeshesFromFile(List<String> file){
        file = preProcessFile(file);
        printFile(file);

        List<String> materialFile = readFile(
                "\\Resources\\Objects\\"
                        +getLinesWith("mtllib", file)
                        .getFirst()
                        .trim());


        vertexSubstrings = getLinesWith("v", file);
        textureSubstrings = getLinesWith("vt", file);
        normalSubstrings = getLinesWith("vn", file);

        printFile(vertexSubstrings);
        printFile(textureSubstrings);
        printFile(normalSubstrings);

        materials = loadMaterialsFromFile(materialFile);
        _PRINT_("Generated materials:");
        printFile(materials.values().stream().map(Material::getName).toList());
        _PRINT_("Material file:");
        printFile(materialFile);

        List<Mesh> meshes = new ArrayList<>();
        List<String> currentFaces = new ArrayList<>();

        boolean foundGroup = false;
        String currentMaterial = "";

        for(String line : file){
            if(line.startsWith("usemtl")){
                currentMaterial = line.replace("usemtl", "").trim();
                continue;
            }
            if(line.startsWith("g ") || line.startsWith("o ")){
                if(!currentFaces.isEmpty()){
                    meshes.add(loadMesh(currentFaces, materials.getOrDefault(currentMaterial, new Material("default"))));
                }
                currentFaces = new ArrayList<>();
                currentFaces.add(line);
                foundGroup = true;
                continue;
            }
            if(!foundGroup) continue;
            if(line.startsWith("f ")) currentFaces.add(line);
        }
        if(!currentFaces.isEmpty()){
            meshes.add(loadMesh(currentFaces, materials.getOrDefault(currentMaterial, new Material("default"))));
        }
        return meshes;
    }

    private Mesh loadMesh(List<String> file, Material material){
        String name = file.getFirst().replace("g ", "").replace("o ", "").trim();

        List<Vector3f> vertices = parseVec3f(vertexSubstrings);
        List<Vector2f> textures = parseVec2f(textureSubstrings);
        List<Vector3f> normals = parseVec3f(normalSubstrings);

        //_PRINT_("All data:");
        //printFile(vertices);
        //printFile(textures);
        //printFile(normals);

        List<Vector3f> finalPositions = new ArrayList<>();
        List<Vector2f> finalTexCoords = new ArrayList<>();
        List<Vector3f> finalNormals = new ArrayList<>();
        List<Integer> indicesList= new ArrayList<>();

        List<Face> faces = file.stream().filter(s -> s.startsWith("f ")).map(Face::new).toList();

        for(Face polygon : faces){
            List<Face> triangles = polygon.triangulatePolygons();
            for(Face triangle : triangles){
                //_PRINT_("This is a triangle:");
                //printFile(triangle.idxGroups);
                for(IdxGroup idx : triangle.idxGroups){
                    finalPositions.add(vertices.get(idx.pos - 1));
                    finalTexCoords.add(textures.get(idx.tex - 1));
                    finalNormals.add(normals.get(idx.nor - 1));
                    indicesList.add(finalPositions.size()-1);
                }
            }
        }
        MeshData meshData = generateMeshData(
                finalPositions.toArray(new Vector3f[0]),
                finalTexCoords,
                finalNormals,
                indicesList,
                material
        );

        return new Mesh(meshData, name);
    }

    private Map<String, Material> loadMaterialsFromFile(List<String> file){
        file = preProcessFile(file);
        return group(file, "newmtl").stream()
                .map(this::loadMaterial)
                .collect(Collectors.toMap(
                        Material::getName,
                        m -> m,
                        (a, b) -> a,
                        HashMap::new
                ));
    }

    private Material loadMaterial(List<String> lines){
        String header = lines.getFirst();
        String name = header.replace("newmtl", "").trim();
        if(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!false){
            _PRINT_("---------------------------");
            _PRINT_("New material: " +name);
            _PRINT_("With file:");
            _PRINT_(String.valueOf(lines));
        }

        Material material;

        Vector4f ambientColor = getVec4f("Ka", lines, defaults.AMBIENT_COLOR).getFirst();
        Vector4f diffuseColor = getVec4f("Kd", lines, defaults.DIFFUSE_COLOR).getFirst();
        Vector4f specularColor = getVec4f("Ks", lines, defaults.SPECULAR_COLOR).getFirst();

        Vector4f emissiveColor = getVec4f("Ke", lines, defaults.EMISSION_COLOR).getFirst();

        float specularExponent = getFloat("Ns", lines, defaults.SPECULAR_EXPONENT).getFirst();
        float opticalDensity = getFloat("Ni", lines, defaults.OPTICAL_DENSITY).getFirst();
        float dissolve = getFloat("d", lines, defaults.DISSOLVE).getFirst();

        int illum = getInt("illum", lines, defaults.ILLUM).getFirst();

        Texture texture = getTexture("map_Kd", lines, defaults.TEXTURE);

        material = new Material(
                name,
                ambientColor,
                diffuseColor,
                specularColor,
                emissiveColor,
                specularExponent,
                opticalDensity,
                dissolve,
                illum,
                texture
        );
        return material;
    }

    private MeshData generateMeshData(Vector3f[] verticesArray, List<Vector2f> texturesList, List<Vector3f> normalsList, List<Integer> indicesList, Material material){
        float[] vertexArray = new float[verticesArray.length * 3];
        float[] texturesArray = new float[verticesArray.length * 2];
        float[] normalsArray = new float[verticesArray.length * 3];
        int[] indexArray = indicesList.stream().mapToInt(Integer::intValue).toArray();
        int i = 0;
        for(Vector3f vec3 : verticesArray){
            vertexArray[i * 3] = vec3.x;
            vertexArray[i * 3 + 1] = vec3.y;
            vertexArray[i * 3 + 2] = vec3.z;
            i++;
        }

        i = 0;
        for(Vector2f vec2 : texturesList){
            texturesArray[i * 2] = vec2.x;
            texturesArray[i * 2 + 1] = 1 - vec2.y;
            i++;
        }

        i = 0;
        for(Vector3f vec3 : normalsList){
            normalsArray[i * 3] = vec3.x;
            normalsArray[i * 3 + 1] = vec3.y;
            normalsArray[i * 3 + 2] = vec3.z;
            i++;
        }

        return new MeshData(vertexArray, normalsArray, texturesArray, indexArray, material);
    }

    private List<List<String>> group(List<String> list, String key){
        boolean foundGroup = false;
        List<List<String>> result = new ArrayList<>();
        List<String> currentFile = new ArrayList<>();
        for(String line : list){
            if(line.startsWith(key)){
                if(!currentFile.isEmpty()){
                    result.add(currentFile);
                }
                currentFile = new ArrayList<>();
                currentFile.add(line.trim());
                foundGroup = true;
                continue;
            }
            if(foundGroup) currentFile.add(line);
        }
        if (!currentFile.isEmpty()) {
            result.add(currentFile);
        }
        return result;
    }

    private List<String> preProcessFile(List<String> file){
        return  file.stream()
                .map(s -> s.replaceAll("\\s+", " "))
                .map(s -> s.replaceAll("#.*$", ""))
                .map(String::trim)
                .filter(s -> !s.isBlank())
                .toList();
    }

    private <T> void printFile(List<T> file){
        _PRINT_();
        for(T s : file){
            _PRINT_(s);
        }
        _PRINT_();
    }

    private List<Vector4f> getVec4f(String prefix, List<String> lines, Vector4f defaultValue){
        List<String> matchedLines = getLinesWith(prefix, lines);

        if (matchedLines.isEmpty()) {
            return List.of(defaultValue);
        }

        return matchedLines.stream()
                .map(Parsing::parseVec3f)
                .map(vec3f -> new Vector4f(vec3f, 1.0f))
                .toList();
    }

    private List<Float> getFloat(String prefix, List<String> lines, float defaultValue){
        List<String> matchedLines = getLinesWith(prefix, lines);

        if (matchedLines.isEmpty()) {
            return List.of(defaultValue);
        }

        return matchedLines.stream()
                .map(Float::parseFloat)
                .toList();
    }

    private List<Integer> getInt(String prefix, List<String> lines, int defaultValue){
        List<String> matchedLines = getLinesWith(prefix, lines);

        if (matchedLines.isEmpty()) {
            return List.of(defaultValue);
        }

        return matchedLines.stream()
                .map(Integer::parseInt)
                .toList();
    }

    private Texture getTexture(String prefix, List<String> lines, Texture defaultValue){
        return getLinesWith(prefix, lines).stream()
                .findFirst()
                .map(this::loadTexture)
                .orElse(defaultValue);
    }

    private Texture loadTexture(String textureName){
        return new Texture(textureName);
    }



    static final class Defaults {
        public final Vector4f AMBIENT_COLOR = new Vector4f(0.5f);
        public final Vector4f DIFFUSE_COLOR = new Vector4f(0.5f);
        public final Vector4f SPECULAR_COLOR = new Vector4f(0.5f);
        public final Vector4f EMISSION_COLOR = new Vector4f(0.5f);

        public final float SPECULAR_EXPONENT = 2.2f;
        public final float OPTICAL_DENSITY = 1.0f;
        public final float DISSOLVE = 1.0f;

        public final int ILLUM = 2;
        public final Texture TEXTURE = new Texture();
    }

    private void _PRINT_(String s){
        if(!DEBUG) return;
        System.out.println(s);
    }
    private void _PRINT_(){
        if(!DEBUG) return;
        System.out.println();
    }
    private <T> void _PRINT_(T input){
        if(!DEBUG) return;
        System.out.println(input.toString());
    }

    protected static class Face{
        private final List<IdxGroup> idxGroups;

        public Face(String substring){
            String[] vertexTokens = substring
                    .replaceFirst("^f\\s+", "")
                    .split(" ");

            idxGroups = new ArrayList<>();
            for (String token : vertexTokens) {
                idxGroups.add(parseVertexData(token));
            }
        }

        private Face(IdxGroup idxGroup0, IdxGroup idxGroup1, IdxGroup idxGroup2){
            idxGroups = List.of(idxGroup0, idxGroup1, idxGroup2);
        }

        public List<Face> triangulatePolygons(){
            if(idxGroups.size() <= 3) return List.of(this);
            List<Face> result = new ArrayList<>();
            for(int i = 1 ; i < idxGroups.size()-1 ; i++){
                Face face = new Face(
                        idxGroups.getFirst(),
                        idxGroups.get(i),
                        idxGroups.get(i+1)
                );
                result.add(face);
            }
            return result;
        }

        private IdxGroup parseVertexData(String token){
            String[] parts = token.split("/");
            int pos = Integer.parseInt(parts[0]);
            int tex = parts.length > 1 && !parts[1].isEmpty() ? Integer.parseInt(parts[1]) : 0;
            int nor = parts.length > 2 && !parts[2].isEmpty() ? Integer.parseInt(parts[2]) : 0;
            return new IdxGroup(pos, tex, nor);
        }
    }

    protected record IdxGroup(int pos, int tex, int nor){}
}
