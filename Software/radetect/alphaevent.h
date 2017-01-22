#ifndef ALPHAEVENT_H
#define ALPHAEVENT_H

#include <QObject>
#include <QDateTime>

// alphaevent - detect a single alpha particle event and record the time

class alphaevent : public QObject
{
    Q_OBJECT
public:
    explicit alphaevent( int pixelCount, QObject *parent = 0);

    QDateTime Time() const { return m_time; }
    int PixelCount() const { return m_pixelCount; }

protected:
    QDateTime m_time;
    int m_pixelCount;

signals:

public slots:

};

#endif // ALPHAEVENT_H
