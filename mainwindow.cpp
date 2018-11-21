#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogpresets.h"
#include "mdnslookup.h"
#include <QDebug>
#include <QJsonObject>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // THE API object
    api = new BovineAPI();
    connect(api, SIGNAL(updateMeterLevel(int, double, double)), this,
            SLOT(on_set_level(int, double, double)));

    connect(api, SIGNAL(deviceConnected()), this,
            SLOT(on_device_connected()));

    api->addDevicePath("/devices/0123456789abcdef0123456789abcdef/",
                       "preset_name", ui->leCurrentPreset, LINE_EDIT);

    // Cabinet
    addDevPathInput("cabinet", ui->cbCab, COMBO);
    addDevPathInput("breakup", ui->vsCabSpeakerBreakup, SLIDER);
    addDevPathInput("breakup", ui->leCabSpeakerBreakup, LINE_EDIT);

    // Mic 1
    addDevPathInput("close_mic_1", ui->cbMic1, COMBO);
    addDevPathInput("close_mic_1_axis", ui->pbMic1Axis, PUSH_BUTTON);
    addDevPathInput("close_mic_1_hp", ui->pbMic1HP, PUSH_BUTTON);
    addDevPathMixer("mic_1", "gain", ui->vsMic1Gain, SLIDER);
    addDevPathMixer("mic_1", "gain", ui->leMic1Gain, LINE_EDIT);
    addDevPathMixer("mic_1", "mute", ui->pbMic1Mute, PUSH_BUTTON);
    addDevPathMixer("mic_1", "solo", ui->pbMic1Solo, PUSH_BUTTON);
    addDevPathMixer("mic_1", "pan", ui->hsMic1Pan, SLIDER);

    // Mic 2
    addDevPathInput("close_mic_2", ui->cbMic2, COMBO);
    addDevPathInput("close_mic_2_axis", ui->pbMic2Axis, PUSH_BUTTON);
    addDevPathInput("close_mic_2_hp", ui->pbMic2HP, PUSH_BUTTON);
    addDevPathMixer("mic_2", "gain", ui->vsMic2Gain, SLIDER);
    addDevPathMixer("mic_2", "gain", ui->leMic2Gain, LINE_EDIT);
    addDevPathMixer("mic_2", "mute", ui->pbMic2Mute, PUSH_BUTTON);
    addDevPathMixer("mic_2", "solo", ui->pbMic2Solo, PUSH_BUTTON);
    addDevPathMixer("mic_2", "pan", ui->hsMic2Pan, SLIDER);

    // Mic 2
    addDevPathInput("room_mic", ui->cbRoomMic, COMBO);
    addDevPathInput("room_mic_damp", ui->pbRoomMicDamp, PUSH_BUTTON);
    addDevPathInput("room_mic_hp", ui->pbRoomMicHP, PUSH_BUTTON);
    addDevPathMixer("room", "gain", ui->vsRoomMicGain, SLIDER);
    addDevPathMixer("room", "gain", ui->leRoomMicGain, LINE_EDIT);
    addDevPathMixer("room", "mute", ui->pbRoomMicMute, PUSH_BUTTON);
    addDevPathMixer("room", "solo", ui->pbRoomMicSolo, PUSH_BUTTON);
    addDevPathInput("room_mic_balance", ui->hsRoomMicBalance, SLIDER);

    // EQ Mic 1
    addDevPathFX("mic_1", 0, "lc_freq", ui->dEQMic1LCFreq, DIAL);
    addDevPathFX("mic_1", 0, "lc_freq", ui->leEQMic1LCFreq, LINE_EDIT);
    addDevPathFX("mic_1", 0, "lf_q", ui->dEQMic1LOQ, DIAL);
    addDevPathFX("mic_1", 0, "lf_q", ui->leEQMic1LOQ, LINE_EDIT);
    addDevPathFX("mic_1", 0, "lf_freq", ui->dEQMic1LOFreq, DIAL);
    addDevPathFX("mic_1", 0, "lf_freq", ui->leEQMic1LOFreq, LINE_EDIT);
    addDevPathFX("mic_1", 0, "lf_gain", ui->dEQMic1LOGain, DIAL);
    addDevPathFX("mic_1", 0, "lf_gain", ui->leEQMic1LOGain, LINE_EDIT);
    addDevPathFX("mic_1", 0, "lf_shelf", ui->pbEQMic1LOShelf, PUSH_BUTTON);
    addDevPathFX("mic_1", 0, "lmf_q", ui->dEQMic1LOMIDQ, DIAL);
    addDevPathFX("mic_1", 0, "lmf_q", ui->leEQMic1LOMIDQ, LINE_EDIT);
    addDevPathFX("mic_1", 0, "lmf_freq", ui->dEQMic1LOMIDFreq, DIAL);
    addDevPathFX("mic_1", 0, "lmf_freq", ui->leEQMic1LOMIDFreq, LINE_EDIT);
    addDevPathFX("mic_1", 0, "lmf_gain", ui->dEQMic1LOMIDGain, DIAL);
    addDevPathFX("mic_1", 0, "lmf_gain", ui->leEQMic1LOMIDGain, LINE_EDIT);
    addDevPathFX("mic_1", 0, "mf_q", ui->dEQMic1MIDQ, DIAL);
    addDevPathFX("mic_1", 0, "mf_q", ui->leEQMic1MIDQ, LINE_EDIT);
    addDevPathFX("mic_1", 0, "mf_freq", ui->dEQMic1MIDFreq, DIAL);
    addDevPathFX("mic_1", 0, "mf_freq", ui->leEQMic1MIDFreq, LINE_EDIT);
    addDevPathFX("mic_1", 0, "mf_gain", ui->dEQMic1MIDGain, DIAL);
    addDevPathFX("mic_1", 0, "mf_gain", ui->leEQMic1MIDGain, LINE_EDIT);
    addDevPathFX("mic_1", 0, "hf_q", ui->dEQMic1HIQ, DIAL);
    addDevPathFX("mic_1", 0, "hf_q", ui->leEQMic1HIQ, LINE_EDIT);
    addDevPathFX("mic_1", 0, "hf_freq", ui->dEQMic1HIFreq, DIAL);
    addDevPathFX("mic_1", 0, "hf_freq", ui->leEQMic1HIFreq, LINE_EDIT);
    addDevPathFX("mic_1", 0, "hf_gain", ui->dEQMic1HIGain, DIAL);
    addDevPathFX("mic_1", 0, "hf_gain", ui->leEQMic1HIGain, LINE_EDIT);
    addDevPathFX("mic_1", 0, "hf_shelf", ui->pbEQMic1HIShelf, PUSH_BUTTON);
    addDevPathFX("mic_1", 0, "hc_freq", ui->dEQMic1HCFreq, DIAL);
    addDevPathFX("mic_1", 0, "hc_freq", ui->leEQMic1HCFreq, LINE_EDIT);
    addDevPathFX("mic_1", 0, "master_enable", ui->pbEQMic1, PUSH_BUTTON);
    addDevPathFX("mic_1", 0, "master_level", ui->dEQMic1Lvl, DIAL);
    addDevPathFX("mic_1", 0, "master_level", ui->leEQMic1Lvl, LINE_EDIT);
    addDevPathFX("mic_1", 0, "lc_type", ui->cbEQMic1LCType, COMBO);
    addDevPathFX("mic_1", 0, "hc_type", ui->cbEQMic1HCType, COMBO);

    // EQ Mic 2
    addDevPathFX("mic_2", 0, "lc_freq", ui->dEQMic2LCFreq, DIAL);
    addDevPathFX("mic_2", 0, "lc_freq", ui->leEQMic2LCFreq, LINE_EDIT);
    addDevPathFX("mic_2", 0, "lf_q", ui->dEQMic2LOQ, DIAL);
    addDevPathFX("mic_2", 0, "lf_q", ui->leEQMic2LOQ, LINE_EDIT);
    addDevPathFX("mic_2", 0, "lf_freq", ui->dEQMic2LOFreq, DIAL);
    addDevPathFX("mic_2", 0, "lf_freq", ui->leEQMic2LOFreq, LINE_EDIT);
    addDevPathFX("mic_2", 0, "lf_gain", ui->dEQMic2LOGain, DIAL);
    addDevPathFX("mic_2", 0, "lf_gain", ui->leEQMic2LOGain, LINE_EDIT);
    addDevPathFX("mic_2", 0, "lf_shelf", ui->pbEQMic2LOShelf, PUSH_BUTTON);
    addDevPathFX("mic_2", 0, "lmf_q", ui->dEQMic2LOMIDQ, DIAL);
    addDevPathFX("mic_2", 0, "lmf_q", ui->leEQMic2LOMIDQ, LINE_EDIT);
    addDevPathFX("mic_2", 0, "lmf_freq", ui->dEQMic2LOMIDFreq, DIAL);
    addDevPathFX("mic_2", 0, "lmf_freq", ui->leEQMic2LOMIDFreq, LINE_EDIT);
    addDevPathFX("mic_2", 0, "lmf_gain", ui->dEQMic2LOMIDGain, DIAL);
    addDevPathFX("mic_2", 0, "lmf_gain", ui->leEQMic2LOMIDGain, LINE_EDIT);
    addDevPathFX("mic_2", 0, "mf_q", ui->dEQMic2MIDQ, DIAL);
    addDevPathFX("mic_2", 0, "mf_q", ui->leEQMic2MIDQ, LINE_EDIT);
    addDevPathFX("mic_2", 0, "mf_freq", ui->dEQMic2MIDFreq, DIAL);
    addDevPathFX("mic_2", 0, "mf_freq", ui->leEQMic2MIDFreq, LINE_EDIT);
    addDevPathFX("mic_2", 0, "mf_gain", ui->dEQMic2MIDGain, DIAL);
    addDevPathFX("mic_2", 0, "mf_gain", ui->leEQMic2MIDGain, LINE_EDIT);
    addDevPathFX("mic_2", 0, "hf_q", ui->dEQMic2HIQ, DIAL);
    addDevPathFX("mic_2", 0, "hf_q", ui->leEQMic2HIQ, LINE_EDIT);
    addDevPathFX("mic_2", 0, "hf_freq", ui->dEQMic2HIFreq, DIAL);
    addDevPathFX("mic_2", 0, "hf_freq", ui->leEQMic2HIFreq, LINE_EDIT);
    addDevPathFX("mic_2", 0, "hf_gain", ui->dEQMic2HIGain, DIAL);
    addDevPathFX("mic_2", 0, "hf_gain", ui->leEQMic2HIGain, LINE_EDIT);
    addDevPathFX("mic_2", 0, "hf_shelf", ui->pbEQMic2HIShelf, PUSH_BUTTON);
    addDevPathFX("mic_2", 0, "hc_freq", ui->dEQMic2HCFreq, DIAL);
    addDevPathFX("mic_2", 0, "hc_freq", ui->leEQMic2HCFreq, LINE_EDIT);
    addDevPathFX("mic_2", 0, "master_enable", ui->pbEQMic2, PUSH_BUTTON);
    addDevPathFX("mic_2", 0, "master_level", ui->dEQMic2Lvl, DIAL);
    addDevPathFX("mic_2", 0, "master_level", ui->leEQMic2Lvl, LINE_EDIT);
    addDevPathFX("mic_2", 0, "lc_type", ui->cbEQMic2LCType, COMBO);
    addDevPathFX("mic_2", 0, "hc_type", ui->cbEQMic2HCType, COMBO);

    // EQ Room Mic
    addDevPathFX("room", 0, "lc_freq", ui->dEQRoomMicLCFreq, DIAL);
    addDevPathFX("room", 0, "lc_freq", ui->leEQRoomMicLCFreq, LINE_EDIT);
    addDevPathFX("room", 0, "lf_q", ui->dEQRoomMicLOQ, DIAL);
    addDevPathFX("room", 0, "lf_q", ui->leEQRoomMicLOQ, LINE_EDIT);
    addDevPathFX("room", 0, "lf_freq", ui->dEQRoomMicLOFreq, DIAL);
    addDevPathFX("room", 0, "lf_freq", ui->leEQRoomMicLOFreq, LINE_EDIT);
    addDevPathFX("room", 0, "lf_gain", ui->dEQRoomMicLOGain, DIAL);
    addDevPathFX("room", 0, "lf_gain", ui->leEQRoomMicLOGain, LINE_EDIT);
    addDevPathFX("room", 0, "lf_shelf", ui->pbEQRoomMicLOShelf, PUSH_BUTTON);
    addDevPathFX("room", 0, "lmf_q", ui->dEQRoomMicLOMIDQ, DIAL);
    addDevPathFX("room", 0, "lmf_q", ui->leEQRoomMicLOMIDQ, LINE_EDIT);
    addDevPathFX("room", 0, "lmf_freq", ui->dEQRoomMicLOMIDFreq, DIAL);
    addDevPathFX("room", 0, "lmf_freq", ui->leEQRoomMicLOMIDFreq, LINE_EDIT);
    addDevPathFX("room", 0, "lmf_gain", ui->dEQRoomMicLOMIDGain, DIAL);
    addDevPathFX("room", 0, "lmf_gain", ui->leEQRoomMicLOMIDGain, LINE_EDIT);
    addDevPathFX("room", 0, "mf_q", ui->dEQRoomMicMIDQ, DIAL);
    addDevPathFX("room", 0, "mf_q", ui->leEQRoomMicMIDQ, LINE_EDIT);
    addDevPathFX("room", 0, "mf_freq", ui->dEQRoomMicMIDFreq, DIAL);
    addDevPathFX("room", 0, "mf_freq", ui->leEQRoomMicMIDFreq, LINE_EDIT);
    addDevPathFX("room", 0, "mf_gain", ui->dEQRoomMicMIDGain, DIAL);
    addDevPathFX("room", 0, "mf_gain", ui->leEQRoomMicMIDGain, LINE_EDIT);
    addDevPathFX("room", 0, "hf_q", ui->dEQRoomMicHIQ, DIAL);
    addDevPathFX("room", 0, "hf_q", ui->leEQRoomMicHIQ, LINE_EDIT);
    addDevPathFX("room", 0, "hf_freq", ui->dEQRoomMicHIFreq, DIAL);
    addDevPathFX("room", 0, "hf_freq", ui->leEQRoomMicHIFreq, LINE_EDIT);
    addDevPathFX("room", 0, "hf_gain", ui->dEQRoomMicHIGain, DIAL);
    addDevPathFX("room", 0, "hf_gain", ui->leEQRoomMicHIGain, LINE_EDIT);
    addDevPathFX("room", 0, "hf_shelf", ui->pbEQRoomMicHIShelf, PUSH_BUTTON);
    addDevPathFX("room", 0, "hc_freq", ui->dEQRoomMicHCFreq, DIAL);
    addDevPathFX("room", 0, "hc_freq", ui->leEQRoomMicHCFreq, LINE_EDIT);
    addDevPathFX("room", 0, "master_enable", ui->pbEQRoomMic, PUSH_BUTTON);
    addDevPathFX("room", 0, "master_level", ui->dEQRoomMicLvl, DIAL);
    addDevPathFX("room", 0, "master_level", ui->leEQRoomMicLvl, LINE_EDIT);
    addDevPathFX("room", 0, "lc_type", ui->cbEQRoomMicLCType, COMBO);
    addDevPathFX("room", 0, "hc_type", ui->cbEQRoomMicHCType, COMBO);

    // EQ Master
    addDevPathBusFX("master", 0, "lc_freq", ui->dEQMasterLCFreq, DIAL);
    addDevPathBusFX("master", 0, "lc_freq", ui->leEQMasterLCFreq, LINE_EDIT);
    addDevPathBusFX("master", 0, "lf_q", ui->dEQMasterLOQ, DIAL);
    addDevPathBusFX("master", 0, "lf_q", ui->leEQMasterLOQ, LINE_EDIT);
    addDevPathBusFX("master", 0, "lf_freq", ui->dEQMasterLOFreq, DIAL);
    addDevPathBusFX("master", 0, "lf_freq", ui->leEQMasterLOFreq, LINE_EDIT);
    addDevPathBusFX("master", 0, "lf_gain", ui->dEQMasterLOGain, DIAL);
    addDevPathBusFX("master", 0, "lf_gain", ui->leEQMasterLOGain, LINE_EDIT);
    addDevPathBusFX("master", 0, "lf_shelf", ui->pbEQMasterLOShelf, PUSH_BUTTON);
    addDevPathBusFX("master", 0, "lmf_q", ui->dEQMasterLOMIDQ, DIAL);
    addDevPathBusFX("master", 0, "lmf_q", ui->leEQMasterLOMIDQ, LINE_EDIT);
    addDevPathBusFX("master", 0, "lmf_freq", ui->dEQMasterLOMIDFreq, DIAL);
    addDevPathBusFX("master", 0, "lmf_freq", ui->leEQMasterLOMIDFreq, LINE_EDIT);
    addDevPathBusFX("master", 0, "lmf_gain", ui->dEQMasterLOMIDGain, DIAL);
    addDevPathBusFX("master", 0, "lmf_gain", ui->leEQMasterLOMIDGain, LINE_EDIT);
    addDevPathBusFX("master", 0, "mf_q", ui->dEQMasterMIDQ, DIAL);
    addDevPathBusFX("master", 0, "mf_q", ui->leEQMasterMIDQ, LINE_EDIT);
    addDevPathBusFX("master", 0, "mf_freq", ui->dEQMasterMIDFreq, DIAL);
    addDevPathBusFX("master", 0, "mf_freq", ui->leEQMasterMIDFreq, LINE_EDIT);
    addDevPathBusFX("master", 0, "mf_gain", ui->dEQMasterMIDGain, DIAL);
    addDevPathBusFX("master", 0, "mf_gain", ui->leEQMasterMIDGain, LINE_EDIT);
    addDevPathBusFX("master", 0, "hf_q", ui->dEQMasterHIQ, DIAL);
    addDevPathBusFX("master", 0, "hf_q", ui->leEQMasterHIQ, LINE_EDIT);
    addDevPathBusFX("master", 0, "hf_freq", ui->dEQMasterHIFreq, DIAL);
    addDevPathBusFX("master", 0, "hf_freq", ui->leEQMasterHIFreq, LINE_EDIT);
    addDevPathBusFX("master", 0, "hf_gain", ui->dEQMasterHIGain, DIAL);
    addDevPathBusFX("master", 0, "hf_gain", ui->leEQMasterHIGain, LINE_EDIT);
    addDevPathBusFX("master", 0, "hf_shelf", ui->pbEQMasterHIShelf, PUSH_BUTTON);
    addDevPathBusFX("master", 0, "hc_freq", ui->dEQMasterHCFreq, DIAL);
    addDevPathBusFX("master", 0, "hc_freq", ui->leEQMasterHCFreq, LINE_EDIT);
    addDevPathBusFX("master", 0, "master_enable", ui->pbEQMaster, PUSH_BUTTON);
    addDevPathBusFX("master", 0, "master_level", ui->dEQMasterLvl, DIAL);
    addDevPathBusFX("master", 0, "master_level", ui->leEQMasterLvl, LINE_EDIT);
    addDevPathBusFX("master", 0, "lc_type", ui->cbEQMasterLCType, COMBO);
    addDevPathBusFX("master", 0, "hc_type", ui->cbEQMasterHCType, COMBO);

    // Master FX Delay
    addDevPathBusFX("master", 2, "mode", ui->cbDelayMode, COMBO);
    addDevPathBusFX("master", 2, "delay_a", ui->dFXDelayTimeA, DIAL);
    addDevPathBusFX("master", 2, "delay_a", ui->leFXDelayTimeA, LINE_EDIT);
    addDevPathBusFX("master", 2, "mute_a", ui->pbFXDelayMuteA, PUSH_BUTTON);
    addDevPathBusFX("master", 2, "delay_b", ui->dFXDelayTimeB, DIAL);
    addDevPathBusFX("master", 2, "delay_b", ui->leFXDelayTimeB, LINE_EDIT);
    addDevPathBusFX("master", 2, "mute_b", ui->pbFXDelayMuteB, PUSH_BUTTON);
    addDevPathBusFX("master", 2, "link", ui->pbFXDelayLink, PUSH_BUTTON);
    addDevPathBusFX("master", 2, "recirc", ui->dFXDelayFeedback, DIAL);
    addDevPathBusFX("master", 2, "recirc", ui->leFXDelayFeedback, LINE_EDIT);
    addDevPathBusFX("master", 2, "rate", ui->dFXDelayModulationRate, DIAL);
    addDevPathBusFX("master", 2, "rate", ui->leFXDelayModulationRate,
                    LINE_EDIT);
    addDevPathBusFX("master", 2, "lfo", ui->cbFXDelayModulationLFO, COMBO);
    addDevPathBusFX("master", 2, "depth", ui->dFXDelayModulationDepth, DIAL);
    addDevPathBusFX("master", 2, "depth", ui->leFXDelayModulationDepth,
                    LINE_EDIT);
    addDevPathBusFX("master", 2, "hi_pass", ui->dFXDelayFilterHiPass, DIAL);
    addDevPathBusFX("master", 2, "hi_pass", ui->leFXDelayFilterHiPass,
                    LINE_EDIT);
    addDevPathBusFX("master", 2, "lo_pass", ui->dFXDelayFilterLoPass, DIAL);
    addDevPathBusFX("master", 2, "lo_pass", ui->leFXDelayFilterLoPass,
                    LINE_EDIT);
    addDevPathBusFX("master", 2, "pan_a", ui->dFXDelayPanA, DIAL);
    addDevPathBusFX("master", 2, "pan_a", ui->leFXDelayPanA, LINE_EDIT);
    addDevPathBusFX("master", 2, "pan_b", ui->dFXDelayPanB, DIAL);
    addDevPathBusFX("master", 2, "pan_b", ui->leFXDelayPanB, LINE_EDIT);
    addDevPathBusFX("master", 2, "pan_link", ui->pbFXDelayPanLink, PUSH_BUTTON);
    addDevPathBusFX("master", 2, "level", ui->dFXDelayLevel, DIAL);
    addDevPathBusFX("master", 2, "level", ui->leFXDelayLevel, LINE_EDIT);
    addDevPathBusFX("master", 2, "mix", ui->dFXDelayMix, DIAL);
    addDevPathBusFX("master", 2, "mix", ui->leFXDelayMix, LINE_EDIT);
    addDevPathBusFX("master", 2, "power", ui->pbFXDelayPower, PUSH_BUTTON);

    // Master FX Reverb
    addDevPathBusFX("master", 3, "dampb", ui->dFXReverbTime, DIAL);
    addDevPathBusFX("master", 3, "dampb", ui->leFXReverbTime, LINE_EDIT);
    addDevPathBusFX("master", 3, "lowcut", ui->cbFXReverbLowcut, COMBO);
    addDevPathBusFX("master", 3, "predly", ui->dFXReverbPredly, DIAL);
    addDevPathBusFX("master", 3, "predly", ui->leFXReverbPredly, LINE_EDIT);
    addDevPathBusFX("master", 3, "hgain", ui->dFXReverbHiGain, DIAL);
    addDevPathBusFX("master", 3, "hgain", ui->leFXReverbHiGain, LINE_EDIT);
    addDevPathBusFX("master", 3, "lgain", ui->dFXReverbLGain, DIAL);
    addDevPathBusFX("master", 3, "lgain", ui->leFXReverbLGain, LINE_EDIT);
    addDevPathBusFX("master", 3, "bal", ui->dFXReverbBal, DIAL);
    addDevPathBusFX("master", 3, "bal", ui->leFXReverbBal, LINE_EDIT);
    addDevPathBusFX("master", 3, "mix", ui->dFXReverbMix, DIAL);
    addDevPathBusFX("master", 3, "mix", ui->leFXReverbMix, LINE_EDIT);
    addDevPathBusFX("master", 3, "power", ui->pbFXReverbPower, PUSH_BUTTON);

    // Master FX Compressor
    addDevPathBusFX("master", 1, "attack", ui->dFXCompAttack, DIAL);
    addDevPathBusFX("master", 1, "attack", ui->leFXCompAttack, LINE_EDIT);
    addDevPathBusFX("master", 1, "input", ui->dFXCompInput, DIAL);
    addDevPathBusFX("master", 1, "input", ui->leFXCompInput, LINE_EDIT);
    addDevPathBusFX("master", 1, "output", ui->dFXCompOutput, DIAL);
    addDevPathBusFX("master", 1, "output", ui->leFXCompOutput, LINE_EDIT);
    addDevPathBusFX("master", 1, "release", ui->dFXCompRelease, DIAL);
    addDevPathBusFX("master", 1, "release", ui->leFXCompRelease, LINE_EDIT);
    addDevPathBusFX("master", 1, "ratio", ui->cbFXCompRatio, COMBO);
    addDevPathBusFX("master", 1, "meter", ui->pbFXCompPower, PUSH_BUTTON);

    // Level Meter
    levelMeter[0] = new LevelMeter(ui->frMic1Lvl);
    QSize s = ui->frMic1Lvl->size();
    levelMeter[0]->resize(s.width()-2, s.height() - 2);
    levelMeter[0]->move(1,1);
    levelMeter[0]->show();

    levelMeter[1] = new LevelMeter(ui->frMic2Lvl);
    s = ui->frMic2Lvl->size();
    levelMeter[1]->resize(s.width()-2, s.height() - 2);
    levelMeter[1]->move(1,1);
    levelMeter[1]->show();

    levelMeter[2] = new LevelMeter(ui->frRoomMicLvl);
    s = ui->frRoomMicLvl->size();
    int width = (s.width()-2)/2;
    levelMeter[2]->resize(width-2, s.height() - 1);
    levelMeter[2]->move(1,1);
    levelMeter[2]->show();

    levelMeter[3] = new LevelMeter(ui->frRoomMicLvl);
    s = ui->frRoomMicLvl->size();
    levelMeter[3]->resize(width-2, s.height() - 1);
    levelMeter[3]->move(width,1);
    levelMeter[3]->show();

    levelMeter[4] = new LevelMeter(ui->frMasterLvl);
    s = ui->frMasterLvl->size();
    width = (s.width()-2)/2;
    levelMeter[4]->resize(width-2, s.height() - 1);
    levelMeter[4]->move(1,1);
    levelMeter[4]->show();

    levelMeter[5] = new LevelMeter(ui->frMasterLvl);
    s = ui->frMasterLvl->size();
    levelMeter[5]->resize(width-2, s.height() - 11);
    levelMeter[5]->move(width,1);
    levelMeter[5]->show();

    // Filling combo boxes with values
    ui->cbMic1->addItem("DYNAMIC 57", "dyn_57");
    ui->cbMic1->addItem("DYNAMIC 421", "dyn_421");
    ui->cbMic1->addItem("RIBBON 121", "rib_121");
    ui->cbMic1->addItem("RIBBON 160", "rib_160");
    ui->cbMic1->addItem("CONDENSER 67", "con_67");
    ui->cbMic1->addItem("CONDENSER 414", "con_414");
    ui->cbMic1->addItem("DIRECT", "direct");

    ui->cbMic2->addItem("DYNAMIC 57", "dyn_57");
    ui->cbMic2->addItem("DYNAMIC 421", "dyn_421");
    ui->cbMic2->addItem("RIBBON 121", "rib_121");
    ui->cbMic1->addItem("RIBBON 160", "rib_160");
    ui->cbMic2->addItem("CONDENSER 67", "con_67");
    ui->cbMic2->addItem("CONDENSER 414", "con_414");
    ui->cbMic2->addItem("DIRECT", "direct");

    ui->cbCab->addItem("1x10 BLACK CHA", "cab_1x10_black_cha");
    ui->cbCab->addItem("1x12 BLUE J", "cab_1x12_tweed_jp12");
    ui->cbCab->addItem("1x12 GB 25", "cab_1x12_tweed_gb25");
    ui->cbCab->addItem("1x12 BLUE 15", "cab_1x12_tweed_bluc15");
    ui->cbCab->addItem("1x12 BLACK D-UX", "cab_1x12_black_lux");
    ui->cbCab->addItem("1x12 BLACK GB 30", "cab_1x12_black_gb30");
    ui->cbCab->addItem("2x10 V-UX", "cab_2x10_vibrolux");
    ui->cbCab->addItem("2x12 TWO VERB", "cab_2x12_silver_double");
    ui->cbCab->addItem("2x12 BLACK 8H", "cab_2x12_black_8");
    ui->cbCab->addItem("2x12 ACE TOP", "cab_2x12_ace_top");
    ui->cbCab->addItem("2x12 BOUTIQUE D65", "cab_2x12_boutique_d65");
    ui->cbCab->addItem("2x12 ALNICO 50", "cab_2x12_alnico_50");
    ui->cbCab->addItem("4x10 BMAN", "cab_4x10_tweed_bman");
    ui->cbCab->addItem("4x12 GB 25 THICK", "cab_4x12_vin_25_air");
    ui->cbCab->addItem("4x12 GB 25 PUNCH", "cab_4x12_vin_25_mid");
    ui->cbCab->addItem("4x12 SUPER 80", "cab_4x12_super_80");
    ui->cbCab->addItem("4x12 WHITE 75", "cab_4x12_white_75");

    ui->cbRoomMic->addItem("RIBBON STEREO", "ribbon");
    ui->cbRoomMic->addItem("CONDENSER STEREO", "condensor");
    ui->cbRoomMic->addItem("CONDENSER MAN MONO", "condensor_mono_l");
    ui->cbRoomMic->addItem("CONDENSER 67 MONO", "condensor_mono_r");
    ui->cbRoomMic->addItem("RIBBON 84 MONO", "ribbon_mono_l");
    ui->cbRoomMic->addItem("RIBBON 121 MONO", "ribbon_mono_r");

    // Values for all filter types
    QVector<QComboBox*> pcbEQ({
        ui->cbEQMic1LCType,
        ui->cbEQMic1HCType,
        ui->cbEQMic2LCType,
        ui->cbEQMic2HCType,
        ui->cbEQRoomMicLCType,
        ui->cbEQRoomMicHCType,
        ui->cbEQMasterLCType,
        ui->cbEQMasterHCType
    });
    for(int i = 0 ; i < pcbEQ.size() ; ++i) {
        pcbEQ[i]->addItem("6dB/Oct", 0);
        pcbEQ[i]->addItem("12dB/Oct", 1);
        pcbEQ[i]->addItem("18dB/Oct.", 2);
        pcbEQ[i]->addItem("24dB/Oct.", 3);
        pcbEQ[i]->addItem("Butterworth 2", 11);
        pcbEQ[i]->addItem("Butterworth 3", 12);
        pcbEQ[i]->addItem("Butterworth 4", 13);
    }

    // Values for Delay Type
    ui->cbDelayMode->addItem("DUAL DELAY", 0);
    ui->cbDelayMode->addItem("XOVR DELAY", 1);
    ui->cbDelayMode->addItem("PING PONG", 2);
    ui->cbDelayMode->addItem("CHORUS", 3);
    ui->cbDelayMode->addItem("FLANGER", 4);

    ui->cbFXDelayModulationLFO->addItem("SINE 0", 0);
    ui->cbFXDelayModulationLFO->addItem("SINE 90", 1);
    ui->cbFXDelayModulationLFO->addItem("TRI 0", 3);
    ui->cbFXDelayModulationLFO->addItem("TRI 90", 4);
    ui->cbFXDelayModulationLFO->addItem("TRI 180", 5);

    // Values for Reverb
    ui->cbFXReverbLowcut->addItem("OFF", 0);
    ui->cbFXReverbLowcut->addItem("90 Hz", 1);
    ui->cbFXReverbLowcut->addItem("180 Hz", 3);

    // Values for compressor ratio
    ui->cbFXCompRatio->addItem("4", 0);
    ui->cbFXCompRatio->addItem("8", 1);
    ui->cbFXCompRatio->addItem("12", 2);
    ui->cbFXCompRatio->addItem("20", 3);

    // Now, everything is set up. Let's find and connect to the device.
    ui->statusBar->showMessage("Searching for device...");
    api->connectToDevice();
}

MainWindow::~MainWindow()
{
    //delete lmMic1;
    for (int i = 0 ; i < 6 ; ++i)
        delete levelMeter[i];
    delete api;
    delete ui;
}


void MainWindow::on_actionQuit_triggered()
{
}

void MainWindow::on_actionConnect_triggered()
{
}

void MainWindow::on_cbMic1_activated(int index)
{
    api->selectComboBox(ui->cbMic1, index);
}


void MainWindow::on_pbPreset_clicked()
{
    DialogPresets dp(this, api);
    dp.exec();
}

void MainWindow::on_device_connected()
{
    ui->statusBar->showMessage("Device connected.");
}

void MainWindow::on_cbCab_activated(int index)
{
    api->selectComboBox(ui->cbCab, index);
}


void MainWindow::on_cbRoomMic_activated(int index)
{
    api->selectComboBox(ui->cbRoomMic, index);
}

void MainWindow::on_pbMic1Axis_toggled(bool checked)
{
    api->setPushButton(ui->pbMic1Axis, checked);
}

void MainWindow::on_pbMic2Axis_toggled(bool checked)
{
    api->setPushButton(ui->pbMic2Axis, checked);
}

void MainWindow::on_pbMic2HP_toggled(bool checked)
{
    api->setPushButton(ui->pbMic2HP, checked);
}

void MainWindow::on_pbMic1HP_toggled(bool checked)
{
    api->setPushButton(ui->pbMic1HP, checked);
}

void MainWindow::on_pbMic1Mute_toggled(bool checked)
{
    api->setPushButton(ui->pbMic1Mute, checked);
}

void MainWindow::on_pbMic1Solo_toggled(bool checked)
{
    api->setPushButton(ui->pbMic1Solo, checked);
}

void MainWindow::on_pbMic2Mute_toggled(bool checked)
{
    api->setPushButton(ui->pbMic2Mute, checked);
}

void MainWindow::on_pbMic2Solo_toggled(bool checked)
{
    api->setPushButton(ui->pbMic2Solo, checked);
}

void MainWindow::on_pbRoomMicHP_toggled(bool checked)
{
    api->setPushButton(ui->pbRoomMicHP, checked);
}

void MainWindow::on_pbRoomMicDamp_toggled(bool checked)
{
    api->setPushButton(ui->pbRoomMicDamp, checked);
}

void MainWindow::on_pbRoomMicMute_toggled(bool checked)
{
    api->setPushButton(ui->pbRoomMicMute, checked);
}

void MainWindow::on_pbRoomMicSolo_toggled(bool checked)
{
    api->setPushButton(ui->pbRoomMicSolo, checked);
}

void MainWindow::on_vsCabSpeakerBreakup_valueChanged(int value)
{
    api->setSlider(ui->vsCabSpeakerBreakup, value);
}

void MainWindow::on_hsMic1Pan_valueChanged(int value)
{
    api->setSlider(ui->hsMic1Pan, value);
}

void MainWindow::on_hsMic2Pan_valueChanged(int value)
{
    api->setSlider(ui->hsMic2Pan, value);
}

void MainWindow::on_vsMic1Gain_valueChanged(int value)
{
    api->setSlider(ui->vsMic1Gain, value);
}

void MainWindow::on_vsMic2Gain_valueChanged(int value)
{
    api->setSlider(ui->vsMic2Gain, value);
}

void MainWindow::on_hsRoomMicBalance_valueChanged(int value)
{
    api->setSlider(ui->hsRoomMicBalance, value);
}

void MainWindow::on_dEQMic1LCFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic1LCFreq, value);
}

void MainWindow::on_dEQMic1LOQ_valueChanged(int value)
{
    api->setDial(ui->dEQMic1LOQ, value);
}

void MainWindow::on_dEQMic1LOFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic1LOFreq, value);
}

void MainWindow::on_dEQMic1LOGain_valueChanged(int value)
{
    api->setDial(ui->dEQMic1LOGain, value);
}

void MainWindow::on_pbEQMic1LOShelf_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMic1LOShelf, checked);
}

void MainWindow::on_dEQMic1LOMIDQ_valueChanged(int value)
{
    api->setDial(ui->dEQMic1LOMIDQ, value);
}

void MainWindow::on_dEQMic1LOMIDFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic1LOMIDFreq, value);
}

void MainWindow::on_dEQMic1LOMIDGain_valueChanged(int value)
{
    api->setDial(ui->dEQMic1LOMIDGain, value);
}

void MainWindow::on_dEQMic1MIDQ_valueChanged(int value)
{
    api->setDial(ui->dEQMic1MIDQ, value);
}

void MainWindow::on_dEQMic1MIDFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic1MIDFreq, value);
}

void MainWindow::on_dEQMic1MIDGain_valueChanged(int value)
{
    api->setDial(ui->dEQMic1MIDGain, value);
}

void MainWindow::on_dEQMic1HIQ_valueChanged(int value)
{
    api->setDial(ui->dEQMic1HIQ, value);
}

void MainWindow::on_dEQMic1HIFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic1HIFreq, value);
}

void MainWindow::on_dEQMic1HIGain_valueChanged(int value)
{
    api->setDial(ui->dEQMic1HIGain, value);
}

void MainWindow::on_pbEQMic1HIShelf_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMic1HIShelf, checked);
}

void MainWindow::on_dEQMic1HCFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic1HCFreq, value);
}

void MainWindow::on_pbEQMic1_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMic1, checked);
}

void MainWindow::on_dEQMic1Lvl_valueChanged(int value)
{
    api->setDial(ui->dEQMic1Lvl, value);
}

void MainWindow::on_dEQMic2LCFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LCFreq, value);
}

void MainWindow::on_dEQMic2LOQ_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOQ, value);
}

void MainWindow::on_dEQMic2LOFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOFreq, value);
}

void MainWindow::on_dEQMic2LOGain_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOGain, value);
}

void MainWindow::on_pbEQMic2LOShelf_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMic2LOShelf, checked);
}

void MainWindow::on_dEQMic2LOMIDQ_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOMIDQ, value);
}

void MainWindow::on_dEQMic2LOMIDFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOMIDFreq, value);
}

void MainWindow::on_dEQMic2LOMIDGain_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOMIDGain, value);
}

void MainWindow::on_dEQMic2MIDQ_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOMIDQ, value);
}

void MainWindow::on_dEQMic2MIDFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOMIDFreq, value);
}

void MainWindow::on_dEQMic2MIDGain_valueChanged(int value)
{
    api->setDial(ui->dEQMic2LOMIDGain, value);
}

void MainWindow::on_dEQMic2HIQ_valueChanged(int value)
{
    api->setDial(ui->dEQMic2HIQ, value);
}

void MainWindow::on_dEQMic2HIFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic2HIFreq, value);
}

void MainWindow::on_dEQMic2HIGain_valueChanged(int value)
{
    api->setDial(ui->dEQMic2HIGain, value);
}

void MainWindow::on_pbEQMic2HIShelf_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMic2HIShelf, checked);
}

void MainWindow::on_dEQMic2HCFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMic2HCFreq, value);
}

void MainWindow::on_pbEQMic2_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMic2, checked);
}

void MainWindow::on_dEQMic2Lvl_valueChanged(int value)
{
    api->setDial(ui->dEQMic2Lvl, value);
}

void MainWindow::on_dEQRoomMicLCFreq_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicLCFreq, value);
}

void MainWindow::on_dEQRoomMicLOQ_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicLOQ, value);
}

void MainWindow::on_dEQRoomMicLOFreq_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicLOFreq, value);
}

void MainWindow::on_dEQRoomMicLOGain_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicLOGain, value);
}

void MainWindow::on_pbEQRoomMicLOShelf_toggled(bool checked)
{
    api->setPushButton(ui->pbEQRoomMicLOShelf, checked);
}

void MainWindow::on_dEQRoomMicLOMIDQ_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicLOMIDQ, value);
}

void MainWindow::on_dEQRoomMicLOMIDFreq_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicLOMIDFreq, value);
}

void MainWindow::on_dEQRoomMicLOMIDGain_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicLOMIDGain, value);
}

void MainWindow::on_dEQRoomMicMIDQ_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicMIDQ, value);
}

void MainWindow::on_dEQRoomMicMIDFreq_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicMIDFreq, value);
}

void MainWindow::on_dEQRoomMicMIDGain_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicMIDGain, value);
}

void MainWindow::on_dEQRoomMicHIQ_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicHIQ, value);
}

void MainWindow::on_dEQRoomMicHIFreq_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicHIFreq, value);
}

void MainWindow::on_dEQRoomMicHIGain_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicHIGain, value);
}

void MainWindow::on_pbEQRoomMicHIShelf_toggled(bool checked)
{
    api->setPushButton(ui->pbEQRoomMicHIShelf, checked);
}

void MainWindow::on_dEQRoomMicHCFreq_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicHCFreq, value);
}

void MainWindow::on_dEQRoomMicLvl_valueChanged(int value)
{
    api->setDial(ui->dEQRoomMicLvl, value);
}

void MainWindow::on_pbEQRoomMic_toggled(bool checked)
{
    api->setPushButton(ui->pbEQRoomMic, checked);
}

void MainWindow::on_dEQMasterLCFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMasterLCFreq, value);
}

void MainWindow::on_dEQMasterLOQ_valueChanged(int value)
{
    api->setDial(ui->dEQMasterLOQ, value);
}

void MainWindow::on_dEQMasterLOFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMasterLOFreq, value);
}

void MainWindow::on_dEQMasterLOGain_valueChanged(int value)
{
    api->setDial(ui->dEQMasterLOGain, value);
}

void MainWindow::on_dEQMasterLOMIDQ_valueChanged(int value)
{
    api->setDial(ui->dEQMasterLOMIDQ, value);
}

void MainWindow::on_dEQMasterLOMIDFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMasterLOMIDFreq, value);
}

void MainWindow::on_dEQMasterLOMIDGain_valueChanged(int value)
{
    api->setDial(ui->dEQMasterLOMIDGain, value);
}

void MainWindow::on_pbEQMasterLOShelf_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMasterLOShelf, checked);
}

void MainWindow::on_dEQMasterMIDQ_valueChanged(int value)
{
    api->setDial(ui->dEQMasterMIDQ, value);
}

void MainWindow::on_dEQMasterMIDFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMasterMIDFreq, value);
}

void MainWindow::on_dEQMasterMIDGain_valueChanged(int value)
{
    api->setDial(ui->dEQMasterMIDGain, value);
}

void MainWindow::on_dEQMasterHIQ_valueChanged(int value)
{
    api->setDial(ui->dEQMasterHIQ, value);
}

void MainWindow::on_dEQMasterHIFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMasterHIFreq, value);
}

void MainWindow::on_dEQMasterHIGain_valueChanged(int value)
{
    api->setDial(ui->dEQMasterHIGain, value);
}

void MainWindow::on_pbEQMasterHIShelf_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMasterHIShelf, checked);
}

void MainWindow::on_dEQMasterHCFreq_valueChanged(int value)
{
    api->setDial(ui->dEQMasterHCFreq, value);
}

void MainWindow::on_pbEQMaster_toggled(bool checked)
{
    api->setPushButton(ui->pbEQMaster, checked);
}

void MainWindow::on_dEQMasterLvl_valueChanged(int value)
{
    api->setDial(ui->dEQMasterLvl, value);
}

void MainWindow::on_cbEQMic1LCType_activated(int index)
{
    api->selectComboBox(ui->cbEQMic1LCType, index);
}

void MainWindow::on_cbEQMic1HCType_activated(int index)
{
    api->selectComboBox(ui->cbEQMic1HCType, index);
}

void MainWindow::on_cbEQMic2LCType_activated(int index)
{
    api->selectComboBox(ui->cbEQMic2LCType, index);
}

void MainWindow::on_cbEQMic2HCType_activated(int index)
{
    api->selectComboBox(ui->cbEQMic2HCType, index);
}

void MainWindow::on_cbEQRoomMicLCType_activated(int index)
{
    api->selectComboBox(ui->cbEQRoomMicLCType, index);
}

void MainWindow::on_cbEQRoomMicHCType_activated(int index)
{
    api->selectComboBox(ui->cbEQRoomMicHCType, index);
}

void MainWindow::on_cbEQMasterLCType_activated(int index)
{
    api->selectComboBox(ui->cbEQMasterLCType, index);
}

void MainWindow::on_cbEQMasterHCType_activated(int index)
{
    api->selectComboBox(ui->cbEQMasterHCType, index);
}

void MainWindow::on_cbDelayMode_activated(int index)
{
    api->selectComboBox(ui->cbDelayMode, index);
}

void MainWindow::on_set_level(int index, double value, double peakValue)
{
    if (index >= 0 && index < 6) {
        levelMeter[index]->levelChanged(value, peakValue);
    }
}


/**
 * @brief MainWindow::widgetType2propertyName
 * @param wtype Type of widget
 * @return Property name
 * @details Small helper function to map property names
 */
QString MainWindow::widgetType2propertyName(WidgetType wtype) {
    switch(wtype) {
    case DIAL:
    case SLIDER:
    case PUSH_BUTTON:
    case RADIO:
        return "normalized_value";
    case COMBO:
        return "real_value";
    case LINE_EDIT:
        return "string_value";
    }
    return "val";
}


/**
 * @brief MainWindow::addDevPathInput
 * @param control Name of the control
 * @param widget Widget to map
 * @param wtype Type of the widget
 * @details Helper function to map a widget to an input device path.
 */
void MainWindow::addDevPathInput(const QString &control, QWidget *widget,
                                 WidgetType wtype)
{
    QString path = QString(
        "/devices/0123456789abcdef0123456789abcdef/device/inputs/"
        "cabinet_mic_room/effects/0/controls/%1"
    ).arg(control);

    QString propertyName = widgetType2propertyName(wtype);

    api->addDevicePath(path, propertyName, widget, wtype);
}


/**
 * @brief MainWindow::addDevPathMixer
 * @param channel Channel name on the mixer
 * @param control Type of control
 * @param widget Widget to map
 * @param wtype Type of widget
 * @details Helper function to map a widget to a mixer device path
 */
void MainWindow::addDevPathMixer(const QString &channel, const QString &control,
                                 QWidget *widget, WidgetType wtype)
{
    QString path = QString(
        "/devices/0123456789abcdef0123456789abcdef/mixers/0/channels/%1/"
        "controls/%2"
    ).arg(channel, control);

    QString propertyName = widgetType2propertyName(wtype);

    api->addDevicePath(path, propertyName, widget, wtype);
}


/**
 * @brief MainWindow::addDevPathFX
 * @param channel Channel name to map.
 * @param effect_id ID of the effect
 * @param control Control of the effect
 * @param widget Widget to map
 * @param wtype Type of the widget
 * @details Helper function to map a widget to a FX device path
 */
void MainWindow::addDevPathFX(const QString &channel, int effect_id,
                              const QString &control, QWidget *widget,
                              WidgetType wtype)
{
    QString path = QString(
        "/devices/0123456789abcdef0123456789abcdef/mixers/0/channels/%1/"
        "effects/%2/controls/%3"
    ).arg(channel, QString::number(effect_id), control);

    QString propertyName = widgetType2propertyName(wtype);

    api->addDevicePath(path, propertyName, widget, wtype);
}

/**
 * @brief MainWindow::addDevPathBusFX
 * @param channel Channel name to map.
 * @param effect_id ID of the effect
 * @param control Control of the effect
 * @param widget Widget to map
 * @param wtype Type of the widget
 * @details Helper function to map a widget to a FX device path. Bus edition.
 */
void MainWindow::addDevPathBusFX(const QString &channel, int effect_id,
                              const QString &control, QWidget *widget,
                              WidgetType wtype)
{
    QString path = QString(
        "/devices/0123456789abcdef0123456789abcdef/mixers/0/buses/%1/"
        "effects/%2/controls/%3"
    ).arg(channel, QString::number(effect_id), control);

    QString propertyName = widgetType2propertyName(wtype);

    api->addDevicePath(path, propertyName, widget, wtype);
}

void MainWindow::on_dFXDelayTimeA_valueChanged(int value)
{
    api->setDial(ui->dFXDelayTimeA, value);
}

void MainWindow::on_dFXDelayTimeB_valueChanged(int value)
{
    api->setDial(ui->dFXDelayTimeB, value);
}

void MainWindow::on_pbFXDelayMuteA_toggled(bool checked)
{
    api->setPushButton(ui->pbFXDelayMuteA, checked);
}

void MainWindow::on_pbFXDelayMuteB_toggled(bool checked)
{
    api->setPushButton(ui->pbFXDelayMuteB, checked);
}

void MainWindow::on_pbFXDelayLink_toggled(bool checked)
{
    api->setPushButton(ui->pbFXDelayLink, checked);
}

void MainWindow::on_dFXDelayFeedback_valueChanged(int value)
{
    api->setDial(ui->dFXDelayFeedback, value);
}

void MainWindow::on_dFXDelayModulationRate_valueChanged(int value)
{
    api->setDial(ui->dFXDelayModulationRate, value);
}

void MainWindow::on_cbFXDelayModulationLFO_activated(int index)
{
    api->selectComboBox(ui->cbFXDelayModulationLFO, index);
}

void MainWindow::on_dFXDelayModulationDepth_valueChanged(int value)
{
    api->setDial(ui->dFXDelayModulationDepth, value);
}

void MainWindow::on_dFXDelayFilterHiPass_valueChanged(int value)
{
   api->setDial(ui->dFXDelayFilterHiPass, value);
}

void MainWindow::on_dFXDelayFilterLoPass_valueChanged(int value)
{
   api->setDial(ui->dFXDelayFilterLoPass, value);
}

void MainWindow::on_dFXDelayPanA_valueChanged(int value)
{
   api->setDial(ui->dFXDelayPanA, value);
}

void MainWindow::on_dFXDelayPanB_valueChanged(int value)
{
   api->setDial(ui->dFXDelayPanB, value);
}

void MainWindow::on_pbFXDelayPanLink_toggled(bool checked)
{
    api->setPushButton(ui->pbFXDelayPanLink, checked);
}

void MainWindow::on_dFXDelayLevel_valueChanged(int value)
{
   api->setDial(ui->dFXDelayLevel, value);
}

void MainWindow::on_dFXDelayMix_valueChanged(int value)
{
   api->setDial(ui->dFXDelayMix, value);
}

void MainWindow::on_pbFXDelayPower_toggled(bool checked)
{
    api->setPushButton(ui->pbFXDelayPower, checked);
}

void MainWindow::on_dFXReverbTime_valueChanged(int value)
{
   api->setDial(ui->dFXReverbTime, value);
}

void MainWindow::on_dFXReverbLGain_valueChanged(int value)
{
   api->setDial(ui->dFXReverbLGain, value);
}

void MainWindow::on_dFXReverbHiGain_valueChanged(int value)
{
   api->setDial(ui->dFXReverbHiGain, value);
}

void MainWindow::on_dFXReverbPredly_valueChanged(int value)
{
   api->setDial(ui->dFXReverbPredly, value);
}

void MainWindow::on_dFXReverbMix_valueChanged(int value)
{
   api->setDial(ui->dFXReverbMix, value);
}

void MainWindow::on_dFXReverbBal_valueChanged(int value)
{
   api->setDial(ui->dFXReverbBal, value);
}

void MainWindow::on_cbFXReverbLowcut_activated(int index)
{
    api->selectComboBox(ui->cbFXReverbLowcut, index);
}

void MainWindow::on_pbFXReverbPower_toggled(bool checked)
{
   api->setPushButton(ui->pbFXReverbPower, checked);
}

void MainWindow::on_dFXCompAttack_valueChanged(int value)
{
   api->setDial(ui->dFXCompAttack, value);
}

void MainWindow::on_dFXCompInput_valueChanged(int value)
{
   api->setDial(ui->dFXCompInput, value);
}

void MainWindow::on_cbFXCompRatio_activated(int index)
{
   api->selectComboBox(ui->cbFXCompRatio, index);
}

void MainWindow::on_dFXCompRelease_valueChanged(int value)
{
   api->setDial(ui->dFXCompRelease, value);
}

void MainWindow::on_dFXCompOutput_valueChanged(int value)
{
   api->setDial(ui->dFXCompOutput, value);
}

void MainWindow::on_pbFXCompPower_toggled(bool checked)
{
   api->setPushButton(ui->pbFXCompPower, checked);
}

void MainWindow::on_vsRoomMicGain_valueChanged(int value)
{
    api->setSlider(ui->vsRoomMicGain, value);
}
