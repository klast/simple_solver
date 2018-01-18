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

class Drawer :
    def drawField(widget, field, tmin, tmax) :

        # fig = self.widget_MapOfPressure.figure
        # fig.clear()
        #
        # # ###############################################################################################################
        # alpha = self.horizontalSlider.value() / 100
        # self.horizontalSlider.setValue(self.horizontalSlider.value() + 10)
        # phi_ext = 2 * np.pi * 0.5
        #
        # def flux_qubit_potential(phi_m, phi_p):
        #     return 2 + alpha - 2 * np.cos(phi_p) * np.cos(phi_m) - alpha * np.cos(phi_ext - 2 * phi_p)
        #
        # phi_m = np.linspace(0, 2 * np.pi, 100)
        # phi_p = np.linspace(0, 2 * np.pi, 100)
        # X, Y = np.meshgrid(phi_p, phi_m)
        # Z = flux_qubit_potential(X, Y).T
        # # ###############################################################################################################
        #
        # ax = fig.add_subplot(111)
        #
        # p = ax.pcolor(X / (2 * np.pi), Y / (2 * np.pi), Z, cmap=plt.cm.RdBu, vmin=abs(Z).min(), vmax=abs(Z).max())
        #
        # cb = fig.colorbar(p)
        #
        # self.widget_MapOfPressure.canvas.draw()

        fig = widget.figure
        fig.clear()

        grid_x = np.linspace(1, field[:, 0].size, field[:, 0].size)
        grid_y = np.linspace(1, field[0, :].size, field[0, :].size)
        X, Y = np.meshgrid(grid_y, grid_x)

        plot = fig.add_subplot(111)
        colors = plot.pcolor(X, Y, field, cmap=plt.cm.RdBu, vmin=tmin, vmax=tmax)
        colorbar = fig.colorbar(colors)

        widget.canvas.draw()


    def drawLineGraph(widget, data_x, data_y, title):
        fig = widget.figure
        fig.clear()

        plot = fig.add_subplot(111)
        plot.axes.set_title(title)

        plot.plot(data_y, '*-')

        widget.canvas.draw()

