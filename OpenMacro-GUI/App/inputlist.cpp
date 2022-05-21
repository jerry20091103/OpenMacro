#include "inputlist.h"

InputList::InputList(QWidget *parent) : QListWidget(parent) {

}

void InputList::injectDependencies()
{

}

void InputList::onNewInput()
{
    qDebug() << "onNewInput";
    this->addItem("Dummy Input");
}

void InputList::onDeleteSelected()
{
    qDebug() << "onDeleteSelected";
    qDeleteAll(this->selectedItems());

}

void InputList::onSelectChanged()
{
    qDebug() << "onSelectChanged";

}
