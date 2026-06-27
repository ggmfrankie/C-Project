package com.Extern;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class Parser {
    LinkedList<String> substrings = new LinkedList<>();
    LinkedList<Integer> substringsIndex = new LinkedList<>();
    LinkedList<Boolean> isWildcard = new LinkedList<>();
    int remainingLength;

    public boolean isMatch(String s, String p) {

        String[] patternString;

        patternString = splitAt(p, '*');

        return false;

    }
    private String[] splitAt(String string, char key){
        List<String> splitString = new ArrayList<>();
        int numSplit = 0;
        for(int i = 0; i < string.length(); i++){

            char c = string.charAt(i);
            if(c == key) {
                numSplit++;
                continue;
            }
            splitString.set(numSplit, splitString.get(numSplit)+c);
        }
        return splitString.toArray(new String[0]);
    }
}

