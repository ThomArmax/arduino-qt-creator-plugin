#ifndef ARDUINOTOOLSMENU_H
#define ARDUINOTOOLSMENU_H

#include <QObject>


namespace Arduino {
namespace Internal {

class ArduinoToolsMenu : public QObject
{
public:
    ArduinoToolsMenu();

private:
    void triggerActionDownloadProgram();
    void triggerActionOpenSerialMonitor();

};

} // namespace Internal
} // namespace Arduino

#endif // ARDUINOTOOLSMENU_H
