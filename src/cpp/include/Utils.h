#pragma once
#include "glib.h"
#include <QStringList>
#include <vector>
#include <QVariantList>

namespace PamacQt {
namespace Utils {
std::vector<char*> qStringListToCStringVector(const QStringList&);
QStringList cStringArrayToQStringList(char** arr,int size);
GList* qVariantListToGList(const QVariantList& lst);
GVariant* qVariantToGVariant(const QVariant&);
} // namespace Utils
} // namespace PamacQt
