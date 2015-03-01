import wx
import wx.grid as  gridlib
import wx
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import string
import socket
MSGSIZE = 28192
class Dialog1(wx.Frame):
    def __init__(self, parent, id,title):
        wx.Frame.__init__(self, None, id, "table of bond", size=((1120,700)))
        #self.Centre()
        # wx.Frame.__init__(self, None, wx.ID_ANY, 
        #                   "Grid with Popup Menu",size =(1120,600))
        
        panel = wx.Panel(self, wx.ID_ANY)
        self.grid = gridlib.Grid(panel)
        self.grid.CreateGrid(21,11)
        for i in range(231):
            row, col = divmod(i, 11)
            self.grid.SetRowSize(row, 24)
            self.grid.SetColSize(col, 90)
        self.grid.SetCellAlignment
        # self.Bind(gridlib.EVT_GRID_CELL_LEFT_CLICK, self.OnCellLeftClick)
        self.Bind(gridlib.EVT_GRID_CELL_RIGHT_CLICK, self.OnCellRightClick)
#----------------------------------------------------------------merge cells
        self.grid.SetCellSize(0, 0, 3, 11)
        self.grid.SetCellAlignment(0, 0, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetCellValue(0, 0, "Daily Change by Quality (amounts in 000's)")
        self.grid.SetCellFont(0, 0, wx.Font(19, wx.SWISS, wx.NORMAL, wx.BOLD))
        self.grid.SetCellBackgroundColour(0,0,wx.Colour(0x93,0xDB,0x70))

        self.grid.SetCellSize(3, 0, 2, 2)
        self.grid.SetCellBackgroundColour(3,0,wx.WHITE)

        self.grid.SetCellSize(3, 2, 2, 3)
        self.grid.SetCellAlignment(3, 2, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetCellValue(3, 2, "Opening Position")
        self.grid.SetCellBackgroundColour(3,2,wx.Colour(0xAD,0xEA,0xEA))

        self.grid.SetCellSize(3, 5, 2, 3)
        self.grid.SetCellAlignment(3, 5, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetCellValue(3, 5, "Closing Position")
        self.grid.SetCellBackgroundColour(3,5,wx.Colour(0xFF,0x7F,0x00))

        self.grid.SetCellSize(3, 8, 2, 3)
        self.grid.SetCellAlignment(3, 8, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetCellValue(3, 8, "Intra-day Change")
        self.grid.SetCellBackgroundColour(3,8,wx.Colour(0xFF,0xFF,0x00))

        for i in range(5,31):
            self.grid.SetCellSize(i, 0, 1, 2)
            self.grid.SetCellBackgroundColour(i,0,wx.WHITE)

        for i in range(5,21):
            self.grid.SetCellValue(i,0,"")
            self.grid.SetCellTextColour(i,0,wx.BLACK)
            if i<18:
                self.grid.SetCellBackgroundColour(i,0,wx.Colour(0xD8,0xBF,0xD8))
#----------------------------------------------------------------merge cells
#----------------------------------------------------------------insert titles
        titles = "Ticker Notional Risk LGD Notional Risk LGD Notional Risk LGD".split()
        for i in range(0,10):
            pos = i
            if ( pos >= 1):
                pos += 1
            self.grid.SetCellAlignment(5, pos, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
            self.grid.SetCellValue(5,pos, titles[i])
#----------------------------------------------------------------insert titles
#----------------------------------------------------------------insert quality
        self.grid.SetCellValue(5,0,"Quality")
        values = (self.content).split()
        lenth = len(values)
        # values = "AA 5000 17 0 30000 15 0 -20000 -2 0 BB 40000 44 19286 40000 44 3000 0 0 19286 BBB -90000 -43 0 900000 43 0 180000 87 0 CCC -90000 -43 0 900000 43 0 180000 87 0".split()
        for i in range(0,lenth):
            x, col = divmod(i, 10)
            row = x + 6
            pos = col
            if ( pos >= 1):
                pos += 1
            self.grid.SetCellAlignment(row , pos, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
            self.grid.SetCellValue(row ,pos, values[i])
#----------------------------------------------------------------insert quality
#----------------------------------------------------------------insert var
        # self.grid.SetCellValue(20 ,0, "VaR - credit spread")
        # self.grid.SetCellValue(21 ,0, "VaR - interest rate")
        # self.grid.SetCellValue(22 ,0, "Total - total")
        # for i in range(20,23):
        #     self.grid.SetCellTextColour(i, 0, wx.RED)
        #     self.grid.SetCellAlignment(i , 0, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        # values = "2.1 3 0.9 1.2 1.5 0.3 3.3 4.5 1.2".split()
        # for i in range(9):
        #     x, col = divmod(i, 3)
        #     row = x + 24
        #     pos = col*3 + 1
        #     if ( pos >= 1):
        #         pos += 1
        #     self.grid.SetCellTextColour(row, pos, wx.RED)
        #     self.grid.SetCellAlignment(row , pos, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        #     self.grid.SetCellValue(row ,pos, values[i])

#----------------------------------------------------------------insert var
#----------------------------------------------------------------insert time
        fix=19
        self.grid.SetCellValue(fix,0,"Server Time:")
        self.grid.SetCellTextColour(fix, 0, wx.RED)
        self.grid.SetCellValue(fix,2,"Real")
        self.grid.SetCellValue(fix,3,"User")
        self.grid.SetCellValue(fix,4,"System")
        var = self.time2
        values = var.split();
        for i in range(2,5):
            self.grid.SetCellValue(fix+1,i,values[i-2])
            self.grid.SetCellTextColour(fix, i, wx.RED)
            self.grid.SetCellTextColour(fix+1, i, wx.RED)
            self.grid.SetCellAlignment(fix, i, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
            self.grid.SetCellAlignment(fix+1, i, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetCellAlignment(fix, 0, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetRowSize(18,47)
        sizer = wx.BoxSizer(wx.VERTICAL)
        
        sizer.Add(self.grid, 1, wx.EXPAND|wx.ALIGN_TOP, 5)
        sizer.Add((-1, 30))

        hbox5 = wx.BoxSizer(wx.HORIZONTAL)
        btn1 = wx.Button(panel, label='Ticker', size=(70, 30))
        hbox5.Add(btn1, flag=wx.LEFT,border=450)
        btn2 = wx.Button(panel, label='Quality', size=(70, 30))
        hbox5.Add(btn2, flag=wx.LEFT,border=20)
        btn3 = wx.Button(panel, label='Var', size=(70, 30))
        hbox5.Add(btn3, flag=wx.LEFT,border=20)
        
        sizer.Add(hbox5, flag=wx.ALIGN_LEFT|wx.ALL, border=5)
        sizer.Add((-1, 7))
        panel.SetSizer(sizer)
        # btn4.Bind(wx.EVT_BUTTON, self.OnClose)
        btn1.Bind(wx.EVT_BUTTON, self.OnTicker)
        btn3.Bind(wx.EVT_BUTTON, self.OnVar)
        btn2.Bind(wx.EVT_BUTTON, self.OnQuality)
        self.grid.Bind(gridlib.EVT_GRID_CELL_CHANGE, self.OnSet)



        self.Show()
        
#----------------------------------------------------------------------pop up fun
    def OnCellRightClick(self, event):
        """
        Create and display a popup menu on right-click event
        """

        # Title = "Ticker Ticker Notional Risk LGD Notional Risk LGD Notional Risk LGD"
        # titles = Title.split(" ")
        row = event.GetRow()
        col = event.GetCol()
        # rownum = row + 1
        # if not hasattr(self, "popupID1"):
        #     self.popupID1 = wx.NewId()
            # self.popupID2 = wx.NewId()
        menu = wx.Menu()
        if col<5:
            item = wx.MenuItem(menu, 323,"Click to see the pie chart 1")
            self.Bind(wx.EVT_MENU, self.OnTest1, id=323)
        if col>4 and col<8:
        # item = wx.MenuItem(menu, self.popupID1,"You want know the %s ?" % titles[col])
            item = wx.MenuItem(menu, 324,"Click to see the pie chart 2")
            self.Bind(wx.EVT_MENU, self.OnTest2, id=324)
        if col>7:
        # item = wx.MenuItem(menu, self.popupID1,"You want know the %s ?" % titles[col])
            item = wx.MenuItem(menu, 325,"Click to see the pie chart 3")
            self.Bind(wx.EVT_MENU, self.OnTest3, id=325)

        # menu.Append(self.popupID2,titles[col])
        menu.AppendItem(item)
        self.PopupMenu(menu)
        
        menu.Destroy()



    def OnTest1(self, event):
        
        labels = 'Credit Spread', 'Interest Rate'
        credit_string = self.grid.GetCellValue(7,2)
        credit = abs(float(credit_string))

        interest_string = self.grid.GetCellValue(10,2)
        interest= abs(float(interest_string))
        # if credit>1:
        
        sizes = [credit,interest]
        colors = ['gold', 'lightskyblue']
        if credit > interest:
            explode = (0, 0.1) # only "explode" the 2nd slice (i.e. 'Hogs')
        else :
            explode = (0.1,0) # only "explode" the 2nd slice (i.e. 'Hogs')
        # if credit<1:
        #     sizes = [interest]
        #     colors = ['gold']
        # plt.figure(figsize=(14,10),dpi=100)
        plt.pie(sizes, explode=explode, labels=labels, colors=colors,
                autopct='%1.1f%%', shadow=True)
            # Set aspect ratio to be equal so that pie is drawn as a circle.
        plt.axis('equal')
        plt.show(block=False)    

    def OnTest2(self, event):
        
        labels = 'Credit Spread', 'Interest Rate'
        credit_string = self.grid.GetCellValue(7,5)
        credit = abs(float(credit_string))
        interest_string = self.grid.GetCellValue(10,5)
        interest= abs(float(interest_string))
        sizes = [credit,interest]
        colors = [ 'gold','lightskyblue']
        if credit > interest:
            explode = (0, 0.1) # only "explode" the 2nd slice (i.e. 'Hogs')
        else :
            explode = (0.1,0)# only "explode" the 2nd slice (i.e. 'Hogs')

        plt.pie(sizes, explode=explode, labels=labels, colors=colors,
                autopct='%1.1f%%', shadow=True)
            # Set aspect ratio to be equal so that pie is drawn as a circle.
        plt.axis('equal')
        plt.show(block=False)

    def OnTest3(self, event):
        
        labels = 'Credit Spread', 'Interest Rate'
        credit_string = self.grid.GetCellValue(7,8)
        credit = abs(float(credit_string))
        interest_string = self.grid.GetCellValue(10,8)
        interest= abs(float(interest_string))
        sizes = [credit,interest]
        colors = [ 'gold', 'lightskyblue']
        if credit > interest:
            explode = (0, 0.1) # only "explode" the 2nd slice (i.e. 'Hogs')
        else :
            explode = (0.1,0)
        plt.pie(sizes, explode=explode, labels=labels, colors=colors,
                autopct='%1.1f%%', shadow=True)
            # Set aspect ratio to be equal so that pie is drawn as a circle.
        plt.axis('equal')
        plt.show(block=False)
            
        

    def OnTicker(self, event):
# ----------------------------------------------------------------insert tickers
        for i in range(5,21):
            self.grid.SetRowSize(i, 24)
            for j in range(2,11):
                self.grid.SetCellSize(i, j, 1, 1)
                self.grid.SetCellBackgroundColour(i,j,wx.WHITE)
                self.grid.SetCellValue(i,j,"")
                self.grid.SetCellAlignment(i , j, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
                self.grid.SetCellTextColour(i,j,wx.BLACK)


        self.grid.SetCellValue(0, 0, "Daily Change by Issuer (amounts in 000's)")
        self.grid.SetCellValue(5,0,"Ticker")
        self.grid.SetCellValue(3, 2, "Opening Position")
        self.grid.SetCellValue(3, 5, "Closing Position")
        for i in range(5,21):
            self.grid.SetCellValue(i,0,"")
            self.grid.SetCellTextColour(i,0,wx.BLACK)
            self.grid.SetCellBackgroundColour(i,0,wx.WHITE)
        for i in range(5,11):
            self.grid.SetRowSize(i,50)
            if i<11:
                self.grid.SetCellBackgroundColour(i,0,wx.Colour(0xD8,0xBF,0xD8))
        fix=14
        self.grid.SetCellValue(fix,0,"Server Time:")
        self.grid.SetCellTextColour(fix, 0, wx.RED)
        self.grid.SetCellValue(fix,2,"Real")
        self.grid.SetCellValue(fix,3,"User")
        self.grid.SetCellValue(fix,4,"System")
        
        var = self.time1
        values = var.split();
        for i in range(2,5):
            self.grid.SetCellValue(fix+1,i,values[i-2])
            self.grid.SetCellTextColour(fix, i, wx.RED)
            self.grid.SetCellTextColour(fix+1, i, wx.RED)
            self.grid.SetCellAlignment(fix, i, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
            self.grid.SetCellAlignment(fix+1, i, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetCellAlignment(fix, 0, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        titles = "Ticker Notional Risk LGD Notional Risk LGD Notional Risk LGD".split()
        for i in range(0,10):
            pos = i
            if ( pos >= 1):
                pos += 1
            self.grid.SetCellValue(5,pos, titles[i])

        values = (self.ticker).split()
        lenth = len(values)
        for i in range(0,lenth):
            x, col = divmod(i, 10)
            row = x + 6
            pos = col
            if ( pos >= 1):
                pos += 1
            self.grid.SetCellValue(row ,pos, values[i])
           

    def OnQuality(self, event):   
        self.grid.SetCellValue(0, 0, "Daily Change by Quality (amounts in 000's)")
        self.grid.SetCellValue(5,0,"Quality")
        self.grid.SetCellValue(3, 2, "Opening Position")
        self.grid.SetCellValue(3, 5, "Closing Position")
        for i in range(5,21):
            self.grid.SetRowSize(i, 24)
            for j in range(2,11):
                self.grid.SetCellSize(i, j, 1, 1)
                self.grid.SetCellBackgroundColour(i,j,wx.WHITE)
                self.grid.SetCellValue(i,j,"")
                self.grid.SetCellAlignment(i , j, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
                self.grid.SetCellTextColour(i,j,wx.BLACK)
        for i in range(5,21):
            self.grid.SetCellValue(i,0,"")
            self.grid.SetCellTextColour(i,0,wx.BLACK)
            if i<14:
                self.grid.SetCellBackgroundColour(i,0,wx.Colour(0xD8,0xBF,0xD8))

        fix=19
        self.grid.SetCellValue(fix,0,"Server Time:")
        self.grid.SetCellTextColour(fix, 0, wx.RED)
        self.grid.SetCellValue(fix,2,"Real")
        self.grid.SetCellValue(fix,3,"User")
        self.grid.SetCellValue(fix,4,"System")
        
        var = self.time2
        values = var.split();
        for i in range(2,5):
            self.grid.SetCellValue(fix+1,i,values[i-2])
            self.grid.SetCellTextColour(fix, i, wx.RED)
            self.grid.SetCellTextColour(fix+1, i, wx.RED)
            self.grid.SetCellAlignment(fix, i, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
            self.grid.SetCellAlignment(fix+1, i, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetCellAlignment(fix, 0, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetRowSize(18,47)
        titles = "Notional Risk LGD Notional Risk LGD Notional Risk LGD".split()
        for i in range(2,11):
            self.grid.SetCellValue(5,i, titles[i-2])
        self.grid.SetCellValue(5,0, "Quality")

        values = (self.content).split()
        lenth = len(values)
        
        for i in range(0,60):
            x, pos = divmod(i, 10)
            row = x + 6
            if ( pos >= 1):
                pos += 1
            self.grid.SetCellValue(row ,pos, values[i])
        for i in range(100,120):
            x, pos = divmod(i, 10)
            row = x + 2
            if ( pos >= 1):
                pos += 1
            self.grid.SetCellValue(row ,pos, values[i])
        for i in range(14,20):
            self.grid.SetCellBackgroundColour(i,0,wx.WHITE)


    def OnVar(self, event):
        self.grid.SetCellValue(0, 0, "Daily Change of VaR (amounts in 000000's)")
        self.grid.SetCellValue(5,0,"")
        self.grid.SetCellValue(3, 2, "Opening Value")
        self.grid.SetCellValue(3, 5, "Closing Value")
        for i in range(5,21):
            self.grid.SetRowSize(i, 24)
            for j in range(2,11):
                self.grid.SetCellSize(i, j, 1, 1)
                self.grid.SetCellBackgroundColour(i,j,wx.WHITE)
                self.grid.SetCellValue(i,j,"")
                self.grid.SetCellAlignment(i , j, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
                self.grid.SetCellTextColour(i,j,wx.BLACK)
        for i in range(5,21):
            self.grid.SetCellValue(i,0,"")
            self.grid.SetCellBackgroundColour(i,0,wx.WHITE)
        
            # self.grid.SetCellFont(i, 0, wx.Font(10, wx.SWISS, wx.NORMAL, wx.BOLD))
            # self.grid.SetCellBackgroundColour(i,0,wx.Colour(0xFF,0x00,0x00))
        # self.grid.SetCellSize(7, 0, 1, 2)
        for i in range(7,16,3):
            self.grid.SetRowSize(i,40)
            for j in range(2,9,3):
                self.grid.SetCellSize(i, j, 1, 3)
                if j==2:
                    self.grid.SetCellBackgroundColour(i,j,wx.Colour(0xAD,0xEA,0xEA))
                elif j==5:
                    self.grid.SetCellBackgroundColour(i,j,wx.Colour(0xFF,0x7F,0x00))
                elif j==8:
                    self.grid.SetCellBackgroundColour(i,j,wx.Colour(0xFF,0xFF,0x00))
        self.grid.SetCellBackgroundColour(10,8,wx.Colour(0x00,0xCC,0xFF))
        self.grid.SetCellBackgroundColour(10,5,wx.Colour(0x00,0xCC,0xFF))
        self.grid.SetCellBackgroundColour(7,5,wx.Colour(0xFF,0x80,0x99))
        self.grid.SetCellBackgroundColour(7,2,wx.Colour(0xFF,0x80,0x99))
        for i in (0,2,5,8):
            self.grid.SetCellBackgroundColour(7,i,wx.Colour(0xFF,0xFF,0x00))
            self.grid.SetCellBackgroundColour(10,i,wx.Colour(0x00,0xCC,0xFF))
            self.grid.SetCellBackgroundColour(13,i,wx.Colour(0xAD,0xEA,0xEA))


      
        for i in range (15,19,3):
            for j in range (8,11,2):
                self.grid.SetCellSize(i, j, 2, 1)
                self.grid.SetCellBackgroundColour(i,j,wx.Colour(0x93,0xDB,0x70))
                self.grid.SetCellValue(i,j,"")
                self.grid.SetCellAlignment(i , j, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
                
        self.grid.SetCellValue(7, 0, "VaR - credit spread")
        self.grid.SetCellValue(10, 0, "VaR - interest rate")
        self.grid.SetCellValue(13, 0, "Total - total")
        
       
        values = (self.var).split()   
        lenth = len(values)
        
        i=0;
        for row in range(7,14,3):
            for pos in range(2,9,3):
                self.grid.SetCellAlignment(row , pos, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
                self.grid.SetCellValue(row ,pos, values[i])
                i +=1
                
        self.grid.SetCellValue(18,8,"Interval")
        self.grid.SetCellValue(18,10,"99")
        self.grid.SetCellFont(18, 8, wx.Font(15, wx.SWISS, wx.NORMAL, wx.BOLD))

        self.grid.SetCellValue(15,8,"Tail_VaR")
        self.grid.SetCellFont(15, 8, wx.Font(13, wx.SWISS, wx.NORMAL, wx.BOLD))

        self.mesg.send("tailv 0")
        string2 = self.mesg.recv(MSGSIZE)
        self.grid.SetCellValue(15,10,string2)


        fix=18
        self.grid.SetCellValue(fix,0,"Server Time:")
        self.grid.SetCellTextColour(fix, 0, wx.RED)
        self.grid.SetCellValue(fix,2,"Real")
        self.grid.SetCellValue(fix,3,"User")
        self.grid.SetCellValue(fix,4,"System")
        
        var=self.time3
        values = var.split();
        for i in range(2,5):
            self.grid.SetCellValue(fix+1,i,values[i-2])
            self.grid.SetCellTextColour(fix, i, wx.RED)
            self.grid.SetCellTextColour(fix+1, i, wx.RED)
            self.grid.SetCellAlignment(fix, i, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
            self.grid.SetCellAlignment(fix+1, i, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
        self.grid.SetCellAlignment(fix, 0, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)


    def OnSet(self,event):
        row1 =event.GetRow()
        col = event.GetCol()
        if row1==18 and col==10:
            a=self.grid.GetCellValue(18,10)
            for i in range (0,5-len(a)):
                a = a +"#"
        
            msg = "varis "+a
           
            self.mesg.send(msg)
            string1 = self.mesg.recv(MSGSIZE)
            values = string1.split()   
            lenth = len(values)
            i=0;
            for row in range(7,14,3):
                for pos in range(2,9,3):
                    self.grid.SetCellAlignment(row , pos, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE)
                    self.grid.SetCellValue(row ,pos, values[i])
                    i +=1
            self.mesg.send("wtime 0")
            time4= self.mesg.recv(MSGSIZE)
            values = time4.split()
            fix=18
            for i in range(2,5):
                self.grid.SetCellValue(fix+1,i,values[i-2])
        # values = var.split()

            
