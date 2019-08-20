#include "Transaction.h"
#include "Utils.h"
#include <QQmlEngine>
#include <atomic>
#include <QVariant>


void LibQPamac::Transaction::startWritePamacConfig(const QVariantMap &map){
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

    pamac_transaction_start_write_pamac_config(m_handle,tabl);
    for(auto &el :keys){
        delete el;
    }

}

void LibQPamac::Transaction::startWriteAlpmConfig(const QVariantMap &map){
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

    pamac_transaction_start_write_alpm_config(m_handle,tabl);
    for(auto &el :keys){
        delete el;
    }

}

GenericQmlFuture LibQPamac::Transaction::getBuildFiles(const QString &pkgname){
    auto future = new QmlFutureImpl;
    pamac_transaction_get_build_files(m_handle,pkgname.toUtf8(),
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
    return GenericQmlFuture(future);

}

void LibQPamac::Transaction::start(const QStringList& toInstall, const QStringList& toRemove, const QStringList& toLoad,
                                   const QStringList& toBuild, const QStringList& tempIgnore, const QStringList& overwriteFiles)
{
    using Utils::qStringListToCStringVector;

    auto install = qStringListToCStringVector(toInstall);
    auto remove = qStringListToCStringVector(toRemove);
    auto load = qStringListToCStringVector(toLoad);
    auto build = qStringListToCStringVector(toBuild);
    auto ignore = qStringListToCStringVector(tempIgnore);
    auto overwrite = qStringListToCStringVector(overwriteFiles);

    pamac_transaction_start(m_handle,install.data(),int(install.size()),
                            remove.data(),int(remove.size()),
                            load.data(),int(load.size()),
                            build.data(),int(build.size()),
                            ignore.data(),int(ignore.size()),
                            overwrite.data(),int(overwrite.size()));
    setProperty("started",true);

}

void LibQPamac::Transaction::startSysupgrade(bool forceRefresh, bool enableDowngrade, const QStringList &tempIgnore, const QStringList &overwriteFiles)
{
    using Utils::qStringListToCStringVector;

    auto ignore = qStringListToCStringVector(tempIgnore);
    auto overwrite = qStringListToCStringVector(overwriteFiles);

    pamac_transaction_start_sysupgrade(m_handle,forceRefresh,enableDowngrade,
                                       ignore.data(),int(ignore.size()),
                                       overwrite.data(),int(overwrite.size()));

    setProperty("started",true);
}

void LibQPamac::Transaction::setDatabase(LibQPamac::Database *database)
{
    if (m_database == database) {
        return;
    }
    m_handle = pamac_transaction_new(database->getHandle());
    init();

    Q_EMIT databaseChanged(m_database);
}


void LibQPamac::Transaction::init()
{
    //methods override
        PAMAC_TRANSACTION_GET_CLASS(m_handle)->ask_commit=
                Utils::cify([=](PamacTransaction* self, PamacTransactionSummary* summary)->gboolean{
        auto engine = qmlEngine(this);
        return gboolean(engine->fromScriptValue<QVariant>(
                            this->m_requestCommit.call({engine->toScriptValue(TransactionSummary(summary))}))
                        .toBool());
    });
    PAMAC_TRANSACTION_GET_CLASS(m_handle)->choose_optdeps=
            Utils::cify([=](PamacTransaction* self, const gchar* pkgname, gchar** optdeps, int optdeps_length1)->GList*{

        QStringList lst = Utils::cStringArrayToQStringList(optdeps,optdeps_length1);

        auto engine = qmlEngine(this);
        auto varList = engine->fromScriptValue<QList<bool>>(this->m_requestOptDepends.call({{QString::fromUtf8(pkgname),engine->toScriptValue(lst)}}));

        GList* result = nullptr;

        int minLength = std::min(optdeps_length1,varList.length());

        for(int i =0;i<minLength;i++){
            char* res = new char[lst[i].toUtf8().length()];
            std::memcpy(res,lst[i].toUtf8(),lst[i].toUtf8().size());
            result = g_list_prepend(result,res);
        }


        return result;
    });
    PAMAC_TRANSACTION_GET_CLASS(m_handle)->choose_provider=
            Utils::cify([=](PamacTransaction* self, const gchar* depend, gchar** providers, int providers_length1)->gint{
        QStringList lst = Utils::cStringArrayToQStringList(providers,providers_length1);


        auto engine = qmlEngine(this);

        return engine->fromScriptValue<int>(this->m_requestChooseProvider.call({{QString::fromUtf8(depend),engine->toScriptValue(lst)}}));
    });

    PAMAC_TRANSACTION_GET_CLASS(m_handle)->ask_import_key=
            Utils::cify([=](PamacTransaction* self, const gchar* pkgname, const gchar* key, const gchar* owner)->gboolean{

        auto engine = qmlEngine(this);
        return gboolean(engine->fromScriptValue<bool>(this->m_requestImportKey.call({{QString::fromUtf8(pkgname),QString::fromUtf8(key),QString::fromUtf8(owner)}})));
    });
    PAMAC_TRANSACTION_GET_CLASS(m_handle)->ask_edit_build_files =
            Utils::cify([=](PamacTransaction* self, PamacTransactionSummary* summary)->gboolean{
        auto engine = qmlEngine(this);

        return false;
    });
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool success,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->finished(success);
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit-action",
                     reinterpret_cast<GCallback>(+Utils::cify([this](GObject* obj,char* action){
                         Q_UNUSED(obj);
                         Q_EMIT this->emitAction(QString::fromUtf8(action));
                     })),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_action_progress",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* action,char* status,double progress,Transaction* t){
                         Q_UNUSED(obj);

                         Q_EMIT t->emitActionProgress(QString::fromUtf8(action),QString::fromUtf8(status),progress);
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_error",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* message,char** details,int size,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->emitError(QString::fromUtf8(message),
                         Utils::cStringArrayToQStringList(details,size));
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_warning",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* warning,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->emitWarning(QString::fromUtf8(warning));
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_preparing",
                     reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->startPreparing();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"stop_preparing",
                     reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->stopPreparing();
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_script_output",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* message,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->emitScriptOutput(QString::fromUtf8(message));
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"important_details_outpout",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool message,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->importantDetailsOutput(message);
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"sysupgrade_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool success,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->finished(success);
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"write_alpm_config_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool checkspace,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->writeAlpmConfigFinished();
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"write_pamac_config_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool recurse, uint64_t refresh_period, bool no_update_hide_icon,
                                                 bool enable_aur, char* aur_build_dir, bool check_aur_updates,
                                                 bool check_aur_vcs_updates, bool download_updates
                                                 ,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->writePamacConfigFinished();
                     }),this);


    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_waiting",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->startWaiting();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"stop_waiting",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->stopWaiting();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_downloading",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->startDownloading();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"stop_downloading",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->stopDownloading();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_building",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->startBuilding();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"stop_building",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->stopBuilding();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"downloading_updates_finished",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->downloadingUpdatesFinished();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"sysupgrade_finished",reinterpret_cast<GCallback>(+[](GObject* obj, bool success,Transaction* t){
                         Q_EMIT t->sysupgradeFinished(success);
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_generating_mirrors_list",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->startGeneratingMirrorsList();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"generate_mirrors_list_finished",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->generateMirrorsListFinished();
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"clean_cache_finished",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->cleanCacheFinished();
                     }),this);


    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"clean_build_files_finished",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->cleanBuildFilesFinished();
                     }),this);

    for(auto signal : {&Transaction::startWaiting,&Transaction::startBuilding,&Transaction::startPreparing,&Transaction::startDownloading}){
        connect(this,signal,[=](){
            setProperty("indeterminate",true);});
    }

    for(auto signal : {&Transaction::stopWaiting,&Transaction::stopBuilding,&Transaction::stopPreparing,&Transaction::stopDownloading}){
        connect(this,signal,[=](){this->setProperty("indeterminate",false);});
    }

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
        qDebug()<<action;
        this->setProperty("action",action);
        QString details = this->property("details").toString();
        details.append(action+"\n");
        this->setProperty("details",details);
    });
    connect(this,&Transaction::finished,[=](bool success){
        Q_UNUSED(success)
        this->setProperty("progress",0);
        this->setProperty("started",false);
    });
    connect(this,&Transaction::emitScriptOutput,[=](QString message){
        QString details = this->property("details").toString();
        details.append(message+"\n");
        this->setProperty("details",details);
    });


}
