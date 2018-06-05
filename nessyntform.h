#ifndef NESSYNTFORM_H
#define NESSYNTFORM_H

#include <QDialog>
#include "nessynth.h"

namespace Ui {
class NesSyntForm;
}

class NesSyntForm : public QDialog
{
    Q_OBJECT

public:
    explicit NesSyntForm(QWidget *parent = 0);
    ~NesSyntForm();
    NesSynth synt;

private:
    Ui::NesSyntForm *ui;
};

#endif // NESSYNTFORM_H
