反射系统:生成、收集、构造注册、绑定链接

/* ref：insideue5
一个UClass*要经历这么几个阶段：

内存构造。刚创建出来一块白纸一般的内存，简单的调用了UClass的构造函数。UE里一个对象的构造，构造函数的调用只是个起点而已。
注册。给自己一个名字，把自己登记在对象系统中。这步是通过DeferredRegister而不是Register完成的。
对象构造。往对象里填充属性、函数、接口和元数据的信息。这步我们应该记得是在gen.cpp里的那些函数完成的。
绑定链接。属性函数都有了，把它们整理整理，尽量优化一下存储结构，为以后的使用提供更高性能和便利。
CDO创建。既然类型已经有了，那就万事俱备只差国家包分配一个类默认对象了。每个UClass都有一个CDO（Class Default Object），有了CDO，相当于有了一个存档备份和参照，其他对象就心不慌。
引用记号流构建。一个Class是怎么样有可能引用其他别的对象的，这棵引用树怎么样构建的高效，也是GC中一个非常重要的话题。有了引用记号流，就可以对一个对象高效的分析它引用了其他多少对象。
*/

生成阶段：一部分是生成各种Z_辅助方法用来构造出各种UClass*等对象；另一部分是都包含着一两个static对象用来在程序启动的时候驱动登记，继而调用到前者的Z_方法，最终完成注册。
    UHT分析源码的宏标记并生成了包含程序元信息的代码，继而编译进程序，在程序启动的时候，开始启动类型系统的后续构建阶段。
//MyObject.h
//--------------------------------------------
    #pragma once

    #include "CoreMinimal.h"
    #include "UObject/NoExportTypes.h"
    #include "MyObject.generated.h"

    UCLASS()
    class MYPROJECT_API UMyObject : public UObject
    {
        GENERATED_BODY()
    };
//--------------------------------------------

//GENERATED_BODY() 展开
//--------------------------------------------
    BODY_MACRO_COMBINE(CURRENT_FILE_ID,_,__LINE__,_GENERATED_BODY) 展开
        BODY_MACRO_COMBINE_INNER(A,B,C,D) 展开
            A##B##C##D
    CURRENT_FILE_ID的定义是在MyClass.generated.h的89行 #define CURRENT_FILE_ID FID_MyProject_Source_MyProject_MyObject_h
    __LINE__标准宏指向了该宏使用时候的的函数，这里是15。加了一个__LINE__宏的目的是为了支持在同一个文件内声明多个类，比如在UMyObject.h里接着再声明UMyCObject2，就可以支持生成不同的宏名称。
    所以总的宏展开为 FID_MyProject_Source_MyProject_MyObject_h_15_GENERATED_BODY 定义在MyObject.generated.h的75行
//--------------------------------------------


//MyObject.generated.h 解析
//--------------------------------------------

    #include "UObject/ObjectMacros.h"
    #include "UObject/ScriptMacros.h"

    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    #ifdef MYPROJECT_MyObject_generated_h
    #error "MyObject.generated.h already included, missing '#pragma once' in MyObject.h"
    #endif
    #define MYPROJECT_MyObject_generated_h

    #define FID_MyProject_Source_MyProject_MyObject_h_15_SPARSE_DATA
    #define FID_MyProject_Source_MyProject_MyObject_h_15_RPC_WRAPPERS
    #define FID_MyProject_Source_MyProject_MyObject_h_15_RPC_WRAPPERS_NO_PURE_DECLS
    #define FID_MyProject_Source_MyProject_MyObject_h_15_INCLASS_NO_PURE_DECLS \
    private: \
        static void StaticRegisterNativesUMyObject(); \
        friend struct Z_Construct_UClass_UMyObject_Statics; \
    public: \
        DECLARE_CLASS(UMyObject, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/MyProject"), NO_API) \ 
        DECLARE_SERIALIZER(UMyObject)


    #define FID_MyProject_Source_MyProject_MyObject_h_15_INCLASS \
    private: \
        static void StaticRegisterNativesUMyObject(); \
        friend struct Z_Construct_UClass_UMyObject_Statics; \//一个构造该类UClass对象的辅助函数
    public: \
        DECLARE_CLASS(UMyObject, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/MyProject"), NO_API) \//声明该类的一些通用基本函数
        DECLARE_SERIALIZER(UMyObject)//声明序列化函数


    #define FID_MyProject_Source_MyProject_MyObject_h_15_STANDARD_CONSTRUCTORS \//构造函数的默认实现
        /** Standard constructor, called after all reflected properties have been initialized */ \
        NO_API UMyObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
        DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UMyObject) \
        DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UMyObject); \
        DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMyObject); \
    private: \
        /** Private move- and copy-constructors, should never be used */ \
        NO_API UMyObject(UMyObject&&); \
        NO_API UMyObject(const UMyObject&); \
    public:


    #define FID_MyProject_Source_MyProject_MyObject_h_15_ENHANCED_CONSTRUCTORS \//调用Super(ObjectInitializer) { }; 的构造函数的默认实现
        /** Standard constructor, called after all reflected properties have been initialized */ \
        NO_API UMyObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
    private: \
        /** Private move- and copy-constructors, should never be used */ \
        NO_API UMyObject(UMyObject&&); \
        NO_API UMyObject(const UMyObject&); \
    public: \
        DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UMyObject); \
        DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMyObject); \
        DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UMyObject)


    #define FID_MyProject_Source_MyProject_MyObject_h_12_PROLOG
    #define FID_MyProject_Source_MyProject_MyObject_h_15_GENERATED_BODY_LEGACY \
    PRAGMA_DISABLE_DEPRECATION_WARNINGS \
    public: \
        FID_MyProject_Source_MyProject_MyObject_h_15_SPARSE_DATA \
        FID_MyProject_Source_MyProject_MyObject_h_15_RPC_WRAPPERS \
        FID_MyProject_Source_MyProject_MyObject_h_15_INCLASS \
        FID_MyProject_Source_MyProject_MyObject_h_15_STANDARD_CONSTRUCTORS \ 
    public: \
    PRAGMA_ENABLE_DEPRECATION_WARNINGS


    #define FID_MyProject_Source_MyProject_MyObject_h_15_GENERATED_BODY \ //GENERATED_BODY的展开
    PRAGMA_DISABLE_DEPRECATION_WARNINGS \
    public: \
        FID_MyProject_Source_MyProject_MyObject_h_15_SPARSE_DATA \
        FID_MyProject_Source_MyProject_MyObject_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
        FID_MyProject_Source_MyProject_MyObject_h_15_INCLASS_NO_PURE_DECLS \
        FID_MyProject_Source_MyProject_MyObject_h_15_ENHANCED_CONSTRUCTORS \ 
    private: \
    PRAGMA_ENABLE_DEPRECATION_WARNINGS


    template<> MYPROJECT_API UClass* StaticClass<class UMyObject>();

    #undef CURRENT_FILE_ID
    #define CURRENT_FILE_ID FID_MyProject_Source_MyProject_MyObject_h //文件的ID


    PRAGMA_ENABLE_DEPRECATION_WARNINGS
//--------------------------------------------

//MyObject.gen.cpp ：大概就是根据一些参数构造一个单例静态类
//--------------------------------------------
    #include "UObject/GeneratedCppIncludes.h"
    #include "MyProject/MyObject.h"
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    void EmptyLinkFunctionForGeneratedCodeMyObject() {}
    // Cross Module References
        MYPROJECT_API UClass* Z_Construct_UClass_UMyObject_NoRegister();//构造UMyObject对应的UClass对象，没有后续的注册过程
        MYPROJECT_API UClass* Z_Construct_UClass_UMyObject();//构造UMyObject对应的UClass对象
        COREUOBJECT_API UClass* Z_Construct_UClass_UObject();//构造UObject对应的UClass对象
        UPackage* Z_Construct_UPackage__Script_MyProject();// //构造MyProject本身的UPackage对象
    // End Cross Module References
        void UMyObject::StaticRegisterNativesUMyObject()
        {
        }
        IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMyObject);
        UClass* Z_Construct_UClass_UMyObject_NoRegister()
        {
            return UMyObject::StaticClass();//直接返回静态类
        }
        struct Z_Construct_UClass_UMyObject_Statics //构造单例需要的参数
        {
            static UObject* (*const DependentSingletons[])(); 
    #if WITH_METADATA
            static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
    #endif
            static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
            static const UECodeGen_Private::FClassParams ClassParams;
        };
        UObject* (*const Z_Construct_UClass_UMyObject_Statics::DependentSingletons[])() = { //构造一个单例UObject类
            (UObject* (*)())Z_Construct_UClass_UObject,
            (UObject* (*)())Z_Construct_UPackage__Script_MyProject,
        };
    #if WITH_METADATA
        const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMyObject_Statics::Class_MetaDataParams[] = {
            { "Comment", "/**\n * \n */" },
            { "IncludePath", "MyObject.h" },
            { "ModuleRelativePath", "MyObject.h" },
        };
    #endif
        const FCppClassTypeInfoStatic Z_Construct_UClass_UMyObject_Statics::StaticCppClassTypeInfo = {
            TCppClassTypeTraits<UMyObject>::IsAbstract,
        };
            /*
        	struct FClassParams
                {
                    UClass*                                   (*ClassNoRegisterFunc)();
                    const char*                                 ClassConfigNameUTF8;
                    const FCppClassTypeInfoStatic*              CppClassInfo;
                    UObject*                           (*const *DependencySingletonFuncArray)();
                    const FClassFunctionLinkInfo*               FunctionLinkArray;
                    const FPropertyParamsBase* const*           PropertyArray;
                    const FImplementedInterfaceParams*          ImplementedInterfaceArray;
                    int32                                       NumDependencySingletons;
                    int32                                       NumFunctions;
                    int32                                       NumProperties;
                    int32                                       NumImplementedInterfaces;
                    uint32                                      ClassFlags; // EClassFlags
            #if WITH_METADATA
                    const FMetaDataPairParam*                   MetaDataArray;
                    int32                                       NumMetaData;
            #endif
                };
            */
        const UECodeGen_Private::FClassParams Z_Construct_UClass_UMyObject_Statics::ClassParams = { //上述类参数
            &UMyObject::StaticClass,
            nullptr,
            &StaticCppClassTypeInfo,
            DependentSingletons,
            nullptr,
            nullptr,
            nullptr,
            UE_ARRAY_COUNT(DependentSingletons),
            0,
            0,
            0,
            0x001000A0u,
            METADATA_PARAMS(Z_Construct_UClass_UMyObject_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UMyObject_Statics::Class_MetaDataParams))
        };
        UClass* Z_Construct_UClass_UMyObject()
        {
            if (!Z_Registration_Info_UClass_UMyObject.OuterSingleton)
            {
                UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMyObject.OuterSingleton, Z_Construct_UClass_UMyObject_Statics::ClassParams);
            }
            return Z_Registration_Info_UClass_UMyObject.OuterSingleton;
        }
        template<> MYPROJECT_API UClass* StaticClass<UMyObject>()
        {
            return UMyObject::StaticClass();
        }
        DEFINE_VTABLE_PTR_HELPER_CTOR(UMyObject); //hotload相关
        struct Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_Statics
        {
            static const FClassRegisterCompiledInInfo ClassInfo[];
        };
        const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_Statics::ClassInfo[] = {//注册编译信息
            { Z_Construct_UClass_UMyObject, UMyObject::StaticClass, TEXT("UMyObject"), &Z_Registration_Info_UClass_UMyObject, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMyObject), 464720050U) },
        };
        static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_148784150(TEXT("/Script/MyProject"),
            Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_Statics::ClassInfo),
            nullptr, 0,
            nullptr, 0);
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
//--------------------------------------------

//在MyObject.h 添加属性和函数
//--------------------------------------------
    UCLASS()
    class MYPROJECT_API UMyObject : public UObject
    {
        GENERATED_BODY()
    public:
        UPROPERTY(BlueprintReadWrite)
            float Score;
    public:
        UFUNCTION(BlueprintCallable, Category = "Hello")
            void CallableFunc();    //C++实现，蓝图调用

        UFUNCTION(BlueprintNativeEvent, Category = "Hello")
            void NativeFunc();//C++实现默认版本，蓝图可重载实现
            virtual void NativeFunc_Implementation();
        UFUNCTION(BlueprintImplementableEvent, Category = "Hello")
            void ImplementableFunc();   //C++不实现，蓝图实现
    };
//--------------------------------------------

//MyObject.generated.h 新增
//--------------------------------------------
因为CallableFunc是C++里实现的，所以这里并不需要再定义函数体。而另外两个函数其实是在蓝图里定义的，就需要专门生成exec前缀的函数供蓝图虚拟机调用。
#define FID_MyProject_Source_MyProject_MyObject_h_15_RPC_WRAPPERS \
	virtual void NativeFunc_Implementation(); \
 \
	DECLARE_FUNCTION(execNativeFunc); \ 
	DECLARE_FUNCTION(execCallableFunc);//声明供蓝图调用的函数


#define FID_MyProject_Source_MyProject_MyObject_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execNativeFunc); \
	DECLARE_FUNCTION(execCallableFunc);
//--------------------------------------------


//MyObject.gen.cpp 新增
//--------------------------------------------
    DEFINE_FUNCTION(UMyObject::execNativeFunc)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->NativeFunc_Implementation();//包含c++端的实现
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UMyObject::execCallableFunc)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->CallableFunc();
		P_NATIVE_END;
	}
	static FName NAME_UMyObject_ImplementableFunc = FName(TEXT("ImplementableFunc"));
	void UMyObject::ImplementableFunc()
	{
		ProcessEvent(FindFunctionChecked(NAME_UMyObject_ImplementableFunc),NULL);
	}
	static FName NAME_UMyObject_NativeFunc = FName(TEXT("NativeFunc"));
	void UMyObject::NativeFunc()
	{
		ProcessEvent(FindFunctionChecked(NAME_UMyObject_NativeFunc),NULL);
	}
	void UMyObject::StaticRegisterNativesUMyObject()//注册函数名字和函数指针映射，StaticRegisterNativesUMyObject总是空的，在这里UHT为它加上了把函数注册进程序内存的操作。
	{
		UClass* Class = UMyObject::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CallableFunc", &UMyObject::execCallableFunc },
			{ "NativeFunc", &UMyObject::execNativeFunc },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UMyObject_CallableFunc_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UMyObject_CallableFunc_Statics::Function_MetaDataParams[] = {
		{ "Category", "Hello" },
		{ "ModuleRelativePath", "MyObject.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMyObject_CallableFunc_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMyObject, nullptr, "CallableFunc", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UMyObject_CallableFunc_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UMyObject_CallableFunc_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UMyObject_CallableFunc()//对于CallableFunc这种C++实现，蓝图只是调用的方法，生成的代码只是生成了相应的UFunction*对象。
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMyObject_CallableFunc_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UMyObject_ImplementableFunc_Statics//对于NativeFunc和ImplementableFunc，我们不会在C++里写上它们的实现，因此为了编译通过，也为了可以从C++端直接调用，就需要在生成的代码的时候也同样生成一份默认实现。
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UMyObject_ImplementableFunc_Statics::Function_MetaDataParams[] = {
		{ "Category", "Hello" },
		{ "ModuleRelativePath", "MyObject.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMyObject_ImplementableFunc_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMyObject, nullptr, "ImplementableFunc", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UMyObject_ImplementableFunc_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UMyObject_ImplementableFunc_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UMyObject_ImplementableFunc()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMyObject_ImplementableFunc_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UMyObject_NativeFunc_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UMyObject_NativeFunc_Statics::Function_MetaDataParams[] = {
		{ "Category", "Hello" },
		{ "Comment", "//C++\xca\xb5?\xd6\xa3???\xcd\xbc????\n" },
		{ "ModuleRelativePath", "MyObject.h" },
		{ "ToolTip", "C++\xca\xb5?\xd6\xa3???\xcd\xbc????" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMyObject_NativeFunc_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMyObject, nullptr, "NativeFunc", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UMyObject_NativeFunc_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UMyObject_NativeFunc_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UMyObject_NativeFunc()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMyObject_NativeFunc_Statics::FuncParams);
		}
		return ReturnFunction;
	}
    //添加函数名字映射
    const FClassFunctionLinkInfo Z_Construct_UClass_UMyObject_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UMyObject_CallableFunc, "CallableFunc" }, // 1654178037
		{ &Z_Construct_UFunction_UMyObject_ImplementableFunc, "ImplementableFunc" }, // 3520952485
		{ &Z_Construct_UFunction_UMyObject_NativeFunc, "NativeFunc" }, // 2943164579
	};
//--------------------------------------------



收集&&注册阶段：把散乱分布在各个.h.cpp文件里的元数据都收集到我们想要的数据结构里保存，以便下一个阶段的使用。
在程序启动的时候，UE利用了Static自动注册模式把所有类的信息都一一登记一遍,完成收集工作
//--------------------------------------------
    // 自动注册
    // //--------------------------------------------
    // template <typename TClass>
    // struct TClassCompiledInDefer : public FFieldCompiledInInfo
    // {
    //     TClassCompiledInDefer(const TCHAR* InName, SIZE_T InClassSize, uint32 InCrc)
    //     : FFieldCompiledInInfo(InClassSize, InCrc)
    //     {
    //         UClassCompiledInDefer(this, InName, InClassSize, InCrc);
    //     }
    //     virtual UClass* Register() const override
    //     {
    //         return TClass::StaticClass();
    //     }
    // };

    // static TClassCompiledInDefer<TClass> AutoInitialize##TClass(TEXT(#TClass), sizeof(TClass), TClassCrc); 
    // //--------------------------------------------
    // or
    // //--------------------------------------------
    // struct FCompiledInDefer
    // {
    //     FCompiledInDefer(class UClass *(*InRegister)(), class UClass *(*InStaticClass)(), const TCHAR* Name, bool bDynamic, const TCHAR* DynamicPackageName = nullptr, const TCHAR* DynamicPathName = nullptr, void (*InInitSearchableValues)(TMap<FName, FName>&) = nullptr)
    //     {
    //         if (bDynamic)
    //         {
    //             GetConvertedDynamicPackageNameToTypeName().Add(FName(DynamicPackageName), FName(Name));
    //         }
    //         UObjectCompiledInDefer(InRegister, InStaticClass, Name, bDynamic, DynamicPathName, InInitSearchableValues);
    //     }
    // };
    // static FCompiledInDefer Z_CompiledInDefer_UClass_UMyClass(Z_Construct_UClass_UMyClass, &UMyClass::StaticClass, TEXT("UMyClass"), false, nullptr, nullptr, nullptr);
    // //--------------------------------------------

    class的收集
    //--------------------------------------------
    struct Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UMyObject, UMyObject::StaticClass, TEXT("UMyObject"), &Z_Registration_Info_UClass_UMyObject, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMyObject), 2545286149U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_3056373916(TEXT("/Script/MyProject"),
		Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MyProject_Source_MyProject_MyObject_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
    //--------------------------------------------

    将注册信息存到一个专用的的数据结构中
    //--------------------------------------------
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    void UClassCompiledInDefer(FFieldCompiledInInfo* ClassInfo, const TCHAR* Name, SIZE_T ClassSize, uint32 Crc)
    {
        FClassDeferredRegistry& Registry = FClassDeferredRegistry::Get();
        FClassRegistrationInfo& Info = Registry.MakeDeprecatedInfo(ClassInfo->ClassPackage(), Name);
        Registry.AddRegistration(nullptr, nullptr, ClassInfo->ClassPackage(), Name, Info, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, ClassSize, Crc), ClassInfo);
    }
    //--------------------------------------------
//--------------------------------------------


    Static初始化
    //--------------------------------------------
    NoExportTypes.h：把CoreUObject模块里的一些基础类型喂给UHT来生成类型的元数据信息。
    这里面的部分是不参与编译的，所以不会产生定义冲突，但是却可以让UHT分析，因为UHT只是个文本分析器而已。
    //--------------------------------------------
        UCLASS(abstract, noexport)
        class UObject
        {
            GENERATED_BODY()
        public:

            UObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
            UObject(FVTableHelper& Helper);
            
            /**
            * Executes some portion of the ubergraph.
            *
            * @param	EntryPoint	The entry point to start code execution at.
            */
            UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintInternalUseOnly = "true"))
            void ExecuteUbergraph(int32 EntryPoint);
        };
    //--------------------------------------------
    而NoExportTypes.gen.cpp就和之前讲过的元数据代码生成一样的内容结构了。文件拉到最后你就会看到IMPLEMENT_CLASS(UObject, 1563732853);的定义了。//没找到对应文件，但是先跟着文章走了
    而IMPLEMENT_CLASS根据前文的介绍的展开，里面定义着GetPrivateStaticClass的实现。 

        //GetPrivateStaticClass
        //--------------------------------------------
        UClass* TClass::GetPrivateStaticClass() \
        { \
            if (!Z_Registration_Info_UClass_##TClass.InnerSingleton) \
            { \
                /* this could be handled with templates, but we want it external to avoid code bloat */ \
                GetPrivateStaticClassBody( \
                    StaticPackage(), \ //包名
                    (TCHAR*)TEXT(#TClass) + 1 + ((StaticClassFlags & CLASS_Deprecated) ? 11 : 0), \ //类名，+1去掉U、A、F前缀，+11去掉Deprecated_前缀
                    Z_Registration_Info_UClass_##TClass.InnerSingleton, \ 
                    StaticRegisterNatives##TClass, \ //注册类Native函数的指针
                    sizeof(TClass), \  //类大小
                    alignof(TClass), \
                    (EClassFlags)TClass::StaticClassFlags, \ //类标记，值为CLASS_Intrinsic，表示在C++代码里定义的类
                    TClass::StaticClassCastFlags(), \ //虽然是调用，但只是简单返回值CASTCLASS_None
                    TClass::StaticConfigName(), \  //配置文件名，用于从config里读取值
                    (UClass::ClassConstructorType)InternalConstructor<TClass>, \  //构造函数指针，包了一层
                    (UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>, \ //hotreload的时候使用来构造虚函数表，暂时不管
                    &TClass::AddReferencedObjects, \//GC使用的添加额外引用对象的静态函数指针，若没有定义，则会调用到UObject::AddReferencedObjects，默认函数体为空。
                    &TClass::Super::StaticClass, \  //获取基类UClass*的函数指针，这里Super是UObject
                    &TClass::WithinClass::StaticClass \  //获取对象外部类UClass*的函数指针，默认是UObject
                ); \
            } \
            return Z_Registration_Info_UClass_##TClass.InnerSingleton; \
        }
        //--------------------------------------------

        //GetPrivateStaticClassBody
        //--------------------------------------------
                void GetPrivateStaticClassBody(
                const TCHAR* PackageName,
                const TCHAR* Name,
                UClass*& ReturnClass,
                void(*RegisterNativeFunc)(),
                uint32 InSize,
                uint32 InAlignment,
                EClassFlags InClassFlags,
                EClassCastFlags InClassCastFlags,
                const TCHAR* InConfigName,
                UClass::ClassConstructorType InClassConstructor,
                UClass::ClassVTableHelperCtorCallerType InClassVTableHelperCtorCaller,
                UClass::ClassAddReferencedObjectsType InClassAddReferencedObjects,
                UClass::StaticClassFunctionType InSuperClassFn,
                UClass::StaticClassFunctionType InWithinClassFn
                )
            {
                //。。。

                ReturnClass = (UClass*)GUObjectAllocator.AllocateUObject(sizeof(UClass), alignof(UClass), true); //分配内存
                ReturnClass = ::new (ReturnClass) //用placement new在内存上手动调用构造函数
                    UClass
                    (
                    EC_StaticConstructor,
                    Name,
                    InSize,
                    InAlignment,
                    InClassFlags,
                    InClassCastFlags,
                    InConfigName,
                    EObjectFlags(RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet),
                    InClassConstructor,
                    InClassVTableHelperCtorCaller,
                    InClassAddReferencedObjects
                    );
                check(ReturnClass);
                
                InitializePrivateStaticClass( //初始化UClass*对象
                    InSuperClassFn(),
                    ReturnClass,
                    InWithinClassFn(),
                    PackageName,
                    Name
                    );
                    //调用到 TClass_PrivateStaticClass->Register(PackageName, Name); 简单的先记录一下信息到一个全局单件Map里和一个全局链表里（LRU）。等后续对象的存储结构准备好了，就可以把这些UClass*对象再拉出来继续构造了。
                    //后续的初始化对象存储机制的函数调用是InitUObject()，继续构造的操作是在ProcessNewlyLoadedUObjects()里的。
                    //--------------------------------------------
                    /** Enqueue the registration for this object. */
                    void UObjectBase::Register(const TCHAR* PackageName,const TCHAR* InName)
                    {
                        TMap<UObjectBase*, FPendingRegistrantInfo>& PendingRegistrants = FPendingRegistrantInfo::GetMap();////添加到全局单件Map里，用对象指针做Key，Value是对象的名字和所属包的名字。

                        FPendingRegistrant* PendingRegistration = new FPendingRegistrant(this);
                        PendingRegistrants.Add(this, FPendingRegistrantInfo(InName, PackageName)); // //添加到全局链表里，每个链表节点带着一个本对象指针，简单的链表添加操作。

                    #if USE_PER_MODULE_UOBJECT_BOOTSTRAP
                        if (FName(PackageName) != FName("/Script/CoreUObject"))
                        {
                            TMap<FName, TArray<FPendingRegistrant*>>& PerModuleMap = GetPerModuleBootstrapMap();

                            PerModuleMap.FindOrAdd(FName(PackageName)).Add(PendingRegistration);
                        }
                        else
                    #endif
                        {
                            if (GLastPendingRegistrant)
                            {
                                GLastPendingRegistrant->NextAutoRegister = PendingRegistration;
                            }
                            else
                            {
                                check(!GFirstPendingRegistrant);
                                GFirstPendingRegistrant = PendingRegistration;
                            }
                            GLastPendingRegistrant = PendingRegistration;
                        }
                    }

                    //--------------------------------------------

                // Register the class's native functions.
                RegisterNativeFunc();//注册Native函数到UClass中去
                //展开
                //--------------------------------------------
                    void UMyObject::StaticRegisterNativesUMyObject()
                    {
                        UClass* Class = UMyObject::StaticClass();
                        static const FNameNativePtrPair Funcs[] = {
                            { "CallableFunc", &UMyObject::execCallableFunc },
                            { "NativeFunc", &UMyObject::execNativeFunc },
                        };
                        FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
                    }
                    //展开
                    //--------------------------------------------
                        void FNativeFunctionRegistrar::RegisterFunctions(class UClass* Class, const FNameNativePtrPair* InArray, int32 NumFunctions)
                        {
                            for (; NumFunctions; ++InArray, --NumFunctions)
                            {
                                Class->AddNativeFunction(UTF8_TO_TCHAR(InArray->NameUTF8), InArray->Pointer);
                            }
                        }
                        //展开
                            void UClass::AddNativeFunction(const WIDECHAR* InName, FNativeFuncPtr InPointer)
                            {
                                FName InFName(InName);
                            #if WITH_RELOAD
                                if (IsReloadActive())
                                {
                                    // Find the function in the class's native function lookup table.
                                    if (ReplaceNativeFunction(InFName, InPointer, true))
                                    {
                                        return;
                                    }
                                    else
                                    {
                                        // function was not found, so it's new
                                        UE_LOG(LogClass, Log, TEXT("Function %s is new or belongs to a modified class."), *InFName.ToString());
                                    }
                                }
                            #endif
                                new(NativeFunctionLookupTable)FNativeFunctionLookup(InFName, InPointer);//定位new 在NativeFunctionLookupTable添加Native函数的数据。
                            }
                        //--------------------------------------------
                    //--------------------------------------------
                //--------------------------------------------

                //非native函数：其实就是指的就是BlueprintImplementableEvent的函数，它不需要我们自己定义函数体。
                //而UHT会帮我们生成一个函数体，当我们在C++里调用ImplementableFunc的时候，其实会触发一次函数查找，如果在蓝图中有定义该名字的函数，则会得到调用。
    
    // PreInit
    //--------------------------------------------  
        int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
        {
            const int32 rv1 = PreInitPreStartupScreen(CmdLine);
            if (rv1 != 0)
            {
                PreInitContext.Cleanup();
                return rv1;
            }

            const int32 rv2 = PreInitPostStartupScreen(CmdLine);
            if (rv2 != 0)
            {
                PreInitContext.Cleanup();
                return rv2;
            }

            return 0;
        }
    //--------------------------------------------
    展开
        //PreInitPreStartupScreen
        int32 FEngineLoop::PreInitPreStartupScreen(const TCHAR* CmdLine)
        {
            //...
            LoadCoreModules();  //加载CoreUObject模块，注册所有编译进来的类。
            //LoadCoreModules()把CoreUObject里面定义的类的UClass都给先构建出来。但是其实这些UClass对象内部的值还没有完成初始化设置，因此下一个步骤的AppInit()和ProcessNewlyLoadedUObjects()还会继续这个注册的步程。
            //...
            //LoadPreInitModules();   //加载一些PreInit的模块，比如Engine，Renderer
            //...
            AppInit();  //程序初始化，最后一步用一个多播委托通知了程序初始化事件，让别的代码有机会继续进行一些初始化工作。
            //...
            ProcessNewlyLoadedUObjects();   //处理最近加载的对象
            //...
            //LoadStartupModules();   //自己写的LoadingPhase为PreDefault的模块在这个时候加载
            //...
            GUObjectArray.CloseDisregardForGC();    //对象池启用，最开始是关闭的
            //...
            //NotifyRegistrationComplete();   //注册完成事件通知，完成Package加载
        }

        /* 摘抄自 inside ue5，和当前版本略有不符
        GetDeferredClassRegistration()里的元素是之前收集文章里讲的静态初始化的时候添加进去的，在XXX.gen.cpp里用static TClassCompiledInDefer这种形式添加。
        TClassCompiledInDefer<TClass>::Register()内部只是简单的转调TClass::StaticClass()。
        TClass::StaticClass()是在XXX.generated.h里的DECLARE_CLASS宏里定义的，内部只是简单的转到GetPrivateStaticClass(TPackage)。
        GetPrivateStaticClass(TPackage)的函数是实现是在IMPLEMENT_CLASS宏里。其内部会真正调用到GetPrivateStaticClassBody。这个函数的内部会创建出UClass对象并调用Register()，在上篇已经具体讲解过了。
        总结这里的逻辑就是对之前收集到的所有的XXX.gen.cpp里定义的类，都触发一次其UClass的构造，其实也只有UObject比较特殊，会在Static初始化的时候就触发构造。因此这个过程其实是类型系统里每一个类的UClass的创建过程。
        这个函数会被调用多次，在后续的ProcessNewlyLoadedUObjects的里仍然会触发该调用。在FCoreUObjectModule::StartupModule()的这次调用是最先的，这个时候加载编译进来的的类都是引擎启动一开始就链接进来的。
        */

        //AppInit 展开 会调到 InitUObject
        //--------------------------------------------
            void InitUObject()
            {
                //...
                FCoreDelegates::OnExit.AddStatic(StaticExit);   //注册退出的事件
                //注册模块加载的
                FModuleManager::Get().OnProcessLoadedObjectsCallback().AddStatic(ProcessNewlyLoadedUObjects);
                //...
                StaticUObjectInit();//调用另一个函数
            }
        //--------------------------------------------
        
        //StaticUObjectInit展开
        //--------------------------------------------
            void StaticUObjectInit()
            {
                UObjectBaseInit();  //继续转发
                //最后，创建临时包
                GObjTransientPkg = NewObject<UPackage>(nullptr, TEXT("/Engine/Transient"), RF_Transient);
                //在UObjectBaseInit初始化结束后，就已经可以开始NewObject了，标志着整个UObject系统的成功创建！
                    //template< class T >
                    // T* NewObject(UObject* Outer = (UObject*)GetTransientPackage())
                    // {
                    //     //...
                    // }
                //GObjTransientPkg是个全局变量，所有没有Outer的对象都会放在这个包里。我们在NewObject的时候，如果不提供Outer，则会返回这个临时包，符合了UObject对象必须在UPackage里的一贯基本原则。
                GObjTransientPkg->AddToRoot();  //这个临时包总不会释放
                //...
            }
        //--------------------------------------------
        //UObjectBaseInit展开
            void UObjectBaseInit()
            {
                //...

                //初始化UObject的内存分配存储系统和对象的Hash系统
                GUObjectAllocator.AllocatePermanentObjectPool(SizeOfPermanentObjectPool);//初始化对象分配器
                GUObjectArray.AllocateObjectPool(MaxUObjects, MaxObjectsNotConsideredByGC, bPreAllocateUObjectArray);//初始化对象管理数组

                void InitAsyncThread();
                InitAsyncThread();  //初始化Package(uasset)的异步加载线程，用来后续Package(uasset)的加载

                Internal::GObjInitialized = true;   //指定UObject系统初始化完毕  在后续就可以用bool UObjectInitialized()来判断对象系统是否可用。 

                UObjectProcessRegistrants();    //继续转发到UObjectProcessRegistrants来把注册项一一处理。
                //...
            }
        //--------------------------------------------

        //UObjectProcessRegistrants展开
        //--------------------------------------------
            static void UObjectProcessRegistrants()
            {
                TArray<FPendingRegistrant> PendingRegistrants;
                DequeuePendingAutoRegistrants(PendingRegistrants);  //从链表中提取注册项列表

                for(int32 RegistrantIndex = 0;RegistrantIndex < PendingRegistrants.Num();++RegistrantIndex)
                {
                    const FPendingRegistrant& PendingRegistrant = PendingRegistrants[RegistrantIndex];
                    UObjectForceRegistration(PendingRegistrant.Object); //真正的注册
                    DequeuePendingAutoRegistrants(PendingRegistrants);  //继续尝试提取
                }
            }
        //--------------------------------------------

        //UObjectForceRegistration展开
        void UObjectForceRegistration(UObjectBase* Object)
        {
            TMap<UObjectBase*, FPendingRegistrantInfo>& PendingRegistrants = FPendingRegistrantInfo::GetMap();//得到对象的注册信息 先判断是否PendingRegistrants里还存在该元素。
            FPendingRegistrantInfo* Info = PendingRegistrants.Find(Object);
            if (Info)   //有可能为空，因为之前已经被注册过了
            {
                const TCHAR* PackageName = Info->PackageName;//对象所在的Package
                const TCHAR* Name = Info->Name; //对象名字
                PendingRegistrants.Remove(Object);//删除
                Object->DeferredRegister(UClass::StaticClass(),PackageName,Name);//延迟注册
            }
        }
        //--------------------------------------------

        //DeferredRegister展开 ————————————真正注册的地方
            /*ref： insideUE5
            Deferred是延迟的意思，区分于之前的UObjectBase::Register，延迟的意思是在对象系统初始化（GUObjectAllocator和GUObjectArray）之后的注册。
            Register的时候还不能正常NewObject和加载Package，而初始化之后这个阶段就可以开始正常的使用UObject系统的功能了。所以这里面才可以开始CreatePackage。
            Register注册，确定一点的意思是对代码里的class生成相应的UClass*对象并添加（注册）到全局对象数组里。

            这里的作为是创建出UClass*的Outer指向的Package，并设置ClassPrivate（这里都是UClass*对象，所以其实都是UClass::StaticClass()）。
            然后在AddObject里设置NamePrivate。因此这步之后这些一个个UClass*对象才有名字，之间的联系才算完整。 
            但同时也需要注意的是，这些UClass*对象里仍然没有UProperty和UFunciton
            */
        //--------------------------------------------
            void UObjectBase::DeferredRegister(UClass *UClassStaticClass,const TCHAR* PackageName,const TCHAR* InName)
            {
                // Set object properties.
                UPackage* Package = CreatePackage(nullptr, PackageName);    //创建属于的Package
                Package->SetPackageFlags(PKG_CompiledIn);
                OuterPrivate = Package; //设定Outer到该Package

                ClassPrivate = UClassStaticClass;   //设定属于的UClass*类型

                // Add to the global object table.
                AddObject(FName(InName), EInternalObjectFlags::None);   //注册该对象的名字
            }
        //--------------------------------------------

        //ProcessNewlyLoadedUObjects展开
        //--------------------------------------------
        void ProcessNewlyLoadedUObjects()
        {
            UClassRegisterAllCompiledInClasses();   //为每一个编译进来的class调用TClass::StaticClass()来构造出UClass*对象。
            //提取收集到的注册项信息
            const TArray<UClass* (*)()>& DeferredCompiledInRegistration=GetDeferredCompiledInRegistration();//是之前信息收集的时候static FCompiledInDefer变量初始化时收集到的全局数组，和定义的class一对一。
            const TArray<FPendingStructRegistrant>& DeferredCompiledInStructRegistration=GetDeferredCompiledInStructRegistration();//之前信息收集的时候static FCompiledInDeferStruct变量初始化时收集到的全局数组，和定义的struct一对一。
            const TArray<FPendingEnumRegistrant>& DeferredCompiledInEnumRegistration=GetDeferredCompiledInEnumRegistration();//之前信息收集的时候static FCompiledInDeferEnum变量初始化时收集到的全局数组，和定义的enum一对一。
            //有待注册项就继续循环注册
            bool bNewUObjects = false;
            while (GFirstPendingRegistrant || 
            DeferredCompiledInRegistration.Num() || 
            DeferredCompiledInStructRegistration.Num() || 
            DeferredCompiledInEnumRegistration.Num())
            {
                bNewUObjects = true;
                UObjectProcessRegistrants();    //为之前生成的UClass*注册，生成其Package。这里调用的目的是在后续的操作之前确保内存里已经把相关的类型UClass*对象都已经注册完毕。
                UObjectLoadAllCompiledInStructs();  //为enum和struct分别生成UEnum和UScriptStruct对象。后文详细讲解。
                UObjectLoadAllCompiledInDefaultProperties();    //为UClass*们继续构造和创建类默认对象(CDO)。
            }

            if (bNewUObjects && !GIsInitialLoad)
            {
                UClass::AssembleReferenceTokenStreams();    //为UClass创建引用记号流（一种辅助GC分析对象引用的数据结构）
            }
        }
        //--------------------------------------------
        //UObjectLoadAllCompiledInStructs展开
        static void UObjectLoadAllCompiledInStructs()
        {
            TArray<FPendingEnumRegistrant> PendingEnumRegistrants = MoveTemp(GetDeferredCompiledInEnumRegistration());
            for (const FPendingEnumRegistrant& EnumRegistrant : PendingEnumRegistrants)
            {
                CreatePackage(nullptr, EnumRegistrant.PackageName); //创建其所属于的Package（先查找该名字的Package是否已经存在，不会重复创建。）
            }

            TArray<FPendingStructRegistrant> PendingStructRegistrants = MoveTemp(GetDeferredCompiledInStructRegistration()); //右移引用赋值，把源数组里的数据迁移到目标数组里去。所以外层的while判断值才会改变。
            for (const FPendingStructRegistrant& StructRegistrant : PendingStructRegistrants)
            {
                CreatePackage(nullptr, StructRegistrant.PackageName);   //创建其所属于的Package
            }

            for (const FPendingEnumRegistrant& EnumRegistrant : PendingEnumRegistrants)
            {
                EnumRegistrant.RegisterFn();    //调用生成代码里Z_Construct_UEnum_Hello_EMyEnum 先enum再struct的调用其注册函数RegisterFn()。RegisterFn是个函数指针，指向生成代码里Z_Construct开头的函数，用来真正构造出UEnum和UScriptStruct对象。
            }
            for (const FPendingStructRegistrant& StructRegistrant : PendingStructRegistrants)
            {
                StructRegistrant.RegisterFn(); //调用生成代码里Z_Construct_UScriptStruct_FMyStruct
            }
        }
        //--------------------------------------------

        //UObjectLoadAllCompiledInDefaultProperties 展开
            static void UObjectLoadAllCompiledInDefaultProperties()
            {
                static FName LongEnginePackageName(TEXT("/Script/Engine")); //引擎包的名字
                if(GetDeferredCompiledInRegistration().Num() <= 0) return;
                TArray<UClass*> NewClassesInCoreUObject;
                TArray<UClass*> NewClassesInEngine;
                TArray<UClass*> NewClasses;
                TArray<UClass* (*)()> PendingRegistrants = MoveTemp(GetDeferredCompiledInRegistration());
                for (UClass* (*Registrant)() : PendingRegistrants) //从GetDeferredCompiledInRegistration()的源数组里MoveTemp出来遍历。
                {
                    UClass* Class = Registrant();//继续构造UClass*，这个函数指向了生成代码里形如Z_Construct_UClass_UMyClass的函数。
                    
                    //对生成的UClass*对象，依照属于的Package划分到3个数组里。
                    if (Class->GetOutermost()->GetFName() == GLongCoreUObjectPackageName)//
                    {
                        NewClassesInCoreUObject.Add(Class);
                    }
                    else if (Class->GetOutermost()->GetFName() == LongEnginePackageName)
                    {
                        NewClassesInEngine.Add(Class);
                    }
                    else
                    {
                        NewClasses.Add(Class);
                    }
                }
                //分别构造CDO对象
                /* ref：inside UE5
                对3个数组分别依次手动构造CDO对象。这三个数组的顺序是：CoreUObject、Engine和其他。按照此顺序构造的原因是根据依赖关系。
                构造CDO的过程，有可能触发uassset的加载和UObject构造函数的调用，所以就可能在内部触发其他Package里对象的加载构造。
                CoreUObject最底层（它不会引用其他的Package里的对象）、Engine次之（它有可能引用底层的对象）、其他（就不确定会引用啥了）。
                所以依照此顺序能避免依赖倒置，从而减少重复调用查找。
                */
                for (UClass* Class : NewClassesInCoreUObject)   { Class->GetDefaultObject(); }
                for (UClass* Class : NewClassesInEngine)        { Class->GetDefaultObject(); }
                for (UClass* Class : NewClasses)                { Class->GetDefaultObject(); }
            }
        //--------------------------------------------

    //CloseDisregardForGC展开 在类型系统都构建完了之后，就可以放心的打开GC了，因为后续就有可能NewObject来生成对象了。
    void FUObjectArray::CloseDisregardForGC()
    {
        if (!GIsRequestingExit)
        {
            ProcessNewlyLoadedUObjects();//之前仍然有可能加载了别的模块dll
        }

        UClass::AssembleReferenceTokenStreams(); //此时才是真正的第一次为所有的UClass\*构建引用记号流
        //...
        OpenForDisregardForGC = false;
        GIsInitialLoad = false;//初始化载入阶段结束
    }
    //--------------------------------------------

    //MyObject.gen.cpp 直接用 insideUE5 排版后的代码
        //函数参数...
        struct Z_Construct_UClass_UMyClass_Statics
        {
            //依赖项列表
            static UObject* (*const DependentSingletons[])()=
            {
                (UObject* (*)())Z_Construct_UClass_UObject,//依赖基类UObject
                (UObject* (*)())Z_Construct_UPackage__Script_Hello,//依赖所属于的Hello模块
            };
            //属性参数...
            //函数参数...
            //接口
            static const UE4CodeGen_Private::FImplementedInterfaceParams InterfaceParams[]= 
            {
                {
                    Z_Construct_UClass_UMyInterface_NoRegister,//构造UMyInterface所属的UClass*函数指针
                    (int32)VTABLE_OFFSET(UMyClass, IMyInterface),//多重继承的指针偏移
                    false   //是否是在蓝图实现
                }
            };

            static const FCppClassTypeInfoStatic StaticCppClassTypeInfo= {
                TCppClassTypeTraits<UMyClass>::IsAbstract,//c++类信息，是否是虚类
            };

            static const UE4CodeGen_Private::FClassParams ClassParams = 
            {
                &UMyClass::StaticClass,//取出UClass*的函数指针
                DependentSingletons, ARRAY_COUNT(DependentSingletons),//依赖项
                0x001000A0u,//类标志
                FuncInfo, ARRAY_COUNT(FuncInfo),//函数列表
                PropPointers, ARRAY_COUNT(PropPointers),//属性列表
                nullptr,//Config文件名
                &StaticCppClassTypeInfo,//c++类信息
                InterfaceParams, ARRAY_COUNT(InterfaceParams)//接口列表
            };
        };

        UClass* Z_Construct_UClass_UMyClass()
        {
            static UClass* OuterClass = nullptr;
            if (!OuterClass)
            {
                UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UMyClass_Statics::ClassParams);
            }
            return OuterClass;
        }

        IMPLEMENT_CLASS(UMyClass, 4008851639); //收集点
        static FCompiledInDefer Z_CompiledInDefer_UClass_UMyClass(Z_Construct_UClass_UMyClass, &UMyClass::StaticClass, TEXT("/Script/Hello"), TEXT("UMyClass"), false, nullptr, nullptr, nullptr); //收集点
        //--------------------------------------------

        //ConstructUClass展开
        void ConstructUClass(UClass*& OutClass, const FClassParams& Params)
        {
            if (OutClass && (OutClass->ClassFlags & CLASS_Constructed)) {return;}  //防止重复构造
            for(int i=0;i<Params.NumDependencySingletons;++i)
            {
                Params.DependencySingletonFuncArray[i]();   //构造依赖的对象
            }

            UClass* NewClass = Params.ClassNoRegisterFunc();    //取得先前生成的UClass*，NoRegister是指没有经过DeferRegister
            OutClass = NewClass;

            if (NewClass->ClassFlags & CLASS_Constructed) {return;}//防止重复构造

            UObjectForceRegistration(NewClass); //确保此UClass*已经注册

            NewClass->ClassFlags |= (EClassFlags)(Params.ClassFlags | CLASS_Constructed);//标记已经构造

            if ((NewClass->ClassFlags & CLASS_Intrinsic) != CLASS_Intrinsic)
            {
                check((NewClass->ClassFlags & CLASS_TokenStreamAssembled) != CLASS_TokenStreamAssembled);
                NewClass->ReferenceTokenStream.Empty();//对于蓝图类需要重新生成一下引用记号流
            }
            //构造函数列表
            NewClass->CreateLinkAndAddChildFunctionsToMap(Params.FunctionLinkArray, Params.NumFunctions);
                //展开
                //--------------------------------------------
                    void UClass::CreateLinkAndAddChildFunctionsToMap(const FClassFunctionLinkInfo* Functions, uint32 NumFunctions)//一个个创建UFunction*对象然后添加到FuncMap里去就是了。
                    {
                        for (; NumFunctions; --NumFunctions, ++Functions)
                        {
                            const char* FuncNameUTF8 = Functions->FuncNameUTF8;
                            UFunction*  Func         = Functions->CreateFuncPtr();//调用构造UFunction*对象

                            Func->Next = Children;
                            Children = Func;//新函数挂在UField*链表的开头

                            AddFunctionToFunctionMap(Func, FName(UTF8_TO_TCHAR(FuncNameUTF8)));
                            //内部实现是：FuncMap.Add(FuncName, Function);添加到FuncMap里
                        }
                    }
                //--------------------------------------------
            //构造属性列表
            ConstructUProperties(NewClass, Params.PropertyArray, Params.NumProperties);

            if (Params.ClassConfigNameUTF8)
            {   //配置文件名
                NewClass->ClassConfigName = FName(UTF8_TO_TCHAR(Params.ClassConfigNameUTF8));
            }

            NewClass->SetCppTypeInfoStatic(Params.CppClassInfo);//C++类型信息

            if (Params.NumImplementedInterfaces)
            {
                NewClass->Interfaces.Reserve(Params.NumImplementedInterfaces);
                for(int i=0;i<Params.Params.NumImplementedInterfaces;++i)
                {
                    const auto& ImplementedInterface = Params.ImplementedInterfaceArray[i];
                    UClass* (*ClassFunc)() = ImplementedInterface.ClassFunc;
                    UClass* InterfaceClass = ClassFunc ? ClassFunc() : nullptr;//取得UMyInterface所属于的UClass*对象

                    NewClass->Interfaces.Emplace(InterfaceClass, ImplementedInterface.Offset, ImplementedInterface.bImplementedByK2);//添加实现的接口
                }
            }

            NewClass->StaticLink();//链接
        }
        /*
        先确保一下依赖对象已经存在，然后一一把各种信息：函数、属性、配置文件名、C++类型信息和接口添加到UClass*里去。
        重要的只有三步：      ConstructUProperties和UScriptStruct的时候一样；
                            接口的实现是通过UClass*里的TArray<FImplementedInterface> Interfaces数组表达的；
                            CreateLinkAndAddChildFunctionsToMap创造函数列表。
        */

链接阶段：在构造完各类型对象后，还需要再梳理一遍，完成一些后初始化工作。通过符号定位到函数地址来替换，绑定链接。
    //--------------------------------------------
        void UClass::Bind()
        {
            UStruct::Bind();
            UClass* SuperClass = GetSuperClass();
            if (SuperClass && 
                    (ClassConstructor == nullptr || 
                    ClassAddReferencedObjects == nullptr || 
                    ClassVTableHelperCtorCaller == nullptr)
                )
            {
                SuperClass->Bind();//确保基类已经绑定
                if (!ClassConstructor)
                {
                    ClassConstructor = SuperClass->ClassConstructor;//绑定构造函数指针
                }
                if (!ClassVTableHelperCtorCaller)
                {
                    ClassVTableHelperCtorCaller = SuperClass->ClassVTableHelperCtorCaller;//绑定热载函数指针
                }
                if (!ClassAddReferencedObjects)
                {
                    ClassAddReferencedObjects = SuperClass->ClassAddReferencedObjects;//绑定ARO函数指针
                }

                ClassCastFlags |= SuperClass->ClassCastFlags;
            }
        }
    //--------------------------------------------

    // StaticLink 展开
        void UStruct::StaticLink(bool bRelinkExistingProperties /*= false*/)
        {
            FArchive ArDummy;   //一个空的序列化归档类
            Link(ArDummy, bRelinkExistingProperties);
        }
    //--------------------------------------------
        //Link 展开
        /*
        把之前AddCppProperty添加到UField* Children里的字段们，抽出UProperty们，然后再串成4个链条：

        PropertyLink：所有的属性
        RefLink：包含对象引用（UObject*）的属性，这些属性对GC有影响，所以单独分类出来加速分析。
        PostConstructLink：所有需要从CDO中获取初始值的属性，属性可以从Config文件中或者CDO中获取初始值，因此在序列化后需要初始化一下属性们的值。
        DestructorLink：需要额外析构的属性，在析构的时候，需要去调用一下属性的析构函数。否则的话，如一个int属性，就不用管它，释放内存就可以了。
        */
        void UStruct::Link(FArchive& Ar, bool bRelinkExistingProperties)
        {
            for (UField* Field=Children; (Field != NULL) && (Field->GetOuter() == this); Field = Field->Next)
            {
                if (UProperty* Property = dynamic_cast<UProperty*>(Field))
                {
                    Property->LinkWithoutChangingOffset(Ar);//对所有属性先Link一下。
                }
            }
            UProperty** PropertyLinkPtr = &PropertyLink;
            UProperty** DestructorLinkPtr = &DestructorLink;
            UProperty** RefLinkPtr = (UProperty**)&RefLink;
            UProperty** PostConstructLinkPtr = &PostConstructLink;
            TArray<const UStructProperty*> EncounteredStructProps;
            for (TFieldIterator<UProperty> It(this); It; ++It)  //遍历出所有属性
            {
                UProperty* Property = *It;
                if (Property->ContainsObjectReference(EncounteredStructProps) || Property->ContainsWeakObjectReference())
                {
                    *RefLinkPtr = Property;//包含对象引用的属性
                    RefLinkPtr = &(*RefLinkPtr)->NextRef;
                }
                const UClass* OwnerClass = Property->GetOwnerClass();
                bool bOwnedByNativeClass = OwnerClass && OwnerClass->HasAnyClassFlags(CLASS_Native | CLASS_Intrinsic);
                if (!Property->HasAnyPropertyFlags(CPF_IsPlainOldData | CPF_NoDestructor) &&
                    !bOwnedByNativeClass) // these would be covered by the native destructor
                {   
                    *DestructorLinkPtr = Property;//需要额外析构的属性
                    DestructorLinkPtr = &(*DestructorLinkPtr)->DestructorLinkNext;
                }
                if (OwnerClass && (!bOwnedByNativeClass || (Property->HasAnyPropertyFlags(CPF_Config) && !OwnerClass->HasAnyClassFlags(CLASS_PerObjectConfig))))
                {
                    *PostConstructLinkPtr = Property;//需要从CDO中获取初始值的属性
                    PostConstructLinkPtr = &(*PostConstructLinkPtr)->PostConstructLinkNext;
                }
                *PropertyLinkPtr = Property;//所有属性
                PropertyLinkPtr = &(*PropertyLinkPtr)->PropertyLinkNext;
            }
            *PropertyLinkPtr = nullptr;
            *DestructorLinkPtr = nullptr;
            *RefLinkPtr = nullptr;
            *PostConstructLinkPtr = nullptr;
        }
    //--------------------------------------------

    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------