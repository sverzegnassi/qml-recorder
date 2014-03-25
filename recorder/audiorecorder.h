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

#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QAudioRecorder>
#include <QMediaRecorder>
#include <QMultimedia>
#include <QFileInfo>
#include <QFile>
#include <QUrl>
#include <QDir>

class AudioRecorder : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AudioRecorder)
    Q_PROPERTY  (QString    path            READ path            WRITE setPath       NOTIFY pathChanged)
    Q_PROPERTY  (QString    codec           READ codec           WRITE setCodec      NOTIFY codecChanged)
    Q_PROPERTY  (int        quality         READ quality         WRITE setQuality    NOTIFY qualityChanged)
    Q_PROPERTY  (qreal      volume          READ volume          WRITE setVolume     NOTIFY volumeChanged)
    Q_PROPERTY  (QString    input           READ input           WRITE setInput      NOTIFY inputChanged)
    Q_PROPERTY  (qint64     duration        READ duration                            NOTIFY durationChanged)
    Q_PROPERTY  (Error      error           READ error                               NOTIFY errorChanged)
    Q_PROPERTY  (QString    errorString     READ errorString                         NOTIFY errorChanged)
    Q_PROPERTY  (State      state           READ state                               NOTIFY stateChanged)
    Q_ENUMS(Error)
    Q_ENUMS(State)

public:
    enum Error
    {
         NoError = QMediaRecorder::NoError,
         ResourceError = QMediaRecorder::ResourceError,
         FormatError = QMediaRecorder::FormatError,
         OutOfSpaceError = QMediaRecorder::OutOfSpaceError
    };

    enum State
    {
        StoppedState = QAudioRecorder::StoppedState,
        RecordingState = QAudioRecorder::RecordingState,
        PausedState = QAudioRecorder::PausedState
    };

    AudioRecorder(QObject *parent = 0);
    ~AudioRecorder();

    QString path() const;
    void setPath(const QString &path);

    QString codec() const;
    void setCodec(const QString &codec);

    QString input() const;
    void setInput(const QString &input);

    int quality() const;
    void setQuality(const int &quality);

    qreal volume() const;
    void setVolume(const qreal &volume);

    qint64 duration() const;

    Error error() const;
    QString errorString() const;

    State state() const;

    Q_INVOKABLE QStringList getSupportedCodecs();
    Q_INVOKABLE QStringList getAudioInputs();
    Q_INVOKABLE QString getFilePath();

public Q_SLOTS:
    void start();
    void stop();
    void pause();
    void resume();

Q_SIGNALS:
    void pathChanged();

    void codecChanged();
    void inputChanged();
    void qualityChanged();
    void volumeChanged();

    void durationChanged();

    void stateChanged();
    void recording();
    void stopped();
    void paused();
    void resumed();

    void errorChanged();

private Q_SLOTS:
    void _q_stateChanged();
    void _q_error();
    void _q_durationChanged();

private:
    QAudioRecorder *audioRecorder;
    QString cPath;

    QString m_path;
    QString m_codec;
    QString m_input;
    int m_quality;
    qreal m_volume;

    qint64 m_duration;

    QMediaRecorder::State m_state;

    QMediaRecorder::Error m_error;
    QString m_errorString;

    QString getContainerFromCodec(QString codec);
    QString getExtensionFromCodec(QString codec);

};

#endif // AUDIORECORDER_H

