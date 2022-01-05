#ifndef ELEMENT_H
#define ELEMENT_H

#include <QSharedPointer>

class Element
{
public:
    Element(const QString &userText = QString());

    QString className() const               { return m_className; }

    QString userText() const                { return m_userText; }
    void setUserText(const QString &text)   { m_userText = text; }

    int counterValue() const                { return m_counterValue; }
    void incCounter()                       { ++m_counterValue; }

    QString displayedText() const;

private:
    friend class Element1;
    friend class Element2;
    friend class Element3;

    QString m_className;
    QString m_userText;
    int m_counterValue;
};

typedef QSharedPointer<Element> ElementPtr;

#define ELEMENT_DECL(n) \
    class Element##n : public Element \
    { public: Element##n(const QString &userText = QString()); };

ELEMENT_DECL(1)
ELEMENT_DECL(2)
ELEMENT_DECL(3)

#endif // ELEMENT_H
