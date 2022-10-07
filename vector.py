from __future__ import annotations
from math import sqrt
from typing import Tuple

class Vector:
    x1: float = 0.0
    y1: float = 0.0
    x2: float = 0.0
    y2: float = 0.0

    def __init__(self, x1: float, y1: float, x2: float = None, y2: float = None) -> None:
        if x2 is None or y2 is None:
            self.x2 = x1
            self.y2 = y1
        else:
            self.x1 = x1
            self.y1 = y1
            self.x2 = x2
            self.y2 = y2
    
    def abs(self) -> Tuple[float, float]:
        return ((self.x2 - self.x1), (self.y2 - self.y1))
    
    def abs_int(self) -> Tuple[int, int]:
        return (int(round(self.abs()[0])), int(round(self.abs()[1])))

    def length(self) -> float:
        return sqrt(self.abs()[0]**2.0 + self.abs()[1]**2.0)
    
    def normalised(self) -> Vector:
        if self.length() == 0:
            return Vector(0.0, 0.0)
        return Vector(self.abs()[0] / self.length(), self.abs()[1] / self.length())

    def __add__(self, other: Vector):
        other_abs_x = other.x2 - other.x1
        other_abs_y = other.y2 - other.y1

        return Vector(self.x1, self.y1, self.x2 + other_abs_x, self.y2 + other_abs_y)
    
    def __sub__(self, other: Vector):
        other_abs_x = other.x2 - other.x1
        other_abs_y = other.y2 - other.y1

        return Vector(self.x1, self.y1, self.x2 - other_abs_x, self.y2 - other_abs_y)
    
    def __mul__(self, mul: float):
        abs_multiplied = (self.abs()[0] * mul, self.abs()[1] * mul)
        return Vector(self.x1, self.y1, self.x1 + abs_multiplied[0], self.y1 + abs_multiplied[1])