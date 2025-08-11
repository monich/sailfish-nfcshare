/*
 * Copyright (C) 2025 Slava Monich <slava@monich.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer
 *     in the documentation and/or other materials provided with the
 *     distribution.
 *
 *  3. Neither the names of the copyright holders nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * any official policies, either expressed or implied.
 */

#include "sharingplugininterface.h"

#include <QtCore/QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#ifdef DEBUG
#  define DBG(x) qDebug() << x
#else
#  define DBG(x) ((void)0)
#endif
#define WARN(x) qWarning() << x

// org.sailfishos.nfc.Daemon version 4 (or later) is required
#define NFCD_MIN_INTERFACE_VERSION 4
#define NFCSHARE_PLUGIN_ID "NfcShare"

#ifdef USE_SVG
#define NFCSHARE_ICON NFCSHARE_UI_DIR "/icon-m-share-nfc.svg"
#else
#define NFCSHARE_ICON "image://theme/icon-m-share-nfc"
#endif

class Q_DECL_EXPORT NfcSharePlugin :
    public QObject,
    public SharingPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sailfishos.nfcshare.plugin")
    Q_INTERFACES(SharingPluginInterface)
    class PluginInfo;

public:
    SharingPluginInfo* infoObject() Q_DECL_OVERRIDE;
    QString pluginId() const Q_DECL_OVERRIDE;
};

//===========================================================================
// NfcSharePlugin::PluginInfo
//===========================================================================

class NfcSharePlugin::PluginInfo :
    public SharingPluginInfo
{
public:
    static SharingMethodInfo nfcPluginInfo();

    // SharingPluginInfo
    QList<SharingMethodInfo> info() const Q_DECL_OVERRIDE;
    void query() Q_DECL_OVERRIDE;

private:
    QList<SharingMethodInfo> iInfoList;
};

//static
SharingMethodInfo
NfcSharePlugin::PluginInfo::nfcPluginInfo()
{
    SharingMethodInfo info;

    info.setDisplayName("NFC");
    info.setMethodId(NFCSHARE_PLUGIN_ID);
    info.setMethodIcon(NFCSHARE_ICON);
    info.setShareUIPath(NFCSHARE_UI_DIR "/" NFCSHARE_UI_FILE);
    info.setCapabilities(QStringList("text/*"));
    return info;
}

QList<SharingMethodInfo>
NfcSharePlugin::PluginInfo::info() const
{
    return iInfoList;
}

void
NfcSharePlugin::PluginInfo::query()
{
    iInfoList.clear();

    // Interface version 4 (or later) is required
    // NFC must be enabled
    QDBusConnection bus(QDBusConnection::systemBus());
    QDBusInterface daemon("org.sailfishos.nfc.daemon", "/",
        "org.sailfishos.nfc.Daemon", bus);
    QDBusInterface settings("org.sailfishos.nfc.settings", "/",
        "org.sailfishos.nfc.Settings", bus);

    if (!settings.isValid()) {
        WARN(settings.lastError());
    } else if (!daemon.isValid()) {
        WARN(daemon.lastError());
    } else {
        QDBusReply<int> version = daemon.call("GetInterfaceVersion");
        QDBusReply<bool> enabled = settings.call("GetEnabled");

        if (!version.isValid()) {
            WARN(version.error());
        } else if (!enabled.isValid()) {
            WARN(enabled.error());
        } else {
            int v = version.value();

            DBG("NFC interface version" << v);
            if (v >= NFCD_MIN_INTERFACE_VERSION) {
                if (enabled.value()) {
                    DBG("NFC is enabled");
                    iInfoList.append(nfcPluginInfo());
                } else {
                    DBG("NFC is disabled");
                }
            }
        }
    }

    Q_EMIT infoReady();
}

//===========================================================================
// NfcSharePlugin
//===========================================================================

SharingPluginInfo*
NfcSharePlugin::infoObject()
{
    return new PluginInfo;
}

QString
NfcSharePlugin::pluginId() const
{
    return NFCSHARE_PLUGIN_ID;
}

#include "nfcshareplugin.moc"
