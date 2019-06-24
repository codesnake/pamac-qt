#include "Transaction.h"
#include "Utils.h"
#include "QQuickDialog.h"
#include <QQmlComponent>
#include <QtDBus/QtDBus>



PamacQt::Transaction::Transaction(Database *db, QObject *parent):QObject(parent),
    m_transaction(std::shared_ptr<PamacTransaction>(pamac_transaction_new(*db),g_object_unref)),
    m_database(db)
{
    init();
}

void PamacQt::Transaction::startWritePamacConfig(const QVariantMap &map){
    GHashTable* tabl = g_hash_table_new(g_str_hash,
                                        g_str_equal);
    auto keys = Utils::qStringListToCStringVector(map.keys());
    for(auto& el :keys){
        auto value = map[el];
        if(value.type()==QVariant::Int){
            value.convert(QVariant::ULongLong);
        }

        g_hash_table_insert(tabl,el,Utils::qVariantToGVariant(value));
    }

    pamac_transaction_start_write_pamac_config(m_transaction.get(),tabl);
    for(auto &el :keys){
        delete el;
    }

}

void PamacQt::Transaction::startWriteAlpmConfig(const QVariantMap &map){
    GHashTable* tabl = g_hash_table_new(g_str_hash,
                                        g_str_equal);
    auto keys = Utils::qStringListToCStringVector(map.keys());
    for(auto& el :keys){
        auto value = map[el];
        if(value.type()==QVariant::Int){
            value.convert(QVariant::ULongLong);
        }

        g_hash_table_insert(tabl,el,Utils::qVariantToGVariant(value));
    }

    pamac_transaction_start_write_alpm_config(m_transaction.get(),tabl);
    for(auto &el :keys){
        delete el;
    }

}

QmlFuture PamacQt::Transaction::getBuildFiles(const QString &pkgname){
    auto future = new QmlFutureImpl;
    pamac_transaction_get_build_files(m_transaction.get(),pkgname.toUtf8(),
                                      [](GObject* parent,GAsyncResult* res,gpointer futurePtr){
        auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);
        if(future->isRunning()){

            auto transaction = reinterpret_cast<PamacTransaction*>(parent);
            int length = 0;
            gchar** result = pamac_transaction_get_build_files_finish(transaction,res,&length);
            QStringList resList;

            for(int i =0;i<length;i++){
                QFileInfo file(QString::fromUtf8(result[i]));
                resList.append(file.fileName());
                g_free(result[i]);
            }

            g_free(result);
            future->setFuture(QVariant::fromValue(resList));
        }
        else {
            delete future;
        }
    },future);
    return QmlFuture(future);

}

void PamacQt::Transaction::start(const QStringList& toInstall, const QStringList& toRemove, const QStringList& toLoad,
                                 const QStringList& toBuild, const QStringList& tempIgnore, const QStringList& overwriteFiles)
{
    using Utils::qStringListToCStringVector;

    auto install = qStringListToCStringVector(toInstall);
    auto remove = qStringListToCStringVector(toRemove);
    auto load = qStringListToCStringVector(toLoad);
    auto build = qStringListToCStringVector(toBuild);
    auto ignore = qStringListToCStringVector(tempIgnore);
    auto overwrite = qStringListToCStringVector(overwriteFiles);

    pamac_transaction_start(m_transaction.get(),install.data(),int(install.size()),
                            remove.data(),int(remove.size()),
                            load.data(),int(load.size()),
                            build.data(),int(build.size()),
                            ignore.data(),int(ignore.size()),
                            overwrite.data(),int(overwrite.size()));
    setProperty("started",true);

}

void PamacQt::Transaction::startSysupgrade(bool forceRefresh, bool enableDowngrade, const QStringList &tempIgnore, const QStringList &overwriteFiles)
{
    using Utils::qStringListToCStringVector;

    auto ignore = qStringListToCStringVector(tempIgnore);
    auto overwrite = qStringListToCStringVector(overwriteFiles);

    pamac_transaction_start_sysupgrade(m_transaction.get(),forceRefresh,enableDowngrade,
                                       ignore.data(),int(ignore.size()),
                                       overwrite.data(),int(overwrite.size()));
    setProperty("started",true);
}

void PamacQt::Transaction::setDatabase(PamacQt::Database *database)
{
    if (m_database == database) {
        return;
    }

    if (m_transaction==nullptr){
        m_transaction = std::shared_ptr<PamacTransaction>(pamac_transaction_new(*database),g_object_unref);
    } else{
        pamac_transaction_set_database(m_transaction.get(),*database);
    }
    m_database = database;
    init();

    emit databaseChanged(m_database);
}
void PamacQt::Transaction::init()
{
    //methods override
    PAMAC_TRANSACTION_GET_CLASS(m_transaction.get())->ask_commit=
            [](PamacTransaction* self, PamacTransactionSummary* summary)->gboolean{

        QQuickDialog dlg(QUrl("qrc:/src/qml/TransactionSummaryDialog.qml"));
        dlg.setWindowTitle("Transaction Summary");

        dlg.setQQuickWidgetProperty("summary",QVariant::fromValue(TransactionSummary(summary)));

        return dlg.exec();


    };
    PAMAC_TRANSACTION_GET_CLASS(m_transaction.get())->choose_optdeps=
            [](PamacTransaction* self, const gchar* pkgname, gchar** optdeps, int optdeps_length1)->GList*{
        QQuickDialog dlg(QUrl("qrc:/src/qml/TransactionOptDependsDialog.qml"));
        dlg.setWindowTitle("Choose optional dependecies for"+QString::fromUtf8(pkgname));

        QStringList lst = Utils::cStringArrayToQStringList(optdeps,optdeps_length1);

        dlg.setQQuickWidgetProperty("pkgName",QString::fromUtf8(pkgname));
        dlg.setQQuickWidgetProperty("opDeps",lst);

        dlg.exec();

        QVariant var = dlg.qQuickWidgetProperty("opted");
        GList* result = nullptr;

        if(var.canConvert(QMetaType::QVariantList)){
            QVariantList varList = var.toList();

            int minLength = std::min(optdeps_length1,varList.length());

            for(int i =0;i<minLength;i++){
                if(varList[i].toBool()){
                    char* res = new char[lst[i].toUtf8().length()];
                    std::memcpy(res,lst[i].toUtf8(),lst[i].toUtf8().size());
                    result = g_list_prepend(result,res);
                }
            }
            g_list_foreach(result,[](void* data,void*){
                std::cout<<((char*)data)<<std::endl;
            },nullptr);

        }
        return result;
    };
    PAMAC_TRANSACTION_GET_CLASS(m_transaction.get())->choose_provider=
            [](PamacTransaction* self, const gchar* depend, gchar** providers, int providers_length1)->gint{
        std::cout<<(depend)<<std::endl;
        return 0;
    };


    g_signal_connect(m_transaction.get(),"get_authorization_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool authorized,Transaction* t){
                         Q_UNUSED(obj);
                         emit t->getAuthorizationFinished(authorized);
                     }),this);


    g_signal_connect(m_transaction.get(),"finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool success,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Finished with result: "<<success<<std::endl;
                         emit t->finished(success);
                     }),this);

    g_signal_connect(m_transaction.get(),"emit_action",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* action,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Action: "<<action<<std::endl;
                         emit t->emitAction(QString::fromUtf8(action));
                     }),this);

    g_signal_connect(m_transaction.get(),"emit_action_progress",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* action,char* status,double progress,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Progress: "<<action<<status<<progress<<std::endl;

                         emit t->emitActionProgress(QString::fromUtf8(action),QString::fromUtf8(status),progress);
                     }),this);

    g_signal_connect(m_transaction.get(),"emit_error",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* message,char** details,int size,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Error: "<<message<<std::endl<<"Details:"<<std::endl;
                         for(int i =0;i<size;i++){
                             std::cout<<"- "<<details[i]<<std::endl;
                         }
                         emit t->emitError(QString::fromUtf8(message),
                         Utils::cStringArrayToQStringList(details,size));
                     }),this);
    g_signal_connect(m_transaction.get(),"emit_warning",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* warning,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Warning: "<<warning<<std::endl;
                         emit t->emitWarning(QString::fromUtf8(warning));
                     }),this);
    g_signal_connect(m_transaction.get(),"start_preparing",
                     reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Started preparing"<<std::endl;
                         emit t->startPreparing();
                     }),this);
    g_signal_connect(m_transaction.get(),"stop_preparing",
                     reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Stopped preparing"<<std::endl;
                         emit t->stopPreparing();
                     }),this);

    g_signal_connect(m_transaction.get(),"emit_script_output",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* message,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Script: "<<message<<std::endl;
                         emit t->emitScriptOutput(QString::fromUtf8(message));
                     }),this);

    g_signal_connect(m_transaction.get(),"important_details_outpout",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool message,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Important details: "<<message<<std::endl;
                         emit t->importantDetailsOutput(message);
                     }),this);
    g_signal_connect(m_transaction.get(),"sysupgrade_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool success,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"System upgrade finished with rusult: "<<success<<std::endl;
                         emit t->finished(success);
                     }),this);

    g_signal_connect(m_transaction.get(),"write_alpm_config_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool checkspace,Transaction* t){
                         Q_UNUSED(obj);
                         emit t->writeAlpmConfigFinished();
                     }),this);

    g_signal_connect(m_transaction.get(),"write_pamac_config_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool recurse, uint64_t refresh_period, bool no_update_hide_icon,
                                                 bool enable_aur, char* aur_build_dir, bool check_aur_updates,
                                                 bool check_aur_vcs_updates, bool download_updates
                                                 ,Transaction* t){
                         Q_UNUSED(obj);
                         emit t->writePamacConfigFinished();
                     }),this);




    connect(this,&Transaction::startPreparing,[=](){this->setProperty("indeterminate",true);});
    connect(this,&Transaction::stopPreparing,[=](){this->setProperty("indeterminate",false);});

    connect(this,&Transaction::emitActionProgress,[=](const QString& action,const QString& status,double progress){
        this->setProperty("action",action);
        this->setProperty("progress",progress);

        QString details = this->property("details").toString();
        if(details.endsWith(action+"\n")){
            return;
        }
        details.append(action+"\n");
        this->setProperty("details",details);

    });
    connect(this,&Transaction::emitAction,[=](const QString& action){
        this->setProperty("action",action);
        QString details = this->property("details").toString();
        details.append(action+"\n");
        this->setProperty("details",details);
    });
    connect(this,&Transaction::finished,[=](bool success){
        Q_UNUSED(success);
        this->setProperty("progress",0);
        this->setProperty("started",false);
    });
    connect(this,&Transaction::emitScriptOutput,[=](QString message){
        QString details = this->property("details").toString();
        details.append(message+"\n");
        this->setProperty("details",details);
    });


}
