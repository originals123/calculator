#-------------------------------------------------
#
# Project created by QtCreator 2015-07-19T15:56:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MathematicalCal
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        basic.cpp \
    VariableElement.cpp \
    SineFunctionElement.cpp \
    PowerFunctionElement.cpp \
    PlusFunctionElement.cpp \
    MultipleFunctionElement.cpp \
    MinusFucntionElement.cpp \
    FunctionElement.cpp \
    FormulaElement.cpp \
    DivideFunctionElement.cpp \
    CosineFunctionElement.cpp \
    ConstantElement.cpp \
    TanFunctionElement.cpp \
    ASineFunctionElement.cpp \
    ACosineFunctionElement.cpp \
    ATanFunctionElement.cpp \
    SqrtFunctionElement.cpp \
    FactFunctionElement.cpp

HEADERS  += basic.h \
    Formulator.h \
    Functions.h

FORMS    += basic.ui
