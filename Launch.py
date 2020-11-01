import ctypes
import sys

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import Ui_QuestionBankUI

QBlibc = ctypes.WinDLL("./QuestionBank.dll")
QBlibc.py_test.restype = ctypes.c_char_p
QBlibc.py_generate.restype = ctypes.c_char_p
QBlibc.py_calculate.restype = ctypes.c_int

_translate = QCoreApplication.translate

'''
print(QBlibc.py_test(b"1234567899"))
s = QBlibc.py_generate().decode()
print(s)
print(QBlibc.py_calculate(bytes(s,encoding='utf-8')))
'''

class QBWindows(QMainWindow,Ui_QuestionBankUI.Ui_MainWindow):

    def __init__(self,parent = None):
        super().__init__(parent)
        self.setupUi(self)
        self.negative.stateChanged.connect(self.set_negative)
        self.decimal.stateChanged.connect(self.set_decimal)
        self.bracket.stateChanged.connect(self.set_bracket)
        self.remain.currentIndexChanged.connect(self.set_remain)
        self.len.currentIndexChanged.connect(self.set_len)
        self.get_questions.clicked.connect(self.QB_get_questions)
        self.judge_questions.clicked.connect(self.QB_judge_questions)
        self.clear_questions.clicked.connect(self.clear_all)

        self.problems = []
        self.ans = []
        for i in range(10):
            self.problems.append(QLabel(self.gridLayoutWidget_2)) 
            self.gridLayout_2.addWidget(self.problems[i],i+1, 1, 1, 1)
        for i in range(10):
            self.ans.append(QHBoxLayout(self.gridLayoutWidget_2))
            self.gridLayout_2.addLayout(self.ans[i],i+1, 2, 1, 1)
    def set_negative(self,state):
        if state==Qt.Checked:
            QBlibc.py_set_negative(1)
        else:
            QBlibc.py_set_negative(0)
    def set_decimal(self,state):
        if state==Qt.Checked:
            QBlibc.py_set_decimal(1)
        else:
            QBlibc.py_set_decimal(0)
    def set_bracket(self,state):
        if state==Qt.Checked:
            QBlibc.py_set_bracket(1)
        else:
            QBlibc.py_set_bracket(0)
    def set_remain(self,state):
        QBlibc.py_set_remain(state+2)
    def set_len(self,state):
        QBlibc.py_set_len(10**(state+1))
    def QB_get_questions(self):
        self.clear_all()
        self.add_blank(1)
        self.add_blank(2)
        self.add_judge(3)
        self.add_choice(4)
    def add_blank(self,index):
        s = QBlibc.py_generate().decode()
        QBlibc.py_set_remain(self.remain.currentIndex()+2)
        self.problems[index-1].setText(_translate("MainWindow",s))
        wid = []
        wid.append(QLineEdit())
        self.ans[index-1].addWidget(wid[0])
    def add_choice(self,index):
        s = QBlibc.py_generate().decode()
        QBlibc.py_set_remain(self.remain.currentIndex()+2)
        self.problems[index-1].setText(_translate("MainWindow",s))
        buttonGroup=QButtonGroup(self.gridLayoutWidget_2)
        wid = []
        wid.append(QRadioButton())
        wid[0].setText(_translate("MainWindow", "A"))
        wid.append(QRadioButton())
        wid[1].setText(_translate("MainWindow", "B"))
        wid.append(QRadioButton())
        wid[2].setText(_translate("MainWindow", "C"))
        wid.append(QRadioButton())
        wid[3].setText(_translate("MainWindow", "D"))
        buttonGroup.addButton(wid[0])
        buttonGroup.addButton(wid[1])
        buttonGroup.addButton(wid[2])
        buttonGroup.addButton(wid[3])
        wid[0].setChecked(True)
        self.ans[index-1].addWidget(wid[0])
        self.ans[index-1].addWidget(wid[1])
        self.ans[index-1].addWidget(wid[2])
        self.ans[index-1].addWidget(wid[3])
    def add_judge(self,index):
        s = QBlibc.py_generate().decode()
        QBlibc.py_set_remain(self.remain.currentIndex()+2)
        self.problems[index-1].setText(_translate("MainWindow",s))
        buttonGroup=QButtonGroup(self.gridLayoutWidget_2)
        wid = []
        wid.append(QRadioButton())
        wid[0].setText(_translate("MainWindow", "√"))
        wid.append(QRadioButton())
        wid[1].setText(_translate("MainWindow", "×"))
        buttonGroup.addButton(wid[0])
        buttonGroup.addButton(wid[1])
        wid[0].setChecked(True)
        self.ans[index-1].addWidget(wid[0])
        self.ans[index-1].addWidget(wid[1])
    def QB_judge_questions(self):
        self.analysis.setText(_translate("MainWindow","verygood"))
    def clear_all(self):
        for i in self.ans:
            while i.count():
                i.takeAt(0).widget().deleteLater()
        for i in self.problems:
            i.setText(_translate("MainWindow",""))

        

if __name__ == "__main__":
    app=QApplication(sys.argv)
    QBui = QBWindows()
    QBui.show()
    sys.exit(app.exec())
