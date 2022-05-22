#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

#include <QMessageBox>

#define EXCEPT_ALERT(code) try { code } catch(const QString& msg) { QMessageBox::critical(this, "An exception has occurred", msg, QMessageBox::Ok); }


#endif // EXCEPTION_HANDLER_H
