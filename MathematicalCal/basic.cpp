#include "basic.h"
#include "ui_basic.h"
#include "Formulator.h"
#include "QInputDialog"
#include "QDir"
#include "QTextBlock"
#include "QMessageBox"

Basic::Basic(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Basic)
{
    ui->setupUi(this);
    ui->textEdit->setText("0");
       ui->textEdit->moveCursor(QTextCursor::End);
}

Basic::~Basic()
{
    delete ui;
}


double sumInMemory = 0.0;
double sumSoFar = 0.0;
double factorSoFar = 0.0;
bool waitingForOperand = true;
bool clickedPlus=false;
bool clickedMinus=false;
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

            return_vec.push_back(token);continue;
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
    if(s == "tan") {
        return new TanFunctionElement;
    }
    if(s == "acos") {
        return new ACosineFunctionElement;
    }
    if(s == "asin") {
        return new ASineFunctionElement;
    }
    if(s == "atan") {
        return new ATanFunctionElement;
    }
    if(s == "sqrt") {
        return new SqrtFunctionElement;
    }
    if(s == "fact") {
        return new FactFunctionElement;
    }
    if(FormulaElement::isVariable(s)) {
        return new VariableElement(s);
    }
    /*if(FormulaElement::isNumber(s)) {
        return new ConstantElement(stod(s));
    }*/
    if(FormulaElement::isNumber(s)) {
        if(s[0] == '~')
            s[0] = '-';
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
string printResult(FormulaElement *funObj) {

    int type = funObj->getElementType();

    switch(type) {
    case FLAG_CONSTANT: {
        ConstantElement* conObj = dynamic_cast<ConstantElement*>(funObj);
         return conObj->toString();
        break;
    }
    case FLAG_VARIABLE: {
        VariableElement* varObj = dynamic_cast<VariableElement*>(funObj);
          return varObj->toString();
        break;
    }
    case FLAG_FUNCTION: {
        FunctionElement* funObj1 = dynamic_cast<FunctionElement*>(funObj);
        int func_type = funObj1->getFunctionType();
        switch(func_type) {
        case FLAG_PLUS: {
            PlusFunctionElement* plusObj = dynamic_cast<PlusFunctionElement*>(funObj);
             return  plusObj->toString();
            break;
        }
        case FLAG_MINUS: {
            MinusFucntionElement* minusObj = dynamic_cast<MinusFucntionElement*>(funObj);
              return minusObj->toString();
            break;
        }
        case FLAG_MULTIPLY: {
            MultipleFunctionElement* mulObj = dynamic_cast<MultipleFunctionElement*>(funObj);
             return mulObj->toString();
            break;
        }
        case FLAG_DIVIDE: {
            DivideFunctionElement* divObj = dynamic_cast<DivideFunctionElement*>(funObj);
             return divObj->toString();
            break;
        }
        case FLAG_SINE: {
            SineFunctionElement* sinObj = dynamic_cast<SineFunctionElement*>(funObj);
              return sinObj->toString();
            break;
        }
        case FLAG_COSINE: {
            CosineFunctionElement* cosObj = dynamic_cast<CosineFunctionElement*>(funObj);
             return cosObj->toString();
            break;
        }
        case FLAG_TAN: {
            TanFunctionElement* tanObj = dynamic_cast<TanFunctionElement*>(funObj);
             return tanObj->toString();
            break;
        }
        case FLAG_ASINE: {
            ASineFunctionElement* asinObj = dynamic_cast<ASineFunctionElement*>(funObj);
              return asinObj->toString();
            break;
        }
        case FLAG_ACOSINE: {
            ACosineFunctionElement* acosObj = dynamic_cast<ACosineFunctionElement*>(funObj);
             return acosObj->toString();
            break;
        }
        case FLAG_ATAN: {
            ATanFunctionElement* atanObj = dynamic_cast<ATanFunctionElement*>(funObj);
             return atanObj->toString();
            break;
        }
        case FLAG_POWER: {
            PowerFunctionElement* powObj = dynamic_cast<PowerFunctionElement*>(funObj);
          return powObj->toString();
            break;
        }
        case FLAG_SQRT: {
            SqrtFunctionElement* sqrtObj = dynamic_cast<SqrtFunctionElement*>(funObj);
             return sqrtObj->toString();
            break;
        }
        case FLAG_FACT: {
            FactFunctionElement* factObj = dynamic_cast<FactFunctionElement*>(funObj);
             return factObj->toString();
            break;
        }
        }
        break;
    }
    }
}

// Prints the evaluated result
double printEvaluation(FormulaElement *funObj) {
    int type = funObj->getElementType();
    switch(type) {
    case FLAG_CONSTANT: {
        ConstantElement* conObj = dynamic_cast<ConstantElement*>(funObj);
         return conObj->evaluate();
        break;
    }
    case FLAG_VARIABLE: {
        VariableElement* varObj = dynamic_cast<VariableElement*>(funObj);
          return varObj->evaluate();
        break;
    }
    case FLAG_FUNCTION: {
        FunctionElement* funObj1 = dynamic_cast<FunctionElement*>(funObj);
        int ft = funObj1->getFunctionType();
        switch(ft) {
        case FLAG_PLUS: {
            PlusFunctionElement* plusObj = dynamic_cast<PlusFunctionElement*>(funObj);
              return plusObj->evaluate();
            break;
        }
        case FLAG_MINUS: {
            MinusFucntionElement* minusObj = dynamic_cast<MinusFucntionElement*>(funObj);
            return minusObj->evaluate();
            break;
        }
        case FLAG_MULTIPLY: {
            MultipleFunctionElement* mulObj = dynamic_cast<MultipleFunctionElement*>(funObj);
              return mulObj->evaluate();
            break;
        }
        case FLAG_DIVIDE: {
            DivideFunctionElement* divObj = dynamic_cast<DivideFunctionElement*>(funObj);
              return divObj->evaluate();
            break;
        }
        case FLAG_SINE: {
            SineFunctionElement* sinObj = dynamic_cast<SineFunctionElement*>(funObj);
              return sinObj->evaluate();
            break;
        }
        case FLAG_TAN: {
            TanFunctionElement* tanObj = dynamic_cast<TanFunctionElement*>(funObj);
              return tanObj->evaluate();
            break;
        }
        case FLAG_COSINE: {
            CosineFunctionElement* cosObj = dynamic_cast<CosineFunctionElement*>(funObj);
             return cosObj->evaluate();
            break;
        }case FLAG_ASINE: {
            ASineFunctionElement* asinObj = dynamic_cast<ASineFunctionElement*>(funObj);
              return asinObj->evaluate();
            break;
        }
        case FLAG_ATAN: {
            ATanFunctionElement* atanObj = dynamic_cast<ATanFunctionElement*>(funObj);
              return atanObj->evaluate();
            break;
        }
        case FLAG_ACOSINE: {
            ACosineFunctionElement* acosObj = dynamic_cast<ACosineFunctionElement*>(funObj);
             return acosObj->evaluate();
            break;
        }
        case FLAG_POWER: {
            PowerFunctionElement* powObj = dynamic_cast<PowerFunctionElement*>(funObj);
              return powObj->evaluate();
            break;
        }
        case FLAG_SQRT: {
            SqrtFunctionElement* sqrtObj = dynamic_cast<SqrtFunctionElement*>(funObj);
             return sqrtObj->evaluate();
            break;
        }
        case FLAG_FACT: {
            FactFunctionElement* factObj = dynamic_cast<FactFunctionElement*>(funObj);
             return factObj->evaluate();
            break;
        }
        }
        break;
    }
    }
}



void Basic::on_pushButton_clicked()
{

    QString unicodes=ui->textEdit->toPlainText();
    QString unicodesTemp=unicodes;
    //square root modification
    if(unicodes.contains("\u221A",Qt::CaseInsensitive)){
        unicodes.replace(QString("\u221A"), QString("sqrt"));
        ui->textEdit->setText(unicodes);
    }

    //factorial modification
    if(unicodes.contains("!"))
    {
        unicodes.replace(QString("!"), QString(""));
        ui->textEdit->setText("fact("+unicodes+")");
    }
   QString text = ui->textEdit->textCursor().block().text().trimmed().toLatin1();
   if(text.contains("(",Qt::CaseInsensitive) && !text.contains(")",Qt::CaseInsensitive))
   {
       QMessageBox messageBox;
       messageBox.critical(0,"Error","Braces are not closed properly !");
       messageBox.setFixedSize(500,200);
       ui->textEdit->setText(ui->textEdit->toPlainText()+")");
   }
   ui->textEdit->setText(unicodesTemp);
   string exp=text.toStdString();

    vector<string> tokens;
    vector<string> fixed;
    vector<FormulaElement*> elements;
    FormulaElement* f;


    tokens = tokenize(modifyFormula(exp));
    fixed=infixToPostfix(tokens);

    elements=getObjectTokens(fixed);
    for(int i=0;i<tokens.size();i++){
        cout<<tokens.at(i)<<endl;

    }

    f = representFormula(fixed, elements);

    ui->textEdit->append(QString::fromStdString(printResult(f))+"\n");

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

        bool ok;
            QInputDialog* inputDialog = new QInputDialog();
            inputDialog->setOptions(QInputDialog::NoButtons);

            string head="Enter value for "+vars[i];

            QString text =  inputDialog->getText(NULL ,head.c_str(), "Value", QLineEdit::Normal, "Enter Here", &ok);

             if (ok && !text.isEmpty())
             {
                 func->setVariableValue(vars[i], text.toDouble());

             }

       // ui->textEdit->append(QString::fromStdString("Enter value for "+vars[i]+"  :"));

       // temp= ui->textEdit->toPlainText().toDouble();


       // func->setVariableValue(vars[i], temp);
      //  ui->textEdit->append(QString::fromStdString("\n"));

    }
    //Prints the result

    ui->textEdit->append(QString::number(printEvaluation(func)));

    //set cursor position to last line
    ui->textEdit->setFocus();
    ui->textEdit->moveCursor (QTextCursor::End);



}

void Basic::on_btn1_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("1");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"1");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btn2_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("2");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"2");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btnplus_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+"+");
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Basic::on_btn3_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("3");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"3");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btn4_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("4");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"4");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btn5_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("5");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"5");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btn6_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("6");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"6");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btn7_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("7");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"7");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btn8_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("8");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"8");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btn9_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("9");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"9");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btnminus_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+"-");
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Basic::on_btnmul_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+"*");
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Basic::on_btndiv_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+"/");
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Basic::on_btn0_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("0");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"0");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btndot_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+".");
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Basic::on_pushButton_37_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+"~");
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Basic::on_btnbck_clicked()
{
    QString text = ui->textEdit->toPlainText();
         text.chop(1);
         if (text.isEmpty()) {
             text = "0";
         }
         ui->textEdit->setText(text);
   // ui->textEdit->setText(ui->textEdit->toPlainText()+".");
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Basic::on_btnclr_clicked()
{
    ui->textEdit->setText("0");
    sumSoFar=0.0;
    clickedMinus=false;
    clickedPlus=false;
}

void Basic::on_pushButton_39_clicked()
{

    if(clickedPlus==false){
    QString text = ui->textEdit->textCursor().block().text().trimmed().toLatin1();
    string exp=text.toStdString();
    sumSoFar=stod(exp);
    clickedPlus=true;
    }else{
        clickedPlus=true;
        ui->textEdit->setText(ui->textEdit->toPlainText()+"+"+QString::number(sumSoFar));
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_40_clicked()
{
    if(clickedMinus==false){
    QString text = ui->textEdit->textCursor().block().text().trimmed().toLatin1();
    string exp=text.toStdString();
    sumSoFar=stod(exp);
    clickedMinus=true;
    }else{
        clickedMinus=true;
        ui->textEdit->setText(ui->textEdit->toPlainText()+"-"+QString::number(sumSoFar));
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_36_clicked()
{
    sumSoFar=0.0;
    clickedMinus=false;
    clickedPlus=false;
}

void Basic::on_btnpcn_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+QString::number(sumSoFar));
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Basic::on_BasicCal_clicked()
{


}

void Basic::on_ScientificCal_clicked()
{


}

void Basic::on_pushButton_43_clicked()
{

}

void Basic::on_btnlbrkt_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("(");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"(");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btnrbrkt_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText(")");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+")");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_24_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("sin");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"sin");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_26_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("cos");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"cos");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_23_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("tan");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"tan");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_31_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("asin");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"asin");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_33_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("acos");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"acos");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_32_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("atan");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"atan");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_17_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
        ui->textEdit->setText("\u221A");
        ui->textEdit->moveCursor (QTextCursor::End);
        ui->textEdit->insertPlainText("(");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"\u221A"+"(");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_29_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("!");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"!");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_28_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("10^");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"10^");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_35_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("^2");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"^2");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_27_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("^");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"^");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_pushButton_30_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("^3");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"^3");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}

void Basic::on_btnpi_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    if(str=="0"){
    ui->textEdit->setText("3.14159");
    ui->textEdit->moveCursor(QTextCursor::End);
    }else{
        ui->textEdit->setText(ui->textEdit->toPlainText()+"3.14159");
        ui->textEdit->moveCursor(QTextCursor::End);
    }
}
