#pragma once
#include <pamac.h>
#include <QFileInfo>
#include <QObject>
#include <memory>
#include <cstring>
#include "Database.h"
#include <QJSValue>

#include <SnapPackage.h>
namespace LibQPamac{
class Database;
class TransactionSummary{
    Q_GADGET
    Q_PROPERTY(QVariantList toInstall READ toInstall CONSTANT)
    Q_PROPERTY(QVariantList toRemove READ toRemove CONSTANT)
    Q_PROPERTY(QVariantList toReinstall READ toReinstall CONSTANT)
    Q_PROPERTY(QVariantList toBuild READ toBuild CONSTANT)
    Q_PROPERTY(QVariantList toUpgrade READ toUpgrade CONSTANT)

public:
    TransactionSummary(PamacTransactionSummary* s):summary(s){}
    TransactionSummary()=default;
    QVariantList toInstall() const{
        return Utils::gSListToQList<QVariant>(pamac_transaction_summary_get_to_install(summary),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package));
    }
    QVariantList toRemove() const
    {
        return Utils::gSListToQList<QVariant>(pamac_transaction_summary_get_to_remove(summary),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package));
    }
    QVariantList toReinstall() const
    {
        return Utils::gSListToQList<QVariant>(pamac_transaction_summary_get_to_reinstall(summary),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package));
    }
    QVariantList toBuild() const
    {
        return Utils::gSListToQList<QVariant>(pamac_transaction_summary_get_to_build(summary),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package));
    }
    QVariantList toUpgrade() const
    {
        return Utils::gSListToQList<QVariant>(pamac_transaction_summary_get_to_upgrade(summary),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package));
    }
private:
    PamacTransactionSummary* summary;

};
class Transaction : public QObject
{

    Q_OBJECT
    Q_PROPERTY(Database* database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(bool indeterminate MEMBER m_indeterminate NOTIFY indeterminateChanged)
    Q_PROPERTY(QString action MEMBER m_action NOTIFY actionChanged)
    Q_PROPERTY(double progress MEMBER m_progress NOTIFY progressChanged)
    Q_PROPERTY(bool started MEMBER m_started NOTIFY startedChanged)
    Q_PROPERTY(QString details MEMBER m_details NOTIFY detailsChanged)
    Q_PROPERTY(QJSValue requestCommit WRITE setRequestCommit FINAL)
    Q_PROPERTY(QJSValue requestImportKey WRITE setRequestImportKey FINAL)
    Q_PROPERTY(QJSValue requestChooseProvider WRITE setRequestChooseProvider FINAL)
    Q_PROPERTY(QJSValue requestOptDepends WRITE setRequestOptDepends FINAL)
    Q_PROPERTY(QJSValue requestEditBuildFiles WRITE setRequestEditBuildFiles FINAL)

public:
    Transaction(QObject * parent = nullptr):QObject(parent){}
    ~Transaction(){quitDaemon();}

    inline Q_INVOKABLE void getAuthorization(){
        pamac_transaction_get_authorization(m_handle);
    }


    Database* database() const
    {
        return m_database;
    }

    Q_INVOKABLE void generateMirrorsList(const QString& country = "all"){
        pamac_transaction_generate_mirrors_list(m_handle,country.toUtf8());
    }
    Q_INVOKABLE void cleanCache(){
        pamac_transaction_clean_cache(m_handle);
    }
    Q_INVOKABLE void quitDaemon(){
        pamac_transaction_quit_daemon(m_handle);
    }

    Q_INVOKABLE bool run(const QStringList& toInstall = QStringList(), const QStringList& toRemove = QStringList(), const QStringList& toLoad = QStringList(),
                           const QStringList& toBuild = QStringList(), const QStringList& tempIgnore = QStringList(), const QStringList& overwriteFiles = QStringList(),
                         const QList<SnapPackage>& toInstallSnap = QList<SnapPackage>(), const QList<SnapPackage>& toRemoveSnap = QList<SnapPackage>());

    Q_INVOKABLE bool runSysupgrade(bool forceRefresh);


public Q_SLOTS:

    void setDatabase(Database* database);

    void setRequestCommit(const QJSValue& requestCommit)
    {
        if(requestCommit.isCallable()) {
            m_requestCommit = requestCommit;
        }
    }

    void setRequestOptDepends(const QJSValue& requestOptDepends)
    {
        if(requestOptDepends.isCallable()){
            m_requestOptDepends = requestOptDepends;
        }
    }

    void setRequestImportKey(const QJSValue& requestImportKey)
    {
        if(requestImportKey.isCallable()) {
            m_requestImportKey = requestImportKey;
        }
    }

    void setRequestChooseProvider(const QJSValue& requestChooseProvider)
    {
        if(requestChooseProvider.isCallable()) {
            m_requestChooseProvider = requestChooseProvider;
        }
    }

    void setRequestEditBuildFiles(const QJSValue &requestEditBuildFiles){
        if(requestEditBuildFiles.isCallable()){
            m_requestEditBuildFiles = requestEditBuildFiles;
        }
    }


Q_SIGNALS:
    void getAuthorizationFinished(bool authorized);
    void finished(bool success);
    void emitAction(const QString& emitAction);
    void emitActionProgress(const QString& emitAction,const QString& status, double progress);
    void emitError(const QString& error,const QStringList& details);
    void emitWarning(const QString& message);
    void startPreparing();
    void stopPreparing();
    void emitScriptOutput(const QString& message);
    void importantDetailsOutput(bool imporant);
    void writeAlpmConfigFinished();
    void writePamacConfigFinished();

    void startWaiting ();
    void stopWaiting ();
    void startDownloading ();
    void stopDownloading ();
    void startBuilding ();
    void stopBuilding ();
    void sysupgradeFinished (bool success);
    void setPkgreasonFinished ();
    void startGeneratingMirrorsList ();
    void generateMirrorsListFinished ();
    void cleanCacheFinished ();
    void cleanBuildFilesFinished ();
    void downloadingUpdatesFinished();



    void databaseChanged(Database* database);

    void indeterminateChanged(bool indeterminate);

    void actionChanged(QString action);

    void progressChanged(double progress);

    void startedChanged(bool started);

    void detailsChanged(QString details);


private:
    void init();
    Database* m_database = nullptr;
    PamacTransaction* m_handle;

    QString m_action;
    double m_progress = 0;
    QString m_details;

    bool m_indeterminate = false;
    bool m_started = false;
    QJSValue m_requestCommit;
    QJSValue m_requestOptDepends;
    QJSValue m_requestImportKey;
    QJSValue m_requestChooseProvider;
    QJSValue m_requestEditBuildFiles;
};
} //namespace LibQPamac

Q_DECLARE_METATYPE(LibQPamac::TransactionSummary)
