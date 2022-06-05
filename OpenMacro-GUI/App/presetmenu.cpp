#include "presetmenu.h"
#include <QFileDialog>
static const char* noFileSelectedText = "No file selected. Go to Presets > Create New Preset to start.";
static const char* fileSelectedText = "Current file: ";

bool PresetMenu::getDirty() const
{
    return dirty;
}

void PresetMenu::setDirty(bool newDirty)
{
    dirty = newDirty;
    saveAutoAction->setEnabled(dirty);
    updateFileNameLabel();
}

PresetMenu::PresetMenu(QMenuBar* parent) : QMenu(parent)
{
    // ! IMPORTANT !
    // We can't do initialization here because this node and the children nodes aren't ready yet
}

void PresetMenu::injectDependencies(QLabel* fileNameLabel, QAction* saveAction, QAction* saveAutoAction, InputList *inputList){
    qDebug() << "PresetMenu init";
    this->fileNameLabel = fileNameLabel;
    this->saveAction = saveAction;
    this->saveAutoAction = saveAutoAction;
    this->inputList = inputList;
    saveAction->setDisabled(true);
    setDirty(false);

}

bool PresetMenu::hasActivePreset() const { return !activeFileName.isEmpty(); }

bool PresetMenu::activePresetHasMaxNumOfInputs() const
{
    return false; // TODO: Decide a maximum number here!
}

bool PresetMenu::activePresetHasMinNumOfInputs() const
{
    return activePreset.getInputs().size() <= NUM_BTN_INPUTS;
}

void PresetMenu::updateFileNameLabel() const
{
    if(!hasActivePreset())
        fileNameLabel->setText(noFileSelectedText);
    else if(dirty) fileNameLabel->setText(fileSelectedText + activeFileName + " (*)");
    else fileNameLabel->setText(fileSelectedText + activeFileName);

}

void PresetMenu::addNewInput()
{
    activePreset.addInput();
    setDirty(true);
}

void PresetMenu::removeInputSlot()
{
    activePreset.popInput();
    setDirty(true);
}

void PresetMenu::savePreset(bool askForPath)
{
    EXCEPT_ALERT(
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        QString fileName = !askForPath ? activeFileName : dialog.getSaveFileName(
                    this,
                    "Save the current preset file", "/untitled", "Macro Presets (*" + Preset::fileExtension + ")");
        // Empty -> user cancels -> silently return
        if(fileName.isEmpty()) return;
        activePreset.saveAs(fileName);
        activeFileName = fileName;
        saveAction->setEnabled(true);
        setDirty(false);
    );
}

void PresetMenu::onCreateNewPreset()
{
    qDebug() << "onCreateNewPreset";
    EXCEPT_ALERT(
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        QString fileName = dialog.getSaveFileName(
                    this,
                    "Create a new preset file", "/untitled", "Macro Presets (*" + Preset::fileExtension + ")");
        // Empty -> user cancels -> silently return
        if(fileName.isEmpty()) return;
        activeFileName = fileName;
        saveAction->setEnabled(true);
        saveAutoAction->setEnabled(true);
        setDirty(false);
        // Reset to get a new Preset
        activePreset = Preset();
        activePreset.saveAs(activeFileName); // Save on create.
        this->inputList->onNewPreset(activePreset);
    );
}

void PresetMenu::onLoadPreset()
{
    qDebug() << "onLoadPreset";
    EXCEPT_ALERT(
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        QString fileName = dialog.getOpenFileName(
                    this,
                    "Load an existing preset file", "/untitled", "Macro Presets (*" + Preset::fileExtension + ")");
        // Empty -> user cancels -> silently return
        if(fileName.isEmpty()) return;
        activePreset.readFrom(fileName);
        activeFileName = fileName;
        saveAction->setEnabled(true);
        setDirty(false);
        this->inputList->onNewPreset(activePreset);
    );
}

void PresetMenu::onSavePreset()
{
    qDebug() << "onSavePreset";
    savePreset(true);
}

void PresetMenu::onSavePresetAuto()
{
    qDebug() << "onSavePresetAuto";
    savePreset(false);
}

void PresetMenu::downloadPreset(QSerialPort& serialPort)
{
    activePreset.loadFromSerial(serialPort);
    activeFileName = serialPort.portName() + ".json";
    setDirty(true);
}

void PresetMenu::uploadPreset(QSerialPort& serialPort)
{
    activePreset.uploadToSerial(serialPort);
}
