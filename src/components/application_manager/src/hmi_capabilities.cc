/**
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "config_profile/profile.h"
#include "smart_objects/smart_object.h"
#include "application_manager/message_helper.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/application_manager_impl.h"

namespace application_manager {

HMICapabilities::HMICapabilities(ApplicationManagerImpl* const app_mngr)
  : is_vr_cooperating_(false),
    is_tts_cooperating_(false),
    is_ui_cooperating_(false),
    is_navi_cooperating_(false),
    is_ivi_cooperating_(false),
    is_vr_ready_response_recieved_(false),
    is_tts_ready_response_recieved_(false),
    is_ui_ready_response_recieved_(false),
    is_navi_ready_response_recieved_(false),
    is_ivi_ready_response_recieved_(false),
    ui_language_(hmi_apis::Common_Language::INVALID_ENUM),
    vr_language_(hmi_apis::Common_Language::INVALID_ENUM),
    tts_language_(hmi_apis::Common_Language::INVALID_ENUM),
    vehicle_type_(NULL),
    attenuated_supported_(false),
    ui_supported_languages_(NULL),
    tts_supported_languages_(NULL),
    vr_supported_languages_(NULL),
    display_capabilities_(NULL),
    hmi_zone_capabilities_(NULL),
    soft_buttons_capabilities_(NULL),
    button_capabilities_(NULL),
    preset_bank_capabilities_(NULL),
    vr_capabilities_(NULL),
    speech_capabilities_(NULL),
    audio_pass_thru_capabilities_(NULL),
    app_mngr_(app_mngr){
}

HMICapabilities::~HMICapabilities() {
  delete vehicle_type_;
  delete ui_supported_languages_;
  delete tts_supported_languages_;
  delete vr_supported_languages_;
  delete display_capabilities_;
  delete hmi_zone_capabilities_;
  delete soft_buttons_capabilities_;
  delete button_capabilities_;
  delete preset_bank_capabilities_;
  delete vr_capabilities_;
  delete speech_capabilities_;
  delete audio_pass_thru_capabilities_;
}

bool HMICapabilities::is_hmi_capabilities_initialized() const {
  bool result = true;

  printf("\n##############1\n");
    if (false == profile::Profile::instance()->launch_hmi()) {
      // TODO(DK) : load HMI capabilities from file
      printf("\n##############2\n");
      return true;
    }

    if (is_vr_ready_response_recieved_ && is_tts_ready_response_recieved_
        && is_ui_ready_response_recieved_ && is_navi_ready_response_recieved_
        && is_ivi_ready_response_recieved_) {
      if (is_vr_cooperating_) {
        if ((!vr_supported_languages_)
            || (hmi_apis::Common_Language::INVALID_ENUM == vr_language_)) {
          printf("\n##############3\n");
          result = false;
        }
      }

      if (is_tts_cooperating_) {
        if ((!tts_supported_languages_)
            || (hmi_apis::Common_Language::INVALID_ENUM == tts_language_)) {
          printf("\n##############4\n");
          result = false;
        }
      }

      if (is_ui_cooperating_) {
        if ((!ui_supported_languages_)
            || (hmi_apis::Common_Language::INVALID_ENUM == ui_language_)) {
          printf("\n##############5\n");
          result = false;
        }
      }

      if (is_ivi_cooperating_) {
        if (!vehicle_type_) {
          printf("\n##############6\n");
          result = false;
        }
      }
    } else {
      printf("\n##############7\n");
      result = false;
    }

    printf("\n############## %d \n", result);
    return result;
}

bool HMICapabilities::is_vr_cooperating() const {
  return is_vr_cooperating_;
}

void HMICapabilities::set_is_vr_cooperating(bool value) {
  is_vr_ready_response_recieved_ = true;
  is_vr_cooperating_ = value;
  if (is_vr_cooperating_) {
    utils::SharedPtr<smart_objects::SmartObject> get_language(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetLanguage));
    app_mngr_->ManageHMICommand(get_language);
    utils::SharedPtr<smart_objects::SmartObject> get_all_languages(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetSupportedLanguages));
    app_mngr_->ManageHMICommand(get_all_languages);
    utils::SharedPtr<smart_objects::SmartObject> get_capabilities(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetCapabilities));
    app_mngr_->ManageHMICommand(get_capabilities);

    MessageHelper::SendHelpVrCommand();
  }
}

bool HMICapabilities::is_tts_cooperating() const {
  return is_tts_cooperating_;
}

void HMICapabilities::set_is_tts_cooperating(bool value) {
  is_tts_ready_response_recieved_ = true;
  is_tts_cooperating_ = value;
  if (is_tts_cooperating_) {
    utils::SharedPtr<smart_objects::SmartObject> get_language(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::TTS_GetLanguage));
    app_mngr_->ManageHMICommand(get_language);
    utils::SharedPtr<smart_objects::SmartObject> get_all_languages(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::TTS_GetSupportedLanguages));
    app_mngr_->ManageHMICommand(get_all_languages);
    utils::SharedPtr<smart_objects::SmartObject> get_capabilities(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::TTS_GetCapabilities));
    app_mngr_->ManageHMICommand(get_capabilities);
  }
}

bool HMICapabilities::is_ui_cooperating() const {
  return is_ui_cooperating_;
}

void HMICapabilities::set_is_ui_cooperating(bool value) {
  is_ui_ready_response_recieved_ = true;
  is_ui_cooperating_ = value;
  if (is_ui_cooperating_) {
    utils::SharedPtr<smart_objects::SmartObject> get_language(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::UI_GetLanguage));
    app_mngr_->ManageHMICommand(get_language);
    utils::SharedPtr<smart_objects::SmartObject> get_all_languages(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::UI_GetSupportedLanguages));
    app_mngr_->ManageHMICommand(get_all_languages);
    utils::SharedPtr<smart_objects::SmartObject> get_capabilities(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::UI_GetCapabilities));
    app_mngr_->ManageHMICommand(get_capabilities);
  }
}

bool HMICapabilities::is_navi_cooperating() const {
  return is_navi_cooperating_;
}

void HMICapabilities::set_is_navi_cooperating(bool value) {
  is_navi_ready_response_recieved_ = true;
  is_navi_cooperating_ = value;
}

bool HMICapabilities::is_ivi_cooperating() const {
  return is_ivi_cooperating_;
}

void HMICapabilities::set_is_ivi_cooperating(bool value) {
  is_ivi_ready_response_recieved_ = true;
  is_ivi_cooperating_ = value;
  if (is_ivi_cooperating_) {
    utils::SharedPtr<smart_objects::SmartObject> get_type(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::VehicleInfo_GetVehicleType));
    app_mngr_->ManageHMICommand(get_type);
  }
}

bool HMICapabilities::attenuated_supported() const {
  return attenuated_supported_;
}

void HMICapabilities::set_attenuated_supported(bool state) {
  attenuated_supported_ = state;
}

void HMICapabilities::set_active_ui_language(
  const hmi_apis::Common_Language::eType& language) {
  ui_language_ = language;
}

void HMICapabilities::set_active_vr_language(
  const hmi_apis::Common_Language::eType& language) {
  vr_language_ = language;
}

void HMICapabilities::set_active_tts_language(
  const hmi_apis::Common_Language::eType& language) {
  tts_language_ = language;
}

void HMICapabilities::set_vehicle_type(
  const smart_objects::SmartObject& vehicle_type) {
  if (vehicle_type_) {
    delete vehicle_type_;
  }
  vehicle_type_ = new smart_objects::SmartObject(vehicle_type);
}

const smart_objects::SmartObject* HMICapabilities::vehicle_type() const {
  return vehicle_type_;
}

void HMICapabilities::set_ui_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (ui_supported_languages_) {
    delete ui_supported_languages_;
  }
  ui_supported_languages_ = new smart_objects::SmartObject(supported_languages);
}

void HMICapabilities::set_tts_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (tts_supported_languages_) {
    delete tts_supported_languages_;
  }
  tts_supported_languages_ = new smart_objects::SmartObject(
      supported_languages);
}

void HMICapabilities::set_vr_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (vr_supported_languages_) {
    delete vr_supported_languages_;
  }
  vr_supported_languages_ = new smart_objects::SmartObject(supported_languages);
}

bool HMICapabilities::VerifyImageType(int image_type) const {
  if (!display_capabilities_) {
    return false;
  }

  if (display_capabilities_->keyExists(hmi_response::image_capabilities)) {
    const smart_objects::SmartObject& image_caps = display_capabilities_
        ->getElement(hmi_response::image_capabilities);
    for (int i = 0; i < image_caps.length(); ++i) {
      if (image_caps.getElement(i).asInt() == image_type) {
        return true;
      }
    }
  }

  return false;
}

void HMICapabilities::set_display_capabilities(
    const smart_objects::SmartObject& display_capabilities) {
  if (display_capabilities_) {
    delete display_capabilities_;
  }
  display_capabilities_ = new smart_objects::SmartObject(display_capabilities);
}

void HMICapabilities::set_hmi_zone_capabilities(
    const smart_objects::SmartObject& hmi_zone_capabilities) {
  if (hmi_zone_capabilities_) {
    delete hmi_zone_capabilities_;
  }
  hmi_zone_capabilities_ = new smart_objects::SmartObject(
      hmi_zone_capabilities);
}

void HMICapabilities::set_soft_button_capabilities(
    const smart_objects::SmartObject& soft_button_capabilities) {
  if (soft_buttons_capabilities_) {
    delete soft_buttons_capabilities_;
  }
  soft_buttons_capabilities_ = new smart_objects::SmartObject(
      soft_button_capabilities);
}

void HMICapabilities::set_button_capabilities(
    const smart_objects::SmartObject& button_capabilities) {
  if (button_capabilities_) {
    delete button_capabilities_;
  }
  button_capabilities_ = new smart_objects::SmartObject(button_capabilities);
}

void HMICapabilities::set_vr_capabilities(
    const smart_objects::SmartObject& vr_capabilities) {
  if (vr_capabilities_) {
    delete vr_capabilities_;
  }
  vr_capabilities_ = new smart_objects::SmartObject(vr_capabilities);
}

void HMICapabilities::set_speech_capabilities(
    const smart_objects::SmartObject& speech_capabilities) {
  if (speech_capabilities_) {
    delete speech_capabilities_;
  }
  speech_capabilities_ = new smart_objects::SmartObject(speech_capabilities);
}

void HMICapabilities::set_audio_pass_thru_capabilities(
    const smart_objects::SmartObject& audio_pass_thru_capabilities) {
  if (audio_pass_thru_capabilities_) {
    delete audio_pass_thru_capabilities_;
  }
  audio_pass_thru_capabilities_ = new smart_objects::SmartObject(
      audio_pass_thru_capabilities);
}

void HMICapabilities::set_preset_bank_capabilities(
    const smart_objects::SmartObject& preset_bank_capabilities) {
  if (preset_bank_capabilities_) {
    delete preset_bank_capabilities_;
  }
  preset_bank_capabilities_ = new smart_objects::SmartObject(
      preset_bank_capabilities);
}

}  //  namespace application_manager
