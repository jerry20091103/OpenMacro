#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include "App/configmacroform.h"
#include "App/presetmenu.h"
#include "Core/preset.h"
#include <QListWidget>
#include <QSpinBox>

class InputList;
class ConfigMacroForm;

class CommandList : public QListWidget
{
    Q_OBJECT
    InputList* inputList;
    ConfigMacroForm* configMacroForm;
    QWidget* actionNewCommand;
    QWidget* actionDeleteSelectedCommand;
    QSpinBox* delayInput;
public:
    CommandList(QWidget *parent = nullptr);
    void injectDependencies(InputList* inputList, ConfigMacroForm* configMacroForm, QWidget* actionNewCommand, QWidget* actionDeleteSelectedCommand, QSpinBox* delayInput);
    void updateCommandList(const std::vector<MacroPacket> &commands);
    void setDirty(bool newDirty);
    void disable();
    MacroPacket &getCurrentCommand();
public slots:
    void onNewCommand();
    void onDeleteCommand();
};

#endif // COMMANDLIST_H
