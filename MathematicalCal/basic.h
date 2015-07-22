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

    void on_btndot_clicked();

    void on_pushButton_37_clicked();

    void on_btnbck_clicked();

    void on_btnclr_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_36_clicked();

    void on_btnpcn_clicked();

    void on_BasicCal_clicked();

    void on_ScientificCal_clicked();

    void on_pushButton_43_clicked();

    void on_btnlbrkt_clicked();

    void on_btnrbrkt_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_32_clicked();

private:
Ui::Basic *ui;
};

#endif // BASIC_H
