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
    QToolButton* inputModeInput;
    QToolButton* mouseButtonInput;
    QComboBox *keyInput;
    QComboBox *modifierInput;
    QMetaEnum keys = QMetaEnum::fromType<InoInterface::KeyboardKeycode>();
    QMetaEnum modifiers = QMetaEnum::fromType<Qt::KeyboardModifier>();
public:
    ConfigMacroForm(QWidget* parent);
    void injectDependencies(CommandList* commandList,
                            QSpinBox* mouseXInput,
                            QSpinBox* mouseYInput,
                            QSpinBox* mouseScrollInput,
                            QToolButton* inputModeInput,
                            QToolButton* mouseButtonInput,
                            QComboBox* keyInput,
                            QComboBox* modifierInput);

public slots:
    void onUpdateMacro();

private:
    void initKeyInput(QComboBox *keyInput);
    void initModifierInput(QComboBox *modifierInput);
};

#endif // CONFIGMACROFORM_H
