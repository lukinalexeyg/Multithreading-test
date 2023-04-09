#ifndef ELEMENTMODELITEM_H
#define ELEMENTMODELITEM_H

#include <QMetaType>

class ElementModelItem {
public:
    ElementModelItem();
    ElementModelItem(const QString &text, bool locked);

    void set(const QString &text, bool locked);

    QString text() const    { return m_text; }
    bool locked() const     { return m_locked; }

private:
    QString m_text;
    bool m_locked;
};

Q_DECLARE_METATYPE(ElementModelItem*)

#endif // ELEMENTMODELITEM_H
