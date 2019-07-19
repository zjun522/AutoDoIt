#ifndef CLISTWIDGETITEM_H
#define CLISTWIDGETITEM_H

#include <QListWidgetItem>
#include <memory>

class CListWidgetItem : public QListWidgetItem
{
public:
    CListWidgetItem(const QString &text, std::shared_ptr<void> ptr);

    std::shared_ptr<void> ptr() const;
    void setPtr(const QString& text, const std::shared_ptr<void> &ptr);

private:
    std::shared_ptr<void> m_ptr;
};

#endif // CLISTWIDGETITEM_H
