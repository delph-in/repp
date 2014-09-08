#ifndef _FROM_PET_SETTINGS_H_
#define _FROM_PET_SETTINGS_H_

/*
 * Get the necessary settings from cheap_settings to create UT model
 */
#include "settings.h"
#include "tdl_options.h"
#include "repp.h"

tReppTokenizer *createReppTokenizer(settings *cheap_settings);

#endif
