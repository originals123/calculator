#include "Formulator.h"
#include "string"
#include "typeinfo"
using namespace std;

FunctionElement::FunctionElement()
{
    this->setElementType(FLAG_FUNCTION);

}

//adds a new argument instance
void FunctionElement::addArgument(FormulaElement *arg)
{
    arguments.push_back(arg);
}

//returns a list of existing arguments
vector<FormulaElement*> FunctionElement::returnArgument()
{
    return arguments;
}

void FunctionElement::setFunctionType(int type) {
    this->function_type = type;
}

int FunctionElement::getFunctionType() {
    return this->function_type;
}

void FunctionElement::setVariableValue(string name, double value) {
    FormulaElement *arg1 = dynamic_cast<FormulaElement*>(this->arguments[0]);
    FormulaElement *arg2 = dynamic_cast<FormulaElement*>(this->arguments[1]);
    int arg1_type = arg1->getElementType();
    int arg2_type = arg2->getElementType();

    // arg1 = Constant		arg2 = Constant
    if(arg1_type == FLAG_CONSTANT && arg2_type == FLAG_CONSTANT) {
        return;
    }

    // arg1 = Constant		arg2 = Variable
    if(arg1_type == FLAG_CONSTANT && arg2_type == FLAG_VARIABLE) {
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg2);
        if(varObj->getName() == name)
            varObj->setValue(value);
        return;
    }

    // arg1 = Constant		arg2 = Function
    if(arg1_type == FLAG_CONSTANT && arg2_type == FLAG_FUNCTION) {
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg2);
        FunctionVariableSetter::setVariableValue(funObj, name, value);
        return;
    }

    // arg1 = Variable		arg2 = Variable
    if(arg1_type == FLAG_VARIABLE && arg2_type == FLAG_VARIABLE) {
        VariableElement *varObj1 = dynamic_cast<VariableElement*>(arg1);
        VariableElement *varObj2 = dynamic_cast<VariableElement*>(arg2);
        if(varObj1->getName() == name)
            varObj1->setValue(value);
        if(varObj2->getName() == name)
            varObj2->setValue(value);
        return;
    }

    // arg1 = Variable		arg2 = Constant
    if(arg1_type == FLAG_VARIABLE && arg2_type == FLAG_CONSTANT) {
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg1);
        if(varObj->getName() == name)
            varObj->setValue(value);
        return;
    }

    // arg1 = Variable		arg2 = Function
    if(arg1_type == FLAG_VARIABLE && arg2_type == FLAG_FUNCTION) {
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg1);
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg2);
        if(varObj->getName() == name)
            varObj->setValue(value);
        FunctionVariableSetter::setVariableValue(funObj, name, value);
        return;
    }

    // arg1 = Function		arg2 = Function
    if(arg1_type == FLAG_FUNCTION && arg2_type == FLAG_FUNCTION) {
        FunctionElement *funObj1 = dynamic_cast<FunctionElement*>(arg1);
        FunctionElement *funObj2 = dynamic_cast<FunctionElement*>(arg2);
        FunctionVariableSetter::setVariableValue(funObj1, name, value);
        FunctionVariableSetter::setVariableValue(funObj2, name, value);
        return;
    }

    // arg1 = Function		arg2 = Constant
    if(arg1_type == FLAG_FUNCTION && arg2_type == FLAG_CONSTANT) {
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg1);
        FunctionVariableSetter::setVariableValue(funObj, name, value);
        return;
    }

    // arg1 = Function		arg2 = Variable
    if(arg1_type == FLAG_FUNCTION && arg2_type == FLAG_VARIABLE) {
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg1);
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg2);
        FunctionVariableSetter::setVariableValue(funObj, name, value);
        if(varObj->getName() == name)
            varObj->setValue(value);
        return;
    }
}

