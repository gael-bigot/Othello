import tkinter
import os
import time
import random


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


class GameWin:
    def __init__(self, player):
        self.board = start_board()
        if player == -1:
            self.player = "X"
            self.enemy = "O"
        else:
            self.player = "O"
            self.enemy = "X"
            pos = random.choice([20, 29, 34, 43])
            res = os.popen(f"./bot -check {board_to_string(self.board)} X {pos}").read()
            self.board = string_to_board(res)


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

            ai_move = os.popen(f"./bot -play {res} {self.enemy} 8").read()

            if (ai_move != -1):

                new = os.popen(f"./bot -check {res} {self.enemy} {ai_move}").read()

                self.board = string_to_board(new)

                draw_board(self.canvas, self.board, self.win)


class PlayerChoice:
    def __init__(self):
        self.win = tkinter.Tk()
        self.win.title("Othello")
        self.win.resizable(0,0)
        label = tkinter.Label(self.win, text = "Pick a color", font = ("Arial", 25))
        label.pack(side = "top", padx = 5, pady = 5)
        btn_white = tkinter.Button(self.win, text = "White", font = ("Arial", 25), command = self.choose_white)
        btn_white.pack(side = "left", padx = 5, pady = 5)
        btn_black = tkinter.Button(self.win, text = "Black", font = ("Arial", 25), command = self.choose_black)
        btn_black.pack(side = "right", padx = 5, pady = 5)
        self.win.mainloop()

    def choose_white(self):
        self.choice = 1
        self.win.destroy()

    def choose_black(self):
        self.choice = -1
        self.win.destroy()


btn = PlayerChoice()

GameWin(btn.choice)