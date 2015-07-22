#include "Formulator.h"

ACosineFunctionElement::ACosineFunctionElement() {
    this->setFunctionType(FLAG_ACOSINE);
}


string ACosineFunctionElement::toString() {
    return StringManupulator::getSingleArg(this->arguments[0], OP_ACOSINE);
}


// throws an exception if number of aruments are greater than 1
void ACosineFunctionElement::addArgument(FormulaElement *e) {
    if(this->arguments.empty() == true)
        this->arguments.push_back(e);
    else
        throw std::string("Cannot enter more than one argument");
}


double ACosineFunctionElement::evaluate() {
    FormulaElement *arg = this->arguments[0];
    int arg_type = arg->getElementType();

    // arg = Constant
    if(arg_type == FLAG_CONSTANT) {
        ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg);
        return acos(conObj->evaluate());
    }

    // arg = Variable
    if(arg_type == FLAG_VARIABLE) {
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg);
        return acos(varObj->evaluate());
    }

    // arg = Function
    if(arg_type == FLAG_FUNCTION) {
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg);
        return acos(FunctionEvaluator::evaluateFunction(funObj));
    }
}

void ACosineFunctionElement::setVariableValue(std::string name, double value) {
    FormulaElement *arg = dynamic_cast<FormulaElement*>(this->arguments[0]);
    int arg_type = arg->getElementType();

    // arg = Constant
    if(arg_type == FLAG_CONSTANT) {
        return;
    }

    // arg = Variable
    if(arg_type == FLAG_VARIABLE) {
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg);
        if(varObj->getName() == name)
            varObj->setValue(value);
        return;
    }

    // arg = Function
    if(arg_type == FLAG_FUNCTION) {
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg);
        FunctionVariableSetter::setVariableValue(funObj, name, value);
        return;
    }
}

