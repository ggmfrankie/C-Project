import com.Parser

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
fun main() {
    val parser = Parser()

    println(parser.wildcardMatch("*a*b", "adceb"))
}