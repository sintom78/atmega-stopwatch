#ifndef QTAPPLICATIONSTOPWATCH_H
#define QTAPPLICATIONSTOPWATCH_H

#include <QTimer>
#include <QMap>
#include <QObjectList>
#include "qtquick2applicationviewer.h"
#include "integration.hpp"

class QtSegDisplay: public QObject
{
    Q_OBJECT
public:
    QtSegDisplay();
    void setState(PinFunctions pinf, bool state);
    bool getState(PinFunctions pinf);

private:
    bool segA;
    bool segB;
    bool segC;
    bool segD;
    bool segE;
    bool segF;
    bool segG;
};

class QtPinStates : public QObject
{
    Q_OBJECT
public:
    QtPinStates();
    bool setState(PinFunctions pinf, bool state);
    bool getState(PinFunctions pinf);

Q_SIGNALS:
    Q_SIGNAL void pinStateChanged(PinFunctions pinf);


private:
    QtSegDisplay display;
    bool disp0Enabled;
    bool disp1Enabled;
    bool disp2Enabled;
    bool disp3Enabled;
    bool leds;
    bool buttonReset;
    bool buttonMode;
    bool buttonSet;
    bool buttonStartStop;
};

class QtApplicationStopWatch : public QtQuick2ApplicationViewer, public IntegrationInterface
{
    Q_OBJECT
public:
    explicit QtApplicationStopWatch(QWindow *parent = 0);
    void integrate(integration_t*);

    virtual void mapPin(unsigned int pin, PinFunctions pinF);
    virtual void digitalWriteHandler(uint8_t,uint8_t);
    virtual void initHandler(void);
    virtual int digitalReadHandler(uint8_t);

private slots:
    void onTimeout();
    void onButtonMode(bool pressed);
    void onButtonStartStop(bool pressed);
    void onButtonReset(bool pressed);
    void onButtonSetup(bool pressed);
    void onPinStateChanged(PinFunctions pinf);

private:
    QTimer timer;
    integration_t* pIntegration;
    QObject *pActiveDisplay;
    QObject *pSegment;
    QObject *pRoot;
    QtPinStates pinStates;
    QMap<int,PinFunctions> pinsMap;
    QMap<QObject*,QObjectList> displaysMap;
    QTypedArrayData<QObject*> *displaysArray;
    static const int sInterval;

    void handlePinChangedDisplay(PinFunctions pinF);
    void updateActiveDisplay(PinFunctions pinf);
    void updateSegmentColor(PinFunctions pinf, QString segname);
    void disableDisplay(QObject* display);
    QString getPinfName(PinFunctions pinf);
    void buildDisplaysMap(QObject* display);
    void findDisplays();

};

#endif // QTAPPLICATIONSTOPWATCH_H
