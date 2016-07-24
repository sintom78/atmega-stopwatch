#include <QDebug>
#include <QQuickItem>
#include <QObjectList>
#include <QRegularExpression>
#include "qtapplicationstopwatch.h"
#include "integration.hpp"

QtSegDisplay::QtSegDisplay()
{

}

void QtSegDisplay::setState(PinFunctions pinf, bool state)
{
    switch(pinf)
    {
    case PINF_SEGA:
        segA = state;
        break;
    case PINF_SEGB:
        segB = state;
        break;
    case PINF_SEGC:
        segC = state;
        break;
    case PINF_SEGD:
        segD = state;
        break;
    case PINF_SEGE:
        segE = state;
        break;
    case PINF_SEGF:
        segF = state;
        break;
    case PINF_SEGG:
        segG = state;
        break;
    default:
        qDebug() << "Error: unhadled pin in display";
    }
}

bool QtSegDisplay::getState(PinFunctions pinf)
{
    bool result;
    switch(pinf)
    {
    case PINF_SEGA:
        result = segA;
        break;
    case PINF_SEGB:
        result = segB;
        break;
    case PINF_SEGC:
        result = segC;
        break;
    case PINF_SEGD:
        result = segD;
        break;
    case PINF_SEGE:
        result = segE;
        break;
    case PINF_SEGF:
        result = segF;
        break;
    case PINF_SEGG:
        result = segG ;
        break;
    default:
        qDebug() << "Error: unhadled pin in display";
    }

    return result;
}

QtPinStates::QtPinStates()
{

}

bool QtPinStates::setState(PinFunctions pinf, bool state)
{
    bool handled = true;
    switch(pinf)
    {
    case PINF_DISP0:
        disp0Enabled = state;
        emit pinStateChanged(pinf);
        break;
    case PINF_DISP1:
        disp1Enabled = state;
        emit pinStateChanged(pinf);
        break;
    case PINF_DISP2:
        disp2Enabled = state;
        emit pinStateChanged(pinf);
        break;
    case PINF_DISP3:
        disp3Enabled = state;
        emit pinStateChanged(pinf);
        break;
    case PINF_SEGA:
    case PINF_SEGB:
    case PINF_SEGC:
    case PINF_SEGD:
    case PINF_SEGE:
    case PINF_SEGF:
    case PINF_SEGG:
        display.setState(pinf, state);
        emit pinStateChanged(pinf);
        break;
    case PINF_BUTTONMODE:
        buttonMode = state;
        emit pinStateChanged(pinf);
        break;
    case PINF_BUTTONRESET:
        buttonReset = state;
        emit pinStateChanged(pinf);
        break;
    case PINF_BUTTONSET:
        buttonSet = state;
        emit pinStateChanged(pinf);
        break;
    case PINF_BUTTONSTARTSTOP:
        buttonStartStop = state;
        emit pinStateChanged(pinf);
        break;
    default:
        handled = false;
        break;
    }

    return handled;
}

bool QtPinStates::getState(PinFunctions pinf)
{
    bool handled = true;
    switch(pinf)
    {
    case PINF_DISP0:
        return disp0Enabled;
        break;
    case PINF_DISP1:
        return disp1Enabled;
        break;
    case PINF_DISP2:
        return disp2Enabled;
        break;
    case PINF_DISP3:
        return disp3Enabled;
        break;
    case PINF_SEGA:
    case PINF_SEGB:
    case PINF_SEGC:
    case PINF_SEGD:
    case PINF_SEGE:
    case PINF_SEGF:
    case PINF_SEGG:
        return display.getState(pinf);
        break;
    case PINF_BUTTONMODE:
        return buttonMode;
        break;
    case PINF_BUTTONRESET:
        return buttonReset;
        break;
    case PINF_BUTTONSET:
        return buttonSet;
        break;
    case PINF_BUTTONSTARTSTOP:
        return buttonStartStop;
        break;
    default:
        handled = false;
        qDebug() << "Error: Unhandled pinf in display";
        break;
    }
    return handled;
}

const int QtApplicationStopWatch::sInterval = 1;

QtApplicationStopWatch::QtApplicationStopWatch(QWindow *parent) :
    QtQuick2ApplicationViewer(parent), timer(this), pIntegration(0),
    pActiveDisplay(0)
{
    setMainQmlFile(QStringLiteral("qml/stopwatch-simu/main.qml"));
    pRoot = rootObject();
    if (pRoot) {
        connect(pRoot,SIGNAL(qmlModeButton(bool)),this,SLOT(onButtonMode(bool)));
        connect(pRoot,SIGNAL(qmlSetupButton(bool)),this,SLOT(onButtonSetup(bool)));
        connect(pRoot,SIGNAL(qmlResetButton(bool)),this,SLOT(onButtonReset(bool)));
        connect(pRoot,SIGNAL(qmlStartStopButton(bool)),this,SLOT(onButtonStartStop(bool)));
    }
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    connect(&pinStates,SIGNAL(pinStateChanged(PinFunctions)),this,
            SLOT(onPinStateChanged(PinFunctions)));
    timer.setInterval(sInterval);
    pSegment = pRoot->findChild<QObject*>(QString("segmentDisplay"));
    findDisplays();
}

void QtApplicationStopWatch::integrate(integration_t* integration)
{
    integration->interface = this;
    pIntegration = integration;
}

void QtApplicationStopWatch::mapPin(unsigned int pin, PinFunctions pinF)
{
    pinsMap[pin] = pinF;
}

void QtApplicationStopWatch::digitalWriteHandler(uint8_t pin, uint8_t value)
{
    //qDebug() << "Pin:" << pin << "mapped to:" << getPinfName(pinsMap[pin]) << " set to: " << value;
    if (!pinStates.setState(pinsMap[pin], value!=0 ))
    {
        qDebug() << "Unhandled pin: " << pin << " mapped to: " << getPinfName(pinsMap[pin]);
    }
}

void QtApplicationStopWatch::initHandler(void)
{
    qDebug() << "INIT handler";
    timer.start();
}

int QtApplicationStopWatch::digitalReadHandler(uint8_t pin)
{
    return pinStates.getState(pinsMap[pin]);
}

void QtApplicationStopWatch::onTimeout()
{
    pIntegration->tickHandler(sInterval);
}


void QtApplicationStopWatch::onButtonMode(bool pressed)
{
    pinStates.setState(PINF_BUTTONMODE, pressed);

}

void QtApplicationStopWatch::onButtonStartStop(bool pressed)
{
    pinStates.setState(PINF_BUTTONSTARTSTOP, pressed);
}

void QtApplicationStopWatch::onButtonReset(bool pressed)
{
    pinStates.setState(PINF_BUTTONRESET, pressed);
}

void QtApplicationStopWatch::onButtonSetup(bool pressed)
{
    pinStates.setState(PINF_BUTTONSET, pressed);
}

void QtApplicationStopWatch::onPinStateChanged(PinFunctions pinf)
{
    switch(pinf)
    {
    case PINF_BUTTONMODE:
        break;
    case PINF_BUTTONRESET:
        break;
    case PINF_BUTTONSET:
        break;
    case PINF_BUTTONSTARTSTOP:
        break;
    default:
        handlePinChangedDisplay(pinf);
    }
}

void QtApplicationStopWatch::handlePinChangedDisplay(PinFunctions pinf)
{
    switch(pinf)
    {
    case PINF_DISP0:
    case PINF_DISP1:
    case PINF_DISP2:
    case PINF_DISP3:
        updateActiveDisplay(pinf);
        break;
    case PINF_SEGA:
    case PINF_SEGB:
    case PINF_SEGC:
    case PINF_SEGD:
    case PINF_SEGE:
    case PINF_SEGF:
    case PINF_SEGG:
        updateSegmentColor(pinf, getPinfName(pinf));
        break;
    default:
        qDebug() << "Warning: Unknown display pin function";
    }
}


void QtApplicationStopWatch::updateActiveDisplay(PinFunctions pinf)
{
    if (!pSegment) {
        qDebug() << "QtApplicationStopWatch::enableDisplay: Segment not found";
        return;
    }

    bool succ = false;
    int activeDisp;
    if (pinStates.getState(pinf)) {
        switch(pinf)
        {
        case PINF_DISP0:
            activeDisp = 0;
            break;
        case PINF_DISP1:
            activeDisp = 1;
            break;
        case PINF_DISP2:
            activeDisp = 2;
            break;
        case PINF_DISP3:
            activeDisp = 3;
            break;
        default:
            break;
        }
        succ = pSegment->setProperty("activeDisplayIndex",activeDisp);
        if (succ) {
            pActiveDisplay = displaysArray->data()[activeDisp];
            if (!pActiveDisplay) {
                qDebug() << "Failed to change active display";
            }
        } else {
            qDebug() << "Failed to change display index";
        }
    } else {
       // disableDisplay(pActiveDisplay);
    }
}

void QtApplicationStopWatch::updateSegmentColor(PinFunctions pinf, QString segname)
{
    if (!pActiveDisplay || !pSegment)
    {
        qDebug() << "updateSegmentColor error: no active display.";
        return;
    }

    QString str = segname.append("Enabled");
    bool pstate = pinStates.getState(pinf);
    if (!pSegment->setProperty(str.toStdString().c_str(), pstate))
    {
        qDebug() << "updateSegmentColor: failed to set property";
    }

}

void QtApplicationStopWatch::disableDisplay(QObject* display)
{
    if (!display)
    {
        qDebug() << "Warning: cannod disable display - display is NULL pointer";
        return;
    }
    if (!displaysMap.contains(display))
    {
        buildDisplaysMap(display);
    }

    QObjectList::iterator chldrn = displaysMap[display].begin();
    for(;chldrn!=displaysMap[display].end(); chldrn++)
    {
        (*chldrn)->setProperty("color",QColor("darkred"));
    }
}

QString QtApplicationStopWatch::getPinfName(PinFunctions pinf)
{
    QString pinfName;
    switch(pinf)
    {
    case PINF_DISP0:
        pinfName = "digit0";
        break;
    case PINF_DISP1:
        pinfName = "digit1";
        break;
    case PINF_DISP2:
        pinfName = "digit2";
        break;
    case PINF_DISP3:
        pinfName = "digit3";
        break;
    case PINF_SEGA:
        pinfName = "segA";
        break;
    case PINF_SEGB:
        pinfName = "segB";
        break;
    case PINF_SEGC:
        pinfName = "segC";
        break;
    case PINF_SEGD:
        pinfName = "segD";
        break;
    case PINF_SEGE:
        pinfName = "segE";
        break;
    case PINF_SEGF:
        pinfName = "segF";
        break;
    case PINF_SEGG:
        pinfName = "segG";
        break;
    case PINF_UNSUPPORTED:
        pinfName = "PINF_UNSUPPORTED";
        break;
    default:
        pinfName = "";
        qDebug() << "Warning: Unknown display pin function";
    }

    return pinfName;
}

void QtApplicationStopWatch::buildDisplaysMap(QObject* display)
{
    if (!display)
    {
        qDebug() << "Warning: cannot build display map as display is NULL pointer";
        return;
    }

    QString obname = display->objectName();
    QObject *seg = pRoot->findChild<QObject*>(obname);
    if (seg) {

        QObjectList chldrn = seg->findChildren<QObject*>(QRegularExpression("seg."));
        if (chldrn.count())
            displaysMap[display] = chldrn;
    }
}

void QtApplicationStopWatch::findDisplays()
{
    QObjectList displays = pSegment->findChildren<QObject*>(QRegularExpression("^digit.+"));
    if (!displays.count())
    {
        displaysArray = NULL;
        qDebug() << "Warning: no displays found.";
        return;
    }

    displaysArray = QTypedArrayData<QObject*>::allocate(displays.count());
    if (!displaysArray)
    {
        qDebug() << "Could not allocate data for displaysArary";
        return;
    }

    qDebug() << "Found " << displays.count() << " displays";

    for(int x=0;x<displays.count(); x++)
    {
        displaysArray->data()[x] = displays.at(x);
    }
}
