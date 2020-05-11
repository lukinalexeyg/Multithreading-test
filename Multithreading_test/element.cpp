#include "element.h"



Element::Element(const QString &userText) :
    m_className(QString(__func__)),
    m_userText(userText),
    m_counterValue(0)
{
}



QString Element::visibleText()
{
    return QString("%1 %2 %3")
            .arg(m_className)
            .arg(m_userText)
            .arg(m_counterValue);
}



#define ELEMENT_IMPL(n) \
    Element##n::Element##n(const QString &userText) : Element(userText) \
    { m_className = QString(__func__); }

ELEMENT_IMPL(1)
ELEMENT_IMPL(2)
ELEMENT_IMPL(3)
