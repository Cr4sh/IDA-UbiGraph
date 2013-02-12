OUTNAME = ubicallback.pyd

ALL: $(OUTNAME)

CC = cl.exe

CFLAGS = /nologo -I".\src" -I".\lib\xmlrpc-c\include" -I"D:\Python-2.6.2\include" -D_X86_=1 -DWIN32 -DCURL_STATICLIB -DDBG /Gy /Gd /TP /EHsc /c

include makefile.inc

LN = link.exe

LFLAGS = /LIBPATH:".\lib\xmlrpc-c\lib" /LIBPATH:"D:\Python-2.6.2\Libs" /DLL /INCREMENTAL:NO /DEF:ubicallback.def /DEBUG /SUBSYSTEM:WINDOWS /OUT:..\$(OUTNAME)

LLIBS = xmlrpc.lib xmlparse.lib xmltok.lib abyss.lib WinInet.lib ws2_32.lib

$(OUTNAME): $(LOBJS)
    $(LN) $(LFLAGS) $(LOBJS) $(LLIBS)

clean:
    @del *.obj ..\*.lib ..\*.exp
