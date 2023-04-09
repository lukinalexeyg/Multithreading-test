#include "element.h"



Element::Element(const QString &userText) :
    m_className(QString(__func__)),
    m_userText(userText),
    m_counterValue(0)
{
}



QString Element::displayedText() const
{
    return QStringLiteral("%1 %2 %3")
            .arg(m_className, m_userText)
            .arg(m_counterValue);
}



#define ELEMENT_IMPL(n) \
    Element##n::Element##n(const QString &userText) : Element(userText) \
    { m_className = QLatin1String(__func__); }

ELEMENT_IMPL(1)
ELEMENT_IMPL(2)
ELEMENT_IMPL(3)
