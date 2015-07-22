#ifndef FORMULATOR_H
#define FORMULATOR_H

#endif // FORMULATOR_H
#ifndef FORMULATOR_FORMULATOR_H_
#define FORMULATOR_FORMULATOR_H_
#define PLUSFUNCTIONELEMENT_H

#include "vector"
#include "iostream"
#include "string"
#include "sstream"
#include "stdlib.h"
#include "cstring"
#include "algorithm"
#include "iterator"
#include "math.h"
#include "set"

#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <stack>
#include <queue>
#include <exception>
#include <string>
#include <sstream>

// element flags
#define FLAG_CONSTANT   1
#define FLAG_VARIABLE   2
#define FLAG_FUNCTION   3

// function flags
#define FLAG_PLUS      1
#define FLAG_MINUS     2
#define FLAG_MULTIPLY  3
#define FLAG_DIVIDE    4
#define FLAG_SINE      5
#define FLAG_COSINE    6
#define FLAG_POWER     7
#define FLAG_TAN       8
#define FLAG_ASINE      9
#define FLAG_ACOSINE      10
#define FLAG_ATAN      11

// operation flags
#define OP_PLUS     1
#define OP_MINUS    2
#define OP_DIVIDE   3
#define OP_MULIPLY  4
#define OP_SINE     5
#define OP_COSINE   6
#define OP_POWER	7
#define OP_TAN      8
#define OP_ASINE    9
#define OP_ACOSINE  10
#define OP_ATAN     11

// associativity flags
#define LEFT	1
#define RIGHT	2
#define PI 3.14159
#include <cmath>
using namespace std;

// holds operator information
struct OperatorInfo {
    std::string operator_name;
    short arguments;
    short precedence;
    short association;
};


//declares the Base class content
class FormulaElement {
protected:
    int element_type;
    bool is_grounded;

    //sets the type of a particular element
    inline void setElementType(int type)
    {
        this->element_type = type;
    }

public:
    FormulaElement();

    //made toString() virtual to gain access to child's overridden toString()
    virtual string toString() {
        return "";
    }

    //returns the type of a particular element
    inline int getElementType()
    {
        return this->element_type;
    }

    //method to evaluate functionlity.Appears in every subclass for its specific evaluation.
    virtual double evaluate() = 0;


    // returns true if a token is a function
    static bool isFunction(string argument) {
        if(argument == "sin")
            return true;
        if(argument == "cos")
            return true;
        if(argument == "tan")
            return true;
        return false;
    }
    // returns true if a token is a variable
    static bool isVariable(string argument) {
        if((argument.length() == 1 && isalpha(argument[0])) || (argument.length() == 2 && argument[0] == '~' && isalpha(argument[1])))
            return true;
        return false;
    }
    // returns true if a token is a number
    static bool isNumber(string argument) {

        for(int i = 0; i < argument.length(); i++) {
             if(argument[i] == '.')
                continue;
             if(argument[i] == '~')
                continue;

            if(!isdigit(argument[i]))
                return false;
        }
        return true;
    }
    // returns true if a string type token is an operator
    static bool isOperator(string argument) {
        if(argument == "+")
            return true;
        if(argument == "-")
            return true;
        if(argument == "*")
            return true;
        if(argument == "/")
            return true;
        if(argument == "^")
            return true;
        return false;
    }

    // returns true if a char type token is an operator or not
    static bool isOperator(char argument) {
        switch(argument) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            return true;
        }
        return false;
    }

    //if char is paranthesis this method returns true
    static bool isParanthesis(char c) {
        if(c == '(' || c == ')')
            return true;
        return false;
    }

    // returns operator information
    static OperatorInfo getOPInfo(string op) {
        OperatorInfo _operator;
        if(op == "+") {
            _operator.operator_name = "+";
            _operator.precedence = 4;
            _operator.association = LEFT;
            _operator.arguments = 2;
        }
        if(op == "-") {
            _operator.operator_name = "-";
            _operator.precedence = 4;
            _operator.association = LEFT;
            _operator.arguments = 2;
        }
        if(op == "*") {
            _operator.operator_name = "*";
            _operator.precedence = 5;
            _operator.association = LEFT;
            _operator.arguments = 2;
        }
        if(op == "/") {
            _operator.operator_name = "/";
            _operator.precedence = 5;
            _operator.association = LEFT;
            _operator.arguments = 2;
        }
        if(op == "^") {
            _operator.operator_name = "^";
            _operator.precedence = 6;
            _operator.association = RIGHT;
            _operator.arguments = 2;
        }
        if(op == "sin") {
            _operator.operator_name = "sin";
            _operator.arguments = 1;
            _operator.precedence = 0;
        }
        if(op == "cos") {
            _operator.operator_name = "cos";
            _operator.arguments = 1;
            _operator.precedence = 0;
        }
        if(op == "tan") {
            _operator.operator_name = "tan";
            _operator.arguments = 1;
            _operator.precedence = 0;
        }
        if(op == "asin") {
            _operator.operator_name = "asin";
            _operator.arguments = 1;
            _operator.precedence = 0;
        }
        if(op == "acos") {
            _operator.operator_name = "acos";
            _operator.arguments = 1;
            _operator.precedence = 0;
        }
        if(op == "atan") {
            _operator.operator_name = "atan";
            _operator.arguments = 1;
            _operator.precedence = 0;
        }
        if(op == ")" || op == "(") {
            _operator.precedence = 0;
        }
        return _operator;
    }


};

//declares the content of FunctionElement class which is a sub class of FormulaElemet
class FunctionElement: public FormulaElement {
protected:
    vector<FormulaElement*> arguments; //declares a vector to hold FormulaElement type arguments
    int function_type;

    void setFunctionType(int);
public:
    FunctionElement();

    void addArgument(FormulaElement *arg); //adds a new argument instance
    vector<FormulaElement*> returnArgument(); //returns a list of existing arguments

    //made toString() virtual to gain access to child's overridden toString()
    virtual string toString() {
        return "";
    }
    inline int getFunctionType();

    virtual double evaluate() = 0;

    virtual void setVariableValue(std::string, double);//sets the value for a particular variable
};

//declares the content of ConstantElement class which is a sub class of FormulaElemet
class ConstantElement: public FormulaElement {
protected:
    double constant; //holds the value of the constant
public:
    ConstantElement(double constant); //sets the value to constant
    double getConstant(); //returns the value of constant
    string toString();
    virtual double evaluate();

};

//declares the content of VariableElement class which is a sub class of FormulaElemet
class VariableElement: public FormulaElement {
protected:
    string x; //name of variable
    int value; //value of variable
public:
    VariableElement(string x); //sets the name of variable to x
    string getName(); //returns the variable name
    void setValue(int value); //sets the value of variable
    int getValue(); //returns the value of variable
    string toString();
    virtual double evaluate();
};

//declares the content of PlusFunctionElement class which is a sub class of FunctionElemet
class PlusFunctionElement: public FunctionElement {
public:
    PlusFunctionElement();
    string toString();
    virtual double evaluate();
};

//declares the content of MinusFunctionElement class which is a sub class of FunctionElemet
class MinusFucntionElement: public FunctionElement {
public:
    MinusFucntionElement();
    string toString();
    virtual double evaluate();
};

//declares the content of MultipleFunctionElement class which is a sub class of FunctionElemet
class MultipleFunctionElement: public FunctionElement {
public:
    MultipleFunctionElement();
    string toString();
    virtual double evaluate();

};

//declares the content of DivideFunctionElement class which is a sub class of FunctionElemet
class DivideFunctionElement: public FunctionElement {
public:
    DivideFunctionElement();
    string toString();
    virtual double evaluate();
};

//declares the content of CosineFunctionElement class which is a sub class of FunctionElemet
class CosineFunctionElement: public FunctionElement {
public:
    CosineFunctionElement();
    string toString();
    virtual double evaluate();

    virtual void addArgument(FormulaElement*);

    virtual void setVariableValue(std::string, double);

};

//declares the content of CosineFunctionElement class which is a sub class of FunctionElemet
class SineFunctionElement: public FunctionElement {
public:
    SineFunctionElement();
    string toString();
    virtual double evaluate();
    virtual void addArgument(FormulaElement*);

    virtual void setVariableValue(std::string, double);
};

//declares the content of TanFunctionElement class which is a sub class of FunctionElemet
class TanFunctionElement: public FunctionElement {
public:
    TanFunctionElement();
    string toString();
    virtual double evaluate();
    virtual void addArgument(FormulaElement*);

    virtual void setVariableValue(std::string, double);
};
//declares the content of ACosineFunctionElement class which is a sub class of FunctionElemet
class ACosineFunctionElement: public FunctionElement {
public:
    ACosineFunctionElement();
    string toString();
    virtual double evaluate();

    virtual void addArgument(FormulaElement*);

    virtual void setVariableValue(std::string, double);

};

//declares the content of ACosineFunctionElement class which is a sub class of FunctionElemet
class ASineFunctionElement: public FunctionElement {
public:
    ASineFunctionElement();
    string toString();
    virtual double evaluate();
    virtual void addArgument(FormulaElement*);

    virtual void setVariableValue(std::string, double);
};

//declares the content of ATanFunctionElement class which is a sub class of FunctionElemet
class ATanFunctionElement: public FunctionElement {
public:
    ATanFunctionElement();
    string toString();
    virtual double evaluate();
    virtual void addArgument(FormulaElement*);

    virtual void setVariableValue(std::string, double);
};

//declares the content of PowerFunctionElement class which is a sub class of FunctionElemet
class PowerFunctionElement: public FunctionElement {
protected:
    double base;
    double power;
public:
    PowerFunctionElement();

    //return power as a string
    string toString();
    virtual double evaluate();
};

class StringManupulator {
public:
    static string getSingleArg(FormulaElement *arg, int operation) {
        int flag = arg->getElementType();
        ostringstream out;

        // if argument is a function
        if(flag == FLAG_FUNCTION) {
            FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg);
            switch(operation) {
            case OP_SINE:
                out << "sin(" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_COSINE:
                out << "cos(" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_TAN:
                out << "tan(" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_ASINE:
                out << "asin(" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_ACOSINE:
                out << "acos(" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_ATAN:
                out << "atan(" << StringManupulator::getStringEx(funObj) << ")";
                break;
            }
            return out.str();
        }

        // if argument is a constant
        if(flag == FLAG_CONSTANT) {
            ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg);
            switch(operation) {
            case OP_SINE:
                out << "sin(" << conObj->getConstant() << ")";
                break;
            case OP_COSINE:
                out << "cos(" << conObj->getConstant() << ")";
                break;
            case OP_TAN:
                out << "tan(" << conObj->getConstant() << ")";
                break;
            case OP_ASINE:
                out << "asin(" << conObj->getConstant() << ")";
                break;
            case OP_ACOSINE:
                out << "acos(" << conObj->getConstant() << ")";
                break;
            case OP_ATAN:
                out << "atan(" << conObj->getConstant() << ")";
                break;
            }
            return out.str();
        }

        // if argument is a variable
        if(flag == FLAG_VARIABLE) {
            VariableElement *varObj = dynamic_cast<VariableElement*>(arg);
            switch(operation) {
            case OP_SINE:
                out << "sin(" << varObj->getName() << ")";
                break;
            case OP_COSINE:
                out << "cos(" << varObj->getName() << ")";
                break;
            case OP_TAN:
                out << "tan(" << varObj->getName() << ")";
                break;
            case OP_ASINE:
                out << "asin(" << varObj->getName() << ")";
                break;
            case OP_ACOSINE:
                out << "acos(" << varObj->getName() << ")";
                break;
            case OP_ATAN:
                out << "atan(" << varObj->getName() << ")";
                break;
            }
            return out.str();
        }


        return NULL;
    }//end of getSingleArg()

    static string getDoubleArgs(FormulaElement *arg1, FormulaElement *arg2, int operation) {

        int flag1 = arg1->getElementType();
        int flag2 = arg2->getElementType();
        ostringstream out;

        //When both arguments are constants
        if(flag1 == FLAG_CONSTANT && flag2 == FLAG_CONSTANT) {
            ConstantElement *conObj1 = dynamic_cast<ConstantElement*>(arg1);
            ConstantElement *conObj2 = dynamic_cast<ConstantElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << conObj1->getConstant() << " + " << conObj2->getConstant();
                break;
            case OP_MINUS:
                out << conObj1->getConstant() << " - " << conObj2->getConstant();
                break;
            case OP_MULIPLY:
                out << (conObj1->getConstant() * conObj2->getConstant());
                break;
            case OP_DIVIDE:
                out << conObj1->getConstant() << " / " << conObj2->getConstant();
                break;
            case OP_POWER:
                out << conObj1->getConstant() << " ^ " << conObj2->getConstant();
                break;
            }
            return out.str();
        }

        // When there is a contant and a variable
        if(flag1 == FLAG_CONSTANT && flag2 == FLAG_VARIABLE) {
            ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg1);
            VariableElement *varObj = dynamic_cast<VariableElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << conObj->getConstant() << " + " << varObj->getName();
                break;
            case OP_MINUS:
                out << conObj->getConstant() << " - " << varObj->getName();
                break;
            case OP_MULIPLY:
                out << conObj->getConstant() << varObj->getName();
                break;
            case OP_DIVIDE:
                out << conObj->getConstant() << " / " << varObj->getName();
                break;
            case OP_POWER:
                out << conObj->getConstant() << " ^ " << varObj->getName();
                break;
            }
            return out.str();
        }

        // When there is a constant and a function
        if(flag1 == FLAG_CONSTANT && flag2 == FLAG_FUNCTION) {
            ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg1);
            FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << conObj->getConstant() << " + (" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_MINUS:
                out << conObj->getConstant() << " - (" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_MULIPLY:
                out << conObj->getConstant() << "(" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_DIVIDE:
                out << conObj->getConstant() << " / (" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_POWER:
                out << conObj->getConstant() << " ^ (" << StringManupulator::getStringEx(funObj) <<  ")";
                break;
            }
            return out.str();
        }

        // When both arguments are variables
        if(flag1 == FLAG_VARIABLE && flag2 == FLAG_VARIABLE) {
            VariableElement *varObj1 = dynamic_cast<VariableElement*>(arg1);
            VariableElement *varObj2 = dynamic_cast<VariableElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << varObj1->getName() << " + " << varObj2->getName();
                break;
            case OP_MINUS:
                out << varObj1->getName() << " - " << varObj2->getName();
                break;
            case OP_MULIPLY:
                out << varObj1->getName() << " * " << varObj2->getName();
                break;
            case OP_DIVIDE:
                out << varObj1->getName() << " / " << varObj2->getName();
                break;
            case OP_POWER:
                out << varObj1->getName() << " ^ " << varObj2->getName();
                break;
            }
            return out.str();
        }

        // When one argument is a variable and other is a constant
        if(flag1 == FLAG_VARIABLE && flag2 == FLAG_CONSTANT) {
            VariableElement *varObj = dynamic_cast<VariableElement*>(arg1);
            ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << varObj->getName() << " + " << conObj->getConstant();
                break;
            case OP_MINUS:
                out << varObj->getName() << " - " << conObj->getConstant();
                break;
            case OP_MULIPLY:
                out << conObj->getConstant() << varObj->getName();
                break;
            case OP_DIVIDE:
                out << varObj->getName() << " / " << conObj->getConstant();
                break;
            case OP_POWER:
                out << varObj->getName() << " ^ " << conObj->getConstant();
                break;
            }
            return out.str();
        }

        // When one argument is a variable and other is a function
        if(flag1 == FLAG_VARIABLE && flag2 == FLAG_FUNCTION) {
            VariableElement *varObj = dynamic_cast<VariableElement*>(arg1);
            FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << varObj->getName() << " + (" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_MINUS:
                out << varObj->getName() << " - (" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_MULIPLY:
                out << "(" << StringManupulator::getStringEx(funObj) << ")" << varObj->getName();
                break;
            case OP_DIVIDE:
                out << varObj->getName() << " / (" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_POWER:
                out << varObj->getName() << " ^ (" << StringManupulator::getStringEx(funObj) << ")";
                break;
            }
            return out.str();
        }

        // When both are functions
        if(flag1 == FLAG_FUNCTION && flag2 == FLAG_FUNCTION) {
            FunctionElement *funObj1 = dynamic_cast<FunctionElement*>(arg1);
            FunctionElement *funObj2 = dynamic_cast<FunctionElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << "(" << StringManupulator::getStringEx(funObj1) << ") + (" << StringManupulator::getStringEx(funObj2) << ")";
                break;
            case OP_MINUS:
                out << "(" << StringManupulator::getStringEx(funObj1) << ") - (" << StringManupulator::getStringEx(funObj2) << ")";
                break;
            case OP_MULIPLY:
                out << "(" << StringManupulator::getStringEx(funObj1) << ")(" << StringManupulator::getStringEx(funObj2) << ")";
                break;
            case OP_DIVIDE:
                out << "(" << StringManupulator::getStringEx(funObj1) << ") / (" << StringManupulator::getStringEx(funObj2) << ")";
                break;
            case OP_POWER:
                out << "(" << StringManupulator::getStringEx(funObj1) << ") ^ (" << StringManupulator::getStringEx(funObj2) << ")";
                break;
            }
            return out.str();
        }

        // When one argument is a function and other is a constant
        if(flag1 == FLAG_FUNCTION && flag2 == FLAG_CONSTANT) {
            FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg1);
            ConstantElement *conObj = dynamic_cast<ConstantElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << "(" << StringManupulator::getStringEx(funObj) << ") + " << conObj->getConstant();
                break;
            case OP_MINUS:
                out << "(" << StringManupulator::getStringEx(funObj) << ") - " << conObj->getConstant();
                break;
            case OP_MULIPLY:
                out << conObj->getConstant() << "(" << StringManupulator::getStringEx(funObj) << ")";
                break;
            case OP_DIVIDE:
                out << "(" << StringManupulator::getStringEx(funObj) << ") / " << conObj->getConstant();
                break;
            case OP_POWER:
                out << "(" << StringManupulator::getStringEx(funObj) << ") ^ " << conObj->getConstant();
                break;
            }
            return out.str();
        }

        // When one argument is a function and other is a variable
        if(flag1 == FLAG_FUNCTION && flag2 == FLAG_VARIABLE) {
            FunctionElement *funObj = dynamic_cast<FunctionElement*>(arg1);
            VariableElement *varObj = dynamic_cast<VariableElement*>(arg2);
            switch(operation) {
            case OP_PLUS:
                out << "(" << StringManupulator::getStringEx(funObj) << ") + (" << varObj->getName() << ")";
                break;
            case OP_MINUS:
                out << "(" << StringManupulator::getStringEx(funObj) << ") - (" << varObj->getName() << ")";
                break;
            case OP_MULIPLY:
                out << "(" << StringManupulator::getStringEx(funObj) << ")" << varObj->getName();
                break;
            case OP_DIVIDE:
                out << "(" << StringManupulator::getStringEx(funObj) << ") / " << varObj->getName();
                break;
            case OP_POWER:
                out << "(" << StringManupulator::getStringEx(funObj) << ") ^ " << varObj->getName();
                break;
            }
            return out.str();
        }
        return NULL;
    }//end of getDoubleArgs()

    static string getStringEx(FunctionElement *funObj) {
        int func_type = funObj->getFunctionType();

        ostringstream out;
        switch(func_type) {
        case FLAG_PLUS:
        {
            PlusFunctionElement *plusObj = dynamic_cast<PlusFunctionElement*>(funObj);
            out << plusObj->toString();
            return out.str();
        }
        case FLAG_MINUS:
        {
            MinusFucntionElement *minusObj = dynamic_cast<MinusFucntionElement*>(funObj);
            out << minusObj->toString();
            return out.str();
        }
        case FLAG_MULTIPLY:
        {
            MultipleFunctionElement *mulObj = dynamic_cast<MultipleFunctionElement*>(funObj);
            out << mulObj->toString();
            return out.str();
        }
        case FLAG_DIVIDE:
        {
            DivideFunctionElement *divObj = dynamic_cast<DivideFunctionElement*>(funObj);
            out << divObj->toString();
            return out.str();
        }
        case FLAG_SINE:
        {
            SineFunctionElement *sinObj = dynamic_cast<SineFunctionElement*>(funObj);
            out << sinObj->toString();
            return out.str();
        }
        case FLAG_COSINE:
        {
            CosineFunctionElement *cosObj = dynamic_cast<CosineFunctionElement*>(funObj);
            out << cosObj->toString();
            return out.str();
        }
        case FLAG_POWER:
        {
            PowerFunctionElement *powObj = dynamic_cast<PowerFunctionElement*>(funObj);
            out << powObj->toString();
            return out.str();
        }
        case FLAG_TAN:
        {
            TanFunctionElement *tanObj = dynamic_cast<TanFunctionElement*>(funObj);
            out << tanObj->toString();
            return out.str();
        }
        case FLAG_ACOSINE:
        {
            ACosineFunctionElement *acosObj = dynamic_cast<ACosineFunctionElement*>(funObj);
            out << acosObj->toString();
            return out.str();
        }
        case FLAG_ASINE:
        {
            ASineFunctionElement *asinObj = dynamic_cast<ASineFunctionElement*>(funObj);
            out << asinObj->toString();
            return out.str();
        }
        case FLAG_ATAN:
        {
            ATanFunctionElement *atanObj = dynamic_cast<ATanFunctionElement*>(funObj);
            out << atanObj->toString();
            return out.str();
        }
        }
        return NULL;
    }
};
// FunctionEvaluator class
class FunctionEvaluator {
public:
    static double evaluateFunction(FunctionElement *funObj) {
        int func_type = funObj->getFunctionType();

        switch(func_type) {
        case FLAG_PLUS:
        {
            PlusFunctionElement *plusObj = dynamic_cast<PlusFunctionElement*>(funObj);
            return plusObj->evaluate();
        }
        case FLAG_MINUS:
        {
            MinusFucntionElement *minusObj = dynamic_cast<MinusFucntionElement*>(funObj);
            return minusObj->evaluate();
        }
        case FLAG_MULTIPLY:
        {
            MultipleFunctionElement *multObj = dynamic_cast<MultipleFunctionElement*>(funObj);
            return multObj->evaluate();
        }
        case FLAG_DIVIDE:
        {
            DivideFunctionElement *divObj = dynamic_cast<DivideFunctionElement*>(funObj);
            return divObj->evaluate();
        }
        case FLAG_SINE:
        {
            SineFunctionElement *sinObj = dynamic_cast<SineFunctionElement*>(funObj);
            return sinObj->evaluate();
        }
        case FLAG_COSINE:
        {
            CosineFunctionElement *cosObj = dynamic_cast<CosineFunctionElement*>(funObj);
            return cosObj->evaluate();
        }
        case FLAG_TAN:
        {
            TanFunctionElement *tanObj = dynamic_cast<TanFunctionElement*>(funObj);
            return tanObj->evaluate();
        }
        case FLAG_ASINE:
        {
            ASineFunctionElement *asinObj = dynamic_cast<ASineFunctionElement*>(funObj);
            return asinObj->evaluate();
        }
        case FLAG_ACOSINE:
        {
            ACosineFunctionElement *acosObj = dynamic_cast<ACosineFunctionElement*>(funObj);
            return acosObj->evaluate();
        }
        case FLAG_ATAN:
        {
            ATanFunctionElement *atanObj = dynamic_cast<ATanFunctionElement*>(funObj);
            return atanObj->evaluate();
        }
        case FLAG_POWER:
        {
            PowerFunctionElement *powObj = dynamic_cast<PowerFunctionElement*>(funObj);
            return powObj->evaluate();
        }

        }
    }
};
// FunctionVariableSetter class
class FunctionVariableSetter {
public:
    static void setVariableValue(FunctionElement *funObj, string name, double value) {
        int func_type = funObj->getFunctionType();
        switch(func_type) {
        case FLAG_PLUS:
        {
            PlusFunctionElement *plusObj = dynamic_cast<PlusFunctionElement*>(funObj);
            return plusObj->setVariableValue(name, value);
        }
        case FLAG_MINUS:
        {
            MinusFucntionElement *minusObj = dynamic_cast<MinusFucntionElement*>(funObj);
            return minusObj->setVariableValue(name, value);
        }
        case FLAG_MULTIPLY:
        {
            MultipleFunctionElement *mulObj = dynamic_cast<MultipleFunctionElement*>(funObj);
            return mulObj->setVariableValue(name, value);
        }
        case FLAG_DIVIDE:
        {
            DivideFunctionElement *divObj = dynamic_cast<DivideFunctionElement*>(funObj);
            return divObj->setVariableValue(name, value);
        }
        case FLAG_SINE:
        {
            SineFunctionElement *sinObj = dynamic_cast<SineFunctionElement*>(funObj);
            return sinObj->setVariableValue(name, value);
        }
        case FLAG_COSINE:
        {
            CosineFunctionElement *cosObj = dynamic_cast<CosineFunctionElement*>(funObj);
            return cosObj->setVariableValue(name, value);
        }
        case FLAG_TAN:
        {
            TanFunctionElement *tanObj = dynamic_cast<TanFunctionElement*>(funObj);
            return tanObj->setVariableValue(name, value);
        }
        case FLAG_ASINE:
        {
            ASineFunctionElement *asinObj = dynamic_cast<ASineFunctionElement*>(funObj);
            return asinObj->setVariableValue(name, value);
        }
        case FLAG_ACOSINE:
        {
            ACosineFunctionElement *acosObj = dynamic_cast<ACosineFunctionElement*>(funObj);
            return acosObj->setVariableValue(name, value);
        }
        case FLAG_ATAN:
        {
            ATanFunctionElement *atanObj = dynamic_cast<ATanFunctionElement*>(funObj);
            return atanObj->setVariableValue(name, value);
        }
        case FLAG_POWER:
        {
            PowerFunctionElement *powObj = dynamic_cast<PowerFunctionElement*>(funObj);
            return powObj->setVariableValue(name, value);
        }

        }
    }
};

#endif /* FORMULATOR_FORMULATOR_H_ */

