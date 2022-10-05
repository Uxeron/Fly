import sys, os

from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QMainWindow, QApplication, QLabel
from vector import Vector

from ctypes import windll, Structure, c_long, byref

import random

class POINT(Structure):
    _fields_ = [("x", c_long), ("y", c_long)]

def get_mouse_pos():
    pt = POINT()
    windll.user32.GetCursorPos(byref(pt))
    windll.user32.SetCursor
    return (pt.x, pt.y)

fly_image_index = 0
fly_speed_chase = 5
fly_speed_idle = 1
target_point = None

app = QApplication(sys.argv)

window = QMainWindow()

window.setAttribute(Qt.WA_TranslucentBackground, True)
window.setAttribute(Qt.WA_TransparentForMouseEvents, True)
window.setAttribute(Qt.WA_NoSystemBackground, True)
window.setWindowFlags(Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint)

label = QLabel(window)

path_prefix = ""
if getattr(sys, 'frozen', False):
    path_prefix = sys._MEIPASS

pixmap_fly_1 = QPixmap(os.path.join(path_prefix, "fly_1.png"))
pixmap_fly_2 = QPixmap(os.path.join(path_prefix, "fly_2.png"))
fly_size = pixmap_fly_1.width()

label.setPixmap(pixmap_fly_1)
label.setGeometry(0, 0, fly_size, fly_size)

window.label = label

window.resize(fly_size, fly_size)

def move_fly():
    global fly_image_index, target_point
    if fly_image_index == 0:
        label.setPixmap(pixmap_fly_2)
        fly_image_index = 1
    else:
        label.setPixmap(pixmap_fly_1)
        fly_image_index = 0

    mouse_pos = get_mouse_pos()
    fly_pos = (window.pos().x() + fly_size//2, window.pos().y() + fly_size//2)

    dist_vec = Vector(fly_pos[0], fly_pos[1], mouse_pos[0], mouse_pos[1])

    if dist_vec.length() > 100:
        target_point = None
        travel = (dist_vec.normalised() * fly_speed_chase).abs_int()
        window.move(window.pos().x() + travel[0], window.pos().y() + travel[1])
    else:
        if target_point == None:
            target_point = (mouse_pos[0] + random.randint(-60, 60), mouse_pos[1] + random.randint(-90, 90))
        target_vec = Vector(fly_pos[0], fly_pos[1], target_point[0], target_point[1])

        if target_vec.length() < 5:
            target_point = (mouse_pos[0] + random.randint(-60, 60), mouse_pos[1] + random.randint(-90, 90))
            target_vec = Vector(fly_pos[0], fly_pos[1], target_point[0], target_point[1])
        
        travel = (target_vec.normalised() * fly_speed_idle).abs_int()
        window.move(window.pos().x() + travel[0], window.pos().y() + travel[1])

timer=QTimer()
timer.timeout.connect(move_fly)
timer.start(33)

window.show()
sys.exit(app.exec_())