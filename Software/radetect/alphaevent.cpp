#include "alphaevent.h"

alphaevent::alphaevent(int pixelCount, QObject *parent) :
    QObject(parent)
{
    m_pixelCount = pixelCount;
    m_time = QDateTime::currentDateTime();
}
