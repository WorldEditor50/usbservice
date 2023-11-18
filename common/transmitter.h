#ifndef TRANSITTER_H
#define TRANSITTER_H
#include <QObject>
#include <QImage>

class Transmitter : public QObject
{
    Q_OBJECT
private:
    Transmitter():QObject(nullptr){}
signals:
    void sendText(const QString &text);
    void sendImage(const QImage &img);
public:
    inline static Transmitter& instance()
    {
        static Transmitter transitter;
        return transitter;
    }
};


#endif // TRANSITTER_H
