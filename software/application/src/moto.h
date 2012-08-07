
#ifndef __MOTO_H_
#define __MOTO_H_

#include "ui_moto.h"
#include <QtGui>
class CtrlboardIo;

class Moto: public QWidget
{
    Q_OBJECT
public:
    Moto( QWidget * parent = 0 );
    virtual ~Moto();

private:
    // INI file IO.
    void loadSettings();
    void saveSettings();
    // Initialize GUI controls.
    void initGui();
    // Load EEPROM parameters.
    void readConfig();
    // Write EEPROM parameters.
    void writeConfig();
    // Reading status.
    void readStatus();
    // Writing user commands.
    void writeSoftware();

    // Asynch hardware access routines.
    void asynchReadConfig();
    void asynchWriteConfig();
    void asynchReadStatus();
    void asynchWriteSoftware();


    Ui_Moto       ui;
    CtrlboardIo * m_board;
    QMutex        m_mutex;
    QFuture<void> m_future;

    static const QString INI_FILE_NAME;
signals:
    void sigConfig();
    void sigStatus();
private slots:
    void slotConfig();
    void slotStatus();
};



#endif



