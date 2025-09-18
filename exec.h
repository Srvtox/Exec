#ifndef _JACKPAL_PROCESS_H
#define _JACKPAL_PROCESS_H 1

#include <stddef.h>
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "jackpal-termexec"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_com_example_clangrunner_NativeExec_run
  (JNIEnv *, jclass, jstring, jobjectArray, jobjectArray, jstring);

#ifdef __cplusplus
}
#endif

#endif /* _JACKPAL_PROCESS_H */