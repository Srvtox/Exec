#ifndef _PROCESS_H
#define _PROCESS_H 1

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "ClangRunner"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL Java_com_example_nativeexec_NativeExec_nativeExecCommand
  (JNIEnv *, jclass, jstring, jobjectArray, jobjectArray, jstring);

#ifdef __cplusplus
}
#endif

#endif /* _PROCESS_H */