#include "configmacroform.h"

ConfigMacroForm::ConfigMacroForm(QWidget *parent) : QGroupBox(parent)
{

}

void ConfigMacroForm::injectDependencies(CommandList *commandList)
{
    this->commandList = commandList;
}
