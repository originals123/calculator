#include "Formulator.h"

//sets the name of variable to x
VariableElement::VariableElement(string x) {
    this->x = x;
    this->setElementType(FLAG_VARIABLE);
    this->is_grounded = false;

}

//returns the variable name
string VariableElement::getName() {
    return x;
}

//sets the value of variable
void VariableElement::setValue(int value) {
    this->value = value;
    this->is_grounded = true;
}

//returns the value of variable
int VariableElement::getValue() {
    return value;
}

string VariableElement::toString() {
    return x;
}

double VariableElement::evaluate() {
    if(this->is_grounded == false)
        throw string("Variable Is Not Grounded");
    return this->value;
}

