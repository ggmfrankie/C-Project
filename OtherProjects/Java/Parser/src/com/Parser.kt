package com

class Parser {
    fun isMatch(s: String, p: String): Boolean {
        return wildcardMatch(p, s)
    }
    // definiert den größtmöglichen Index, wo der Substring gefunden werden darf
    var maxDepth = 0

    // zeichnet die Anzahl der Variablen in pattern auf, wenn = 2, dann wurde ein Substring gefunden
    // eg "*(variable Count = 1)Hallo*(variable Count = 2)*"
    // Vergleichssubstring dann "Hallo" mit maxDepth -1 (kein Limit)

    var variableCount = 0

    fun wildcardMatch(pattern: String , value: String): Boolean{
        var compString = value
        var subString = ""
        var lastChar = 'x'
        var indexOfSubstring = -1

        for(singleChar in pattern){

            when (singleChar) {
                '*' ->{
                    println("* detected")
                    maxDepth = Int.MAX_VALUE
                    variableCount++
                }
                '?' ->{
                    println("? detected")
                    maxDepth = 1
                    variableCount++
                }
                else -> {
                    println("character detected")
                    subString += singleChar
                    println(subString)
                }

            }

            if(variableCount > 1){
                indexOfSubstring = findSubstring(subString, compString, maxDepth)
                if(indexOfSubstring == -1) return false
                compString = compString.substring(indexOfSubstring + compString.length)

                subString = ""
                maxDepth = 0
                variableCount--
            }
            lastChar = singleChar
            println(lastChar)
        }

        println("before: "+compString)

        if(lastChar == '*') return true

        indexOfSubstring = findSubstring(subString, compString, maxDepth)

        if(indexOfSubstring == -1) return false

        compString = compString.substring(indexOfSubstring + compString.length)

        println("after: "+compString)
        return compString.isEmpty()
    }

    fun findSubstring(subString: String, compString: String, maxDepth: Int): Int {
        val index = compString.indexOf(subString)
        if(index <= maxDepth) return index
        return -1
    }
}