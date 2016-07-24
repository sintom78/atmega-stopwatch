#include <QtGui/QGuiApplication>
#include <QQuickItem>
#include "qtapplicationstopwatch.h"
#include "integration.hpp"

extern integration_t integration;
extern int _main(void);
extern void handleTick(uint16_t ms);

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtApplicationStopWatch viewer;
    viewer.showExpanded();
    viewer.integrate(&integration);
    integration.tickHandler = handleTick;
    mapPins();
    _main();
    return app.exec();
}
