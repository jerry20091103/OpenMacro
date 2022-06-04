#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include "App/configmacroform.h"
#include "App/presetmenu.h"
#include <QListWidget>

class InputList;
class ConfigMacroForm;

class CommandList : public QListWidget
{
    Q_OBJECT
    InputList* inputList;
    ConfigMacroForm* configMacroForm;
public:
    CommandList(QWidget *parent = nullptr);
    void injectDependencies(InputList* inputList, ConfigMacroForm* configMacroForm);
    void updateCurrentCommand();
public slots:
    void onNewCommand();
    void onDeleteCommand();
};

#endif // COMMANDLIST_H
