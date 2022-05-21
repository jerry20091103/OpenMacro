#ifndef PRESETMENU_H
#define PRESETMENU_H

#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include "Core/preset.h"

class PresetMenu : public QMenu
{
    Q_OBJECT
private:
    QString activeFileName = "";
    Preset activePreset;
    QLabel* fileNameLabel;
    QAction* saveAction;
public:
    PresetMenu(QMenuBar*);
    void injectDependencies(QLabel* fileNameLabel, QAction* saveAction);
    bool hasActivePreset() const;
    void updateFileNameLabel() const;
public slots:
    void onCreateNewPreset();
    void onLoadPreset();
    void onSavePreset();
};

#endif // PRESETMENU_H
