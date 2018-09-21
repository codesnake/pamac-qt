#ifndef ALPMPKG_H
#define ALPMPKG_H
#include "pamac.h"
#include <QObject>
#include <QUrl>
#include <QMetaType>

#define PAMAC_QT_STRING_PROPERTY(name,method)\
    Q_PROPERTY(QString name READ name)\
    QString name() const\
{\
    return QString::fromUtf8(method);\
}

#define PAMAC_QT_URL_PROPERTY(name,method)\
    Q_PROPERTY(QUrl name READ name)\
    QUrl name() const\
{\
    return QUrl(method);\
}

#define PAMAC_QT_ULONG_PROPERTY(name,method)\
    Q_PROPERTY(ulong name READ name)\
    ulong name() const\
{\
    return method;\
}

class AlpmPkg
{
    Q_GADGET
private:
    PamacPackage* package;

public:

    AlpmPkg():package(new PamacPackage()){}

    AlpmPkg(PamacPackage* package):package(package){}

    PAMAC_QT_STRING_PROPERTY(name,pamac_package_get_name(package))

    PAMAC_QT_STRING_PROPERTY(appName,pamac_package_get_app_name(package))

    PAMAC_QT_STRING_PROPERTY(version,pamac_package_get_version(package))

    PAMAC_QT_STRING_PROPERTY(installedVersion,pamac_package_get_installed_version(package))

    PAMAC_QT_STRING_PROPERTY(desc,pamac_package_get_desc(package))

    PAMAC_QT_URL_PROPERTY(iconUrl,pamac_package_get_icon(package))

    PAMAC_QT_STRING_PROPERTY(repo,pamac_package_get_repo(package))

    PAMAC_QT_ULONG_PROPERTY(size,pamac_package_get_size(package))


    operator PamacPackage*()
    {
        return package;
    }

};

class AlpmPkgDetails
{
    Q_GADGET

    Q_PROPERTY(QStringList licenses READ licenses)
    Q_PROPERTY(QStringList depends READ depends)
    Q_PROPERTY(QStringList optDepends READ optDepends)


public:

    AlpmPkgDetails():m_details(new PamacPackageDetails()){}

    AlpmPkgDetails(PamacPackageDetails* details):m_details(details){}

    PAMAC_QT_STRING_PROPERTY(name,pamac_package_details_get_name(m_details))
    PAMAC_QT_STRING_PROPERTY(appName,pamac_package_details_get_app_name(m_details))
    PAMAC_QT_STRING_PROPERTY(desc,pamac_package_details_get_desc(m_details))

    PAMAC_QT_STRING_PROPERTY(longDesc,pamac_package_details_get_long_desc(m_details))
    PAMAC_QT_STRING_PROPERTY(repo,pamac_package_details_get_repo(m_details))
    PAMAC_QT_STRING_PROPERTY(packager,pamac_package_details_get_packager(m_details))

    PAMAC_QT_URL_PROPERTY(url,pamac_package_details_get_url(m_details))

    PAMAC_QT_URL_PROPERTY(iconUrl,pamac_package_details_get_icon(m_details))

    PAMAC_QT_URL_PROPERTY(screenshotUrl,pamac_package_details_get_screenshot(m_details))

    PAMAC_QT_STRING_PROPERTY(installDate,pamac_package_details_get_installdate(m_details))

    PAMAC_QT_STRING_PROPERTY(buildDate,pamac_package_details_get_builddate(m_details))

    PAMAC_QT_STRING_PROPERTY(installReason,pamac_package_details_get_reason(m_details))

    PAMAC_QT_STRING_PROPERTY(installedVersion,pamac_package_details_get_installed_version(m_details))

    QStringList depends() const
    {
        QStringList result;
        auto licensesGList = pamac_package_details_get_depends(m_details);

        for(auto el = g_list_first(licensesGList);el!=nullptr;el=el->next){
            result.append(static_cast<const char*>(el->data));
        }
        return result;
    }
    QStringList optDepends() const
    {
        QStringList result;
        auto licensesGList = pamac_package_details_get_optdepends(m_details);

        for(auto el = g_list_first(licensesGList);el!=nullptr;el=el->next){
            result.append(static_cast<const char*>(el->data));
        }
        return result;
    }
    QStringList licenses() const
    {
        QStringList result;
        auto licensesGList = pamac_package_details_get_licenses(m_details);

        for(auto el = g_list_first(licensesGList);el!=nullptr;el=el->next){
            result.append(static_cast<const char*>(el->data));
        }
        return result;
    }
private:
    PamacPackageDetails* m_details;
};

Q_DECLARE_METATYPE(AlpmPkg)
Q_DECLARE_METATYPE(AlpmPkgDetails)

#endif // ALPMPKG_H
