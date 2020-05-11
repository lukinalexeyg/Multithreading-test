#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>

class Element
{
public:
    Element(const QString &userText = QString());

    inline QString className()                      { return m_className; }

    inline QString userText()                       { return m_userText; }
    inline void setUserText(const QString &text)    { m_userText = text; }

    inline int counterValue()                       { return m_counterValue; }
    inline void incCounter()                        { ++m_counterValue; }

    QString visibleText();

private:
    friend class Element1;
    friend class Element2;
    friend class Element3;

    QString m_className;
    QString m_userText;
    int m_counterValue;
};

#define ELEMENT_DECL(n) \
    class Element##n : public Element \
    { public: Element##n(const QString &userText = QString()); };

ELEMENT_DECL(1)
ELEMENT_DECL(2)
ELEMENT_DECL(3)

#endif // ELEMENT_H
