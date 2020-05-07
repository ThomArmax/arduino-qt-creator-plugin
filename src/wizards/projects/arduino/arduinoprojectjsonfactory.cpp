#include "arduinoprojectjsonfactory.h"

namespace Arduino {
namespace Internal {

ArduinoProjectJsonFactory::ArduinoProjectJsonFactory()
    : JsonWizardFactory()
{
    addWizardPath(Utils::FileName::fromString(":/wizards/projects/"));
}

} // namespace Internal
} // namespace Arduino
