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
    QRegularExpressionValidator passwordValidator;
public:
    explicit PasswordDialog(QWidget *parent = nullptr, PresetDialog *presetDialog = nullptr);
    ~PasswordDialog();

private:
    Ui::PasswordDialog *ui;
    QLineEdit* pwSlots[MAX_PASSWORDS];
    void closeEvent(QCloseEvent *event) override;
};

#endif // PASSWORDDIALOG_H
