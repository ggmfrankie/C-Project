from dataclasses import dataclass


@dataclass
class Move:
    row: int
    col: int

    capture: bool
    capture_value: int

    check: bool
    checkmate: bool