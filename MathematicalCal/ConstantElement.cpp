

#include "Formulator.h"

//sets the value to constant
ConstantElement::ConstantElement(double constant) {
    this->constant = constant;
    this->FormulaElement::setElementType(FLAG_CONSTANT);
    this->is_grounded = true;

}

//returns the value of constant
double ConstantElement::getConstant() {
    return constant;
}

string ConstantElement::toString() {
    //converts double to string
    std::ostringstream strs;
    strs << constant;
    return strs.str();
}
double ConstantElement::evaluate() {
    return this->constant;
}



