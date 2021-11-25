#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
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

private:
    Ui::Dialog *ui;
    void showProcessed();
    void initDecolor(QString filename);
};
#endif // DIALOG_H
