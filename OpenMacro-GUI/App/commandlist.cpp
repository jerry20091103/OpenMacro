#include "commandlist.h"

CommandList::CommandList(QWidget *parent) : QListWidget(parent)
{
    connect(this, &CommandList::currentRowChanged, this, [&](int currentRow){
        if(currentRow < 0) return;
        this->configMacroForm->updateForm(this->getCurrentCommand());
    });
}

void CommandList::injectDependencies(InputList *inputList, ConfigMacroForm *configMacroForm, QWidget *actionNewCommand, QWidget *actionDeleteSelectedCommand)
{
    this->inputList = inputList;
    this->configMacroForm = configMacroForm;
    this->actionNewCommand = actionNewCommand;
    this->actionDeleteSelectedCommand = actionDeleteSelectedCommand;
    this->actionNewCommand->setEnabled(false);
    this->actionDeleteSelectedCommand->setEnabled(false);
}

void CommandList::updateCommandList(const std::vector<MacroPacket>& commands)
{
    this->clear();
    int idx = 0;
    foreach(const MacroPacket& command, commands){
        this->addItem(QString::number(idx++));
    }
    this->actionNewCommand->setEnabled(true);
    this->actionDeleteSelectedCommand->setEnabled(idx != 0);
}

void CommandList::setDirty(bool newDirty)
{
    this->inputList->setDirty(newDirty);
}

MacroPacket& CommandList::getCurrentCommand() {
    return this->inputList->getCurrentInput().packets[this->currentRow()];
}

void CommandList::onNewCommand()
{
    this->inputList->getCurrentInput().packets.push_back(MacroPacket());
    this->addItem(QString::number(this->count()));
    setDirty(true);
    this->actionDeleteSelectedCommand->setEnabled(true);
}

void CommandList::onDeleteCommand()
{
    this->inputList->getCurrentInput().packets.pop_back();
    this->takeItem(this->currentRow());
    setDirty(true);
    this->actionDeleteSelectedCommand->setEnabled(this->count() != 0);
}
