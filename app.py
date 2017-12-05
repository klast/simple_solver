# -*- coding: utf-8 -*-
"""
Created on Tue Dec  5 14:56:39 2017

@author: student
"""

import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QMainWindow, QPushButton, QApplication, QFileDialog
from interface import Ui_MainWindow
from ctypes import *


class MainWindow(QMainWindow, Ui_MainWindow):
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
        self.filenames = ["", "", "", "", "", ""]
        
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
            
        
        
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())