/*
  Copyright (C) 2014 Stefano Verzegnassi

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

#include "recorder_plugin.h"
#include "audiorecorder.h"
#include "utils.h"

#include <qqml.h>

void LibRecorderPlugin::registerTypes(const char *uri)
{
    // @uri Audiotape.Recorder
    qmlRegisterType<AudioRecorder>(uri, 0, 1, "AudioRecorder");
    qmlRegisterType<Utils>(uri, 0, 1, "Utils");
}


