#ifndef INPUTLIST_H
#define INPUTLIST_H

#include <QListWidget>

class InputList : public QListWidget
{
    Q_OBJECT
public:
    InputList(QWidget *parent = nullptr);
    void injectDependencies();
public slots:
    void onNewInput();
    void onDeleteSelected();
    void onSelectChanged();
};

#endif // INPUTLIST_H
