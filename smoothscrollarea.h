#ifndef SMOOTHSCROLLAREA_H
#define SMOOTHSCROLLAREA_H

#include <QScrollArea>
#include <QWheelEvent>
#include <QScrollBar>
#include <QScroller>
#include <QPropertyAnimation>

class SmoothScrollArea : public QScrollArea {
    Q_OBJECT
public:
    explicit SmoothScrollArea(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

    QScroller *scroller;
};

#endif // SMOOTHSCROLLAREA_H
