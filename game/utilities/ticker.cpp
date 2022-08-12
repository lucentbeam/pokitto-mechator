#include "ticker.h"

void GenericTicker::reset(uint8_t count) { m_count = count; }

void GenericTicker::update() { if (m_count > 0) m_count--; }

bool GenericTicker::ready() const { return m_count == 0; }
