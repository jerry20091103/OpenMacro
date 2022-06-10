#include "commandlist.h"

CommandList::CommandList(QWidget *parent) : QListWidget(parent)
{
    connect(this, &CommandList::currentRowChanged, this, [&](int currentRow){
        if(currentRow < 0) return;
        this->configMacroForm->updateForm(this->getCurrentCommand());
    });
}

void CommandList::injectDependencies(InputList *inputList, ConfigMacroForm *configMacroForm, QWidget *actionNewCommand, QWidget *actionDeleteSelectedCommand, QSpinBox *delayInput)
{
    this->inputList = inputList;
    this->configMacroForm = configMacroForm;
    this->actionNewCommand = actionNewCommand;
    this->actionDeleteSelectedCommand = actionDeleteSelectedCommand;
    this->actionNewCommand->setEnabled(false);
    this->actionDeleteSelectedCommand->setEnabled(false);
    this->delayInput = delayInput;
    connect(delayInput, &QSpinBox::valueChanged, this, [&](int value) {
        setDirty(this->inputList->getCurrentInput().delay != value);
        this->inputList->getCurrentInput().delay = value;
    });
    this->delayInput->setEnabled(false);
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
    this->delayInput->setEnabled(true);
    this->delayInput->setMinimum(0);
    this->delayInput->setMaximum(UINT16_MAX);
    this->delayInput->setValue(this->inputList->getCurrentInput().delay);
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
