#ifndef CONFIGMACROFORM_H
#define CONFIGMACROFORM_H

#include "App/commandlist.h"
#include <QGroupBox>
#include <QSpinBox>
#include <QToolButton>
#include <QComboBox>
#include <QMetaEnum>
#include "../commons.h"

class CommandList;

class ConfigMacroForm : public QGroupBox
{
    Q_OBJECT
    CommandList* commandList;
    QSpinBox* mouseXInput;
    QSpinBox* mouseYInput;
    QSpinBox* mouseScrollInput;
    QComboBox* inputModeInput;
    QComboBox* mouseButtonInput;
    QComboBox *keyInput;
    QComboBox *modifierInput;
public:
    ConfigMacroForm(QWidget* parent);
    void injectDependencies(CommandList* commandList,
                            QSpinBox* mouseXInput,
                            QSpinBox* mouseYInput,
                            QSpinBox* mouseScrollInput,
                            QComboBox *inputModeInput,
                            QComboBox *mouseButtonInput,
                            QComboBox* keyInput,
                            QComboBox* modifierInput);
    void updateForm(const MacroPacket& packet);
    void updateVisibility();
    void disable();
private:
    void initKeyInput(QComboBox *keyInput);
    void initModifierInput(QComboBox *modifierInput);
    void initMouseButtonInput(QComboBox *mouseButtonInput);
};

#endif // CONFIGMACROFORM_H
