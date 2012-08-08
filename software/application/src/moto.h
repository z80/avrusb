
#ifndef __MOTO_H_
#define __MOTO_H_

#include "ui_moto.h"
#include "state.h"
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
    // Write EEPROM parameters.
    void writeConfig();
    // Locking and unlocking GUI.
    void lockConfig();
    void unlockConfig();
    void adjustControls(); // Enable either "speed" or "throttle".

    // Asynch hardware access routines.
    void asynchReadStatus(); // It is invoked one per some time. And open() is dispatched here as well.
    void asynchReadConfig();
    void asynchWriteConfig();
    void asynchWriteThrottle();
    void asynchWriteSpeed();
    void asynchWriteDirection();


    Ui_Moto       ui;
    QTimer      * m_timer;
    CtrlboardIo * m_board;
    QMutex        m_mutex;
    QFuture<void> m_future;
    State m_state;

    static const QString INI_FILE_NAME;
signals:
    void sigConfig();
    void sigStatus();
    void sigOpened();
    void sigClosed();
private slots:
    void slotTimeout();
    void slotConfig();
    void slotStatus();
    void slotOpened();
    void slotClosed();
    // GUI slots.
    void slotSpeedChanged( int value );
    void slotThrottleChanged( int value );
    void slotDirectionChanged( int value );
    void slotUnlock();
    void slotApply();

};



#endif



