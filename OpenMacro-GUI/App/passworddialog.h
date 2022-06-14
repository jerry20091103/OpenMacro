#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "commons.h"
#include "presetdialog.h"

class PresetDialog;

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = nullptr, PresetDialog *presetDialog = nullptr);
    ~PasswordDialog();

private:
    Ui::PasswordDialog *ui;
    QLineEdit* pwSlots[MAX_PASSWORDS];
};

#endif // PASSWORDDIALOG_H
