#include "configmacroform.h"

#define SET_DIRTY_IF(clause) if(clause) this->commandList->setDirty(true)

static const QMetaEnum keys = QMetaEnum::fromType<InoInterface::KeyboardKeycode>();
ConfigMacroForm::ConfigMacroForm(QWidget *parent) : QGroupBox(parent)
{

}

void ConfigMacroForm::injectDependencies(CommandList *commandList,
                                         QSpinBox* mouseXInput,
                                         QSpinBox* mouseYInput,
                                         QSpinBox* mouseScrollInput,
                                         QComboBox* inputModeInput,
                                         QComboBox* mouseButtonInput,
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

    // Set input ranges for spinboxes based on their underlying types
    this->mouseXInput->setMinimum(INT16_MIN);
    this->mouseXInput->setMaximum(INT16_MAX);
    this->mouseYInput->setMinimum(INT16_MIN);
    this->mouseYInput->setMaximum(INT16_MAX);
    this->mouseScrollInput->setMinimum(INT16_MIN);
    this->mouseScrollInput->setMaximum(INT16_MAX);

    disable();
    // Use lambdas for simplicity.
    // "this" is used to access member variables (not to be confused with
    // the constructor's arguments (whose scope is just the constructor body.)
    connect(mouseXInput, &QSpinBox::valueChanged, this, [&](int newVal){
        if(!this->isEnabled()) return;
        qDebug() << "mouseXInput";
        SET_DIRTY_IF(this->commandList->getCurrentCommand().mouseMove.mouseX != newVal);
        this->commandList->getCurrentCommand().mouseMove.mouseX = newVal;
    });
    connect(mouseYInput, &QSpinBox::valueChanged, this, [&](int newVal){
        if(!this->isEnabled()) return;
        qDebug() << "mouseYInput";
        SET_DIRTY_IF(this->commandList->getCurrentCommand().mouseMove.mouseY != newVal);
        this->commandList->getCurrentCommand().mouseMove.mouseY = newVal;
    });
    connect(mouseScrollInput, &QSpinBox::valueChanged, this, [&](int newVal){
        if(!this->isEnabled()) return;
        qDebug() << "mouseScrollInput";
        SET_DIRTY_IF(this->commandList->getCurrentCommand().mouseMove.wheel != newVal);
        this->commandList->getCurrentCommand().mouseMove.wheel = newVal;
    });
    connect(inputModeInput, &QComboBox::currentIndexChanged, this, [&](int newIndex){
        if(!this->isEnabled()) return;
        qDebug() << "inputModeInput" << QString::number(newIndex);
        SET_DIRTY_IF(this->commandList->getCurrentCommand().mode != static_cast<MacroMode>(newIndex));
        this->commandList->getCurrentCommand().mode = static_cast<MacroMode>(newIndex);
        this->updateVisibility();
    });
    connect(mouseButtonInput, &QComboBox::currentIndexChanged, this, [&](int newIndex){
        if(!this->isEnabled()) return;
        qDebug() << "mouseButtonInput" << QString::number(newIndex);
        qDebug() << this->mouseButtonInput->itemText(newIndex);
        uint8_t buttonCode = newIndex == 0 ? 0 : ((1 << (newIndex - 1)));
        qDebug() << "Mouse button code: " << buttonCode;
        SET_DIRTY_IF(this->commandList->getCurrentCommand().mouseMove.mouseBtn != buttonCode);
        this->commandList->getCurrentCommand().mouseMove.mouseBtn = buttonCode;
    });
    connect(keyInput, &QComboBox::currentIndexChanged, this, [&](int newIndex){
        if(!this->isEnabled()) return;
        qDebug() << "keyInput" << QString::number(newIndex);
        qDebug() << this->keyInput->itemText(newIndex);
        std::string enumKey = this->keyInput->itemText(newIndex).toStdString();
        int inputKey = keys.keyToValue(enumKey.c_str());
        qDebug() << "Keycode:" << (int)inputKey;
        SET_DIRTY_IF(this->commandList->getCurrentCommand().keycode != inputKey);
        this->commandList->getCurrentCommand().keycode = inputKey;
    });
    connect(modifierInput, &QComboBox::currentIndexChanged, this, [&](int newIndex){
        if(!this->isEnabled()) return;
        qDebug() << "modifierInput" << QString::number(newIndex);
        qDebug() << this->modifierInput->itemText(newIndex);
        std::string enumKey = this->modifierInput->itemText(newIndex).toStdString();
        int inputKey = keys.keyToValue(enumKey.c_str());
        qDebug() << "Keycode:" << (int)inputKey;
        SET_DIRTY_IF(this->commandList->getCurrentCommand().modifierCode != inputKey);
        this->commandList->getCurrentCommand().modifierCode = inputKey;
    });
    this->inputModeInput->addItem("Keyboard");
    this->inputModeInput->addItem("Mouse");
    initMouseButtonInput(mouseButtonInput);
    initKeyInput(keyInput);
    initModifierInput(modifierInput);
}

void ConfigMacroForm::updateForm(const MacroPacket &packet)
{
    this->inputModeInput->setCurrentIndex(packet.mode);
    updateVisibility();
    switch(packet.mode){
        case MOUSE_MOVE:
        this->mouseButtonInput->setCurrentIndex(packet.mouseMove.mouseBtn == 0 ? 0 : (std::log2(packet.mouseMove.mouseBtn) + 1));
        this->mouseScrollInput->setValue(packet.mouseMove.wheel);
        this->mouseXInput->setValue(packet.mouseMove.mouseX);
        this->mouseYInput->setValue(packet.mouseMove.mouseY);
        break;
        default:
        case KEYBOARD_MOUSE_CLICK:
        this->keyInput->setCurrentIndex(this->keyInput->findText(keys.valueToKey(packet.keycode)));
        this->modifierInput->setCurrentIndex(this->modifierInput->findText(keys.valueToKey(packet.modifierCode)));
        break;
    }
}

void ConfigMacroForm::updateVisibility(){
    this->setEnabled(true);
    switch(this->commandList->getCurrentCommand().mode){
        case MOUSE_MOVE:
        this->mouseButtonInput->parentWidget()->parentWidget()->setVisible(true);
        this->keyInput->parentWidget()->parentWidget()->setVisible(false);
        break;
        default:
        case KEYBOARD_MOUSE_CLICK:
        this->mouseButtonInput->parentWidget()->parentWidget()->setVisible(false);
        this->keyInput->parentWidget()->parentWidget()->setVisible(true);
        break;
    }
}

void ConfigMacroForm::disable(){
    this->setEnabled(false);
    this->mouseButtonInput->parentWidget()->parentWidget()->setVisible(false);
    this->keyInput->parentWidget()->parentWidget()->setVisible(false);
}

void ConfigMacroForm::initMouseButtonInput(QComboBox* mouseButtonInput){
    mouseButtonInput->addItem("NONE"); // 0 reserved for NONE, be careful
    mouseButtonInput->addItem("MOUSE_LEFT");
    mouseButtonInput->addItem("MOUSE_RIGHT");
    mouseButtonInput->addItem("MOUSE_MIDDLE");
    mouseButtonInput->addItem("MOUSE_PREV");
    mouseButtonInput->addItem("MOUSE_NEXT");
}

void ConfigMacroForm::initKeyInput(QComboBox* keyInput){
    for(int i = 0; i < keys.keyCount(); ++i){
        QString key = keys.key(i);
        keyInput->addItem(key);
    }
}

void ConfigMacroForm::initModifierInput(QComboBox* modifierInput){
    for(int i = 0; i < keys.keyCount(); ++i){
        QString key = keys.key(i);
        modifierInput->addItem(key);
    }
}

#undef SET_DIRTY_IF
