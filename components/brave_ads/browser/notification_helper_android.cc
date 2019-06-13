/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/browser/notification_helper_android.h"
#include "jni/NotificationSystemStatusUtil_jni.h"
#include "jni/BraveAds_jni.h"
#include "base/android/jni_string.h"

namespace brave_ads {

NotificationHelperAndroid::NotificationHelperAndroid() {
}

NotificationHelperAndroid::~NotificationHelperAndroid() {
  channels_provider_.reset();
}


bool NotificationHelperAndroid::IsNotificationsAvailable() const {
  int status = Java_NotificationSystemStatusUtil_getAppNotificationStatus(
      base::android::AttachCurrentThread());
   return (status == APP_NOTIFICATIONS_STATUS_ENABLED || status == APP_NOTIFICATIONS_STATUS_UNDETERMINABLE);
}


//Starting in Android 8.0 (API level 26), all notifications must be assigned to a channel.
bool NotificationHelperAndroid::IsBraveAdsChannelEnabled() const {
  JNIEnv* env = base::android::AttachCurrentThread();
  auto j_id = Java_BraveAds_getBraveAdsChannelId (env);
  std::string channel_id = base::android::ConvertJavaStringToUTF8 ( env, j_id);
  auto status = channels_provider_->GetChannelStatus(channel_id);
  return (NotificationChannelStatus::ENABLED == status || NotificationChannelStatus::UNAVAILABLE == status );
}


void NotificationHelperAndroid::OpenPageFromNative(const std::string & url) const {
  JNIEnv* env = base::android::AttachCurrentThread();
  base::android::ScopedJavaLocalRef<jstring> jurl =
      base::android::ConvertUTF8ToJavaString(env, url.c_str());
  Java_BraveAds_openPageFromNative(env, jurl);
}


NotificationHelperAndroid* NotificationHelperAndroid::GetInstance() {
  return base::Singleton<NotificationHelperAndroid>::get();
}

NotificationHelper* NotificationHelper::GetInstance() {
  return NotificationHelperAndroid::GetInstance();
}


}  // namespace brave_ads
