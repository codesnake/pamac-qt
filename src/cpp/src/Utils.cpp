#include "Utils.h"
#include <cstring>

std::vector<char *> PamacQt::Utils::qStringListToCStringVector(const QStringList &inList)
{
    std::vector<char*> cstringVector;

    for(const auto& string : inList){
        int length = string.toUtf8().length();
        auto* ch = new char[ulong(length)+1];
        std::memcpy(ch,string.toUtf8().data(),ulong(length));
        ch[length]='\0';
        cstringVector.push_back(ch);
    }

    return cstringVector;

}

QStringList PamacQt::Utils::cStringArrayToQStringList(char **arr, int size)
{
    QStringList result;
    for(int i =0;i<size;i++){
        result.append(QString::fromUtf8(arr[i]));
    }
    return result;
}

GList *PamacQt::Utils::qVariantListToGList(const QVariantList &lst)
{
    GList* result = nullptr;
    for(auto el:lst){
        result = g_list_append(result,el.data());
    }
    return result;
}

GVariant *PamacQt::Utils::qVariantToGVariant(const QVariant& value)
{
    if (!value.isValid())
        return nullptr;

    switch (value.type()) {
    case QVariant::Invalid:
        return nullptr;
    case QVariant::Bool:
        return g_variant_new_boolean(value.toBool());
    case QVariant::Int:
        return g_variant_new_int32(value.toInt());
    case QVariant::UInt:
        return g_variant_new_uint32(value.toUInt());
    case QVariant::LongLong:
        return g_variant_new_int64(value.toLongLong());
    case QVariant::ULongLong:
        return g_variant_new_uint64(value.toULongLong());
    case QVariant::Double:
        return g_variant_new_double(value.toDouble());
    case QVariant::Char:
        return g_variant_new_byte(value.toChar().toLatin1());
    case QVariant::String:
        return g_variant_new_string(value.toString().toUtf8());
    case QVariant::ByteArray:
        return g_variant_new_bytestring(value.toByteArray());
    default:
        return nullptr;
    }
}

QStringList PamacQt::Utils::gListToQStringList(GList *list,bool freeOriginal)
{
    QStringList result;
    for(auto el = list;el!=nullptr;el=el->next)
    {
        result.append(QString::fromUtf8(static_cast<char*>(el->data)));
    }

    if(freeOriginal){
        g_list_free_full(list,g_free);
    }

    return result;
}
