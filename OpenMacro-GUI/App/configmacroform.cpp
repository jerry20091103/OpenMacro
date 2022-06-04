#include "configmacroform.h"

ConfigMacroForm::ConfigMacroForm(QWidget *parent) : QGroupBox(parent)
{

}

void ConfigMacroForm::injectDependencies(CommandList *commandList,
                                         QSpinBox* mouseXInput,
                                         QSpinBox* mouseYInput,
                                         QSpinBox* mouseScrollInput,
                                         QToolButton* inputModeInput,
                                         QToolButton* mouseButtonInput,
                                         QComboBox *keyInput,
                                         QComboBox *modifierInput)
{
    this->commandList = commandList;
    this->mouseXInput = mouseXInput;
    this->mouseYInput = mouseYInput;
    this->mouseScrollInput = mouseScrollInput;
    this->inputModeInput = inputModeInput;
    this->mouseButtonInput = mouseButtonInput;
    this->keyInput = keyInput;
    this->modifierInput = modifierInput;
    connect(mouseXInput, &QSpinBox::valueChanged, this, [&](int newVal){
        qDebug() << "mouseXInput";
    });
    connect(mouseYInput, &QSpinBox::valueChanged, this, [&](int newVal){
        qDebug() << "mouseYInput";
    });
    connect(mouseScrollInput, &QSpinBox::valueChanged, this, [&](int newVal){
        qDebug() << "mouseScrollInput";
    });
    connect(inputModeInput, &QToolButton::triggered, this, [&](QAction* action){
        qDebug() << "inputModeInput" << action->objectName();
    });
    connect(mouseButtonInput, &QToolButton::triggered, this, [&](QAction* action){
        qDebug() << "mouseButtonInput" << action->objectName();
    });
    connect(keyInput, &QComboBox::currentIndexChanged, this, [&](int newIndex){
       qDebug() << "keyInput" << QString::number(newIndex);
       qDebug() << this->keyInput->itemText(newIndex);
       std::string enumKey = this->keyInput->itemText(newIndex).toStdString();
       int inputKey = keys.keyToValue(enumKey.c_str());
       qDebug() << "Keycode:" << (int)inputKey;

    });
    connect(modifierInput, &QComboBox::currentIndexChanged, this, [&](int newIndex){
       qDebug() << "modifierInput" << QString::number(newIndex);
       qDebug() << this->modifierInput->itemText(newIndex);
    });
    initKeyInput(keyInput);
    initModifierInput(modifierInput);
}

void ConfigMacroForm::onUpdateMacro()
{
    // TODO: Update macro
    this->commandList->updateCurrentCommand();
}

void ConfigMacroForm::initKeyInput(QComboBox* keyInput){
    for(int i = 0; i < keys.keyCount(); ++i){
        QString key = keys.key(i);
        keyInput->addItem(key);
    }
}

void ConfigMacroForm::initModifierInput(QComboBox* modifierInput){
    for(int i = 0; i < modifiers.keyCount(); ++i){
        QString key = modifiers.key(i);
        modifierInput->addItem(key);
    }
}
