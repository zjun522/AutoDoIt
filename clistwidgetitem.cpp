#include "clistwidgetitem.h"

#include <utility>

CListWidgetItem::CListWidgetItem(const QString &text, std::shared_ptr<void> ptr) :
    QListWidgetItem(text),
    m_ptr(std::move(ptr))
{
    this->setSizeHint(QSize(0, 30));
}

std::shared_ptr<void> CListWidgetItem::ptr() const
{
    return m_ptr;
}

void CListWidgetItem::setPtr(const QString& text, const std::shared_ptr<void> &ptr)
{
    this->setText(text);
    m_ptr = ptr;
}
