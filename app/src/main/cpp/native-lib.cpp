#include <jni.h>
#include <string>
#include <android/log.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <locale>
#include <ios>


#define APPNAME "FridaDetectionTest"

#define  TAG    "r0add"

// 定义info信息

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

// 定义debug信息

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

// 定义error信息

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

int r0add(int x, int y) {
    int i;
    for (i = 0; i < x; i++) {
        LOGI("now i is %i", i);
        i+=y;
    }
    return i;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xiaoeryu_demoso1_MainActivity_stringFromJNI(JNIEnv *env, jclass clazz) {
    jclass testClass = env->FindClass("com/xiaoeryu/demoso1/Test");
    jfieldID publicStaticField = env->GetStaticFieldID(testClass, "publicStaticField", "Ljava/lang/String;");
    jstring publicStaticField_value = (jstring) env->GetStaticObjectField(testClass, publicStaticField);

    const char *value_ptr = env->GetStringUTFChars(publicStaticField_value, NULL);
    LOGI("now content is %s", value_ptr);

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL xiaoeryuJNI2(JNIEnv *env, jclass clazz) {
    jclass testClass = env->FindClass("com/xiaoeryu/demoso1/Test");
    jfieldID publicStaticField = env->GetStaticFieldID(testClass, "publicStaticField", "Ljava/lang/String;");
    jstring publicStaticField_value = (jstring) env->GetStaticObjectField(testClass, publicStaticField);

    const char *value_ptr = env->GetStringUTFChars(publicStaticField_value, NULL);
    LOGI("now content is %s", value_ptr);

    std::string hello = "Hello from C++ stringFromJNI2";
    return env->NewStringUTF(hello.c_str());
}



extern "C"
JNIEXPORT jstring JNICALL
Java_com_xiaoeryu_demoso1_MainActivity_myfirstjniJNI(JNIEnv *env, jclass clazz, jstring context) {
    const char* a = env->GetStringUTFChars(context, NULL);
    int context_len = env->GetStringUTFLength(context);
    if (a != 0){

        LOGI("context is %s", a);
        LOGI("context_len is %d", context_len);
    }
    env->ReleaseStringUTFChars(context, a);
    jstring result = env->NewStringUTF("hello I'm from myfirstjniJNI!");
    return result;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_xiaoeryu_demoso1_MainActivity_myfirstjni(JNIEnv *env, jclass thiz) {
    return r0add(11, 22);
}

void *detect_frida_loop(void *){
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    inet_aton("0.0.0.0",&(saddr.sin_addr));
    int sock;
    int i;
    int ret;
    char res[7];
    while (1){
        // 1. Frida Server Detection
        for(i = 20000; i < 30000;i++){
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                LOGE("socket error");
                continue;
            }
            saddr.sin_port = htons(i);
            LOGI("entering frida server detect loop started, now i is %d", i);
            if (connect(sock, (struct sockaddr*)&saddr, sizeof(saddr)) != -1){
                memset(res, 0, 7);
                send(sock, "\x00", 1, NULL);
                send(sock, "AUTH\r\n", 6, NULL);
                usleep(500);    // Give it some time to answer
                if ((ret = recv(sock, res, 6, MSG_DONTWAIT)) != -1){
                    if (strcmp(res, "REJECT") ==0){
                        LOGI("FOUND FRIDA SERVER: %S, FRIDA DETECTED [1] - frida server running on port %d!", APPNAME, i);
                    } else{
                        LOGI("NOT FOUND FRIDA SERVER");
                    }
                }
            }
            close(sock);
        }
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_xiaoeryu_demoso1_MainActivity_init(JNIEnv *env, jobject thiz) {
    pthread_t t;
    pthread_create(&t, NULL, detect_frida_loop, NULL);
    LOGI("frida server detect loop started");
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    JNINativeMethod methods[] = {
            {"stringFromJNI2", "()Ljava/lang/String;", (void *) xiaoeryuJNI2},
    };
    env->RegisterNatives(env->FindClass("com/xiaoeryu/demoso1/MainActivity"), methods, 1);
    return JNI_VERSION_1_6;
}