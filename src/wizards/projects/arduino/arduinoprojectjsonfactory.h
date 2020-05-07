#ifndef ARDUINOPROJECTJSONFACTORY_H
#define ARDUINOPROJECTJSONFACTORY_H

#include <projectexplorer/jsonwizard/jsonwizardfactory.h>

namespace Arduino {
namespace Internal {

class ArduinoProjectJsonFactory : public ProjectExplorer::JsonWizardFactory
{
    Q_OBJECT

public:
    ArduinoProjectJsonFactory();
};

} // namespace Internal
} // namespace Arduino

#endif // ARDUINOPROJECTJSONFACTORY_H
