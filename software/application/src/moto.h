
#ifndef __MOTO_H_
#define __MOTO_H_

#include "ui_moto.h"
#include "state.h"
#include <QtGui>
class CtrlboardIo;

class Moto: public QMainWindow
{
    Q_OBJECT
public:
    Moto( QWidget * parent = 0 );
    virtual ~Moto();

protected:
    void closeEvent( QCloseEvent * e );
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
    void asynchReadSpeed();
    void asynchReadStatus(); // It is invoked one per some time. And open() is dispatched here as well.
    void asynchReadConfig();
    void asynchWriteConfig();
    void asynchWriteThrottle();
    void asynchWriteSpeed();
    void asynchWriteDirection();


    Ui_Moto       ui;
    QTimer      * m_statusTimer,
                * m_speedTimer;
    CtrlboardIo * m_board;
    QMutex        m_mutex;
    QFuture<void> m_speedFuture,
                  m_statusFuture,
                  m_applyFuture;
    QStringList   m_errorCodes;
    QString       m_helpCmd;
    State m_state;

    static const QString INI_FILE_NAME;
    static const int SLEEP;
    static const int TRIES;
signals:
    void sigSpeed();
    void sigStatus();
    void sigConfig();
    void sigOpened();
    void sigClosed();
private slots:
    void slotStatusTimeout();
    void slotSpeedTimeout();
    void slotSpeed();
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
    void slotHelp();
};



#endif



