#include "Formulator.h"

MultipleFunctionElement::MultipleFunctionElement() {
    this->setFunctionType(FLAG_MULTIPLY);

}

string MultipleFunctionElement::toString() {
    return StringManupulator::getDoubleArgs(this->arguments[0], this->arguments[1], OP_MULIPLY);
}


double MultipleFunctionElement::evaluate() {
    FormulaElement *arg1 = this->arguments[0];
    FormulaElement *arg2 = this->arguments[1];
    int arg1_type = arg1->getElementType();
    int arg2_type = arg2->getElementType();

    // arg1 = Constant		arg2 = Constant
    if(arg1_type == FLAG_CONSTANT && arg2_type == FLAG_CONSTANT) {
        ConstantElement *conObj1 = dynamic_cast<ConstantElement*>(arg1);
        ConstantElement *conObj2 = dynamic_cast<ConstantElement*>(arg2);
        return conObj1->evaluate() * conObj2->evaluate();
    }

    // arg1 = Constant		arg2 = Variable
    if(arg1_type == FLAG_CONSTANT && arg2_type == FLAG_VARIABLE) {
        ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg1);
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg2);
        return conObj->evaluate() * varObj->evaluate();
    }

    // arg1 = Constant		arg2 = Function
    if(arg1_type == FLAG_CONSTANT && arg2_type == FLAG_FUNCTION) {
        ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg1);
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg2);
        return conObj->evaluate() * FunctionEvaluator::evaluateFunction(funObj);
    }

    // arg1 = Variable		arg2 = Variable
    if(arg1_type == FLAG_VARIABLE && arg2_type == FLAG_VARIABLE) {
        VariableElement *varObj1 = dynamic_cast<VariableElement*>(arg1);
        VariableElement *varObj2 = dynamic_cast<VariableElement*>(arg2);
        return varObj1->evaluate() * varObj2->evaluate();
    }

    // arg1 = Variable		arg2 = Constant
    if(arg1_type == FLAG_VARIABLE && arg2_type == FLAG_CONSTANT) {
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg1);
        ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg2);
        return varObj->evaluate() * conObj->evaluate();
    }

    // arg1 = Variable		arg2 = Function
    if(arg1_type == FLAG_VARIABLE && arg2_type == FLAG_FUNCTION) {
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg1);
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg2);
        return varObj->evaluate() * FunctionEvaluator::evaluateFunction(funObj);
    }

    // arg1 = Function		arg2 = Function
    if(arg1_type == FLAG_FUNCTION && arg2_type == FLAG_FUNCTION) {
        FunctionElement *funObj1 = dynamic_cast<FunctionElement*>(arg1);
        FunctionElement *funObj2 = dynamic_cast<FunctionElement*>(arg2);
        return FunctionEvaluator::evaluateFunction(funObj1) * FunctionEvaluator::evaluateFunction(funObj2);
    }

    // arg1 = Function		arg2 = Constant
    if(arg1_type == FLAG_FUNCTION && arg2_type == FLAG_CONSTANT) {
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg1);
        ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg2);
        return FunctionEvaluator::evaluateFunction(funObj) * conObj->evaluate();
    }

    // arg1 = Function		arg2 = Variable
    if(arg1_type == FLAG_FUNCTION && arg2_type == FLAG_VARIABLE) {
        FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg1);
        VariableElement *varObj = dynamic_cast<VariableElement*>(arg2);
        return FunctionEvaluator::evaluateFunction(funObj) * varObj->evaluate();

    }
}

