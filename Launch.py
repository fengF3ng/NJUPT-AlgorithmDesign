import ctypes
import sys

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

import Ui_QuestionBankUI

QBlibc = ctypes.WinDLL("./QuestionBank.dll")
QBlibc.py_test.restype = ctypes.c_char_p
QBlibc.py_generate.restype = ctypes.c_char_p
QBlibc.py_calculate.restype = ctypes.c_double

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
        self.blank = 10
        self.choice = 0
        self.judge = 0

        self.actionSave.triggered.connect(self.QB_save)
        self.actionExit.triggered.connect(QCoreApplication.instance().quit)
        self.negative.stateChanged.connect(self.set_negative)
        self.decimal.stateChanged.connect(self.set_decimal)
        self.bracket.stateChanged.connect(self.set_bracket)
        self.remain.currentIndexChanged.connect(self.set_remain)
        self.len.currentIndexChanged.connect(self.set_len)
        self.blank_num.textChanged.connect(self.blank_num_balance)
        self.choice_num.textChanged.connect(self.choice_num_balance)
        self.judge_num.textChanged.connect(self.judge_num_balance)
        self.get_questions.clicked.connect(self.QB_get_questions)
        self.judge_questions.clicked.connect(self.QB_judge_questions)
        self.clear_questions.clicked.connect(self.clear_all)

        self.problems = []
        self.ans = []
        self.user_ans = []
        for i in range(10):
            self.problems.append(QLabel(self.gridLayoutWidget_2)) 
            self.gridLayout_2.addWidget(self.problems[i],i+1, 1, 1, 1)
        for i in range(10):
            self.ans.append(QHBoxLayout(self.gridLayoutWidget_2))
            self.gridLayout_2.addLayout(self.ans[i],i+1, 2, 1, 1)
    def QB_save(self):
        print("save")
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
    def blank_num_balance(self,state):
        try:
            state = int(state)
        except:
            print("error")
        else:
            if state+self.choice+self.judge>10:
                self.blank_num.setText(_translate("MainWindow",str(10-self.choice-self.judge)))
                self.blank = 10-self.choice-self.judge
            else:
                self.blank = state
    def choice_num_balance(self,state):
        try:
            state = int(state)
        except:
            print("error")
        else:
            if state+self.blank+self.judge>10:
                self.choice_num.setText(_translate("MainWindow",str(10-self.blank-self.judge)))
                self.choice = 10-self.blank-self.judge
            else:
                self.choice = state
    def judge_num_balance(self,state):
        try:
            state = int(state)
        except:
            print("error")
        else:
            if state+self.choice+self.judge>10:
                self.judge_num.setText(_translate("MainWindow",str(10-self.choice-self.blank)))
                self.judge = 10-self.choice-self.blank
            else:
                self.judge = state
    def QB_get_questions(self):
        self.clear_all()
        for i in range(1,self.blank+1):
            self.add_blank(i)
        for i in range(self.blank+1,self.blank+self.choice+1):
            self.add_choice(i)
        for i in range(self.blank+self.choice+1,self.blank+self.choice+self.judge+1):
            self.add_judge(i)
    def add_blank(self,index):
        s = QBlibc.py_generate().decode()
        QBlibc.py_set_remain(self.remain.currentIndex()+2)
        self.problems[index-1].setText(_translate("MainWindow",s))

        wid = []
        wid.append(QLineEdit())
        self.ans[index-1].addWidget(wid[0])
        self.user_ans.append(wid)
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
        self.user_ans.append(wid)
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
        self.user_ans.append(wid)
    def QB_judge_questions(self):
        user_analysis = ""
        score = 0
        j = 0
        for i in self.user_ans:
            if len(i)==4: # choice
                pass
            elif len(i)==2: # judge
                pass
            elif len(i)==1: # blank
                if abs(QBlibc.py_calculate(bytes(self.problems[j],encoding='utf-8'))-i)<0.1:
                    score += 1
            j += 1
        self.analysis.setText(_translate("MainWindow",user_analysis))

    def clear_all(self):
        self.user_ans.clear()
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
