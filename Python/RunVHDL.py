from dataclasses import dataclass, field
import re
import random

def main():
    entities = readVHDLFile("")

    return

def readVHDLFile(path):
    with open(path) as file:
        lines = file.readlines()
        return getEntities(lines)
    
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

        
def getLinesWith(lines: list[str], identifier: str, max=0):
    out: list[str] = []
    count = 0
    for l in lines:
        if l.startswith(identifier):
            out.append(l[len(identifier):])
            count += 1
            if max > 0 and count >= max: 
                break

    return out

def getLength(port: Port):
    vhdlType = port.type.lower()
    if vhdlType == "std_logic":
        return 1
    
    match = re.search(r"\((\d+)\s+downto\s+(\d+)\)", vhdlType)

    if match:
        msb = int(match.group(1))
        lsb = int(match.group(2))
        return msb - lsb + 1

    return 1

def toBin(value: int, width: int) -> str:
    return format(value, f"0{width}b")

def randBit():
    return random.choice(["0", "1"])

def randVec(width: int):
    return "".join(randBit() for _ in range(width))

def vhdlZeroInit(vhdl_type: str) -> str:
    return "(others => '0')" if "vector" in vhdl_type else "'0'"

def createTestbench(entity: Entity, numTest=10):
    tb = []

    tb.append(f"entity {entity.name}_tb is end;")
    tb.append(f"architecture sim of {entity.name}_tb is")

    for clock in entity.clocks:
        tb.append(f"    signal {clock.name} : {clock.type} := {vhdlZeroInit(clock.type)};")

    for input in entity.inputs:
        tb.append(f"    signal {input.name} : {input.type} := {vhdlZeroInit(input.type)};")

    tb.append("begin")

    tb.append(f"    port_inst: entity work.{entity.name}")
    tb.append("         port map (")

if __name__ == "__main__":
    main();