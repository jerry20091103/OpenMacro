#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include "App/configmacroform.h"
#include "App/presetmenu.h"
#include "Core/preset.h"
#include <QListWidget>

class InputList;
class ConfigMacroForm;

class CommandList : public QListWidget
{
    Q_OBJECT
    InputList* inputList;
    ConfigMacroForm* configMacroForm;
    QWidget* actionNewCommand;
    QWidget* actionDeleteSelectedCommand;
public:
    CommandList(QWidget *parent = nullptr);
    void injectDependencies(InputList* inputList, ConfigMacroForm* configMacroForm, QWidget* actionNewCommand, QWidget* actionDeleteSelectedCommand);
    void updateCommandList(const std::vector<MacroPacket> &commands);
    void setDirty(bool newDirty);
    MacroPacket &getCurrentCommand();
public slots:
    void onNewCommand();
    void onDeleteCommand();
};

#endif // COMMANDLIST_H
