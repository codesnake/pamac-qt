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
