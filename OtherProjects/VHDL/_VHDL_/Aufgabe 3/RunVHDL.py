from dataclasses import dataclass, field
import re
import random
import argparse
import subprocess
import logging

def main():
    parser = argparse.ArgumentParser(description="VHDL testbench generator")
    parser.add_argument("path", help="Path to VHDL file")
    parser.add_argument("-o", "--output", action="store_true", help="Enable output")
    parser.add_argument("-n", "--numTests", type=int, default=10)
    parser.add_argument("--mode", choices=["random", "all"], default="random")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable debug logging")

    args = parser.parse_args()


    logging.basicConfig(
        level=logging.DEBUG if args.verbose else logging.INFO,
        format="%(levelname)s: %(message)s"
    )

    entity = readVHDLFile(args.path)[0]
    tbFileName = f"{entity.name}_tb.vhdl"
    # Only first entity for now
    if args.output:
        testbench = createTestbench(entity, args.numTests, args.mode)
        with open(tbFileName, "w") as f:
            f.write(testbench)
        
    compileAndRun(args.path, tbFileName)
    return

def readVHDLFile(path):
    with open(path) as file:
        lines = file.readlines()
        return getEntities(lines)
    
def runCmd(exe: str, args: list[str]):
    subprocess.run([exe]+args)

def compileAndRun(mainFile: str, testFile: str):
    testName = testFile.removesuffix(".vhdl")
    try:
        logging.info("Compiling")
        runCmd("ghdl", ["-a", mainFile, testFile])
        logging.info("Running test")
        runCmd("ghdl", ["-e", testName])
        logging.info("Generating output")
        runCmd("ghdl", ["-r", testName, "--vcd=wave.vcd"])
        logging.info("Showing output")
        runCmd("gtkwave", ["wave.vcd"])

    except subprocess.CalledProcessError as e:
        print("Error occurred:", e)
        print("Error output:", e.stderr)
    
@dataclass
class Port:
    name: str
    type: str

@dataclass
class Entity:
    name: str
    inputs: list[Port] = field(default_factory=list)
    outputs: list[Port] = field(default_factory=list)
    clocks: list[Port] = field(default_factory=list)

def parseEntity(lines: list[str], start: int) -> Entity:
    name = getWord(lines[start], 2)
    out = Entity(name=name)

    i = start + 1
    inPort = False

    while i < len(lines):
        line = lines[i].strip()

        if line.startswith("end"):
            break

        if line.startswith("port"):
            inPort = True
            i += 1
            continue

        if line.startswith(")"):
            inPort = False
            i += 1
            continue

        if inPort:
            if ":" in line:
                line = line.replace(";", "")
                left, right = line.split(":", 1)

                names = [n.strip() for n in left.split(",")]

                parts = right.split()

                pKind = parts[0]
                pType = " ".join(parts[1:])

                if pKind == "in":
                    for n in names:
                        if isClock(n):
                            out.clocks.append(Port(n, pType))
                        else:
                            out.inputs.append(Port(n, pType))
                elif pKind == "out":
                    for n in names:
                        out.outputs.append(Port(n, pType))
        i += 1

    return out

def isClock(name: str):
    return re.fullmatch(r"clk(_\d+)?", name.lower()) is not None

def getWord(line: str, index=1):
    return line.strip().split()[index-1].strip()

def getEntities(lines: list[str]) -> list[Entity]:
    entities: list[Entity] = []
    for i, l in enumerate(lines):
        if l.startswith("entity "):
            entities.append(parseEntity(lines, i))
    return entities

def getLength(vType: str):
    vhdlType = vType.lower()
    if vhdlType == "std_logic":
        return 1
    
    match = re.search(r"\((\d+)\s+downto\s+(\d+)\)", vhdlType)

    if match:
        msb = int(match.group(1))
        lsb = int(match.group(2))
        return msb - lsb + 1

    return 1

def allCombinations(inputs: list[Port]):
    widths = [getLength(p.type) for p in inputs]
    totalBits = sum(widths)

    if totalBits > 10:
        raise ValueError("Too many combinations for exhaustive test")

    for i in range(2 ** totalBits):
        yield toBin(i, totalBits)

def toBin(value: int, width: int) -> str:
    return format(value, f"0{width}b")

def randBit():
    return random.choice(["0", "1"])

def randVec(width: int):
    return "\"" + "".join(randBit() for _ in range(width)) + "\""

def vhdlZeroInit(vhdl_type: str) -> str:
    return "(others => '0')" if "vector" in vhdl_type else "'0'"

def randVal(vhdl_type: str):
    return f"'{randBit()}'" if vhdl_type == "std_logic" else randVec(getLength(vhdl_type))

def generateClock(name: str, iterations: int, period: int):
    clock: list[str] = []
    clock.append(f"    {name}_proc: process")
    clock.append( "    begin")
    clock.append(f"        for i in 0 to {iterations} loop")
    clock.append(f"            {name} <= '0'; wait for {period/2} ns;")
    clock.append(f"            {name} <= '1'; wait for {period/2} ns;")
    clock.append( "        end loop;")
    clock.append( "        wait;")
    clock.append( "    end process;")

    return clock

def createTestbench(entity: Entity, numTest=10, mode="random") -> str:
    tb: list[str] = []
    time = 250
    
    tb.append("library ieee;")
    tb.append("use ieee.std_logic_1164.all;")

    tb.append(f"entity {entity.name}_tb is end;")
    tb.append(f"architecture sim of {entity.name}_tb is")

    for clock in entity.clocks:
        tb.append(f"    signal {clock.name} : {clock.type} := {vhdlZeroInit(clock.type)};")

    for input in entity.inputs:
        tb.append(f"    signal {input.name} : {input.type} := {vhdlZeroInit(input.type)};")

    for output in entity.outputs:
        tb.append(f"    signal {output.name} : {output.type} := {vhdlZeroInit(output.type)};")

    tb.append("begin")

    tb.append(f"    port_inst: entity work.{entity.name}")
    tb.append("         port map (")

    for clock in entity.clocks:
        tb.append(f"            {clock.name} => {clock.name},")

    for input in entity.inputs:
        tb.append(f"            {input.name} => {input.name},")

    for output in entity.outputs:
        tb.append(f"            {output.name} => {output.name},")

    tb[-1] = tb[-1].removesuffix(",")

    tb.append("         );")

    for clock in entity.clocks:
        tb.extend(generateClock(clock.name, numTest, time))

    tb.append("")
    tb.append("    tb: process")
    tb.append("    begin")
    if mode == "random":
        for _ in range(numTest):
            line: list[str] = []
            line.append("           ")
        
            for input in entity.inputs:
                line.append(f"{input.name} <= {randVal(input.type)};")
            line.append(f" wait for {time} ns;")
            tb.append("".join(line))

    elif mode == "all":
        for bs in allCombinations(entity.inputs):
            tb.extend(assignFromBits(entity.inputs, bs) + [f"       wait for {time} ns;"])
    tb.append("        wait;")
    tb.append("    end process;")
    tb.append("end architecture;")

    return "\n".join(tb)

def assignFromBits(inputs: list[Port], bitsString: str):
    idx = 0
    lines = []

    for i in inputs:
        w = getLength(i.type)
        value = bitsString[idx:idx+w]
        idx += w

        if w == 1:
            lines.append(f"        {i.name} <= '{value}';")
        else: 
            lines.append(f"        {i.name} <= \"{value}\";")

    return lines

if __name__ == "__main__":
    main()