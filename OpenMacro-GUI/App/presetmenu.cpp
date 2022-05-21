#include "presetmenu.h"
#include <QFileDialog>

static const char* noFileSelectedText = "No file selected. Go to Presets > Create New Preset to start.";
static const char* fileSelectedText = "Current file: ";

PresetMenu::PresetMenu(QMenuBar* parent) : QMenu(parent)
{
    // ! IMPORTANT !
    // We can't do initialization here because this node and the children nodes aren't ready yet
}

void PresetMenu::injectDependencies(QLabel* fileNameLabel, QAction* saveAction){
    qDebug() << "PresetMenu init";
    this->fileNameLabel = fileNameLabel;
    this->saveAction = saveAction;
    saveAction->setDisabled(true);
    fileNameLabel->setText(noFileSelectedText);
}

bool PresetMenu::hasActivePreset() const { return !activeFileName.isEmpty(); }

void PresetMenu::updateFileNameLabel() const
{
    if(!hasActivePreset())
        fileNameLabel->setText(noFileSelectedText);
    else fileNameLabel->setText(fileSelectedText + activeFileName);
}

void PresetMenu::onCreateNewPreset()
{
    qDebug() << "onCreateNewPreset";
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QString fileName = dialog.getSaveFileName(
                this,
                "Create a new preset file", "/untitled", "Macro Presets (*" + Preset::fileExtension + ")");
    // Empty -> user cancels -> silently return
    if(fileName.isEmpty()) return;
    activeFileName = fileName;
    saveAction->setEnabled(true);
    updateFileNameLabel();
    // Reset to get a new Preset
    activePreset = Preset();
    activePreset.saveAs(activeFileName); // Save on create.


}

void PresetMenu::onLoadPreset()
{
    qDebug() << "onLoadPreset";
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QString fileName = dialog.getOpenFileName(
                this,
                "Load an existing preset file", "/untitled", "Macro Presets (*" + Preset::fileExtension + ")");
    // Empty -> user cancels -> silently return
    if(fileName.isEmpty()) return;
    activeFileName = fileName;
    saveAction->setEnabled(true);
    updateFileNameLabel();

}

void PresetMenu::onSavePreset()
{
    qDebug() << "onSavePreset";
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QString fileName = dialog.getSaveFileName(
                this,
                "Save the current preset file", "/untitled", "Macro Presets (*" + Preset::fileExtension + ")");
    // Empty -> user cancels -> silently return
    if(fileName.isEmpty()) return;
    activeFileName = fileName;
    saveAction->setEnabled(true);
    updateFileNameLabel();
}
