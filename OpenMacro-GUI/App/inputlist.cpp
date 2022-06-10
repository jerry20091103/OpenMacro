#include "inputlist.h"
#include "commons.h"

InputList::InputList(QWidget *parent) : QListWidget(parent), addressValidator(0, UINT8_MAX, this) {
}

void InputList::injectDependencies(PresetMenu* presetMenu, QPushButton *newAction, QPushButton *deleteAction, CommandList *commandList, QLineEdit *addressInput, QWidget *addressForm, QLabel *addressLabel)
{
    this->presetMenu = presetMenu;
    this->newAction = newAction;
    this->deleteAction = deleteAction;
    this->commandList = commandList;
    this->addressInput = addressInput;
    this->addressForm = addressForm;
    this->addressLabel = addressLabel;
    this->addressInput->setValidator(&addressValidator);
    connect(this->addressInput, &QLineEdit::textChanged, this, [&](const QString& newAddrText){
        uint8_t newAddr = static_cast<uint8_t>(newAddrText.toInt());
        int curAddrId = (this->currentRow() - NUM_BTN_INPUTS) / 16;

        if(this->presetMenu->activePreset.expanderAddr[curAddrId] != newAddr){
            setDirty(true);
        }
        this->presetMenu->activePreset.expanderAddr[curAddrId] = newAddr;
    });
    this->addressForm->setVisible(false);
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
    this->addressForm->setVisible(this->currentRow() >= NUM_BTN_INPUTS);
    int curAddrId = (this->currentRow() - NUM_BTN_INPUTS) / 16;
    if(curAddrId >= 0){
        this->addressLabel->setText("Address " + QString::number(curAddrId));
        this->addressInput->setText(QString::number(this->presetMenu->activePreset.expanderAddr[curAddrId]));
    }
}

Preset::Input& InputList::getCurrentInput(){
    return this->presetMenu->activePreset
                .getInputs()[this->currentRow()];
}

