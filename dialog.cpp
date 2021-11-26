#include "Decolor.hpp"
#include "dialog.h"
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    p_decolor = NULL;
}

void Dialog::showEvent(QShowEvent *evt)
{
    if (p_decolor == NULL) {
        if (!initDecolor("d:/download/istockphoto-176087199-612x612.jpg")) {
            QString fileName = QFileDialog::getOpenFileName(this,
             tr("Open Image"), NULL, tr("Image Files (*.png *.jpg *.bmp)"));
            if (fileName != "") {
                initDecolor(fileName);
            }
        }

    }
}
bool Dialog::initDecolor(QString filename)
{
    p_decolor = new Decolor(filename.toStdString(), "My window");
    if (!p_decolor->ok()) {
        return false;
    }

    ui->hs_blursize->setValue(p_decolor->getBlur());
    ui->hs_contour_width->setValue(p_decolor->getContourWidth());
    ui->hs_high_edge->setValue(p_decolor->getHighEdge());
    ui->hs_low_edge->setValue(p_decolor->getLowEdge());

    QImage orig;
    QImage processed;

    QRect r1, r2;
    p_decolor->Display(orig, processed);
    r1 = ui->wnd_original->geometry();
    r2 = ui->wnd_processed->geometry();
    QPixmap pm_original = QPixmap::fromImage(orig);
    QPixmap pm_processed = QPixmap::fromImage(processed);

    ui->wnd_original->setPixmap(pm_original.scaled(r1.width(), r1.height(), Qt::KeepAspectRatio));
    ui->wnd_processed->setPixmap(pm_processed.scaled(r2.width(), r2.height(), Qt::KeepAspectRatio));
    return true;
}

Dialog::~Dialog()
{
    delete ui;
    if(!p_decolor) {
        return;
    }
    delete p_decolor;
}


void Dialog::on_btn_open_file_clicked()
{

   QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Image"), NULL, tr("Image Files (*.png *.jpg *.bmp)"));
   if (fileName != "") {
       delete p_decolor;
       initDecolor(fileName);
   }
}


void Dialog::on_hs_blursize_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }

    p_decolor->SetBlur(value);
    p_decolor->Update();
    showProcessed();


}
void Dialog::showProcessed()
{
    if(!p_decolor) {
        return;
    }
    QImage processed;
    QRect r = ui->wnd_processed->geometry();
    p_decolor->DisplayProcessed(processed);
    QPixmap pm_processed = QPixmap::fromImage(processed);
    ui->wnd_processed->setPixmap(pm_processed.scaled(r.width(), r.height(), Qt::KeepAspectRatio));
}

void Dialog::on_hs_contour_width_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }
    p_decolor->SetContourWidth(value);
    p_decolor->Update();
    showProcessed();
}

void Dialog::on_hs_line_width_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }
    p_decolor->SetLineWidth(value);
    p_decolor->Update();
    showProcessed();
}

void Dialog::on_btn_show_output_clicked()
{
    if(!p_decolor) {
        return;
    }
    p_decolor->DisplayOutput();
}


void Dialog::on_hs_low_edge_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }
   p_decolor->SetLowEdge(value);
   p_decolor->Update();
   showProcessed();

}


void Dialog::on_hs_high_edge_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }
    p_decolor->SetHighEdge(value);
    p_decolor->Update();
    showProcessed();

}


void Dialog::on_btn_save_file_clicked()
{
    if(!p_decolor) {
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", ".jpg");
    if (filename != "") {
        p_decolor->Save(filename.toStdString());
    }
}


void Dialog::on_btn_print_clicked()
{
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    if (!dialog) {
        return;
    }
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() == QDialog::Accepted) {
        QImage processed;
        QRect r = ui->wnd_processed->geometry();
        p_decolor->DisplayProcessed(processed);
        QPixmap pm_processed = QPixmap::fromImage(processed);
        QPainter painter;
        painter.begin(&printer);
        painter.drawImage(0, 0, pm_processed.toImage());
        painter.end();

    }
    delete dialog;
}

