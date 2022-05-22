#ifndef INPUTLIST_H
#define INPUTLIST_H

#include <QListWidget>
#include <QPushButton>
#include "./presetmenu.h"

class PresetMenu;

class InputList : public QListWidget
{
    Q_OBJECT
    PresetMenu* presetMenu;
    QPushButton* newAction;
    QPushButton* deleteAction;
public:
    InputList(QWidget *parent = nullptr);
    void injectDependencies(PresetMenu* presetMenu, QPushButton* newAction, QPushButton* deleteAction);
    void updateButtonStates();
public slots:
    void onNewInput();
    void onDeleteSelected();
    void onSelectChanged();
};

#endif // INPUTLIST_H
