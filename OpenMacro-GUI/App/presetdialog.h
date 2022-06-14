#ifndef PRESETDIALOG_H
#define PRESETDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <unordered_set>
#include "App/presetmenu.h"
#include "App/passworddialog.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class PresetDialog;
}
QT_END_NAMESPACE

class PasswordDialog;

class PresetDialog : public QDialog
{
    Q_OBJECT
    QSerialPort serialPort;
    PresetMenu* presetMenu;
    PasswordDialog* passwordDialog;
    QMessageBox* msgBox;
public:
    PresetDialog(QWidget *parent = nullptr, PresetMenu* presetMenu = nullptr);
    ~PresetDialog();
    QSerialPort &getSerialPort();
private:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void setStatus(const QString& text);
    void refreshPortList();
    void refreshState();
    Ui::PresetDialog *ui;
    int refreshTimerId = -1, writeDelayTimerId = -1, readDelayTimerId = -1;
    std::unordered_set<QString> availablePortNames;
};

#endif // PRESETDIALOG_H
