#include "Formulator.h"

ATanFunctionElement::ATanFunctionElement() {
    this->setFunctionType(FLAG_ATAN);
}

string ATanFunctionElement::toString() {
    return StringManupulator::getSingleArg(this->arguments[0], OP_ATAN);
}



// throws an exception if number of aruments are greater than 1
void ATanFunctionElement::addArgument(FormulaElement *e) {
    if(this->arguments.empty() == true)
        this->arguments.push_back(e);
    else
        throw string("Cannot enter more than one argument");
}


double ATanFunctionElement::evaluate() {
    FormulaElement *arg = this->arguments[0];
    int arg_type = arg->getElementType();

    // arg = ConsATant
    if(arg_type == FLAG_CONSTANT) {
        ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg);
        return atan(conObj->evaluate()) * 180.0 / PI;
    }

    // arg = Variable
    if(arg_type == FLAG_VARIABLE) {
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg);
        return atan(varObj->evaluate()) * 180.0 / PI;
    }

    // arg = Function
    if(arg_type == FLAG_FUNCTION) {
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg);
        return atan(FunctionEvaluator::evaluateFunction(funObj)) * 180.0 / PI;
    }
}

void ATanFunctionElement::setVariableValue(string name, double value) {
    FormulaElement *arg = dynamic_cast<FormulaElement*>(this->arguments[0]);
    int arg_type = arg->getElementType();

    // arg = ConsATant
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



