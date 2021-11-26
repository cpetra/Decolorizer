#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include "Decolor.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
    Decolor *p_decolor;
public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void showEvent(QShowEvent *);

private slots:
    void on_btn_open_file_clicked();

    void on_hs_blursize_valueChanged(int value);

    void on_hs_contour_width_valueChanged(int value);


    void on_hs_line_width_valueChanged(int value);

    void on_btn_show_output_clicked();

    void on_hs_low_edge_valueChanged(int value);

    void on_hs_high_edge_valueChanged(int value);

    void on_btn_save_file_clicked();

    void on_btn_print_clicked();

    void on_cb_gaussian_toggled(bool checked);

private:
    Ui::Dialog *ui;
    void showProcessed();
    void displayImage(const Mat *pmat, QLabel *pwnd);
    bool initDecolor(QString filename);
};
#endif // DIALOG_H
