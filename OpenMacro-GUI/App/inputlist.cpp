#include "inputlist.h"

InputList::InputList(QWidget *parent) : QListWidget(parent) {

}

void InputList::injectDependencies(PresetMenu* presetMenu, QPushButton *newAction, QPushButton *deleteAction, CommandList *commandList)
{
    this->presetMenu = presetMenu;
    this->newAction = newAction;
    this->deleteAction = deleteAction;
    this->commandList = commandList;
    updateButtonStates();
}

void InputList::updateButtonStates()
{
    newAction->setEnabled(!presetMenu->activePresetHasMaxNumOfInputs() && presetMenu->hasActivePreset());
    deleteAction->setEnabled(!presetMenu->activePresetHasMinNumOfInputs() && presetMenu->hasActivePreset());
}

void InputList::onNewPreset(const Preset &preset)
{
    clear();
    for(int idx = 0; idx < preset.getInputs().size(); ++idx){
        addItem("Input " + QString::number(idx));
    }
    updateButtonStates();
}

void InputList::setDirty(bool newDirty)
{
    this->presetMenu->setDirty(newDirty);
}

void InputList::onNewInput()
{
    qDebug() << "onNewInput";
    addItem("Input " + QString::number(count()));
    presetMenu->addNewInput();
    updateButtonStates();
}

void InputList::onDeleteSelected()
{
    qDebug() << "onDeleteSelected";
    QListWidgetItem* itemToDelete = item(count() - 1);
    removeItemWidget(itemToDelete);
    delete itemToDelete;
    presetMenu->removeInputSlot();
    updateButtonStates();
}

void InputList::onSelectChanged()
{
    qDebug() << "onSelectChanged";
    this->commandList->disable();
    this->commandList->updateCommandList(
                getCurrentInput()
                        .packets);
}

Preset::Input& InputList::getCurrentInput(){
    return this->presetMenu->activePreset
                .getInputs()[this->currentRow()];
}

