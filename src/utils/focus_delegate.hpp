#ifndef FOCUS_DELEGATE_HPP
#define FOCUS_DELEGATE_HPP

#include <QStyledItemDelegate>
#include <QPainter>

class FocusDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt = option;
        opt.state &= ~QStyle::State_HasFocus;

        QStyledItemDelegate::paint(painter, opt, index);
    }
};

#endif // FOCUS_DELEGATE_HPP