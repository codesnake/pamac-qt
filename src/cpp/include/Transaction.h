#pragma once
#include <pamac.h>
#include <QObject>
#include <memory>
#include <cstring>
#include "Database.h"
#include "PackageList.h"
namespace PamacQt{
class Database;
class TransactionSummary{
    Q_GADGET
    Q_PROPERTY(PackageList toInstall READ toInstall CONSTANT)
    Q_PROPERTY(PackageList toRemove READ toRemove CONSTANT)
    Q_PROPERTY(PackageList toReinstall READ toReinstall CONSTANT)
    Q_PROPERTY(PackageList toBuild READ toBuild CONSTANT)
    Q_PROPERTY(PackageList toUpgrade READ toUpgrade CONSTANT)

public:
    TransactionSummary(PamacTransactionSummary* s):summary(std::shared_ptr<PamacTransactionSummary>(s,g_object_unref)){}
    TransactionSummary()=default;
    PackageList toInstall() const{
        return PackageList(pamac_transaction_summary_get_to_install(summary.get()));
    }
    PackageList toRemove() const
    {
        return PackageList(pamac_transaction_summary_get_to_remove(summary.get()));
    }
    PackageList toReinstall() const
    {
        return PackageList(pamac_transaction_summary_get_to_reinstall(summary.get()));
    }
    PackageList toBuild() const
    {
        return PackageList(pamac_transaction_summary_get_to_build(summary.get()));
    }
    PackageList toUpgrade() const
    {
        return PackageList(pamac_transaction_summary_get_to_upgrade(summary.get()));
    }
private:
    std::shared_ptr<PamacTransactionSummary> summary;

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

public:
    Transaction(Database* db,QObject* parent = nullptr);
    Transaction(QObject * parent = nullptr):QObject(parent){}
    Q_INVOKABLE void startGetAuthorization(){
        pamac_transaction_start_get_authorization(m_transaction.get());
    }

    Q_INVOKABLE bool getLock(){
        return pamac_transaction_get_lock(m_transaction.get());
    }
    Q_INVOKABLE bool unlock(){
        return pamac_transaction_unlock(m_transaction.get());
    }
    Q_INVOKABLE void startWritePamacConfig(const QVariantMap &map);


    Database* database() const
    {
        return m_database;
    }

    Q_INVOKABLE void start(const QStringList& toInstall = QStringList(), const QStringList& toRemove = QStringList(), const QStringList& toLoad = QStringList(),
                           const QStringList& toBuild = QStringList(), const QStringList& tempIgnore = QStringList(), const QStringList& overwriteFiles = QStringList());
    Q_INVOKABLE void startSysupgrade(bool forceRefresh,bool enableDowngrade,const QStringList& tempIgnore = QStringList(),const QStringList& overwriteFiles = QStringList());
public slots:


    void setDatabase(Database* database)
    {
        if (m_database == database)
            return;

        if (m_transaction==nullptr){
            m_transaction = std::shared_ptr<PamacTransaction>(pamac_transaction_new(*database),g_object_unref);
        } else{
            pamac_transaction_set_database(m_transaction.get(),*database);
        }
        m_database = database;
        init();

        emit databaseChanged(m_database);
    }

signals:
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

    void databaseChanged(Database* database);
    void indeterminateChanged(bool indeterminate);

    void actionChanged(QString action);

    void progressChanged(double progress);

    void startedChanged(bool started);

    void detailsChanged(QString details);

private:
    void init();
    std::shared_ptr<PamacTransaction> m_transaction;
    Database* m_database = nullptr;

    QString m_action;
    double m_progress = 0;
    QString m_details;

    bool m_indeterminate = false;
    bool m_started = false;
};
} //namespace PamacQt

Q_DECLARE_METATYPE(PamacQt::TransactionSummary)
