package com.Basics.ReadAndWrite;

import com.Rendering.Textures.Texture;
import org.jetbrains.annotations.NotNull;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.function.Consumer;

import static com.Basics.Parsing.getLinesWith;
import static com.Basics.Utils.readFile;

public class FontLoader {
    List<String> file;
    CommonData commonData;
    HashMap<Integer, Texture> fontPages;
    HashMap<Integer, CharData> charData;

    public FontLoader(String name) {
        file = readFile("\\Resources\\Font\\" +name);
        System.out.println(System.getProperty("user.dir") + "\\Resources\\Font\\" +name);
        charData = new HashMap<>();
        fontPages = new HashMap<>();
        System.out.println(file);
        loadFontData(file);
    }

    private void loadFontData(List<String> file){
        assert file != null;
        loadCommonData(List.of(getLinesWith("common", file).getFirst().trim().split("\\s+")));

        List<String> charLines = getLinesWith("char", file);
        doForEachLine(charLines, this::loadCharData);

        List<String> pageLines = getLinesWith("page", file);
        doForEachLine(pageLines, this::loadPages);
    }

    private void doForEachLine(@NotNull List<String> list,@NotNull Consumer<List<String>> consumer){
        //Splits each line at " " and consumes it with the consumer
        list.stream()
                .map(s -> s.split(" "))
                .map(Arrays::stream)
                .map(s -> s.map(String::trim)
                        .toList())
                .forEach(consumer);
    }

    private void loadCharData(List<String> line){
        int charId = getIntForKey("id", line);
        charData.put(charId, new CharData(
                getIntForKey("x", line),
                getIntForKey("y", line),
                getIntForKey("width", line),
                getIntForKey("height", line),
                getIntForKey("xOffset", line),
                getIntForKey("yOffset", line),
                getIntForKey("xAdvance", line),
                getIntForKey("page", line),
                getIntForKey("chnl", line)
        ));
    }

    private void loadCommonData(List<String> line){
        commonData = new CommonData(
                getIntForKey("lineHeight", line),
                getIntForKey("base", line),
                getIntForKey("scaleW", line),
                getIntForKey("scaleH", line),
                getIntForKey("pages", line),
                getIntForKey("packed", line),
                getIntForKey("alphaChnl", line),
                getIntForKey("redChnl", line),
                getIntForKey("greenChnl", line),
                getIntForKey("blueChnl", line)
        );
    }

    private void loadPages(List<String> file){
        fontPages.put(getIntForKey("id", file), new Texture(
                getStringForKey("file", file)
        ));
    }

    private int getIntForKey(String key, List<String> line){
        return line.stream().filter(s -> s.startsWith(key))
                .findFirst()
                .map(s -> s.replace(key+ "=", ""))
                .map(Integer::parseInt)
                .orElse(0);
    }

    private String getStringForKey(String key, List<String> line){
        return line.stream().filter(s -> s.startsWith(key))
                .findFirst()
                .map(s -> s.replace(key+ "=", ""))
                .orElse("");
    }

    public record CommonData(int lineHeight,
                      int base,
                      int scaleW,
                      int scaleH,
                      int Pages,
                      int packed,
                      int alphaChnl,
                      int redChnl,
                      int greenChnl,
                      int blueChnl
    ){}

    public record CharData(int x,
                    int y,
                    int width,
                    int height,
                    int xOffset,
                    int yOffset,
                    int xAdvance,
                    int page,
                    int chnl
    ){}
}
