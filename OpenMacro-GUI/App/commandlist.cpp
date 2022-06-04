#include "commandlist.h"

CommandList::CommandList(QWidget *parent) : QListWidget(parent)
{

}

void CommandList::injectDependencies(InputList *inputList, ConfigMacroForm *configMacroForm)
{
    this->inputList = inputList;
    this->configMacroForm = configMacroForm;
}

void CommandList::updateCurrentCommand()
{
    
}

void CommandList::onNewCommand()
{
    
}

void CommandList::onDeleteCommand()
{
    
}
