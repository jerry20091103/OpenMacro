#ifndef PRESETMENU_H
#define PRESETMENU_H

#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include "Core/preset.h"
#include "App/inputlist.h"

class InputList;

class PresetMenu : public QMenu
{
    Q_OBJECT
private:
    QString activeFileName = "";
    Preset activePreset;
    QLabel* fileNameLabel;
    QAction* saveAction;
    QAction* saveAutoAction;
    InputList* inputList;
    bool dirty;
public:
    PresetMenu(QMenuBar*);
    void injectDependencies(QLabel* fileNameLabel, QAction* saveAction, QAction* saveAutoAction, InputList* inputList);
    bool hasActivePreset() const;
    bool activePresetHasMaxNumOfInputs() const;
    bool activePresetHasMinNumOfInputs() const;
    void updateFileNameLabel() const;
    void updateListItems();
    void addNewInput();
    void removeInputSlot();
    void savePreset(bool askForPath);
    bool getDirty() const;
    void setDirty(bool newDirty);

public slots:
    void onCreateNewPreset();
    void onLoadPreset();
    void onSavePreset();
    void onSavePresetAuto();
};

#endif // PRESETMENU_H
