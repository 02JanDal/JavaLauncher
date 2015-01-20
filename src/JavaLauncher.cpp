#include "JavaLauncher.h"

#include <dlfcn.h>
#include <string.h>

#define JNI_GetDefaultJavaVMInitArgs static ABC_THIS_DOES_NOT_EXIST_BLA_BLA_A_
#define JNI_CreateJavaVM static ABC_THIS_DOES_NOT_EXIST_BLA_BLA_B_
#define JNI_GetCreatedJavaVMs static ABC_THIS_DOES_NOT_EXIST_BLA_BLA_C_
#define JNI_OnLoad static ABC_THIS_DOES_NOT_EXIST_BLA_BLA_D_
#define JNI_OnUnload static ABC_THIS_DOES_NOT_EXIST_BLA_BLA_E_
#include <jni.h>
#undef JNI_GetDefaultJavaVMInitArgs
#undef JNI_CreateJavaVM
#undef JNI_GetCreatedJavaVMs
#undef JNI_OnLoad
#undef JNI_OnUnload

typedef jint (*JNI_CreateJavaVM_t)(JavaVM **, void **, void *);

#define THROWING_ASSERT(condition, message) if (!condition) { throw JavaLauncherException(message); }
void JavaLauncher::launch()
{
	void *handle = dlopen(m_path.c_str(), RTLD_LAZY);
	THROWING_ASSERT(handle, dlerror());
	JNI_CreateJavaVM_t CreateJavaVM = (JNI_CreateJavaVM_t) dlsym(handle, "JNI_CreateJavaVM");
	THROWING_ASSERT(CreateJavaVM, dlerror());

	JavaVMOption *options = new JavaVMOption[m_options.size()];
	int i = 0;
	for (auto it = m_options.cbegin(); it != m_options.cend(); ++i, ++it)
	{
		options[i].optionString = strdup((*it).c_str());
	}
	JavaVMInitArgs *args = new JavaVMInitArgs;
	args->version = JNI_VERSION_1_6;
	args->nOptions = m_options.size();
	args->options = options;
	args->ignoreUnrecognized = false;

	JNIEnv *env;
	JavaVM *jvm;
	CreateJavaVM(&jvm, (void**)&env, args);
	delete options;

	jclass stringClass = env->FindClass("java/lang/String");
	THROWING_ASSERT(stringClass, "Couldn't find the Java String class");
	jobjectArray userArgs = (jobjectArray) env->NewObjectArray(m_arguments.size(),
														   stringClass,
														   env->NewStringUTF(""));
	int argsI = 0;
	for (auto it = m_arguments.cbegin(); it != m_arguments.cend(); ++argsI, ++it)
	{
		env->SetObjectArrayElement(userArgs, argsI, env->NewStringUTF((*it).c_str()));
	}

	jclass clazz = env->FindClass(m_mainClass.c_str());
	THROWING_ASSERT(clazz, "Couldn't find the main class");
	jmethodID mid = env->GetStaticMethodID(clazz, "main", "([Ljava/lang/String;)V");
	THROWING_ASSERT(mid, "Couldn't find the main method");
	env->CallStaticVoidMethod(clazz, mid, userArgs);

	jvm->DestroyJavaVM();

	dlclose(handle);
}
