#ifndef INPUTLIST_H
#define INPUTLIST_H

#include <QListWidget>
#include <QPushButton>
#include "./presetmenu.h"
#include "./commandlist.h"
class PresetMenu;
class CommandList;

class InputList : public QListWidget
{
    Q_OBJECT
    PresetMenu* presetMenu;
    CommandList* commandList;
    QPushButton* newAction;
    QPushButton* deleteAction;
public:
    InputList(QWidget *parent = nullptr);
    void injectDependencies(PresetMenu* presetMenu, QPushButton* newAction, QPushButton* deleteAction, CommandList* commandList);
    void updateButtonStates();
    void onNewPreset(const Preset& preset);
    void setDirty(bool newDirty);
    Preset::Input &getCurrentInput();
public slots:
    void onNewInput();
    void onDeleteSelected();
    void onSelectChanged();
};

#endif // INPUTLIST_H
