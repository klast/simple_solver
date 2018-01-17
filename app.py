# -*- coding: utf-8 -*-
"""
Created on Tue Dec  5 14:56:39 2017

@author: student
"""

import sys
import random
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QMainWindow, QPushButton, QApplication, QFileDialog
from PyQt5.QtWidgets import QDialog, QApplication, QPushButton, QVBoxLayout
from interface import Ui_MainWindow
from ctypes import *
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
import numpy as np
import math
import h5py
from drawer import Drawer

class MainWindow(QMainWindow, Ui_MainWindow):
    pressures = []
    saturations = []
    pvt = []
    production_1 = []
    production_2 = []
    injection_1 = []
    injection_2 = []
    
    def get_pressures(self):
        f = h5py.File("tests/model4_test/init4.h5", "r")
        h1 = f['pressure_cube']
        size = len([i for i in h1])
        list_pres = []
        for i in range(size):
            tmp = np.array(h1[str(i)])
            s = int(math.sqrt(tmp.shape[0]))
            tmp = tmp.reshape((s, s))
            list_pres.append(tmp)
        list_pres = np.array(list_pres)
        return list_pres
        
        
    def get_saturations(self):
        f = h5py.File("tests/model4_test/init4.h5", "r")
        h1 = f['saturation_cube']
        size = len([i for i in h1])
        list_pres = []
        for i in range(size):
            tmp = np.array(h1[str(i)])
            s = int(math.sqrt(tmp.shape[0]))
            tmp = tmp.reshape((s, s))
            list_pres.append(tmp)
        list_pres = np.array(list_pres)
        return list_pres

    def __init__(self, parent=None):
        QMainWindow.__init__(self, parent)
        self.setupUi(self)
        self.init_well_Button.clicked.connect(self.handle_Button)
        self.GPRO_Button.clicked.connect(self.handle_Button)
        self.GRID_Button.clicked.connect(self.handle_Button)
        self.INIT_Button.clicked.connect(self.handle_Button)
        self.PVT_Button.clicked.connect(self.handle_Button)
        self.SCAL_Button.clicked.connect(self.handle_Button)
        self.push_Button.clicked.connect(self.handle_start_Button)
        self.pushButton.clicked.connect(self.handle_drawInit_Button)
        self.pushButton_5.clicked.connect(self.handle_drawInit_Button)
        self.pushButton_3.clicked.connect(self.handle_startAnimation_Button)
        self.pushButton_7.clicked.connect(self.handle_startAnimation_Button)
        self.pushButton_4.clicked.connect(self.handle_drawFinal_Button)
        self.pushButton_8.clicked.connect(self.handle_drawFinal_Button)
        self.horizontalSlider.valueChanged.connect(self.handle_drawSelected_Slider)
        self.horizontalSlider_2.valueChanged.connect(self.handle_drawSelected_Slider)


        self.filenames = ["", "", "", "", "", ""]

        self.pressures = self.get_pressures()
        self.saturations = self.get_saturations()
        self.presmin = 99#np.min(self.pressures)
        self.presmax = 101#np.max(self.pressures)
        self.satmin = np.min(self.saturations)
        self.satmax = np.max(self.saturations)

        self.horizontalSlider.setMinimum(0)
        self.horizontalSlider.setMaximum(self.pressures.shape[0] - 1)
        self.horizontalSlider_2.setMinimum(0)
        self.horizontalSlider_2.setMaximum(self.pressures.shape[0] - 1)

        self.handle_drawGraphPVT()
        self.handle_drawOilWellsParams()        

        
    def handle_Button(self):
        sender = self.sender()
        if sender == self.init_well_Button:
            dialog_name = "txt файл(*.txt)"
        else:
            dialog_name = "INC файл(*.INC)"
        filename = QFileDialog.getOpenFileName(self, "Открыть файл", "", dialog_name)
        if sender == self.init_well_Button:
            self.filenames[0] = filename[0]
        elif sender == self.SCAL_Button:
            self.filenames[1] = filename[0]
        elif sender == self.PVT_Button:
            self.filenames[2] = filename[0]
        elif sender == self.GRID_Button:
            self.filenames[3] = filename[0]
        elif sender == self.INIT_Button:
            self.filenames[4] = filename[0]
        elif sender == self.GPRO_Button:
            self.filenames[5] = filename[0]
            
    def handle_start_Button(self):
        is_all_set = True
        for i in range(6):
            if self.filenames[i] == "":
                is_all_set = False
        msgBox = QtWidgets.QMessageBox()
        if is_all_set == False:
            msgBox.setText("Ошибка! Какой-то из файлов не задан!")
            msgBox.exec()
            sys.exit()
        else:
            self.lib = cdll.LoadLibrary("lib_simple_solver.dll")
            self.lib.set_file(0, self.filenames[0].encode('utf-8'))
            self.lib.set_file(1, self.filenames[1].encode('utf-8'))
            self.lib.set_file(2, self.filenames[2].encode('utf-8'))
            self.lib.set_file(3, self.filenames[3].encode('utf-8'))
            self.lib.set_file(4, self.filenames[4].encode('utf-8'))
            self.lib.set_file(5, self.filenames[5].encode('utf-8'))
            ret = self.lib.simulate()
            sys.exit()

    def handle_drawInit_Button(self):
        sender = self.sender()
        if sender == self.pushButton:
            self.horizontalSlider.setValue(0)
            current_pressure = self.pressures[0, :, :]
            widget = self.widget_MapOfPressure
            Drawer.drawField(widget, current_pressure, self.presmin, self.presmax)
        elif sender == self.pushButton_5:
            self.horizontalSlider_2.setValue(0)
            current_saturation = self.saturations[0, :, :]
            widget = self.widget_MapOfSaturation
            Drawer.drawField(widget, current_saturation, self.satmin, self.satmax)

    def handle_drawFinal_Button(self):
        sender = self.sender()
        if sender == self.pushButton_4:
            self.horizontalSlider.setValue(self.pressures.shape[0])
            current_pressure = self.pressures[-1, :, :]
            widget = self.widget_MapOfPressure
            Drawer.drawField(widget, current_pressure, self.presmin, self.presmax)
        elif sender == self.pushButton_8:
            self.horizontalSlider_2.setValue(self.saturations.shape[0])
            current_saturation = self.saturations[-1, :, :]
            widget = self.widget_MapOfSaturation
            Drawer.drawField(widget, current_saturation, self.satmin, self.satmax)

    def handle_drawSelected_Slider(self):
        sender = self.sender()
        if sender == self.horizontalSlider:
            i = self.horizontalSlider.value()
            current_pressure = self.pressures[i, :, :]
            widget = self.widget_MapOfPressure
            Drawer.drawField(widget, current_pressure, self.presmin, self.presmax)
        elif sender == self.horizontalSlider_2:
            i = self.horizontalSlider_2.value()
            current_saturation = self.saturations[i, :, :]
            widget = self.widget_MapOfSaturation
            Drawer.drawField(widget, current_saturation, self.satmin, self.satmax)

    def handle_startAnimation_Button(self):
        pass

    def handle_drawOilWellsParams(self):
        title = "Inj. 1"
        widget = self.widget_GraphInj_1
        data_x = np.linspace(1, 20, 20)
        data_y = [random.random() for i in range(data_x.size)]
        Drawer.drawLineGraph(widget, data_x, data_y, title)

        title = "Inj. 2"
        widget = self.widget_GraphInj_2
        data_x = np.linspace(1, 20, 20)
        data_y = [random.random() for i in range(data_x.size)]
        Drawer.drawLineGraph(widget, data_x, data_y, title)

        title = "Prod. 1"
        widget = self.widget_GraphProd_1
        data_x = np.linspace(1, 20, 20)
        data_y = [random.random() for i in range(data_x.size)]
        Drawer.drawLineGraph(widget, data_x, data_y, title)

        title = "Prod. 2"
        widget = self.widget_GraphProd_2
        data_x = np.linspace(1, 20, 20)
        data_y = [random.random() for i in range(data_x.size)]
        Drawer.drawLineGraph(widget, data_x, data_y, title)


    def handle_drawGraphPVT(self):
        title = "PVT-свойства"
        widget = self.widget_GraphPvt
        data_x = np.linspace(1, 20, 20)
        data_y = [random.random() for i in range(data_x.size)]
        Drawer.drawLineGraph(widget, data_x, data_y, title)

        title = "Каппиллярное давление"
        widget = self.widget_GraphCapillar
        data_x = np.linspace(1, 20, 20)
        data_y = [random.random() for i in range(data_x.size)]
        Drawer.drawLineGraph(widget, data_x, data_y, title)



        
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())