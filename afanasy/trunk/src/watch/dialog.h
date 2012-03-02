#pragma once

#include <QtGui/QBoxLayout>
#include <QtCore/QLinkedList>
#include <QtGui/QListWidget>
#include <QtCore/QTimer>
#include <QtGui/QWidget>

#include "../libafanasy/msgclasses/msgclassuserhost.h"

#include "../libafqt/qserver.h"
#include "../libafqt/qthreadclientsend.h"
#include "../libafqt/qthreadclientup.h"

#include "infoline.h"
#include "watch.h"

class ButtonOut;
class ButtonMonitor;
class ListItems;
class OfflineScreen;
class LabelVersion;

class QHBoxLayout;
class QVBoxLayout;
class QScrollArea;

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog();
    ~Dialog();

    inline bool isInitialized() const { return initialized; }
    inline bool isConnected()   const { return connected;   }

    inline int getUid() const { return uid;}

    void inline displayInfo(    const QString &message) { infoline->displayInfo(    message); }
    void inline displayWarning( const QString &message) { infoline->displayWarning( message); }
    void inline displayError(   const QString &message) { infoline->displayError(   message); }

    void sendMsg( af::Msg * msg);

    bool openMonitor( int type, bool open);

    inline MonitorHost * getMonitor() { return monitor;}

    void repaintStart( int mseconds = 1000);
    void repaintFinish();

    void keyPressEvent(    QKeyEvent         * event);

signals:
    void stop();

private slots:
    void newMessage( af::Msg * msg);
    void connectionLost();
    void repaintWatch();

    void actColors();
    void actSounds();
    void actSavePreferencesOnExit();
    void actSaveGUIOnExit();
    void actSaveWndRectsOnExit();
    void actSavePreferences();
    void actShowOfflineNoise();
    void actGuiTheme( QString theme);

protected:
    void contextMenuEvent( QContextMenuEvent * event);
    void closeEvent(       QCloseEvent       * event);
    void paintEvent ( QPaintEvent * event );

private:
    void sendRegister();
    void closeList();
    void connectionEstablished();
    void setDefaultWindowTitle();
    void reloadImages();

private:
    bool initialized;
    bool connected;
    int uid;
    MonitorHost * monitor;

    int monitorType;

    afqt::QThreadClientUp   qThreadClientUpdate;
    afqt::QThreadClientSend qThreadSend;
    afqt::QServer           qServer;

    ListItems * listitems;
    OfflineScreen * offlinescreen;
    InfoLine * infoline;
    LabelVersion * labelversion;

    QHBoxLayout *hlayout_a;
    QVBoxLayout *vlayout_a;
    QHBoxLayout *hlayout_b;
    QVBoxLayout *vlayout_b;

    QTimer repaintTimer;

    ButtonMonitor *btnMonitor[Watch::WLAST];

    ButtonOut *btn_outlft;
    ButtonOut *btn_outrht;

    af::MsgClassUserHost mcuserhost;

    QPixmap img_topleft;
    QPixmap img_topright;
    QPixmap img_botleft;
    QPixmap img_botright;
};
