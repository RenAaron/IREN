from tkinter import *
import serial

port = input("Serial port?")
bluetooth = serial.Serial(port, 9600)
bluetooth.flushInput()

window = Tk()
window.configure(bg = "#000000")
window.geometry('1000x300')

clicked = StringVar()
clicked.set("HOT KEY 1")

clicked2 = StringVar()
clicked2.set("RIT")

clicked3 = StringVar()
clicked3.set("HOT KEY 2")

clicked4 = StringVar()
clicked4.set("RED")

linkHk = "1"
linkLink = "mail.google.com"

irHk = "2"
irIR = "16718565"

def sendLink():
     print(clicked.get() + " " +clicked2.get())
     print(clicked.get()[8:9])
     
     linkHk = clicked.get()[8:9]

     if (clicked2.get() == "RIT"):
          linkLink = "rit.edu"

     if (clicked2.get() == "DRIVE"):
          linkLink = "google.com/drive/"

     if (clicked2.get() == "CLASSROOM"):
          linkLink = "classroom.google.com"

     if (clicked2.get() == "MAPS"):
          linkLink = "maps.google.com/"

     bluetooth.write(("kc" + linkHk + linkLink).encode())

def sendIR():
     
     print(clicked3.get()[8:9])
     
     irHk = clicked3.get()[8:9]

     if (clicked4.get() == "RED"):
          irIR = "16718565"

     if (clicked4.get() == "POWER"):
          irIR = "16712445"

     if (clicked4.get() == "PURPLE"):
          irIR = "16724685"

     if (clicked4.get() == "FADE"):
          irIR = "16736415"

     bluetooth.write(("kc" + irHk + irIR).encode())


LABLE_HOTKEYNUM = Label(window, text = "IREN GUI", bg = '#000000', fg = '#FFFFFF' )
LABLE_HOTKEYNUM.config(font = ("Royal Crescent", 20))
LABLE_HOTKEYNUM.grid(row =0, column=2, pady = 10, padx = 10)

##########################################

LABLE_HOTKEYNUM = Label(window, text = "HOTKEY NUM", bg = '#000000', fg = '#FFFFFF' )
LABLE_HOTKEYNUM.config(font = ("Royal Crescent", 20))
LABLE_HOTKEYNUM.grid(row =1, column=0, pady = 10, padx = 10)

OPT_HOTKEYNUM = OptionMenu(window, clicked, "HOT KEY 1", "HOT KEY 2", "HOT KEY 3", "HOT KEY 4") #
OPT_HOTKEYNUM.config(font = ("Royal Crescent", 20), fg = '#FFFFFF', bg = '#000000')
OPT_HOTKEYNUM.grid(row = 1, column=1, padx = 10)

##########################################

LABLE_LINK = Label(window, text = "LINK", bg = '#000000', fg = '#FFFFFF' )
LABLE_LINK.config(font = ("Royal Crescent", 20))
LABLE_LINK.grid(row = 1, column=2, pady = 10, padx = 10)

OPT_LINK = OptionMenu(window, clicked2, "GMAIL", "DRIVE", "CLASSROOM", "MAPS")
OPT_LINK.config(font = ("Royal Crescent", 20), fg = '#FFFFFF', bg = '#000000')
OPT_LINK.grid(row = 1, column=3, padx = 10)

##########################################

send1 = Button(window, text='SEND LINK!', bg = '#000000', fg = '#FFFFFF')
send1.config(command = sendLink,  font = ("Royal Crescent", 20))
send1.grid(row = 1, column = 4, padx = 10)


##########################################









##########################################

LABLE_HOTKEYNUM = Label(window, text = "HOTKEY NUM", bg = '#000000', fg = '#FFFFFF' )
LABLE_HOTKEYNUM.config(font = ("Royal Crescent", 20))
LABLE_HOTKEYNUM.grid(row = 2, column=0, pady = 10, padx = 10)

OPT_HOTKEYNUM = OptionMenu(window, clicked3, "HOT KEY 1", "HOT KEY 2", "HOT KEY 3", "HOT KEY 4")
OPT_HOTKEYNUM.config(font = ("Royal Crescent", 20), fg = '#FFFFFF', bg = '#000000')
OPT_HOTKEYNUM.grid(row = 2, column=1, padx = 10)

##########################################

LABLE_IR = Label(window, text = "IR CODE", bg = '#000000', fg = '#FFFFFF' )
LABLE_IR.config(font = ("Royal Crescent", 20))
LABLE_IR.grid(row = 2, column=2, pady = 10, padx = 10)

OPT_IR = OptionMenu(window, clicked4, "RED", "POWER", "PURPLE", "FADE")
OPT_IR.config(font = ("Royal Crescent", 20), fg = '#FFFFFF', bg = '#000000')
OPT_IR.grid(row = 2, column=3, padx = 10)

##########################################

send2 = Button(window, text='SEND IR!', bg = '#000000', fg = '#FFFFFF')
send2.config(command=sendIR, font = ("Royal Crescent", 20))
send2.grid(row = 2, column = 4, padx = 10)

##########################################

window.mainloop()
