import ctypes
import sys
from random import *

import win32con
import win32ui
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


class QBWindows(QMainWindow, Ui_QuestionBankUI.Ui_MainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)

        self.blank = 10
        self.choice = 0
        self.judge = 0
        self.bool_decimal = 0
        self.bool_negative = 0
        self.int_len = 10

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

        self.problems = []  # 题面组件
        self.ans = []  # 答题框
        self.user_ans = []  # 答题组件
        self.standard_ans = []  # 标准答案
        self.user_problems = ""  # 题面
        self.user_analysis = ""  # 分析

        for i in range(10):
            self.problems.append(QLabel(self.gridLayoutWidget_2))
            self.gridLayout_2.addWidget(self.problems[i], i+1, 1, 1, 1)
        for i in range(10):
            self.ans.append(QHBoxLayout(self.gridLayoutWidget_2))
            self.gridLayout_2.addLayout(self.ans[i], i+1, 2, 1, 1)

    def QB_save(self):
        dlg = win32ui.CreateFileDialog(
            0, None, None, win32con.OFN_OVERWRITEPROMPT | win32con.OFN_FILEMUSTEXIST, "TXT Files(*.txt)")
        dlg.SetOFNInitialDir("C:")
        if dlg.DoModal() == win32con.IDOK:
            f = open(dlg.GetPathName()+'.txt', 'w')
            f.write(self.user_problems)
            f.write("Aanalysis:\n")
            f.write(self.user_analysis)
            f.close()

    def set_negative(self, state):
        if state == Qt.Checked:
            QBlibc.py_set_negative(1)
            self.bool_negative = 1
        else:
            QBlibc.py_set_negative(0)
            self.bool_negative = 0

    def set_decimal(self, state):
        if state == Qt.Checked:
            QBlibc.py_set_decimal(1)
            self.bool_decimal = 1
        else:
            QBlibc.py_set_decimal(0)
            self.bool_decimal = 0

    def set_bracket(self, state):
        if state == Qt.Checked:
            QBlibc.py_set_bracket(1)
        else:
            QBlibc.py_set_bracket(0)

    def set_remain(self, state):
        QBlibc.py_set_remain(state+2)

    def set_len(self, state):
        QBlibc.py_set_len(10**(state+1))
        self.int_len = 10**(state+1)

    def blank_num_balance(self, state):
        try:
            state = int(state)
        except:
            pass
        else:
            if state+self.choice+self.judge > 10:
                self.blank_num.setText(_translate(
                    "MainWindow", str(10-self.choice-self.judge)))
                self.blank = 10-self.choice-self.judge
            else:
                self.blank = state

    def choice_num_balance(self, state):
        try:
            state = int(state)
        except:
            pass
        else:
            if state+self.blank+self.judge > 10:
                self.choice_num.setText(_translate(
                    "MainWindow", str(10-self.blank-self.judge)))
                self.choice = 10-self.blank-self.judge
            else:
                self.choice = state

    def judge_num_balance(self, state):
        try:
            state = int(state)
        except:
            pass
        else:
            if state+self.choice+self.judge > 10:
                self.judge_num.setText(_translate(
                    "MainWindow", str(10-self.choice-self.blank)))
                self.judge = 10-self.choice-self.blank
            else:
                self.judge = state

    def QB_get_questions(self):
        self.clear_all()
        for i in range(1, self.blank+1):
            self.add_blank(i)
        for i in range(self.blank+1, self.blank+self.choice+1):
            self.add_choice(i)
        for i in range(self.blank+self.choice+1, self.blank+self.choice+self.judge+1):
            self.add_judge(i)

    def add_blank(self, index):
        s = QBlibc.py_generate().decode()
        if self.bool_decimal:
            self.standard_ans.append(
                float('%.1f' % QBlibc.py_calculate(bytes(s, encoding='utf-8'))))
        else:
            self.standard_ans.append(
                int(QBlibc.py_calculate(bytes(s, encoding='utf-8'))))
        self.problems[index-1].setText(_translate("MainWindow", s))
        self.user_problems += s + '\n' + 'Correct answer:' + \
            str(self.standard_ans[index-1]) + '\n'

        wid = []
        wid.append(QLineEdit())
        self.ans[index-1].addWidget(wid[0])
        self.user_ans.append(wid)

    def add_choice(self, index):
        s = QBlibc.py_generate().decode()
        ans_part = []
        if self.bool_decimal:
            tmp = QBlibc.py_calculate(bytes(s, encoding='utf-8'))
            ans_part.append(float('%.1f' % tmp))
        else:
            ans_part.append(
                int(QBlibc.py_calculate(bytes(s, encoding='utf-8'))))
        s += '\n'
        right_ans = randint(0, 3) % 4
        for i in range(4):
            if right_ans == i:
                ans_part.append(i)
                s += chr(ord('A')+i) + "." + str(ans_part[0]) + "  "
            else:
                s += chr(ord('A')+i) + "."
                # while true:
                if self.bool_negative and self.bool_decimal:
                    tmp = uniform(-self.int_len, self.int_len)
                    s += str(float('%.1f' % tmp)) + "  "
                elif self.bool_negative and not self.bool_decimal:
                    s += str(randint(-self.int_len, self.int_len)) + "  "
                elif not self.bool_negative and self.bool_decimal:
                    tmp = uniform(0, self.int_len)
                    s += str(float('%.1f' % tmp)) + "  "
                elif not self.bool_negative and not self.bool_decimal:
                    s += str(randint(0, self.int_len)) + "  "
        self.standard_ans.append(ans_part)
        self.problems[index-1].setText(_translate("MainWindow", s))
        self.user_problems += s + '\n' + 'Correct answer:' + \
            chr(ord('A')+right_ans) + '\n'

        buttonGroup = QButtonGroup(self.gridLayoutWidget_2)
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

    def add_judge(self, index):
        s = QBlibc.py_generate().decode()
        ans_part = []
        if self.bool_decimal:
            tmp = QBlibc.py_calculate(bytes(s, encoding='utf-8'))
            ans_part.append(float('%.1f' % tmp))
        else:
            ans_part.append(
                int(QBlibc.py_calculate(bytes(s, encoding='utf-8'))))
        right_ans = randint(0, 1) % 4
        if right_ans:
            ans_part.append(1)
            s += '=' + str(ans_part[0])
        else:
            ans_part.append(0)
            s += '='
            wrong_ans = ''
            while True:
                if self.bool_negative and self.bool_decimal:
                    tmp = uniform(-self.int_len, self.int_len)
                    wrong_ans = str(float('%.1f' % tmp))
                elif self.bool_negative and not self.bool_decimal:
                    wrong_ans = str(randint(-self.int_len, self.int_len))
                elif not self.bool_negative and self.bool_decimal:
                    tmp = uniform(0, self.int_len)
                    wrong_ans = str(float('%.1f' % tmp))
                elif not self.bool_negative and not self.bool_decimal:
                    wrong_ans = str(randint(0, self.int_len))
                if wrong_ans != ans_part[0]:
                    break
            s += wrong_ans
        self.standard_ans.append(ans_part)
        self.problems[index-1].setText(_translate("MainWindow", s))
        self.user_problems += s + '\n' + ('√' if right_ans else '×') + '\n'

        buttonGroup = QButtonGroup(self.gridLayoutWidget_2)
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
        self.user_analysis = ""
        score = 0
        j = 0
        for i in self.user_ans:
            if len(i) == 4:  # choice
                if i[self.standard_ans[j][1]].isChecked():
                    score += 1
                else:
                    self.user_analysis += 'Wrong in ' + \
                        str(j+1) + ",the corret answer is " + \
                        chr(ord('A')+self.standard_ans[j][1]) + "\n"
            elif len(i) == 2:  # judge
                if i[self.standard_ans[j][1]].isChecked():
                    self.user_analysis += 'Wrong in ' + \
                        str(j+1) + ",the corret answer is " + \
                        ('√' if self.standard_ans[j][1] else '×') + "\n"
                else:
                    score += 1
            elif len(i) == 1:  # blank
                if self.bool_decimal:
                    if float(i[0].text())-self.standard_ans[j] < 0.1:
                        score += 1
                    else:
                        self.user_analysis += 'Wrong in ' + \
                            str(j+1) + ",the corret answer is " + \
                            str(self.standard_ans[j]) + "\n"
                else:
                    if int(i[0].text()) == self.standard_ans[j]:
                        score += 1
                    else:
                        self.user_analysis += 'Wrong in ' + \
                            str(j+1) + ",the corret answer is " + \
                            str(self.standard_ans[j]) + "\n"
            j += 1
        self.user_analysis += 'Your get ' + str(score)
        self.analysis.setText(_translate("MainWindow", self.user_analysis))

    def clear_all(self):
        self.user_ans.clear()
        self.user_problems = ""
        self.user_analysis = ""
        self.standard_ans.clear()
        for i in self.ans:
            while i.count():
                i.takeAt(0).widget().deleteLater()
        for i in self.problems:
            i.setText(_translate("MainWindow", ""))
        self.analysis.setText(_translate("MainWindow", ""))


if __name__ == "__main__":
    app = QApplication(sys.argv)
    QBui = QBWindows()
    try:
        QBui.show()
    except:
        pass
    finally:
        sys.exit(app.exec())
