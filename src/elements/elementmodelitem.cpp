#include "elementmodelitem.h"



ElementModelItem::ElementModelItem() :
    m_text(QString()),
    m_locked(false)
{
}



ElementModelItem::ElementModelItem(const QString &text, const bool locked) :
    m_text(text),
    m_locked(locked)
{
}



void ElementModelItem::set(const QString &text, const bool locked)
{
    m_text = text;
    m_locked = locked;
}
