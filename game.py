import tkinter
import os
import time


def board_to_string(b):
    s = ""
    for i in range(8):
        for j in range(8):
            if b[i][j] == 0:
                s += "."
            elif b[i][j] == 1:
                s += "O"
            elif b[i][j] == -1:
                s += "X"
    return s


def string_to_board(s):
    b = start_board()
    for i in range(8):
        for j in range(8):
            if s[8*i+j] == ".":
                b[i][j] = 0
            elif s[8*i+j] == "O":
                b[i][j] = 1
            elif s[8*i+j] == "X":
                b[i][j] = -1
    return b


def start_board():
    return [[0,0,0,0,0,0,0,0],
            [0,0,0,0,0,0,0,0],
            [0,0,0,0,0,0,0,0],
            [0,0,0,-1,1,0,0,0],
            [0,0,0,1,-1,0,0,0],
            [0,0,0,0,0,0,0,0],
            [0,0,0,0,0,0,0,0],
            [0,0,0,0,0,0,0,0]]


def draw_board(canvas, board, tk):
    canvas.delete("all")
    # canvas.fill("green")
    for i in range(8):
        for j in range(8):
            x1 = 80*j + 5
            y1 = 80*i + 5
            x2 = 80*j + 75
            y2 = 80*i + 75
            if board[i][j] == 1:
                canvas.create_oval(x1, y1, x2, y2, outline = "white", fill = "white")
            elif board[i][j] == -1:
                canvas.create_oval(x1, y1, x2, y2, outline = "black", fill = "black")
    tk.update()


class GUI:
    def __init__(self):
        self.board = start_board()
        self.player = "X"
        self.enemy = "O"

        self.win = tkinter.Tk()
        self.win.resizable(0,0)
        self.win.title("Othello")

        self.canvas = tkinter.Canvas(self.win, width = 640, height = 640, bg = "green", bd = 0, highlightthickness = 0)
        self.canvas.pack()
        draw_board(self.canvas, self.board, self.win)

        self.canvas.bind_all("<Button-1>", self.click_callback)

        self.win.update()

        self.win.mainloop()


    def click_callback(self, evt):
        i = evt.y // 80
        j = evt.x // 80

        pos = 8*i+j

        res = os.popen(f"./bot -check {board_to_string(self.board)} {self.player} {pos}").read()

        if res != "invalid_move":
            self.board = string_to_board(res)
            draw_board(self.canvas, self.board, self.win)

            time.sleep(0.2)

            ai_move = os.popen(f"./bot -play {res} {self.enemy} 6").read()

            if (ai_move != -1):

                new = os.popen(f"./bot -check {res} {self.enemy} {ai_move}").read()

                self.board = string_to_board(new)

                draw_board(self.canvas, self.board, self.win)



        
GUI()