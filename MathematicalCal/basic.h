#ifndef BASIC_H
#define BASIC_H

#include <QMainWindow>

namespace Ui {
class Basic;
}

class Basic : public QMainWindow
{
    Q_OBJECT

public:
    explicit Basic(QWidget *parent = 0);
    ~Basic();


private slots:
    void on_pushButton_clicked();

    void on_btn1_clicked();

    void on_btn2_clicked();

    void on_btnplus_clicked();

    void on_btn3_clicked();

    void on_btn4_clicked();

    void on_btn5_clicked();

    void on_btn6_clicked();

    void on_btn7_clicked();

    void on_btn8_clicked();

    void on_btn9_clicked();

    void on_btnminus_clicked();

    void on_btnmul_clicked();

    void on_btndiv_clicked();

    void on_btn0_clicked();

private:
Ui::Basic *ui;
};

#endif // BASIC_H
