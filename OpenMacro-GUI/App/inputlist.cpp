#include "inputlist.h"

InputList::InputList(QWidget *parent) : QListWidget(parent) {

}

void InputList::injectDependencies(PresetMenu* presetMenu, QPushButton *newAction, QPushButton *deleteAction)
{
    this->presetMenu = presetMenu;
    this->newAction = newAction;
    this->deleteAction = deleteAction;
    updateButtonStates();
}

void InputList::updateButtonStates()
{
    newAction->setEnabled(!presetMenu->activePresetHasMaxNumOfInputs() && presetMenu->hasActivePreset());
    deleteAction->setEnabled(!presetMenu->activePresetHasMinNumOfInputs() && presetMenu->hasActivePreset());
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

}
