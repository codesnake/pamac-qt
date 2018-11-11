#pragma once
#include <pamac.h>
#include <QObject>
#include <memory>
#include <cstring>
#include "Database.h"
namespace PamacQt{
class Database;
class Transaction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Database* database READ database WRITE setDatabase NOTIFY databaseChanged)
public:
    Transaction(Database* db,QObject* parent = nullptr);
    Transaction(QObject * parent = nullptr):QObject(parent){}
    Q_INVOKABLE void startGetAuthorization(){
        pamac_transaction_start_get_authorization(transaction.get());
    }
    Q_INVOKABLE bool getLock(){
        return pamac_transaction_get_lock(transaction.get());
    }
    Q_INVOKABLE bool unlock(){
        return pamac_transaction_unlock(transaction.get());
    }
    Q_INVOKABLE void startWritePamacConfig(const QVariantHash &hash){
        GHashTable* tabl = nullptr;

        for(QVariantHash::const_iterator it = hash.constBegin();it!=hash.constEnd();++it){
            void* key = malloc(it.key().toUtf8().size());
            std::memcpy(key,it.key().toUtf8(),it.key().toUtf8().size());

            void* value = malloc(QMetaType::sizeOf(it.value().type()));
            std::memcpy(value,it.value().data(),QMetaType::sizeOf(it.value().type()));

            g_hash_table_insert(tabl,key,value);
        }
    }

    Database* database() const
    {
        return m_database;
    }

public slots:


    void setDatabase(Database* database)
    {
        if (m_database == database)
            return;

        m_database = database;
        if (transaction.get()==nullptr){
            transaction = std::shared_ptr<PamacTransaction>(pamac_transaction_new(*m_database),g_object_unref);
            init();
        } else{
            pamac_transaction_set_database(transaction.get(),*database);
        }

        emit databaseChanged(m_database);
    }

signals:
    Q_INVOKABLE void getAuthorizationFinished(bool authorized);
    void databaseChanged(Database* database);

private:
    void init();
    std::shared_ptr<PamacTransaction> transaction;
    Database* m_database;

};
} //namespace PamacQt
