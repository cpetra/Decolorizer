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
void Dialog::displayImage(const Mat *pmat, QLabel *pwnd)
{
    QRect r;
    QImage img;
    r = pwnd->geometry();
    img = QImage((uchar*)pmat->data, pmat->cols, pmat->rows, pmat->step, QImage::Format_RGB888);
    QPixmap pm = QPixmap::fromImage(img);
    pwnd->setPixmap(pm.scaled(r.width(), r.height(), Qt::KeepAspectRatio));
}

bool Dialog::initDecolor(QString filename)
{
    p_decolor = new Decolor(filename.toStdString(), "My window");
    if (!p_decolor->ok()) {
        return false;
    }

    ui->hs_blursize->setValue(p_decolor->getBlur());
    ui->hs_contour_width->setValue(p_decolor->getContourWidth());
    ui->hs_line_width->setValue(p_decolor->getLineWidth());
    ui->hs_high_edge->setValue(p_decolor->getHighEdge());
    ui->hs_low_edge->setValue(p_decolor->getLowEdge());

    // get and display images
    const Mat *p_orig, *p_proc;
    p_decolor->getImages(&p_orig, &p_proc);

    displayImage(p_orig, ui->wnd_original);
    displayImage(p_proc, ui->wnd_processed);
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

    p_decolor->setBlur(value);
    p_decolor->update();
    showProcessed();


}
void Dialog::showProcessed()
{
    if(!p_decolor) {
        return;
    }
    // get and display output image
    const Mat *p_proc;
    p_decolor->getOutputImage(&p_proc);
    displayImage(p_proc, ui->wnd_processed);
}

void Dialog::on_hs_contour_width_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }
    p_decolor->setContourWidth(value);
    p_decolor->update();
    showProcessed();
}

void Dialog::on_hs_line_width_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }
    p_decolor->setLineWidth(value);
    p_decolor->update();
    showProcessed();
}

void Dialog::on_btn_show_output_clicked()
{
    if(!p_decolor) {
        return;
    }
    p_decolor->displayOutput();
}


void Dialog::on_hs_low_edge_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }
   p_decolor->setLowEdge(value);
   p_decolor->update();
   showProcessed();

}


void Dialog::on_hs_high_edge_valueChanged(int value)
{
    if(!p_decolor) {
        return;
    }
    p_decolor->setHighEdge(value);
    p_decolor->update();
    showProcessed();

}


void Dialog::on_btn_save_file_clicked()
{
    if(!p_decolor) {
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", ".jpg");
    if (filename != "") {
        p_decolor->save(filename.toStdString());
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
        const Mat *p_proc;
        p_decolor->getOutputImage(&p_proc);
        QImage processed = QImage((uchar*)p_proc->data, p_proc->cols, p_proc->rows, p_proc->step, QImage::Format_RGB888);
        QPixmap pm_processed = QPixmap::fromImage(processed);
        QPainter painter;
        painter.begin(&printer);
        painter.drawImage(0, 0, pm_processed.toImage());
        painter.end();
    }
    delete dialog;
}


void Dialog::on_cb_gaussian_toggled(bool checked)
{
    if(!p_decolor) {
        return;
    }
    p_decolor->setGaussian(checked);
    p_decolor->update();
    showProcessed();
}

