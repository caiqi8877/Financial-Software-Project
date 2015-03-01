#!/usr/bin/python

# layout.py

import wx
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import string
import socket

from dialog1 import Dialog1
from dialog2 import Dialog2
from fei2 import MyForm

HOST = 'Tonys-MacBook-Air.local'
PORT = 0x1233
MSGSIZE = 18192

class MyFrame(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title, wx.DefaultPosition, wx.Size(1170, 700))
        self.Centre()
        panel = wx.Panel(self, -1)
        
        png1 = wx.Image("table1.png", wx.BITMAP_TYPE_ANY)
        png1 = png1.Rescale(540,320).ConvertToBitmap()
        png2 = wx.Image("table2.png", wx.BITMAP_TYPE_ANY)
        png2 = png2.Rescale(540,320).ConvertToBitmap()
        png3 = wx.Image("pnl1.png", wx.BITMAP_TYPE_ANY)
        png3 = png3.Rescale(540,320).ConvertToBitmap()
        png4 = wx.Image("pnl2.png", wx.BITMAP_TYPE_ANY)
        png4 = png4.Rescale(540,320).ConvertToBitmap()
        # ConvertToBitmap()
        wx.BitmapButton(panel, id=1, bitmap=png1, pos=(30, 5), size=(png1.GetWidth()+5, png1.GetHeight()+5))
        wx.BitmapButton(panel, id=2, bitmap=png2, pos=(600, 5), size=(png2.GetWidth()+5, png2.GetHeight()+5))
        wx.BitmapButton(panel, id=3, bitmap=png3, pos=(30, 345), size=(png3.GetWidth()+5, png3.GetHeight()+5))
        wx.BitmapButton(panel, id=4, bitmap=png4, pos=(600, 345), size=(png4.GetWidth()+5, png4.GetHeight()+5))

        self.Bind(wx.EVT_BUTTON, self.OnShowCustomDialog1, id=1)
        self.Bind(wx.EVT_BUTTON, self.OnShowCustomDialog4, id=2)
        self.Bind(wx.EVT_BUTTON, self.OnShowCustomDialog2, id=3)
        self.Bind(wx.EVT_BUTTON, self.OnShowCustomDialog3, id=4)

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect((HOST, PORT))

    def OnShowCustomDialog1(self, event):

        Dialog1.mesg = self.s

        self.s.send('rateb 0')
        quality = self.s.recv(MSGSIZE)
        Dialog1.content = quality
        self.s.send('wtime 0')
        Dialog1.time1=self.s.recv(MSGSIZE)
        
        self.s.send('nameb 0')
        ticker = self.s.recv(MSGSIZE)
        Dialog1.ticker = ticker
        self.s.send('wtime 0')
        Dialog1.time2=self.s.recv(MSGSIZE)

        self.s.send('varis 99###')
        var = self.s.recv(MSGSIZE)
        Dialog1.var = var
        self.s.send('wtime 0')
        Dialog1.time3=self.s.recv(MSGSIZE)
        # self.s.send('varinfo 99')
        # var = self.s.recv(MSGSIZE)
        # print var
        dia = Dialog1(self, -1, 'buttons')
        # dia.ShowModal()
        # dia.Destroy()

    def OnShowCustomDialog4(self, event):
        # self.s.send('ratebucket 0')
        # quality = self.s.recv(MSGSIZE)
        
        self.s.send('riskd 0')
        tickers = self.s.recv(MSGSIZE)
        MyForm.content = tickers

        # self.s.send('shift 0')
        # ticker = self.s.recv(MSGSIZE)
        # MyForm.shift = ticker
     
    
        # self.s.send('dropp 0')
        # ticker1 = self.s.recv(MSGSIZE)
        # MyForm.dropp = ticker1
        
   
        # self.s.send('reset 0')
        # quality = self.s.recv(MSGSIZE)
        # MyForm.reset = quality

        MyForm.mesg = self.s
        # self.s.send('varinfo 99')
        # var = self.s.recv(MSGSIZE)
        # print var
        dia = MyForm(self, -1, 'buttons')
        # dia.ShowModal()
        # dia.Destroy()

    def OnShowCustomDialog2(self, event):
        self.s.send('pnlpc 0')
        data = self.s.recv(MSGSIZE)

        self.s.send('wtime 0')
        time_start = self.s.recv(MSGSIZE)

        strr = data.split()
        strLength = len(strr)

        fig = plt.figure(figsize=(30, 6), dpi=110)
        fig.suptitle('PnL Histrogram       Time:   ' + time_start)
        ax = fig.add_subplot(111, projection='3d')
        xpos = np.ones(strLength)
        ypos = np.ones(strLength)
        zpos = np.ones(strLength)
        dx = np.zeros(strLength)
        dy = np.ones(strLength)
        dz = np.ones(strLength)
        cs = ['r'] * strLength;

        xposs = range(strLength)
        maxNum = 0
        minNum = 0
        for i in range(strLength):
            xpos[i] = xposs[i] + 1
            dz[i] = string.atof(strr[i])
            dx[i] = 0.5
            ypos[i] = strLength / 2
            if dz[i] > 0:
                cs[i] = 'g'
            if dz[i] > maxNum:
                maxNum = dz[i];
            if dz[i] < minNum:
                minNum = dz[i];
        
        ax.bar3d(xpos, ypos, zpos, dx, dy, dz, color=cs, edgecolor="none")
        ax.autoscale_view(True,True,True)
        # ax.set_xlim([40,60])
        ax.set_ylim([0,strLength])
        ax.set_zlim([minNum,maxNum])
        ax.set_zlabel('PnL')
        ax.set_yticks(()) 
        plt.show()


    def OnShowCustomDialog3(self, event):
        self.s.send('distr 0')
        data = self.s.recv(MSGSIZE)

        self.s.send('wtime 0')
        time_start = self.s.recv(MSGSIZE)

        strr = data.split()
        strLength = len(strr)

        fig = plt.figure(figsize=(30, 6), dpi=110)
        fig.suptitle('PnL Distribution        Time:   ' + time_start)
        ax = fig.add_subplot(111, projection='3d')
        xpos = np.ones(strLength)
        ypos = np.ones(strLength)
        zpos = np.ones(strLength)
        dx = np.zeros(strLength)
        dy = np.ones(strLength)
        dz = np.ones(strLength)
        cs = ['b'] * strLength;

        xposs = range(strLength)

        max = 0
        for i in range(strLength):
            if i == 0 or i == strLength-1:
                continue
            xpos[i] = xposs[i] + 1
            dz[i] = string.atof(strr[i])
            dx[i] = 0.5
            ypos[i] = strLength / 2
            if dz[i] > max:
                max = dz[i]
            

        ax.bar3d(xpos, ypos, zpos, dx, dy, dz, color=cs)
        ax.autoscale_view(True,True,True)
        ax.set_ylim([0,strLength])
        ax.set_zlim([0,max * 1.3])
        ax.set_zlabel('PnL')
        ax.set_yticks(()) 
        plt.show()

class MyApp(wx.App):
    def OnInit(self):
        frame = MyFrame(None, -1, 'Financial Project FInal GUI.py')
        frame.Show(True)
        frame.Centre()
        return True

app = MyApp(0)
app.MainLoop()