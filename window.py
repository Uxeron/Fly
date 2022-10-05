import sys, os

from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QMainWindow, QApplication, QLabel

from fly import Fly

fly = Fly()

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
fly.size = pixmap_fly_1.width()

label.setPixmap(pixmap_fly_1)
label.setGeometry(0, 0, fly.size, fly.size)

window.label = label

window.resize(fly.size, fly.size)

def move_fly():
    if fly.image_index == 0:
        label.setPixmap(pixmap_fly_2)
        fly.image_index = 1
    else:
        label.setPixmap(pixmap_fly_1)
        fly.image_index = 0

    new_pos = fly.move((window.pos().x(), window.pos().y()))
    window.move(new_pos[0], new_pos[1])

timer=QTimer()
timer.timeout.connect(move_fly)
timer.start(33)

window.show()
sys.exit(app.exec_())