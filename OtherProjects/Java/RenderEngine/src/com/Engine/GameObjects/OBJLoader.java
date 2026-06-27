package com.Engine.GameObjects;

import com.Rendering.Light.Material;
import com.Rendering.Mesh.Mesh;
import com.Rendering.Mesh.MeshData;
import com.Rendering.Textures.Texture;
import org.joml.*;

import java.lang.Math;
import java.util.*;

import static com.Basics.Utils.*;

public class OBJLoader {
    Map<String, Material> materials;
    Defaults defaults = new Defaults();

    List<String> vertexSubstrings;
    List<String> textureSubstrings;
    List<String> normalSubstrings;

    public OBJLoader() {

    }

    public List<Mesh> loadAllMeshes(String fileName){
        List<String> meshFile;

        if(fileName.isEmpty()) meshFile = readFile("\\Resources\\Objects\\grass_block.mtl");
        else meshFile = readFile("\\Resources\\Objects\\" + fileName);
        //dunno if i need name

        return loadMeshesFromFile(meshFile);
    }

    private List<Mesh> loadMeshesFromFile(List<String> file){
        file = preProcessFile(file);

        List<String> materialFile = readFile(
                "\\Resources\\Objects\\"
                        +getLinesWith("mtllib", file)
                        .getFirst()
                        .replace("mtllib", "")
                        .trim());

        vertexSubstrings = getLinesWith("v", file);
        textureSubstrings = getLinesWith("vt", file);
        normalSubstrings = getLinesWith("vn", file);

//        System.out.println("VertexSubstrings:");
//        System.out.println();
//        for (String ver : vertexSubstrings) {
//            System.out.println(ver);
//        }
//        System.out.println();
//
//        System.out.println("TextureSubstrings:");
//        System.out.println();
//        for (String tex : textureSubstrings) {
//            System.out.println(tex);
//        }
//        System.out.println();
//
//        System.out.println("NormalSubstrings:");
//        System.out.println();
//        for (String norm : normalSubstrings) {
//            System.out.println(norm);
//        }
//        System.out.println();
        materials = loadMaterialsFromFile(materialFile);
        List<Mesh> meshes = new ArrayList<>();
        List<String> currentFaces = new ArrayList<>();

        boolean foundGroup = false;
        String currentMaterial = "";

        for(String line : file){
            if(line.startsWith("usemtl")) currentMaterial = line.replace("usemtl", "").trim();
            if(line.startsWith("g ") || line.startsWith("o ")){
                if(!currentFaces.isEmpty()){

//                    System.out.println();
//                    System.out.println();
//                    System.out.println("The list the mesh sees is: " +currentFaces);
//                    System.out.println();
//                    System.out.println();
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

//            System.out.println("final mesh:");
//            System.out.println();
//            System.out.println("The list the mesh sees is: " +currentFaces);
//            System.out.println();
//            System.out.println();
//            meshes.add(loadMesh(currentFaces, materials.getOrDefault(currentMaterial, new Material("default"))));
        }
        return meshes;
    }

    private Mesh loadMesh(List<String> file, Material material){
        String name = file.getFirst().replace("g ", "").replace("o ", "").trim();
        List<Vector3f> vertices;
        List<Vector2f> textures;
        List<Vector3f> normals;
        List<Face> faces = new ArrayList<>();

        List<Vector3f> finalPositions = new ArrayList<>();
        List<Vector2f> finalTexCoords = new ArrayList<>();
        List<Vector3f> finalNormals = new ArrayList<>();
        List<Integer> indicesList= new ArrayList<>();

        Map<IdxGroup, Integer> uniqueVertexMap = new HashMap<>();

        vertices = parseV3(vertexSubstrings);
        textures = parseV2(textureSubstrings);
        normals = parseV3(normalSubstrings);

        for(String faceString : file){
            if(!faceString.startsWith("f ")) continue;

            faces.add(new Face(faceString.replace("f ", "")));
        }

        int i = 0;
        for (Face face : faces) {
            List<Face> triangles = face.triangulate();
            for (Face triangle : triangles) {

                for (IdxGroup idx : triangle.getFaceVertexIndices()) {
                    if(uniqueVertexMap.containsKey(idx)){
                        int index = uniqueVertexMap.get(idx);
                        indicesList.add(index);

                    } else {
                        uniqueVertexMap.put(idx, finalPositions.size());

                        Vector3f pos = vertices.get(idx.idxPos - 1);
                        Vector2f tex = textures.get(idx.idxTextCoord - 1);
                        Vector3f norm = normals.get(idx.idxVecNormal - 1);

                        finalPositions.add(pos);
                        finalTexCoords.add(tex);
                        finalNormals.add(norm);

                        indicesList.add(i);
                        i++;
                    }
                }
            }
        }

        float[] vertexPositions = flattenListVec3(finalPositions);
        float[] textureCoordinates = flattenListVec2(finalTexCoords);
        float[] normalValues = flattenListVec3(finalNormals);
        int[] indicesArray = indicesList.stream().mapToInt(Integer::intValue).toArray();

//        float[] pos = vertexPositions; // flattenListVec3(...)
//        System.out.println("VertexPositions:");
//        System.out.println(Arrays.toString(vertexPositions));
//        float[] uv = textureCoordinates; // flattenListVec2(...)
//        System.out.println("TextureCoordinates");
//        System.out.println(Arrays.toString(textureCoordinates));
//        float[] n = normalValues; // flattenListVec3(...)
//        System.out.println("NormalValues");
//        System.out.println(Arrays.toString(normalValues));
//        for (int e = 0; e < Math.min(10, pos.length/3); e++) {
//            System.out.printf("v[%d]=(%f,%f,%f) uv=(%f,%f) n=(%f,%f,%f)%n",
//                    e, pos[e*3], pos[e*3+1], pos[e*3+2],
//                    uv[e*2], uv[e*2+1],
//                    n[e*3], n[e*3+1], n[e*3+2]);
//        }

//        System.out.println("Verts: " + (vertexPositions.length / 3));
//        System.out.println("UVs:   " + (textureCoordinates.length / 2));
//        System.out.println("Norms: " + (normalValues.length / 3));
//        System.out.println("Indices: " + indicesArray.length);


        MeshData meshData = new MeshData(
                vertexPositions,
                textureCoordinates,
                normalValues,
                indicesArray,
                material
        );

        return new Mesh(meshData, name);
    }

    private Map<String, Material> loadMaterialsFromFile(List<String> file){
        file = preProcessFile(file);
        Map<String, Material> materials = new HashMap<>();
        List<String> currentMaterialFile = new ArrayList<>();
//        System.out.println("---------------------");
//        System.out.println();
//        System.out.println(getLinesWith("newmtl", file));
//        System.out.println();
//        System.out.println("---------------------");

        for(String line : file){
            if(line.startsWith("newmtl")){
                if(!currentMaterialFile.isEmpty()){
                    Material mat = loadMaterial(currentMaterialFile);
                    materials.put(mat.getName(), mat);
                }
                currentMaterialFile.clear();
                currentMaterialFile.add(line.replace("newmtl", "").trim());
            } else {
                currentMaterialFile.add(line);
            }
        }
        if (!currentMaterialFile.isEmpty()) {
            Material mat = loadMaterial(currentMaterialFile);
            materials.put(mat.getName(), mat);
        }
        return materials;
    }

    private List<Vector3f> parseV3(List<String> lines){
        List<Vector3f> vec3s = new ArrayList<>();
        for(String line : lines){
            vec3s.add(convertToVec3f(line));
        }
        return vec3s;
    }

    private List<Vector2f> parseV2(List<String> lines){
        List<Vector2f> vec2s = new ArrayList<>();
        for(String line : lines){
            vec2s.add(convertToVec2f(line));
        }
        return vec2s;
    }

    private Material loadMaterial(List<String> lines){
        String name = lines.getFirst().trim();
        System.out.println("---------------------------");
        System.out.println("New material: " +name);
        System.out.println("With file:");
        System.out.println(lines);
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

    private List<Vector4f> getVec4f(String prefix, List<String> lines, Vector4f defaultValue){
        List<String> matchedLines = getLinesWith(prefix, lines);

        if (matchedLines.isEmpty()) {
            return List.of(defaultValue);
        }

        return matchedLines.stream()
                .map(this::convertToVec4f)
                .filter(Objects::nonNull)
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

    private List<String> preProcessFile(List<String> file){
        List<String> processedFile = new ArrayList<>();
        for (String s : file) {
            String line = s
                    .replaceAll("\\s+", " ")
                    .replaceAll("#.*$", "")
                    .trim();

            processedFile.add(line);
        }
        return  processedFile;
    }

    private List<String> getLinesWith(String key, List<String> file){
        List<String> Lines = new ArrayList<>();

        for(String lineString : file){
            if(lineString.startsWith(key+ " ")){
                if(Objects.equals(key, "newmtl")){
                    System.out.println("Matching: " +lineString+ " with: " +key);
                }

                Lines.add(lineString.replace(key+ " ", ""));
            }
        }
        return Lines;
    }

    private Vector4f convertToVec4f(String line){
        String[] values = line.split(" ");
        Vector4f vec4 = new Vector4f();
        try{
            vec4.x = Float.parseFloat(values[0]);
            vec4.y = Float.parseFloat(values[1]);
            vec4.z = Float.parseFloat(values[2]);
            if(values.length == 4) vec4.w = Float.parseFloat(values[3]);
            else vec4.w = 1.0f;

        } catch (Exception e){
            System.out.println("Error converting Vec4 to float: " +line);
            return null;
        }
        return vec4;
    }

    private Vector3f convertToVec3f(String line){
        String[] values = line.split(" ");
        Vector3f vec3 = new Vector3f();
        try{
            vec3.x = Float.parseFloat(values[0]);
            vec3.y = Float.parseFloat(values[1]);
            vec3.z = Float.parseFloat(values[2]);

        } catch (Exception e){
            System.out.println("Error converting Vec3 to float: " +line);
            throw new RuntimeException();
        }
        return vec3;
    }

    private Vector3i convertToVec3i(String line){
        String[] values = line.split(" ");
        Vector3i vec3 = new Vector3i();
        try {
            vec3.x = Integer.parseInt(values[0]);
            vec3.y = Integer.parseInt(values[1]);
            vec3.z = Integer.parseInt(values[2]);
        } catch (Exception e){
            throw new RuntimeException();
        }
        return vec3;
    }

    private Vector2f convertToVec2f(String line){
        String[] values = line.split(" ");
        Vector2f vec2 = new Vector2f();
        try {
            vec2.x = Float.parseFloat(values[0]);
            vec2.y = Float.parseFloat(values[1]);
        } catch (Exception e){
            throw new RuntimeException();
        }
        return vec2;
    }

    private Vector2i convertToVec2i(String line){
        String[] values = line.split(" ");
        Vector2i vec2 = new Vector2i();
        try {
            vec2.x = Integer.parseInt(values[0]);
            vec2.y = Integer.parseInt(values[1]);

        } catch (Exception e){
            return null;
        }
        return vec2;
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

    protected static class Face {
        private final List<OBJLoader.IdxGroup> idxGroups;

        public Face(String substring) {
            substring = substring.replaceFirst("^f\\s+", "");
            String[] vertexTokens = substring.split(" ");

            idxGroups = new ArrayList<>();
            for (String token : vertexTokens) {
                idxGroups.add(parseVertexData(token));
            }
        }

        public List<OBJLoader.Face> triangulate() {
            List<OBJLoader.Face> result = new ArrayList<>();

            if (idxGroups.size() < 3) return result; // invalid face

            for (int i = 1; i < idxGroups.size() - 1; i++) {
                OBJLoader.Face triangle = new OBJLoader.Face(idxGroups.getFirst(), idxGroups.get(i), idxGroups.get(i + 1));
                result.add(triangle);
            }

            return result;
        }

        private Face(OBJLoader.IdxGroup v0, OBJLoader.IdxGroup v1, OBJLoader.IdxGroup v2) {
            this.idxGroups = List.of(v0, v1, v2);
        }

        private OBJLoader.IdxGroup parseVertexData(String token) {
            String[] parts = token.split("/");

            OBJLoader.IdxGroup group = new OBJLoader.IdxGroup();
            group.idxPos = parts.length > 0 && !parts[0].isEmpty() ? Integer.parseInt(parts[0]) : -1;
            group.idxTextCoord = parts.length > 1 && !parts[1].isEmpty() ? Integer.parseInt(parts[1]) : -1;
            group.idxVecNormal = parts.length > 2 && !parts[2].isEmpty() ? Integer.parseInt(parts[2]) : -1;

            return group;
        }

        public List<OBJLoader.IdxGroup> getFaceVertexIndices() {
            return idxGroups;
        }
    }

    protected static class IdxGroup{
        public static final int NO_VALUE = -1;
        public int idxPos;
        public int idxTextCoord;
        public int idxVecNormal;

        public IdxGroup(){
            idxPos = NO_VALUE;
            idxTextCoord = NO_VALUE;
            idxVecNormal = NO_VALUE;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof IdxGroup other)) return false;
            return idxPos == other.idxPos &&
                    idxTextCoord == other.idxTextCoord &&
                    idxVecNormal == other.idxVecNormal;
        }

        @Override
        public int hashCode() {
            return Objects.hash(idxPos, idxTextCoord, idxVecNormal);
        }

        @Override
        public String toString(){
            return "idxPos: " +idxPos+ " idxTextcoord: " +idxTextCoord+ " idxVecNormal: " +idxVecNormal;
        }


    }
}
