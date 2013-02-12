#include "stdafx.h"

#define LOG_NAME "ubicallback_xmlrpc.log"
#define PYTHON_MODULE_NAME "ubicallback"

#define XMLRPC_SERVER_PORT 11631

PyObject *init(PyObject* self, PyObject* pArgs);

static PyMethodDef m_PyMethods[] = 
{
    { "init", init, METH_VARARGS, "Initializes XMLRPC server at specified port." },
    { NULL,   NULL, 0,            NULL }
};

DWORD m_dwThreadId = 0;
//--------------------------------------------------------------------------------------
#ifdef DBG
//--------------------------------------------------------------------------------------
void DbgMsg(char *lpszFile, int Line, char *lpszMsg, ...)
{
    va_list mylist;
    va_start(mylist, lpszMsg);

    size_t BuffLen = _vscprintf(lpszMsg, mylist) + 0x100;
    char *lpszBuff = (char *)malloc(BuffLen);
    if (lpszBuff == NULL)
    {
        va_end(mylist);
        return;
    }    

    size_t OutBuffLen = strlen(lpszBuff) + 0x100;
    char *lpszOutBuff = (char *)malloc(OutBuffLen);
    if (lpszOutBuff == NULL)
    {
        free(lpszBuff);
        va_end(mylist);
        return;
    }

    vsprintf(lpszBuff, lpszMsg, mylist);	
    va_end(mylist);

    sprintf_s(
        lpszOutBuff, OutBuffLen, "[%.5d] .\\%s(%d) : %s", 
        GetCurrentProcessId(), lpszFile, Line, lpszBuff
    );

    // write message into the debug output
    OutputDebugString(lpszOutBuff);

    // write message into the console
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStd != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten = 0;
        WriteFile(hStd, lpszBuff, strlen(lpszBuff), &dwWritten, NULL);
    }

    free(lpszBuff);
    free(lpszOutBuff);
}
//--------------------------------------------------------------------------------------
#else
//--------------------------------------------------------------------------------------
#define DbgMsg
//--------------------------------------------------------------------------------------
#endif // DBG
//--------------------------------------------------------------------------------------
void Eval(char *lpszMsg, ...)
{
    va_list mylist;
    va_start(mylist, lpszMsg);

    size_t BuffLen = _vscprintf(lpszMsg, mylist) + 0x100;
    char *lpszBuff = (char *)malloc(BuffLen);
    if (lpszBuff == NULL)
    {
        va_end(mylist);
        return;
    }  

    vsprintf(lpszBuff, lpszMsg, mylist);	
    va_end(mylist);

    PyRun_SimpleString(lpszBuff);

    free(lpszBuff);
}
//--------------------------------------------------------------------------------------
static xmlrpc_value *
vertex_callback(
   xmlrpc_env * const envP,
   xmlrpc_value * const paramArrayP,
   void * const serverInfo,
   void * const channelInfo) 
{
    xmlrpc_int32 vertex_id = 0, ret = 0;

    /* Parse our argument array. */
    xmlrpc_decompose_value(envP, paramArrayP, "(i)", &vertex_id);
    if (envP->fault_occurred)
    {
        return NULL;
    }    

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): 0x%.8x\n", vertex_id);

    // pass vertex callback event to MyNav
    Eval("mybrowser.UbiDispatchVertexCallback(0x%.8x)", vertex_id);

    /* Return our result. */
    return xmlrpc_build_value(envP, "i", ret);
}
//--------------------------------------------------------------------------------------
DWORD WINAPI ServerThread(LPVOID lpParam)
{
    struct xmlrpc_method_info3 const methodInfo = 
    {
        /* .methodName     = */ "vertex_callback",
        /* .methodFunction = */ &vertex_callback,
    };

    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry *registryP;
    xmlrpc_env env;

    xmlrpc_env_init(&env);

    registryP = xmlrpc_registry_new(&env);

    xmlrpc_registry_add_method3(&env, registryP, &methodInfo);

    /* In the modern form of the Abyss API, we supply parameters in memory
       like a normal API.  We select the modern form by setting
       config_file_name to NULL: 
    */
    serverparm.config_file_name = NULL;
    serverparm.registryP = registryP;
    serverparm.port_number = XMLRPC_SERVER_PORT;
    serverparm.log_file_name = "./" LOG_NAME;

    DbgMsg(
        __FILE__, __LINE__, 
        __FUNCTION__"(): Running XML-RPC server at port %d...\n", 
        serverparm.port_number
    );

    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));

    /* xmlrpc_server_abyss() never returns */ 
    return 0;
}
//--------------------------------------------------------------------------------------
PyObject *init(PyObject* self, PyObject* pArgs)
{
    PyObject *Ret = Py_None;

    Py_INCREF(Py_None);

    // check for allready initialized case
    if (m_dwThreadId > 0)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() WARNING: XMLRPC server is allready running\n");
        goto end;
    }

    // run XMLRPC server n the new thread
    HANDLE hThread = CreateThread(NULL, 0, ServerThread, NULL, 0, &m_dwThreadId);
    if (hThread)
    {
        CloseHandle(hThread);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "CreateThread() ERROR %d\n", GetLastError());
    }

end:  

    return Ret;
}
//--------------------------------------------------------------------------------------
PyMODINIT_FUNC initubicallback(void)
{
    // initialize python module
    Py_InitModule(PYTHON_MODULE_NAME, m_PyMethods);
}
//--------------------------------------------------------------------------------------
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fwdreason,
    LPVOID lpvReserved)
{
    switch (fwdreason)
    {
    case DLL_PROCESS_ATTACH:
        {            
            DbgMsg(__FILE__, __LINE__, "UBICALLBACK: DLL_PROCESS_ATTACH\n");            
            break;
        }

    case DLL_PROCESS_DETACH:
        {
            DbgMsg(__FILE__, __LINE__, "UBICALLBACK: DLL_PROCESS_DETACH\n");
            break;
        }
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
// EoF
