import tkinter as tk
import win32con, win32api, pywintypes
import sys, os

from infi.systray import SysTrayIcon
# NOTE on systray:
# It attempts to import pkg_resources in its __init__.py file.
# That call must be removed for packaging into .exe to work.

root = tk.Tk()

from fly import Fly

fly = Fly()

path_prefix = ""
if getattr(sys, 'frozen', False):
    path_prefix = sys._MEIPASS

# The image must be stored to Tk or it will be garbage collected.
root.image_fly_1 = tk.PhotoImage(file=os.path.join(path_prefix, "fly_1.png"))
root.image_fly_2 = tk.PhotoImage(file=os.path.join(path_prefix, "fly_2.png"))

label = tk.Label(root, image=root.image_fly_1, bg='white')

fly.size = root.image_fly_1.width()

root.overrideredirect(True)
root.geometry(f"{fly.size}x{fly.size}+255+255")
root.lift()
root.wm_attributes("-topmost", True)
root.wm_attributes("-disabled", True)
root.wm_attributes("-transparentcolor", "white")

hWindow = pywintypes.HANDLE(int(root.frame(), 16))
exStyle = win32con.WS_EX_COMPOSITED | win32con.WS_EX_LAYERED | win32con.WS_EX_NOACTIVATE | win32con.WS_EX_TOPMOST | win32con.WS_EX_TRANSPARENT
win32api.SetWindowLong(hWindow, win32con.GWL_EXSTYLE, exStyle)

systray = SysTrayIcon(os.path.join(path_prefix, "fly.ico"), "Fly", on_quit=(lambda *_: root.quit()))
systray.start()

def move_fly():
    if fly.image_index == 0:
        label.configure(image=root.image_fly_2)
        fly.image_index = 1
    else:
        label.configure(image=root.image_fly_1)
        fly.image_index = 0

    new_pos = fly.move((root.winfo_x(), root.winfo_y()))
    root.geometry(f"{fly.size}x{fly.size}+{new_pos[0]}+{new_pos[1]}")
    root.after(33, move_fly)

root.after(33, move_fly)

label.pack()
label.mainloop()

systray.shutdown()