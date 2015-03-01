#!/usr/bin/python

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import string
import socket
import wx

class Dialog2(wx.Dialog):
    """ The main frame of the application
    """
    title = 'Demo: wxPython with matplotlib'
    
    def __init__(self, parent, id, title):
        wx.Dialog.__init__(self, parent, id, title, size=((1000,500)))

        HOST = 'Tonys-MacBook-Air.local'
        PORT = 0x1234
        MSGSIZE = 8192
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST, PORT))
        s.send('pnl 0')
        data = s.recv(MSGSIZE)
        s.close()


        fig = plt.figure(figsize=(10, 6), dpi=110)
        ax = fig.add_subplot(111, projection='3d')

        strr = data.split()
        strLength = len(strr)

        xpos = np.ones(strLength)
        ypos = np.ones(strLength)
        zpos = np.ones(strLength)
        dx = np.zeros(strLength)
        dy = np.ones(strLength)
        dz = np.ones(strLength)

        xposs = range(strLength)

        for i in range(strLength):
            xpos[i] = xposs[i] + 1
            dz[i] = string.atof(strr[i])
            dx[i] = 0.5
            ypos[i] = strLength / 2
            

        ax.bar3d(xpos, ypos, zpos, dx, dy, dz, color='blue')
        ax.set_ylim([0,strLength])
        ax.set_zlim([-30000,30000])
        ax.set_zlabel('PnL')
        plt.show()