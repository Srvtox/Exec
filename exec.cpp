#include <jni.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "ClangRunner"

static std::vector<char*> buildArgv(const std::string& cmd, JNIEnv* env, jobjectArray args) {
    std::vector<char*> argv;
    // argv[0] = cmd
    argv.push_back(strdup(cmd.c_str()));
    if (args) {
        jsize n = env->GetArrayLength(args);
        for (jsize i = 0; i < n; ++i) {
            jstring s = (jstring) env->GetObjectArrayElement(args, i);
            const char* c = env->GetStringUTFChars(s, nullptr);
            argv.push_back(strdup(c));
            env->ReleaseStringUTFChars(s, c);
            env->DeleteLocalRef(s);
        }
    }
    argv.push_back(nullptr);
    return argv;
}

static std::vector<std::string> buildEnvStrs(JNIEnv* env, jobjectArray envVars) {
    std::vector<std::string> out;
    if (!envVars) return out;
    jsize n = env->GetArrayLength(envVars);
    out.reserve(n);
    for (jsize i = 0; i < n; ++i) {
        jstring s = (jstring) env->GetObjectArrayElement(envVars, i);
        const char* c = env->GetStringUTFChars(s, nullptr);
        out.emplace_back(c);
        env->ReleaseStringUTFChars(s, c);
        env->DeleteLocalRef(s);
    }
    return out;
}

// ساخت ExecResult(exitCode:int, output:String)
static jobject makeExecResult(JNIEnv* env, int exitCode, const std::string& output) {
    jclass cls = env->FindClass("com/example/clangrunner/ExecResult");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(ILjava/lang/String;)V");
    jstring jout = env->NewStringUTF(output.c_str());
    jobject obj = env->NewObject(cls, ctor, (jint)exitCode, jout);
    env->DeleteLocalRef(jout);
    return obj;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_clangrunner_NativeExec_run(
        JNIEnv* env, jclass,
        jstring jcmd, jobjectArray jargs, jobjectArray jenv, jstring jcwd) {

    // cmd
    const char* cmd_c = env->GetStringUTFChars(jcmd, nullptr);
    std::string cmd(cmd_c ? cmd_c : "");
    env->ReleaseStringUTFChars(jcmd, cmd_c);

    // cwd
    std::string cwd;
    if (jcwd) {
        const char* cwd_c = env->GetStringUTFChars(jcwd, nullptr);
        cwd.assign(cwd_c ? cwd_c : "");
        env->ReleaseStringUTFChars(jcwd, cwd_c);
    }

    // args
    auto argv = buildArgv(cmd, env, jargs);

    // env
    auto envStrs = buildEnvStrs(env, jenv);

    // pipe برای گرفتن stdout+stderr
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        std::string err = std::string("pipe failed: ") + strerror(errno);
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", err.c_str());
        return makeExecResult(env, -1, err);
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::string err = std::string("fork failed: ") + strerror(errno);
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", err.c_str());
        close(pipefd[0]); close(pipefd[1]);
        return makeExecResult(env, -1, err);
    }

    if (pid == 0) {
        // child
        // بستن سمت خواندن در child
        close(pipefd[0]);

        // تغییر فولدر کاری
        if (!cwd.empty()) {
            if (chdir(cwd.c_str()) != 0) {
                // اگر نشد هم ادامه می‌دهیم ولی لاگ می‌کنیم
                __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "chdir failed: %s", strerror(errno));
            }
        }

        // هدایت stdout و stderr
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        // تنظیم محیط
        for (const auto& kv : envStrs) {
            auto pos = kv.find('=');
            if (pos != std::string::npos) {
                std::string k = kv.substr(0, pos);
                std::string v = kv.substr(pos + 1);
                setenv(k.c_str(), v.c_str(), 1);
            }
        }

        // اجرای مستقیم باینری
        execv(cmd.c_str(), argv.data());

        // اگر برگشت یعنی خطا
        // توجه: در child از JNI call نکنید
        // خروجی خطا به همان pipe می‌رود
        _exit(127);
    }

    // parent
    close(pipefd[1]);
    // خواندن خروجی
    std::string output;
    char buf[4096];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, buf + n);
    }
    close(pipefd[0]);

    int status = 0;
    int rc = waitpid(pid, &status, 0);
    int exitCode = (rc < 0) ? -1 : (WIFEXITED(status) ? WEXITSTATUS(status) : -1);

    // پاکسازی argv
    for (auto* p : argv) {
        if (p) free(p);
    }

    return makeExecResult(env, exitCode, output);
}