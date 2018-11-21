#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>

#include "bovineapi.h"
#include "levelmeter.h"
#include "mdnslookup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_actionConnect_triggered();

    void on_cbMic1_activated(int index);

    void on_pbPreset_clicked();

    void on_device_connected();

    void on_cbCab_activated(int index);

    void on_cbRoomMic_activated(int index);

    void on_pbMic1Axis_toggled(bool checked);

    void on_pbMic2Axis_toggled(bool checked);

    void on_pbMic2HP_toggled(bool checked);

    void on_pbMic1HP_toggled(bool checked);

    void on_pbMic1Mute_toggled(bool checked);

    void on_pbMic1Solo_toggled(bool checked);

    void on_pbMic2Mute_toggled(bool checked);

    void on_pbMic2Solo_toggled(bool checked);

    void on_pbRoomMicHP_toggled(bool checked);

    void on_pbRoomMicDamp_toggled(bool checked);

    void on_vsCabSpeakerBreakup_valueChanged(int value);

    void on_hsMic1Pan_valueChanged(int value);

    void on_hsMic2Pan_valueChanged(int value);

    void on_vsMic1Gain_valueChanged(int value);

    void on_vsMic2Gain_valueChanged(int value);

    void on_hsRoomMicBalance_valueChanged(int value);

    void on_dEQMic1LCFreq_valueChanged(int value);

    void on_dEQMic1LOQ_valueChanged(int value);

    void on_dEQMic1LOFreq_valueChanged(int value);

    void on_dEQMic1LOGain_valueChanged(int value);

    void on_pbEQMic1LOShelf_toggled(bool checked);

    void on_dEQMic1LOMIDQ_valueChanged(int value);

    void on_dEQMic1LOMIDFreq_valueChanged(int value);

    void on_dEQMic1LOMIDGain_valueChanged(int value);

    void on_dEQMic1MIDQ_valueChanged(int value);

    void on_dEQMic1MIDFreq_valueChanged(int value);

    void on_dEQMic1MIDGain_valueChanged(int value);

    void on_dEQMic1HIQ_valueChanged(int value);

    void on_dEQMic1HIFreq_valueChanged(int value);

    void on_dEQMic1HIGain_valueChanged(int value);

    void on_pbEQMic1HIShelf_toggled(bool checked);

    void on_dEQMic1HCFreq_valueChanged(int value);

    void on_pbEQMic1_toggled(bool checked);

    void on_dEQMic1Lvl_valueChanged(int value);

    void on_dEQMic2LCFreq_valueChanged(int value);

    void on_dEQMic2LOQ_valueChanged(int value);

    void on_dEQMic2LOFreq_valueChanged(int value);

    void on_dEQMic2LOGain_valueChanged(int value);

    void on_pbEQMic2LOShelf_toggled(bool checked);

    void on_dEQMic2LOMIDQ_valueChanged(int value);

    void on_dEQMic2LOMIDFreq_valueChanged(int value);

    void on_dEQMic2LOMIDGain_valueChanged(int value);

    void on_dEQMic2MIDQ_valueChanged(int value);

    void on_dEQMic2MIDFreq_valueChanged(int value);

    void on_dEQMic2MIDGain_valueChanged(int value);

    void on_dEQMic2HIQ_valueChanged(int value);

    void on_dEQMic2HIFreq_valueChanged(int value);

    void on_dEQMic2HIGain_valueChanged(int value);

    void on_pbEQMic2HIShelf_toggled(bool checked);

    void on_dEQMic2HCFreq_valueChanged(int value);

    void on_pbEQMic2_toggled(bool checked);

    void on_dEQMic2Lvl_valueChanged(int value);

    void on_dEQRoomMicLCFreq_valueChanged(int value);

    void on_dEQRoomMicLOQ_valueChanged(int value);

    void on_dEQRoomMicLOFreq_valueChanged(int value);

    void on_dEQRoomMicLOGain_valueChanged(int value);

    void on_pbEQRoomMicLOShelf_toggled(bool checked);

    void on_dEQRoomMicLOMIDQ_valueChanged(int value);

    void on_dEQRoomMicLOMIDFreq_valueChanged(int value);

    void on_dEQRoomMicLOMIDGain_valueChanged(int value);

    void on_dEQRoomMicMIDQ_valueChanged(int value);

    void on_dEQRoomMicMIDFreq_valueChanged(int value);

    void on_dEQRoomMicMIDGain_valueChanged(int value);

    void on_dEQRoomMicHIQ_valueChanged(int value);

    void on_dEQRoomMicHIFreq_valueChanged(int value);

    void on_dEQRoomMicHIGain_valueChanged(int value);

    void on_pbEQRoomMicHIShelf_toggled(bool checked);

    void on_dEQRoomMicHCFreq_valueChanged(int value);

    void on_dEQRoomMicLvl_valueChanged(int value);

    void on_pbEQRoomMic_toggled(bool checked);

    void on_dEQMasterLCFreq_valueChanged(int value);

    void on_dEQMasterLOQ_valueChanged(int value);

    void on_dEQMasterLOFreq_valueChanged(int value);

    void on_dEQMasterLOGain_valueChanged(int value);

    void on_dEQMasterLOMIDQ_valueChanged(int value);

    void on_dEQMasterLOMIDFreq_valueChanged(int value);

    void on_dEQMasterLOMIDGain_valueChanged(int value);

    void on_pbEQMasterLOShelf_toggled(bool checked);

    void on_dEQMasterMIDQ_valueChanged(int value);

    void on_dEQMasterMIDFreq_valueChanged(int value);

    void on_dEQMasterMIDGain_valueChanged(int value);

    void on_dEQMasterHIQ_valueChanged(int value);

    void on_dEQMasterHIFreq_valueChanged(int value);

    void on_dEQMasterHIGain_valueChanged(int value);

    void on_pbEQMasterHIShelf_toggled(bool checked);

    void on_dEQMasterHCFreq_valueChanged(int value);

    void on_pbEQMaster_toggled(bool checked);

    void on_dEQMasterLvl_valueChanged(int value);

    void on_cbEQMic1LCType_activated(int index);

    void on_cbEQMic1HCType_activated(int index);

    void on_cbEQMic2LCType_activated(int index);

    void on_cbEQMic2HCType_activated(int index);

    void on_cbEQRoomMicLCType_activated(int index);

    void on_cbEQRoomMicHCType_activated(int index);

    void on_cbEQMasterLCType_activated(int index);

    void on_cbEQMasterHCType_activated(int index);

    void on_cbDelayMode_activated(int index);

    void on_set_level(int index, double val, double peakValue);

    void on_pbRoomMicSolo_toggled(bool checked);

    void on_pbRoomMicMute_toggled(bool checked);

    void on_dFXDelayTimeA_valueChanged(int value);

    void on_dFXDelayTimeB_valueChanged(int value);

    void on_pbFXDelayMuteA_toggled(bool checked);

    void on_pbFXDelayMuteB_toggled(bool checked);

    void on_pbFXDelayLink_toggled(bool checked);

    void on_dFXDelayFeedback_valueChanged(int value);

    void on_dFXDelayModulationRate_valueChanged(int value);

    void on_cbFXDelayModulationLFO_activated(int index);

    void on_dFXDelayModulationDepth_valueChanged(int value);

    void on_dFXDelayFilterHiPass_valueChanged(int value);

    void on_dFXDelayFilterLoPass_valueChanged(int value);

    void on_dFXDelayPanA_valueChanged(int value);

    void on_dFXDelayPanB_valueChanged(int value);

    void on_pbFXDelayPanLink_toggled(bool checked);

    void on_dFXDelayLevel_valueChanged(int value);

    void on_dFXDelayMix_valueChanged(int value);

    void on_pbFXDelayPower_toggled(bool checked);

    void on_dFXReverbTime_valueChanged(int value);

    void on_dFXReverbLGain_valueChanged(int value);

    void on_dFXReverbHiGain_valueChanged(int value);

    void on_dFXReverbPredly_valueChanged(int value);

    void on_dFXReverbMix_valueChanged(int value);

    void on_dFXReverbBal_valueChanged(int value);

    void on_cbFXReverbLowcut_activated(int index);

    void on_pbFXReverbPower_toggled(bool checked);

    void on_dFXCompAttack_valueChanged(int value);

    void on_dFXCompInput_valueChanged(int value);

    void on_cbFXCompRatio_activated(int index);

    void on_dFXCompRelease_valueChanged(int value);

    void on_dFXCompOutput_valueChanged(int value);

    void on_pbFXCompPower_toggled(bool checked);

    void on_vsRoomMicGain_valueChanged(int value);

private:
    void addDevPathInput(const QString &control, QWidget *widget,
                         WidgetType wtype);
    void addDevPathMixer(const QString &channel, const QString &control,
                         QWidget* widget,  WidgetType wtype);
    void addDevPathFX(const QString &channel, int effect_id,
                      const QString &control, QWidget* widget,
                      WidgetType wtype);
    Ui::MainWindow *ui;
    LevelMeter* levelMeter[6];

    BovineAPI *api;
    QHash<QWidget*, QString> revPathMap;
    QString widgetType2propertyName(WidgetType wtype);
    void addDevPathBusFX(const QString &channel, int effect_id,
                         const QString &control, QWidget *widget,
                         WidgetType wtype);
};

#endif // MAINWINDOW_H
