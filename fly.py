from typing import Tuple
import mouse
from vector import Vector
import random

class Fly:
    size: int = 32
    image_index: int = 0
    speed_chase: int = 5
    speed_idle: int = 1
    target_point: Tuple[int, int] = None

    def move(self, window_pos: Tuple[int, int]) -> Tuple[int, int]:
        mouse_pos = mouse.get_pos()
        fly_pos = (window_pos[0] + self.size//2, window_pos[1] + self.size//2)

        dist_vec = Vector(fly_pos[0], fly_pos[1], mouse_pos[0], mouse_pos[1])

        if dist_vec.length() > 100:
            self.target_point = None
            travel = (dist_vec.normalised() * self.speed_chase).abs_int()
            return (window_pos[0] + travel[0], window_pos[1] + travel[1])
        else:
            if self.target_point == None:
                self.target_point = (mouse_pos[0] + random.randint(-60, 60), mouse_pos[1] + random.randint(-90, 90))
            target_vec = Vector(fly_pos[0], fly_pos[1], self.target_point[0], self.target_point[1])

            if target_vec.length() < 5:
                self.target_point = (mouse_pos[0] + random.randint(-60, 60), mouse_pos[1] + random.randint(-90, 90))
                target_vec = Vector(fly_pos[0], fly_pos[1], self.target_point[0], self.target_point[1])
            
            travel = (target_vec.normalised() * self.speed_idle).abs_int()
            return (window_pos[0] + travel[0], window_pos[1] + travel[1])