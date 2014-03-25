/*
  Copyright (C) 2013-2014 Stefano Verzegnassi

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

#include "audiorecorder.h"

AudioRecorder::AudioRecorder(QObject *parent):
    QObject(parent),
    m_codec("audio/PCM"),
    m_input("default:"),
    m_quality(4),
    m_volume(100),
    m_state(QMediaRecorder::StoppedState),
    m_error(QMediaRecorder::ResourceError)
{
    audioRecorder = new QAudioRecorder;

    connect(audioRecorder, SIGNAL(durationChanged(qint64)), this,
            SLOT(_q_durationChanged()));
    connect(audioRecorder, SIGNAL(stateChanged(QMediaRecorder::State)), this,
            SLOT(_q_stateChanged()));
    connect(audioRecorder, SIGNAL(error(QMediaRecorder::Error)), this,
            SLOT(_q_error()));
}

void AudioRecorder::_q_error()
{
   m_error = audioRecorder->error();
   m_errorString = audioRecorder->errorString();

   emit errorChanged();
}

AudioRecorder::Error AudioRecorder::error() const
{
    return Error(m_error);
}

QString AudioRecorder::errorString() const
{
    return m_errorString;
}

void AudioRecorder::_q_stateChanged()
{
    const QMediaRecorder::State oldState = m_state;
    const QMediaRecorder::State state = audioRecorder->state();

    m_state = audioRecorder->state();

    if (state != oldState) {
        switch (state) {
            case QMediaRecorder::StoppedState:
                emit stopped();
                break;
            case QMediaRecorder::RecordingState:
                if (oldState == QMediaRecorder::PausedState) {
                    emit resumed();
                    break;
                } else {
                    emit recording();
                    break;
                }
            case QMediaRecorder::PausedState:
                emit paused();
                break;
            }

        emit stateChanged();
    }
}

AudioRecorder::State AudioRecorder::state() const
{
    return State(m_state);
}

QString AudioRecorder::path() const
{
    return m_path;
}

void AudioRecorder::setPath(const QString &path)
{
    if (m_path == path)
        return;

    m_path = path;
    emit pathChanged();
}

QString AudioRecorder::codec() const
{
    return m_codec;
}

void AudioRecorder::setCodec(const QString &codec)
{
    if (m_codec == codec)
        return;

    m_codec = codec;
    emit codecChanged();
}

QString AudioRecorder::input() const
{
    return m_input;
}

void AudioRecorder::setInput(const QString &input)
{
    if (m_input == input)
        return;

    m_input = input;
    emit inputChanged();
}

int AudioRecorder::quality() const
{
    return m_quality;
}

void AudioRecorder::setQuality(const int &quality)
{
    if (m_quality == quality)
        return;

    m_quality = quality;
    emit qualityChanged();
}

qreal AudioRecorder::volume() const
{
    return m_volume;
}

void AudioRecorder::setVolume(const qreal &volume)
{
    if (m_volume == volume)
        return;

    m_volume = volume;
    emit volumeChanged();
}

qint64 AudioRecorder::duration() const
{
    return m_duration;
}

void AudioRecorder::_q_durationChanged()
{
    m_duration = audioRecorder->duration();
    emit durationChanged();
}

void AudioRecorder::start() //TODO: reduce noise settings
{
    if (audioRecorder->state() == QMediaRecorder::StoppedState) {
        QAudioEncoderSettings audioSettings;

        // Set codec
        audioSettings.setCodec(m_codec);

        // Set quality: 0=VeryLow, 1=Low, 2=Normal, 3=High, 4=VeryHigh
        audioSettings.setEncodingMode(QMultimedia::ConstantQualityEncoding);
        audioSettings.setQuality(QMultimedia::EncodingQuality(m_quality));

        if (audioSettings.codec() == "audio/PCM" || "audio/FLAC") {
            audioSettings.setSampleRate(-1);

            if (m_quality == 0)
                {audioSettings.setSampleRate(8000);}

            if (m_quality == 1)
                {audioSettings.setSampleRate(16000);}

            if (m_quality == 2)
                {audioSettings.setSampleRate(22050);}

            if (m_quality == 3)
                {audioSettings.setSampleRate(44100);}

            if (m_quality == 4)
                {audioSettings.setSampleRate(88200);}
            }

        audioRecorder->setEncodingSettings(audioSettings);
        audioRecorder->setAudioInput(m_input);
        audioRecorder->setContainerFormat(getContainerFromCodec(m_codec));

        // Set volume
        audioRecorder->setVolume(m_volume);

        // Set output location
        cPath = m_path + getExtensionFromCodec(m_codec);
        audioRecorder->setOutputLocation(QUrl(cPath));

        audioRecorder->record();
    }
}

QString AudioRecorder::getFilePath()
{
    return cPath;
}

void AudioRecorder::stop()
{
    if (audioRecorder->state() == QMediaRecorder::RecordingState ||
            audioRecorder->state() == QMediaRecorder::PausedState) {

        audioRecorder->stop();
    }
}

void  AudioRecorder::pause()
{
    if (audioRecorder->state() == QMediaRecorder::RecordingState) {
        audioRecorder->pause();
    }
}

void AudioRecorder::resume()
{
    if (audioRecorder->state() == QMediaRecorder::PausedState) {
        audioRecorder->record();
    }
}

// It parses codec name to get the corrisponding container
QString AudioRecorder::getContainerFromCodec(QString codec)
{
    QString container = "";

    if (codec == "audio/vorbis")
        { container = "ogg";}

    if (codec == "audio/PCM")
        { container = "wav";}

    if (codec == "audio/FLAC")
        { container = "raw";}

    if (codec == "audio/AMR")
        { container = "amr";}

    if (codec == "audio/mpeg")
        { container = "raw";}

    return container;
}

// It parses codec name to get the corrisponding extension
QString AudioRecorder::getExtensionFromCodec(QString codec)
{
    QString extension = "";

    if (codec == "audio/vorbis")
        { extension = ".ogg";}

    if (codec == "audio/PCM")
        { extension = ".wav";}

    if (codec == "audio/FLAC")
        { extension = ".flac";}

    if (codec == "audio/AMR")
        { extension = ".amr";}

    if (codec == "audio/mpeg")
        { extension = ".mp3";}

    return extension;
}

QStringList AudioRecorder::getSupportedCodecs()
{
    QStringList allSupportedCodecs = audioRecorder->supportedAudioCodecs();
    QStringList codecsList;

    foreach (QString codec, allSupportedCodecs) {
        if (codec.contains("audio/mpeg", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/vorbis", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/FLAC", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/PCM", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/AMR", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
    }

    return codecsList;
}

QStringList AudioRecorder::getAudioInputs()
{
    return audioRecorder->audioInputs();
}

AudioRecorder::~AudioRecorder()
{
}

