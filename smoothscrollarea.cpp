#include "smoothscrollarea.h"

SmoothScrollArea::SmoothScrollArea(QWidget* parent) : QScrollArea(parent) {
    setWidgetResizable(true);

    scroller = QScroller::scroller(viewport());

    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.2); // Adjust deceleration
    properties.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.8);    // Adjust speed

    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);

    scroller->setScrollerProperties(properties);

    /*setStyleSheet(R"(
    QScrollBar:vertical {
        border: none;
        border-radius: 5px;
        background: #DFDFDF;
        width: 10px;
        margin: 2px 0px 0px 0px;
    }
    QScrollBar::handle:vertical {
        background-color: #B7B7B7;
        min-height: 20px;
        border-radius: 5px;
    }
    QScrollBar::handle:vertical:hover {
        background-color: #919191;
    }
    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
        height: 0px;
    }
    QScrollBar::add-page:vertical,
    QScrollBar::sub-page:vertical {
        background: none;
    }

    QScrollBar:horizontal {
        border: none;
        background: #DFDFDF;
        height: 10px;
        margin: 0px 0px 0px 0px;
    }
    QScrollBar::handle:horizontal {
        background-color: #A2A2A2;
        min-width: 20px;
        border-radius: 5px;
    }
    QScrollBar::handle:horizontal:hover {
        background-color: #919191;
    }
    QScrollBar::add-line:horizontal,
    QScrollBar::sub-line:horizontal {
        height: 0px;
    }
    QScrollBar::add-page:horizontal,
    QScrollBar::sub-page:horizontal {
        background: none;
    }
)");*/
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void SmoothScrollArea::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        int deltaX = event->angleDelta().y();
        QPropertyAnimation *hAnimation = new QPropertyAnimation(horizontalScrollBar(), "value", this);
        hAnimation->setDuration(qMin(400, 100 + abs(deltaX)));
        hAnimation->setStartValue(horizontalScrollBar()->value());
        hAnimation->setEndValue(horizontalScrollBar()->value() - deltaX);
        hAnimation->setEasingCurve(QEasingCurve::OutCubic);
        hAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else {
        int delta = event->angleDelta().y();
        QPropertyAnimation *animation = new QPropertyAnimation(verticalScrollBar(), "value", this);
        animation->setDuration(qMin(400, 100 + abs(delta)));
        animation->setStartValue(verticalScrollBar()->value());
        animation->setEndValue(verticalScrollBar()->value() - delta);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
