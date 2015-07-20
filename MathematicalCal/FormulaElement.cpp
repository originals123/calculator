#include <QCoreApplication>
#include "Formulator.h"
using namespace std;

FormulaElement::FormulaElement() {
    // TODO Auto-generated constructor stub

}


/*
// modify formula notation
string modifyFormula(string argument) {
    string return_str;
    string spaces;
    char prev_char, cur_char, next_char;
    int i;

    for(i = 0; i < argument.length(); i++) {
        if(!isspace(argument[i]))
            spaces += argument[i];
    }
    for(i = 0; i < spaces.length(); i++) {
        prev_char = spaces[i - 1];
        cur_char = spaces[i];
        next_char = spaces[i + 1];

        // makes 2X = 2 * X and 2( = 2 * (
        if(isalpha(cur_char) || cur_char == '(')
            if(isdigit(prev_char))
                return_str += "*";

        // makes )( == ) * (
        if(cur_char == '(')
            if(prev_char == ')')
                return_str += "*";
        return_str += cur_char;
        // makes )X = ) * X
        if(cur_char == ')')
            if(isalpha(next_char))
                return_str += "*";
    }
    return return_str;
}

// tokenizing formula
vector<string> tokenize(string formula) {

    vector<string> return_vec;
    int i;
    char ch;
    string str_buffer;

    for(i = 0; i < formula.length(); i++) {
        ch = formula[i];
        if(!(FormulaElement::isOperator(ch) || FormulaElement::isParanthesis(ch)))
            str_buffer += ch;
        else {
            if(!str_buffer.empty())
                return_vec.push_back(str_buffer);
            return_vec.push_back(formula.substr(i, 1));
            str_buffer.clear();
        }
    }
    if(!str_buffer.empty())
        return_vec.push_back(str_buffer);
    return return_vec;
}

vector<string> infixToPostfix(vector<string> formula) {
    vector<string> return_vec;
    stack<string> operator_stack;
    int i;
    string token, top;
    bool condition1, condition2;
    OperatorInfo tokenInfo, topInfo;

    for(i = 0; i < formula.size(); i++) {
        token = formula[i];
        if(FormulaElement::isNumber(token) || FormulaElement::isVariable(token)) {
            return_vec.push_back(token); continue;
        }
        if(FormulaElement::isFunction(token)) {
            operator_stack.push(token); continue;
        }
        if(FormulaElement::isOperator(token)) {
            if(!operator_stack.empty()) {
                top = operator_stack.top();
                tokenInfo = FormulaElement::getOPInfo(token);
                topInfo = FormulaElement::getOPInfo(top);
                condition1 = (tokenInfo.association == LEFT && tokenInfo.precedence <= topInfo.precedence);
                condition2 = (tokenInfo.association == RIGHT && tokenInfo.precedence < topInfo.precedence);
                while(condition1 || condition2) {
                    return_vec.push_back(top);
                    operator_stack.pop();
                    if(!operator_stack.empty()) {
                        top = operator_stack.top();
                        tokenInfo = FormulaElement::getOPInfo(token);
                        topInfo = FormulaElement::getOPInfo(top);
                        condition1 = (tokenInfo.association == LEFT && tokenInfo.precedence <= topInfo.precedence);
                        condition2 = (tokenInfo.association == RIGHT && tokenInfo.precedence < topInfo.precedence);
                    } else break;
                }
            }
            operator_stack.push(token);
            continue;
        }
        if(formula[i] == "(") {
            operator_stack.push(token); continue;
        }
        if(formula[i] == ")") {
            top = operator_stack.top();
            while(top != "(") {
                return_vec.push_back(top);
                operator_stack.pop();
                if(!operator_stack.empty())
                    top = operator_stack.top();
                else break;
            }
            operator_stack.pop();
            if(!operator_stack.empty())
                if(FormulaElement::isFunction(operator_stack.top())) {
                    return_vec.push_back(operator_stack.top());
                    operator_stack.pop();
                }
            continue;
        }
    }
    while(!operator_stack.empty()) {
        return_vec.push_back(operator_stack.top());
        operator_stack.pop();
    }
    return return_vec;



}
// get a FormulaElement according to the token
FormulaElement* getElement(string s) {
    if(s == "+") {
        return new PlusFunctionElement;
    }
    if(s == "-") {
        return new MinusFucntionElement;
    }
    if(s == "*") {
        return new MultipleFunctionElement;
    }
    if(s == "/") {
        return new DivideFunctionElement;
    }
    if(s == "^") {
        return new PowerFunctionElement;
    }
    if(s == "cos") {
        return new CosineFunctionElement;
    }
    if(s == "sin") {
        return new SineFunctionElement;
    }
    if(FormulaElement::isVariable(s)) {
        return new VariableElement(s);
    }
    if(FormulaElement::isNumber(s)) {
        return new ConstantElement(stod(s));
    }
    return new ConstantElement(strtod(s.c_str(), NULL));
}

// returns a vector of 'FormulaElement' objects in RPN order
vector<FormulaElement*> getObjectTokens(vector<string> postfix) {
    vector<FormulaElement*> return_vec;
    int i;
    for(i = 0; i < postfix.size(); i++) {
        if(FormulaElement::isNumber(postfix[i]))
            return_vec.push_back(getElement(postfix[i]));
        if(FormulaElement::isVariable(postfix[i]))
            return_vec.push_back(getElement(postfix[i]));
        if(FormulaElement::isOperator(postfix[i]))
            return_vec.push_back(getElement(postfix[i]));
        if(FormulaElement::isFunction(postfix[i]))
            return_vec.push_back(getElement(postfix[i]));
    }
    return return_vec;
}

FormulaElement* representFormula(vector<string> tokens, vector<FormulaElement*> objTokens) {
    stack<FormulaElement*> Obj_buffer1;
    stack<FormulaElement*> Obj_buffer2;
    int i;

    for(i = 0; i < tokens.size(); i++) {
        if(FormulaElement::isNumber(tokens[i]) || FormulaElement::isVariable(tokens[i]))
            Obj_buffer1.push(objTokens[i]);
        else {
            FunctionElement* funObj = dynamic_cast<FunctionElement*>(objTokens[i]);
            OperatorInfo op_info = FormulaElement::getOPInfo(tokens[i]);
            int args = op_info.arguments;
            while(args  > 0) {
                FormulaElement* feObj = dynamic_cast<FormulaElement*>(Obj_buffer1.top());
                Obj_buffer2.push(feObj);
                Obj_buffer1.pop();
                args--;
            }
            while(!Obj_buffer2.empty()) {
                funObj->addArgument(Obj_buffer2.top());
                Obj_buffer2.pop();
            }
            Obj_buffer1.push(funObj);
        }
    }
    return Obj_buffer1.top();
}

// calling 'toString()' methods accordning to type
void printResult(FormulaElement *funObj) {

    int type = funObj->getElementType();

    switch(type) {
    case FLAG_CONSTANT: {
        ConstantElement* conObj = dynamic_cast<ConstantElement*>(funObj);
        cout << "Formula Representation :    " << conObj->toString() << endl;
        break;
    }
    case FLAG_VARIABLE: {
        VariableElement* varObj = dynamic_cast<VariableElement*>(funObj);
        cout <<  "Formula Representation :    "  << varObj->toString() << endl;
        break;
    }
    case FLAG_FUNCTION: {
        FunctionElement* funObj1 = dynamic_cast<FunctionElement*>(funObj);
        int func_type = funObj1->getFunctionType();
        switch(func_type) {
        case FLAG_PLUS: {
            PlusFunctionElement* plusObj = dynamic_cast<PlusFunctionElement*>(funObj);
            cout <<  "Formula Representation :    "  << plusObj->toString() << endl;
            break;
        }
        case FLAG_MINUS: {
            MinusFucntionElement* minusObj = dynamic_cast<MinusFucntionElement*>(funObj);
            cout <<  "Formula Representation :    " << minusObj->toString() << endl;
            break;
        }
        case FLAG_MULTIPLY: {
            MultipleFunctionElement* mulObj = dynamic_cast<MultipleFunctionElement*>(funObj);
            cout <<  "Formula Representation :    " << mulObj->toString() << endl;
            break;
        }
        case FLAG_DIVIDE: {
            DivideFunctionElement* divObj = dynamic_cast<DivideFunctionElement*>(funObj);
            cout <<  "Formula Representation :    "  << divObj->toString() << endl;
            break;
        }
        case FLAG_SINE: {
            SineFunctionElement* sinObj = dynamic_cast<SineFunctionElement*>(funObj);
            cout <<  "Formula Representation :    "  << sinObj->toString() << endl;
            break;
        }
        case FLAG_COSINE: {
            CosineFunctionElement* cosObj = dynamic_cast<CosineFunctionElement*>(funObj);
            cout <<  "Formula Representation :    "  << cosObj->toString() << endl;
            break;
        }
        case FLAG_POWER: {
            PowerFunctionElement* powObj = dynamic_cast<PowerFunctionElement*>(funObj);
            cout <<  "Formula Representation :    "  << powObj->toString() << endl;
            break;
        }
        }
        break;
    }
    }
}

// Prints the evaluated result
void printEvaluation(FormulaElement *funObj) {
    int type = funObj->getElementType();
    switch(type) {
    case FLAG_CONSTANT: {
        ConstantElement* conObj = dynamic_cast<ConstantElement*>(funObj);
        cout << "Result =  "  << conObj->evaluate() << endl;
        break;
    }
    case FLAG_VARIABLE: {
        VariableElement* varObj = dynamic_cast<VariableElement*>(funObj);
        cout << "Result =  "  << varObj->evaluate() << endl;
        break;
    }
    case FLAG_FUNCTION: {
        FunctionElement* funObj1 = dynamic_cast<FunctionElement*>(funObj);
        int ft = funObj1->getFunctionType();
        switch(ft) {
        case FLAG_PLUS: {
            PlusFunctionElement* plusObj = dynamic_cast<PlusFunctionElement*>(funObj);
            cout << "Result =  "  << plusObj->evaluate() << endl;
            break;
        }
        case FLAG_MINUS: {
            MinusFucntionElement* minusObj = dynamic_cast<MinusFucntionElement*>(funObj);
            cout << "Result =  " << minusObj->evaluate() << endl;
            break;
        }
        case FLAG_MULTIPLY: {
            MultipleFunctionElement* mulObj = dynamic_cast<MultipleFunctionElement*>(funObj);
            cout << "Result =  "  << mulObj->evaluate() << endl;
            break;
        }
        case FLAG_DIVIDE: {
            DivideFunctionElement* divObj = dynamic_cast<DivideFunctionElement*>(funObj);
            cout << "Result =  "  << divObj->evaluate() << endl;
            break;
        }
        case FLAG_SINE: {
            SineFunctionElement* sinObj = dynamic_cast<SineFunctionElement*>(funObj);
            cout << "Result =  " << sinObj->evaluate() << endl;
            break;
        }
        case FLAG_COSINE: {
            CosineFunctionElement* cosObj = dynamic_cast<CosineFunctionElement*>(funObj);
            cout << "Result =  "  << cosObj->evaluate() << endl;
            break;
        }
        case FLAG_POWER: {
            PowerFunctionElement* powObj = dynamic_cast<PowerFunctionElement*>(funObj);
            cout << "Result =  " << powObj->evaluate() << endl;
            break;
        }
        }
        break;
    }
    }
}


*/

//main method
/*
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    vector<string> tokens;
    vector<string> fixed;
    vector<FormulaElement*> elements;
    FormulaElement* f;

    tokens = tokenize(modifyFormula("(X+10)*(X+10)*(X+10)"));
    fixed=infixToPostfix(tokens);
    elements=getObjectTokens(fixed);
    for(int i=0;i<tokens.size();i++){
        cout<<tokens.at(i)<<endl;
    }
    f = representFormula(fixed, elements);
    printResult(f);
    FunctionElement *func = dynamic_cast<FunctionElement*>(f);

    vector<string> vars;

    double temp;
    //find the variable tokens in the tokenized expression vector
    for(int i=0;i<tokens.size();i++)
    {
        if(FormulaElement::isVariable(tokens[i]))
        {

            vars.push_back(tokens.at(i));



        }else{
            continue;
        }
    }
    //Remove duplicate variables from vector
    vars.erase( unique( vars.begin(), vars.end() ), vars.end() );

    //iterate through vars vector to set values for variables

    for(int i=0;i<vars.size();i++)
    {
        cout<<"Enter value for "<<vars[i]<<"  :";
        cin >>temp;

        func->setVariableValue(vars[i], temp);

    }
    //Prints the result
    printEvaluation(func);
    cout<<endl;

    /*
    //Test Code Week 2
    //Printing Formula : (X+7)(X–2)(X+1)
    //(X+7)
    VariableElement variableObj1("X");
    ConstantElement constantObj1(7);
    PlusFunctionElement plusObj1;
    plusObj1.addArgument(variableObj1);
    plusObj1.addArgument(constantObj1);

    //(X-2)
    ConstantElement constantObj2(2);
    MinusFucntionElement minusObj;
    minusObj.addArgument(variableObj1);
    minusObj.addArgument(constantObj2);

    //(X+1)
    ConstantElement constantObj3(1);
    PlusFunctionElement plusObj2;
    plusObj2.addArgument(variableObj1);
    plusObj2.addArgument(constantObj3);
    //(X+7)(X–2)(X+1)
    MultipleFunctionElement multipleObj1;
    multipleObj1.addArgument(plusObj1);
    multipleObj1.addArgument(minusObj);
    MultipleFunctionElement multipleObj2;
    multipleObj2.addArgument(multipleObj1);
    multipleObj2.addArgument(plusObj2);
    cout << "Test formula 1 :  " + multipleObj2.toString() << endl;

    //Printing Formula : Y2(X2+2)
    //X2
    VariableElement variableObj2("X");
    MultipleFunctionElement multipleObj3;
    multipleObj3.addArgument(variableObj2);
    multipleObj3.addArgument(variableObj2);

    //(X2+2)
    ConstantElement constantObj4(2);
    PlusFunctionElement plusObj3;
    plusObj3.addArgument(multipleObj3);
    plusObj3.addArgument(constantObj4);

    //Y2
    VariableElement variableObj3("Y");
    MultipleFunctionElement multipleObj4;
    multipleObj4.addArgument(variableObj3);
    multipleObj4.addArgument(variableObj3);

    //Y2(X2+2)
    MultipleFunctionElement multipleObj5;
    multipleObj5.addArgument(multipleObj4);
    multipleObj5.addArgument(plusObj3);
    cout << "Test formula 2 :  " + multipleObj5.toString() << endl;

    //Printing Formula : sin(X2+7)cos(Y2-2)
    PlusFunctionElement plusObj4;
    plusObj4.addArgument(multipleObj3);
    plusObj4.addArgument(constantObj1);

    //sin(X2+7)
    SineFunctionElement sineObj1;
    sineObj1.addArgument(plusObj4);

    //(Y2-2)
    MinusFucntionElement minusObj2;
    minusObj2.addArgument(multipleObj4);
    minusObj2.addArgument(constantObj2);

    //cos(Y2-2)
    CosineFunctionElement cosObj1;
    cosObj1.addArgument(minusObj2);

    //sin(X2+7)cos(Y2-2)
    MultipleFunctionElement multipleObj6;
    multipleObj6.addArgument(sineObj1);
    multipleObj6.addArgument(cosObj1);
    cout << "Test formula 3 :  " + multipleObj6.toString() << endl;

    //when two constants are multiplied
    MultipleFunctionElement multipleObj7;
    ConstantElement constantObj5(10);
    ConstantElement constantObj6(3);
    multipleObj7.addArgument(constantObj5);
    multipleObj7.addArgument(constantObj6);
    cout << "Result is      :  " + multipleObj7.toString();*/

    /*
     //Test code week 1
     //2x-7
     MultipleFunctionElement multipleObj1;
     MinusFucntionElement minusObj1;
     ConstantElement constantObj1(2), constantObj2(7);
     VariableElement varialeObj1("X");
     multipleObj1.addArgument(constantObj1);
     multipleObj1.addArgument(varialeObj1);
     minusObj1.addArgument(multipleObj1);
     minusObj1.addArgument(constantObj2);

     //Y2/(X2+2)
     //Y2
     MultipleFunctionElement multipleObj2;
     VariableElement variableObj2("Y");
     multipleObj2.addArgument(variableObj2);
     multipleObj2.addArgument(variableObj2);

     //X2
     MultipleFunctionElement multipleObj3;
     VariableElement variableObj3("X");
     multipleObj3.addArgument(variableObj3);
     multipleObj3.addArgument(variableObj3);

     //X2+2
     PlusFunctionElement plusObj1;
     ConstantElement constantObj3(2);
     plusObj1.addArgument(multipleObj3);
     plusObj1.addArgument(constantObj3);

     //Y2 / (X2+2)
     DivideFunctionElement divideObj;
     divideObj.addArgument(multipleObj2);
     divideObj.addArgument(plusObj1);

     */
    //return 0;

//}*/


