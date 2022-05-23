#ifndef CONFIGMACROFORM_H
#define CONFIGMACROFORM_H

#include "App/commandlist.h"
#include <QGroupBox>

class CommandList;

class ConfigMacroForm : public QGroupBox
{
    Q_OBJECT
    CommandList* commandList;
public:
    ConfigMacroForm(QWidget* parent);
    void injectDependencies(CommandList* commandList);
};

#endif // CONFIGMACROFORM_H
